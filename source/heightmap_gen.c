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
// the noise freq is inversly prop to scale
void write_single(float* height_map, int map_size, float scale, float weight) {
  // introduce offsets
  double x_offset = (double) (defined_random() % map_size) / map_size;
  double y_offset = (double) (defined_random() % map_size) / map_size;
  
  for (int y = 0; y < map_size; y++) {
    for (int x = 0; x < map_size; x++) {
      double sample_x = ((double) x / map_size) / scale + x_offset;
      double sample_y = ((double) y / map_size) / scale + y_offset;
      
      double sample = (noise(sample_x, sample_y) + 1) / 2; // between [0, 1] 
      height_map[y * map_size + x] += sample * weight;
    }
  }
}

void gen_heightmap(float* height_map, int map_size, setting_t setting) {
  // set seed and init permutation array
  init_perm();
  memset(height_map, 0, map_size * map_size * sizeof(float));
  set_random_seed(setting->seed);

  float weight = 1.0f; // inital weight value
  float scale = setting->scale;
  
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
