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
#include <assert.h>

// Uncompressed PNG Library
#include "svpng.c"

// debug
#include "heightmap_gen.h"

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
      int top_left  = calc_vertex_index(x, z, export_size);
      int top_right = calc_vertex_index(x + 1, z, export_size);
      int bot_left  = calc_vertex_index(x, z + 1, export_size);
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
