/***********************************************************************
* FILENAME :        heightmap_gen.h   heightmap_gen.c
*
* DESCRIPTION :
*       Implements the main noise and pre erosion heightmap generation
*       from simplex noise
*
* PUBLIC FUNCTIONS :
*       void gen_heightmap(float* height_map, int map_size, setting_t setting);
* 
*
* PRIVATE FUNCTIONS :
*
* NOTES :
*       Generates heightmap based on seed and number of octaves and 
*       persistence with modifier scale and height
*
* AUTHOR :    Henry Jiang         DATE :    Feb 11, 2021
*/

#include "heightmap_gen.h"
#include "noise.h"

#include <stdlib.h>
#include <string.h>

// add noise to heightmap based on scale and weight
void write_single(float* height_map, int map_size, float scale, float weight) {
  // introduce randomness in offsets
  int x_offset = rand() % map_size;
  int y_offset = rand() % map_size;

  for (int x = 0; x < map_size; x++) {
    for (int y = 0; y < map_size; y++) {
      double sample_x = (double) (x + x_offset) / (double) scale;
      double sample_y = (double) (y + y_offset) / (double) scale;
      
      double val = noise(sample_x, sample_y);         /* val in [-1, 1] */
      float noise_normalized = weight * (val + 1) / 2;
      
      height_map[x * map_size + y] += noise_normalized;
    }
  }
}

void gen_heightmap(float* height_map, int map_size, setting_t setting) {
  // set seed and init permutation array
  init_perm();
  memset(height_map, 0, map_size * map_size);
  
  srand(setting->seed);
  float weight = 1.0f;
  float scale = setting->scale * map_size;
  
  for (int oct = 0; oct < setting->octaves; oct++) {
    // layer noise with decreasing scale and weight
    write_single(height_map, map_size, scale, weight);
    weight *= setting->persistence; /* each noise layer contributes less */
    scale /= 2;
  }

  // normalize heightmap
  float min = 1; 
  float max = 0;
  for (int index = 0; index < map_size * map_size; index++) {
    if (height_map[index] > max) {
        max = height_map[index];
    }
    else if (height_map[index] < min) {
        min = height_map[index];
    }
  }

  // normalize and scale with height 
  for (int index = 0; index < map_size * map_size; index++) {
    float norm = (height_map[index] - min) / (max - min);
    height_map[index] = norm * setting->height;
    // TODO: Maybe passing a mapping function f: R -> R
  }
}
