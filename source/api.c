/***********************************************************************
* FILENAME :        api.c   api.h
*
* DESCRIPTION :
*       The main API file for interacting with the erosion simulator.
*
* PUBLIC FUNCTIONS :
*       void initialize( float* map, int sim_size )
*       void set_parameters(
*                    unsigned int seed, 
*                    int octaves, float persistence, 
*                    float scale, float map_height,
*                    int droplet_life, float inertia,
*                    float sediment_capacity, float min_sediment_capacity,
*                    float deposit_speed, float erode_speed,
*                    float evaporate_speed, float gravity,
*                    int radius )
*       void override_heightmap( float* new_heightmap )
*       void erode_iter( int iterations )
*       void save_obj( char* filename, int size ) 
*       void save_png( char* filename )
*       void save_stl(char* filename)
*
* AUTHOR :    Henry Jiang         DATE :    Feb 13, 2021
*/

#include "api.h"

#include <stdio.h>
#include <stdlib.h>

#include "erosion.h"
#include "heightmap_gen.h"
#include "export.h"

#ifdef _WASM
#include "emscripten.h"
#endif

int     map_size;
float*  heightmap = NULL;
struct setting noise_param;
erosion_setting_t erode_param;


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void initialize(float* map, int sim_size) {
  // erosion: initalize heightmap
  if (map == NULL) {
    heightmap = (float*) calloc(sim_size * sim_size, sizeof(float));
  }

  map_size = sim_size;
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void use_default_erosion_params(unsigned int seed, 
                                int octaves, float persistence, 
                                float scale, float map_height,
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

  // recomputes the weights matrix for erosion
  compute_weights_matrix(radius);
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
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
  write_settings(erode_param);

  // recomputes the weights matrix for erosion
  compute_weights_matrix(radius);
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void generate_noise() {
  gen_heightmap(heightmap, map_size, &noise_param);
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void override_heightmap(float* new_heightmap) {
  if (heightmap) {
    free(heightmap);
  }
  heightmap = new_heightmap;
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void erode_iter(int iterations) {
  srand(noise_param.seed); // sets seed
  
  printf("Starting Erosion: %d Iterations.\n", iterations);

  int delta = iterations / 100;

  for (int i = 0; i < iterations; i++) {
    
    if (i % delta == 0) {
      int percent = (i * 100) / iterations;
      printf("Progress: %d/100.\n", percent);
    }

    // randomize droplet's position
    struct droplet drop = {
      .pos_x = (rand() % (map_size - 2)) + 1,
      .pos_y = (rand() % (map_size - 2)) + 1,
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


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void teardown() {
  free_weights_matrix();
  free(heightmap);
  heightmap = NULL;
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void save_obj(char* filename, int size) {
  export_obj(heightmap, map_size, size, filename);
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void save_png(char* filename) {
  export_png(heightmap, map_size, filename);
}



#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void save_stl(char* filename) {
  export_stl(heightmap, map_size, filename);
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
int version() {
  return 1234;
}
