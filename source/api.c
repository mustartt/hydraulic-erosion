/***********************************************************************
* FILENAME :        api.c   api.h
*
* DESCRIPTION :
*       The main API file for interacting with the erosion simulator.
*
* PUBLIC FUNCTIONS :
*          
* NOTES :
*
* AUTHOR :    Henry Jiang         DATE :    Feb 13, 2021
*/

#include "api.h"

#include <stdio.h>
#include <stdlib.h>

#include "erosion.h"
#include "heightmap_gen.h"
#include "export.h"


int     map_size;
float*  heightmap = NULL;
struct setting noise_param;
erosion_setting_t erode_param;

void initialize(float* map, int sim_size) {
  // erosion: initalize heightmap
  if (!map)
    map = (float*) calloc(sim_size * sim_size, sizeof(float));
}

void set_parameters(/* heightmap gen params */
                    unsigned int seed, 
                    int octaves, float persistence, 
                    float scale, float map_height,
                    /* erosion params */
                    int droplet_life, float inertia,
                    float sediment_capacity, float min_sediment_capacity,
                    float deposit_speed, float erode_speed,
                    float evaporate_speed, float gravity,
                    int radius) {

  free_weights_matrix(); /* assumes that radius param is different */
                         /* and rebuilds the weights matrix for each set_param */

  // configure heightmap generator noise settings
  struct setting generator_param = {
    .seed         = seed,
    .octaves      = octaves,
    .persistence  = persistence,
    .scale        = scale,
    .height       = map_height
  };
  noise_param = generator_param;

  // configure erosion
  erosion_setting_t erosion_param = {
    .DROPLET_LIFETIME = droplet_life,
    .INERTA = inertia,
    .SEDIMENT_CAPACITY_FACTOR = sediment_capacity,
    .MIN_SEDIMENT_CAPACITY = min_sediment_capacity,
    .DEPOSIT_SPEED = deposit_speed,
    .EVAPORATE_SPEED = evaporate_speed,
    .GRAVITY = gravity,
    .drop_radius = radius
  };
  erode_param = erosion_param;

  // recomputes the weights matrix for erosion
  compute_weights_matrix(radius);
}


void override_heightmap(float* new_heightmap) {
  if (heightmap) {
    free(heightmap);
  }
  heightmap = new_heightmap;
}


void erode_iter(int iterations) {
  srand(noise_param.seed); // sets seed
  
  for (int i = 0; i < iterations; i++) {
    // randomize droplet's position
    struct droplet drop = {
      .pos_x = rand() % map_size,
      .pos_y = rand() % map_size,
      .dir_x = 0,
      .dir_y = 0,
      .speed = 1,
      .water = 1,
      .sediment = 0
    };

    // calculates the effect of drop on heightmap
    erode(heightmap, map_size, &drop);
  }
}


void save_obj(char* filename, int size) {
  export_obj(heightmap, map_size, size, filename);
}

void save_png(char* filename) {
  export_png(heightmap, map_size, filename);
}

void save_stl(char* filename) {
  export_stl(heightmap, map_size, filename);
}