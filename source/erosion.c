
/***********************************************************************
* FILENAME :        erosion.h   erosion.c
*
* DESCRIPTION :
*       Implements iterative hydraulic erosion on height height_map 
*
* PUBLIC FUNCTIONS :
*       void    erode( float* height_height_map, int height_map_size, int iters, int seed )
*
* PRIVATE FUNCTIONS :
*
*
* NOTES :
*       Implements basic algorithm from Hans Theobald Beyer
*       https://www.firespark.de/resources/downloads/implementation
*       %20of%20a%20methode%20for%20hydraulic%20erosion.pdf
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*/

#include "erosion.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <math.h>



/** 
 * @struct interp_result
 * @brief  stores the result of bilinear interpolation on the heightheight_map
 * 
 * @var    interp_result::height
 *         height is the height of the interpolated result
 * @var    interp_result::gradient_x
 *         gradient_x is the gradient of the heightheight_map in the x direction
 * @var    interp_result::gradient_y
 *         gradient_y is the gradient of the heightheight_map in the y direction
 */
struct interp_result {
    float height;
    float gradient_x;
    float gradient_y;
};

/**
 * @brief Interpolates the height_map at coordinate pos_x pos_y 
 * 
 * Uses bilinear interpolation to sample the height and gradient in both the 
 * x and y direction, and stores the result in the result struct
 * 
 * @param height_map  heightheight_map
 * @param map_size    size of the heightheight_map
 * @param pos_x       x coordinate to sample
 * @param pos_y       y coordinate to sample
 * @param[out] result struct of type interp_result where the information is 
 *                    stored
 */
void interpolate( float* height_map, int map_size, float pos_x, float pos_y, 
                  struct interp_result* result ) {
    int coord_x = (int) pos_x;
    int coord_y = (int) pos_y;

    // Calculate droplet's offset inside the cell (0,0) to (1,1) 
    float x = pos_x - coord_x;
    float y = pos_y - coord_y;

    // Calculate heights of the four nodes of the droplet's cell
    int height_map_index = coord_y * map_size + coord_x;
    float height_tl = height_map[height_map_index];
    float height_tr = height_map[height_map_index + 1];
    float height_bl = height_map[height_map_index + map_size];
    float height_sr = height_map[height_map_index + map_size + 1];

    // Calculate droplet's direction of flow with bilinear interpolation of height difference along the edges
    float gradient_x = (height_tr - height_tl)   * (1 - y) 
                       + (height_sr - height_bl) * y;
    float gradient_y = (height_bl - height_tl)   * (1 - x) 
                       + (height_sr - height_tr) * x;

    // Calculate height with bilinear interpolation of the heights of the nodes of the cell
    float height = height_tl * (1 - x) * (1 - y) 
                 + height_tr * (1 - y) * x 
                 + height_bl * (1 - x) * y 
                 + height_sr * x       * y;

    // Stores the interpolation result in interp_result
    result->height = height;
    result->gradient_x = gradient_x;
    result->gradient_y = gradient_y;
}


/* default settings */
int   DROPLET_LIFETIME          = 30;
float INERTA                    = .05f;
float SEDIMENT_CAPACITY_FACTOR  = 4;
float MIN_SEDIMENT_CAPACITY     = .01f;
float DEPOSIT_SPEED             = .3f;
float ERODE_SPEED               = .3f;
float EVAPORATE_SPEED           = .01f;
float GRAVITY                   = 4;

void write_settings(erosion_setting_t setting) {
  DROPLET_LIFETIME          = setting.DROPLET_LIFETIME;
  INERTA                    = setting.INERTA;
  SEDIMENT_CAPACITY_FACTOR  = setting.SEDIMENT_CAPACITY_FACTOR;
  MIN_SEDIMENT_CAPACITY     = setting.MIN_SEDIMENT_CAPACITY;
  DEPOSIT_SPEED             = setting.DEPOSIT_SPEED;
  ERODE_SPEED               = setting.ERODE_SPEED;
  EVAPORATE_SPEED           = setting.EVAPORATE_SPEED;
  GRAVITY                   = setting.GRAVITY;
}

/* interal states for the weights matrix */
float* weights;
int    weights_radius;
int    weights_size;

void compute_weights_matrix(int radius) {
  if (weights) {
    return;
  }
  else {
    weights_radius = radius;
    int size = radius * 2 + 1;
    weights_size = size;

    weights = (float*) calloc(size * size, sizeof(float));

    if (weights == NULL)
      return;

    int x_offset = radius;
    int y_offset = radius;
    float weight_sum = 0;

    for (int y = -radius; y <= radius; y++) {
      for (int x = -radius; x <= radius; x++) {
        int coord_x = x_offset + x;
        int coord_y = y_offset + y;
        float dist_sqr = x*x + y*y;
        
        if (dist_sqr < radius * radius) {
          // compute the weight
          float weight = 1 - sqrtf(dist_sqr) / radius;
          weights[coord_y * size + coord_x] = weight;
          weight_sum += weight;
        }
      } 
    }

    // normalize weights matrix
    for (int y = -radius; y <= radius; y++) {
      for (int x = -radius; x <= radius; x++) {
        int coord_x = x_offset + x;
        int coord_y = y_offset + y;
        // normalize
        weights[coord_y * size + coord_x] /= weight_sum;
      } 
    }
  }
}

/* frees the weights matrix */
void free_weights_matrix() {
  if (weights)
    free(weights);
  weights = NULL;
}


void erode( float* height_map, int map_size, struct droplet* drop ) {
  assert(height_map);
  assert(drop);

  for (int life = 0; life < DROPLET_LIFETIME; life++) {
    int node_x = (int) drop->pos_x;
    int node_y = (int) drop->pos_y;
    int drop_index = node_y * map_size + node_x;
    // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
    float cell_offset_x = drop->pos_x - node_x;
    float cell_offset_y = drop->pos_y - node_y;

    struct interp_result gradient = { 0 };
    interpolate(height_map, map_size, drop->pos_x, drop->pos_y, &gradient);

    drop->dir_x = (drop->dir_x * INERTA - gradient.gradient_x * (1 - INERTA));
    drop->dir_y = (drop->dir_y * INERTA - gradient.gradient_y * (1 - INERTA));
    
    // Normalize direction
    float len = sqrtf(drop->dir_x * drop->dir_x + drop->dir_y * drop->dir_y);
    if (len != 0) {
      drop->dir_x /= len;
      drop->dir_y /= len;
    }
    drop->pos_x += drop->dir_x;
    drop->pos_y += drop->dir_y;

    // Stop simulating droplet if it's not moving or has flowed over edge of map
    if ((drop->dir_x == 0 && drop->dir_y == 0) || 
        drop->pos_x < 0 || drop->pos_y >= map_size - 1 || 
        drop->pos_y < 0 || drop->pos_y >= map_size - 1) {
        break;
    }

    // Find the droplet's new height and calculate the deltaHeight
    struct interp_result new_result = { 0 };
    interpolate(height_map, map_size, drop->pos_x, drop->pos_y, &new_result);
    float new_height = new_result.height;
    float delta_height = new_height - gradient.height;

    // Calculate the droplet's sediment capacity (higher when moving fast down a slope and contains lots of water)
    float sediment_capacity = fmaxf(-delta_height 
                                   * drop->speed 
                                   * drop->water 
                                   * SEDIMENT_CAPACITY_FACTOR, 
                                  MIN_SEDIMENT_CAPACITY);

    // If carrying more sediment than capacity, or if flowing uphill:
    if (drop->sediment > sediment_capacity || delta_height > 0) {
      // If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
      float amount_deposit = (delta_height > 0) ? 
                              fminf(delta_height, drop->sediment) : 
                              (drop->sediment - sediment_capacity) * DEPOSIT_SPEED;
      // Update drop sediment amount
      drop->sediment -= amount_deposit;

      // Add the sediment to the four nodes of the current cell using bilinear interpolation
      // Deposition is not distributed over a radius (like erosion) so that it can fill small pits
      height_map[drop_index] += amount_deposit * (1 - cell_offset_x) * (1 - cell_offset_y);
      height_map[drop_index + 1] += amount_deposit * cell_offset_x * (1 - cell_offset_y);
      height_map[drop_index + map_size] += amount_deposit * (1 - cell_offset_x) * cell_offset_y;
      height_map[drop_index + map_size + 1] += amount_deposit * cell_offset_x * cell_offset_y;
    }
    else {
      // Erode a fraction of the droplet's current carry capacity.
      // Clamp the erosion to the change in height so that it doesn't 
      // dig a hole in the terrain behind the droplet
      float amount_to_erode = fminf((sediment_capacity - drop->sediment) * ERODE_SPEED,
                                    -delta_height);
    
      // Use erosion brush to erode from all nodes inside the droplet's erosion radius
      int offset = weights_radius;
      for (int y = -weights_radius; y <= weights_radius; y++) {
        for (int x = -weights_radius; x <= weights_radius; x++) {
          int weight_coord_x = offset + x;
          int weight_coord_y = offset + y;
          
          int map_coord_x = drop->pos_x + x;
          int map_coord_y = drop->pos_y + y;
          
          // check if coord is in heightmap
          if ((map_coord_x >= 0 && map_coord_x < map_size) && 
              (map_coord_y >= 0 && map_coord_y < map_size)) {
            
            float weight = weights[weight_coord_y * weights_size + weight_coord_x];
            float weighted_erode_amount = weight * amount_to_erode;
            int erode_index = map_coord_y * map_size + map_coord_x;
            float delta_sediment = (height_map[erode_index] < weighted_erode_amount) ? 
                                    height_map[erode_index] : weighted_erode_amount;

            height_map[erode_index] -= delta_sediment;
            drop->sediment += delta_sediment;
          }
        } 
      }
    }

    // Update droplet's speed and water content
    drop->speed =  sqrtf(drop->speed * drop->speed + delta_height * GRAVITY);
    drop->water *= (1 - EVAPORATE_SPEED);
  }
}

