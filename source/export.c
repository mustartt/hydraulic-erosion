/***********************************************************************
* FILENAME :        export.h   export.c
*
* DESCRIPTION :
*       Utility Functions for exporting heightmap as .obj files
*       or png heightmaps
*
* PUBLIC FUNCTIONS :
*       
*
* NOTES :
*       This export utils function export file to the virtual file system
*       under the Webassembly VFS, which has to be streamed into a url link
*       the js module: example code snippet
*       https://motley-coder.com/2019/04/01/download-files-emscripten/
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*H*/

#include "export.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

// Uncompressed PNG Library
#include "svpng.c"

#define EXPORT_MSG "# Exported from Hydraulic Erosion https://github.com/mustartt/hydraulic-erosion"

float* read_map(char* filename, int size) {
  FILE* file = fopen(filename, "r");

  float* height_map = (float*) malloc(size * size * sizeof(float));
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      int val;
      fscanf(file, "%d", &val);
      height_map[x * size + y] = (float) val / 255.0f;
    }
  }

  fclose(file);
  return height_map;
}

void write_map(float* height_map, int size, char* filename) {
  FILE* file = fopen(filename, "w");

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      int val = (int) (height_map[x * size + y] * 255);
      fprintf(file, "%d ", val);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}



int calc_vertex_index(int x, int z, int size) {
  return x * size + z + 1; // one based index 
}


void export_obj(float* heightmap, int map_size, int export_size, char* filename) {
  assert(export_size <= map_size);            /* export_size less than map_size */
  assert(map_size % export_size == 0);        /* must be integer multiple */ 
  int export_scale = map_size / export_size; 

  FILE* fp = fopen(filename, "w");
  fprintf(fp, EXPORT_MSG);
  fprintf(fp, "# List of geometric vertices coordinate (x, y, z)\n");

  // format is "v coord_x, coord_y, coord_z"
  for (int x = 0; x < export_size; x++) {
    for (int z = 0; z < export_size; z++) {
      int map_index = (x * export_scale) * map_size + (z * export_scale);
      float coord_y = heightmap[map_index];

      float coord_x = (float) x / export_size;
      float coord_z = (float) z / export_size;
      
      // write rows
      fprintf(fp, "v %f %f %f\n", coord_x, coord_y, coord_z);
    }
  }
  
  fprintf(fp, "# List of faces: f (v1 index, v2 index, v3 index) \n");

  // write face vertex indicies
  // face1: (x, z)      (x + 1, z)  (x, z + 1)
  // face2: (x + 1, z)  (x, z + 1)  (x + 1, z + 1)
  for (int x = 0; x < export_size - 1; x++) {
    for (int z = 0; z < export_size - 1; z++) {
      int top_left  = calc_vertex_index(x,     z,     export_size);
      int top_right = calc_vertex_index(x + 1, z,     export_size);
      int bot_left  = calc_vertex_index(x,     z + 1, export_size);
      int bot_right = calc_vertex_index(x + 1, z + 1, export_size);
      
      fprintf(fp, "f %d %d %d\n", top_left,  top_right, bot_left);
      fprintf(fp, "f %d %d %d\n", top_right, bot_right, bot_left);
    }
  }

  fclose(fp);
}


// clamps val between min and max
float clamp(float val, float min, float max) {
  const float t = val < min ? min : val;
  return t > max ? max : t;
}


void export_png(float* heightmap, int map_size, char* filename) {
  // create temporary buffer
  unsigned char* rgb_buffer = malloc(map_size * map_size * 3 * sizeof(char));
  unsigned char* ptr = rgb_buffer;

  FILE* fp = fopen(filename, "wb");
  
  if (rgb_buffer == NULL || fp == NULL)
    return;
  
  // encode heightmap to color data
  unsigned int x, y;
  for (y = 0; y < map_size; y++) {
    for (x = 0; x < map_size; x++) {
      float val = clamp(heightmap[y * map_size + x], 0, 1);    /* in [0, 1] */
      unsigned char pixel_val = (unsigned char) (val * 255);
      // set channel colors
      *ptr++ = pixel_val;         /* R */
      *ptr++ = pixel_val;         /* G */
      *ptr++ = pixel_val;         /* B */
    }
  }

  svpng(fp, map_size, map_size, rgb_buffer, 0);
  free(rgb_buffer);
  fclose(fp);
}

typedef struct {
  float x;
  float y;
  float z;
} vec3;


/* calculates u - v in R3 */
void subtract(vec3* u, vec3* v, vec3* result) {
  result->x = u->x - v->x;
  result->y = u->y - v->y;
  result->z = u->z - v->z;
}

/* calculates the cross product between u and v */
void cross_product(vec3* u, vec3* v, vec3* result) {
    result->x = u->y * v->z - u->z * v->y;
    result->y = -(u->x * v->z - u->z * v->x);
    result->z = u->x * v->y - u->y * v->x;
}

void print_vec(vec3 vec) {
  printf("vec: %f %f %f\n", vec.x, vec.y, vec.z);
}

void write_vec(vec3* vec, FILE* fp) {
  fwrite(vec, sizeof(vec3), 1, fp);
}

void export_stl(float* heightmap, int map_size, char* filename) {
  FILE* fp = fopen(filename, "wb");
  
  if (fp == NULL)
    return;

  // Header
  uint8_t  header[80] = EXPORT_MSG;
  uint32_t face_count = 2 * (map_size - 1) * (map_size - 1);
  uint16_t attr_buffer[1] = { 0 };

  fwrite(header, sizeof(uint8_t), 80, fp);
  fwrite(&face_count, sizeof(uint32_t), 1, fp); 

  // v1 is top-left, v2 is top-right, v3 is bottom-right, v4 is bottom-left
  for (int z = 0; z < map_size - 1; z++) {
    for (int x = 0; x < map_size - 1; x++) {
      // sample height map
      float sample1 = heightmap[z * map_size + x];
      float sample2 = heightmap[z * map_size + (x + 1)];
      float sample3 = heightmap[(z + 1) * map_size + (x + 1)];
      float sample4  = heightmap[(z + 1) * map_size + x];

      vec3 v1 = { (float) x / (map_size - 1),       sample1, (float) z / (map_size - 1)};
      vec3 v2 = { (float) (x + 1) / (map_size - 1), sample2, (float) z / (map_size - 1)};
      vec3 v3 = { (float) (x + 1) / (map_size - 1), sample3, (float) (z + 1) / (map_size - 1)};
      vec3 v4 = { (float) x / (map_size - 1),       sample4, (float) (z + 1) / (map_size - 1)};
      
      // triangle 1 -> 124
      vec3 edge21, edge41, normal1;
      subtract(&v2, &v1, &edge21);
      subtract(&v4, &v1, &edge41);
      cross_product(&edge41, &edge21, &normal1);

      // triangel 2 -> 234
      vec3 edge23, edge43, normal2; 
      subtract(&v2, &v3, &edge23);
      subtract(&v4, &v3, &edge43);
      cross_product(&edge23, &edge43, &normal2);
      
      // write normal1
      write_vec(&normal1, fp);
      // write v1 v2 v4
      write_vec(&v1, fp);
      write_vec(&v2, fp);
      write_vec(&v4, fp);
      // write uint16_t attribute byte of 0
      fwrite(attr_buffer, sizeof(uint16_t), 1, fp);

      // write normal2
      write_vec(&normal2, fp);
      // write v2 v3 v4
      write_vec(&v2, fp);
      write_vec(&v3, fp);
      write_vec(&v4, fp);
      // write uint16_t attribute byte of 0
      fwrite(attr_buffer, sizeof(uint16_t), 1, fp);
    }
  }

  fclose(fp);
}
