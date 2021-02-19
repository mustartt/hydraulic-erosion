/***********************************************************************
* FILENAME :        api.c   api.h
*
* DESCRIPTION :
*       The main API file for interacting with the erosion simulator.
*
* PUBLIC FUNCTIONS :
*       void initialize( int sim_size )
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
struct erosion_param erode_param;


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void initialize(int sim_size) {
  heightmap = (float*) calloc(sim_size * sim_size, sizeof(float));
  map_size = sim_size;
}

#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
float* get_heightmap() {
  return heightmap;
}

#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
float sample(int x, int y) {
  if ((0 <= x && x < map_size) && (0 <= y && y < map_size)) {
    return heightmap[y * map_size + x];
  }
  else {
    return -1; /* Heap Safety Check */
  }
}



#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void use_default_erosion_params(unsigned int seed, 
                                int octaves, float persistence, 
                                float scale, float map_height) {
  // configure heightmap generator noise settings
  noise_param.seed = seed;
  noise_param.octaves = octaves;
  noise_param.persistence = persistence;
  noise_param.scale = scale;
  noise_param.height = map_height;
  
  // configure default erosion parameters
  erode_param.DROPLET_LIFETIME          = 30;
  erode_param.INERTA                    = .05f;
  erode_param.SEDIMENT_CAPACITY_FACTOR  = 4;
  erode_param.MIN_SEDIMENT_CAPACITY     = .01f;
  erode_param.DEPOSIT_SPEED             = .3f;
  erode_param.ERODE_SPEED               = .3f;
  erode_param.EVAPORATE_SPEED           = .01f;
  erode_param.GRAVITY                   = 4;
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
                    float evaporate_speed, float gravity ) {
  // configure heightmap generator noise settings
  noise_param.seed = seed;
  noise_param.octaves = octaves;
  noise_param.persistence = persistence;
  noise_param.scale = scale;
  noise_param.height = map_height;

  // configure erosion parameters
  erode_param.DROPLET_LIFETIME = droplet_life;
  erode_param.INERTA = inertia;
  erode_param.SEDIMENT_CAPACITY_FACTOR = sediment_capacity;
  erode_param.MIN_SEDIMENT_CAPACITY = min_sediment_capacity;
  erode_param.DEPOSIT_SPEED = deposit_speed;
  erode_param.ERODE_SPEED = erode_speed;
  erode_param.EVAPORATE_SPEED = evaporate_speed;
  erode_param.GRAVITY = gravity;
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
void free_heightmap() {
  free(heightmap);
  map_size = 0;
}


#ifdef _WASM
EMSCRIPTEN_KEEPALIVE
#endif
void erode_iter(int iterations, int radius) {
  printf("Starting with %d iterations with radius %d.\n", iterations, radius);
  // computes the weights matrix only before erosion
  compute_weights_matrix(radius);
  for (int i = 0; i < iterations; i++) {
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
    erode(heightmap, map_size, &drop, &erode_param);
  }

  // frees the weights matrix after erosion
  free_weights_matrix();
  printf("Finished.");
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
  return 1004;
}
