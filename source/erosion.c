
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

#define DROPLET_LIFETIME    30
#define INITAL_SPEED        1
#define INITAL_WATERVOLUME  1
#define INERTA              .05f

#define SEDIMENT_CAPACITY_FACTOR  4
#define MIN_SEDIMENT_CAPACITY     .01f
#define DEPOSIT_SPEED             .3f
#define ERODE_SPEED               .3f
#define EVAPORATE_SPEED           .01f
#define GRAVITY                   4


void erode( float* height_map, int map_size, struct droplet* drop ) {
  assert(height_map);
  assert(drop);

  for (int life = 0; life < DROPLET_LIFETIME; life++) {
    int nodeX = (int) drop->pos_x;
    int nodeY = (int) drop->pos_y;
    int dropletIndex = nodeY * map_size + nodeX;
    // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
    float cellOffsetX = drop->pos_x - nodeX;
    float cellOffsetY = drop->pos_y - nodeY;

    struct interp_result heightAndGradient = { 0 };
    interpolate(height_map, map_size, drop->pos_x, drop->pos_y, &heightAndGradient);

    drop->dir_x = (drop->dir_x * INERTA - heightAndGradient.gradient_x * (1 - INERTA));
    drop->dir_y = (drop->dir_y * INERTA - heightAndGradient.gradient_y * (1 - INERTA));
    
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
    float newHeight = new_result.height;
    float deltaHeight = newHeight - heightAndGradient.height;

    // Calculate the droplet's sediment capacity (higher when moving fast down a slope and contains lots of water)
    float sedimentCapacity = fmaxf(-deltaHeight 
                                   * drop->speed 
                                   * drop->water 
                                   * SEDIMENT_CAPACITY_FACTOR, 
                                  MIN_SEDIMENT_CAPACITY);

    // If carrying more sediment than capacity, or if flowing uphill:
    if (drop->sediment > sedimentCapacity || deltaHeight > 0) {
      // If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
      float amountToDeposit = (deltaHeight > 0) ? 
                              fminf(deltaHeight, drop->sediment) : 
                              (drop->sediment - sedimentCapacity) * DEPOSIT_SPEED;
      // Update drop sediment amount
      drop->sediment -= amountToDeposit;

      // Add the sediment to the four nodes of the current cell using bilinear interpolation
      // Deposition is not distributed over a radius (like erosion) so that it can fill small pits
      height_map[dropletIndex] += amountToDeposit * (1 - cellOffsetX) * (1 - cellOffsetY);
      height_map[dropletIndex + 1] += amountToDeposit * cellOffsetX * (1 - cellOffsetY);
      height_map[dropletIndex + map_size] += amountToDeposit * (1 - cellOffsetX) * cellOffsetY;
      height_map[dropletIndex + map_size + 1] += amountToDeposit * cellOffsetX * cellOffsetY;
    }
    else {
      // Erode a fraction of the droplet's current carry capacity.
      // Clamp the erosion to the change in height so that it doesn't dig a hole in the terrain behind the droplet
      float amountToErode = fminf((sedimentCapacity - drop->sediment) * ERODE_SPEED, -deltaHeight);
    
      // Use erosion brush to erode from all nodes inside the droplet's erosion radius
      // DEBUG: 
      height_map[dropletIndex] += amountToErode;
    
    }

    // Update droplet's speed and water content
    drop->speed =  sqrtf(drop->speed * drop->speed + deltaHeight * GRAVITY);
    drop->water *= (1 - EVAPORATE_SPEED);
  }
}

