
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


#define GRAVITY                     4
#define INITIAL_drop->velocity      1
#define INITIAL_VOLUME              1
#define INERTIA                     0.05f
#define sedimentCapacityFactor      4.0f
#define minSedimentCapacity         0.01f
#define depositSpeed                0.3f
#define erodeSpeed                  0.3f
#define evaporateSpeed              0.01f

#define DROPLET_LIFETIME            30


int* brushIndices;
float* brushWeights;
int brushLength;


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
 * @brief Interpolates the height_height_map at coordinate pos_x pos_y 
 * 
 * Uses bilinear interpolation to sample the height and gradient in both the 
 * x and y direction, and stores the result in the result struct
 * 
 * @param height_height_map  heightheight_map
 * @param height_map_size    size of the heightheight_map
 * @param pos_x       x coordinate to sample
 * @param pos_y       y coordinate to sample
 * @param[out] result struct of type interp_result where the information is 
 *                    stored
 */
void interpolate( float height_height_map[], int height_map_size, float pos_x, float pos_y, 
                  struct interp_result* result ) {
    int coord_x = (int) pos_x;
    int coord_y = (int) pos_y;

    // Calculate droplet's offset inside the cell (0,0) to (1,1) 
    float x = pos_x - coord_x;
    float y = pos_y - coord_y;

    // Calculate heights of the four nodes of the droplet's cell
    int height_map_index = coord_y * height_map_size + coord_x;
    float height_tl = height_height_map[height_map_index];
    float height_tr = height_height_map[height_map_index + 1];
    float height_bl = height_height_map[height_map_index + height_map_size];
    float height_sr = height_height_map[height_map_index + height_map_size + 1];

    // Calculate droplet's direction of flow with bilinear interpolation of height difference along the edges
    float gradient_x = (height_tr - height_tl)   * (1 - y) 
                       + (height_sr - height_bl) * y;
    float gradient_y = (height_bl - height_tl)    * (1 - x) 
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



void erode( float* height_map, int map_size, struct droplet* drop ) {
    assert(height_map);
    assert(drop);

    for ( int life = 0; life < DROPLET_LIFETIME; life++) {
        int nodeX = (int) drop->pos_x;
        int nodeY = (int) drop->pos_y;
        int dropletIndex = nodeY * map_size + nodeX;
        // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
        float cellOffsetX = drop->pos_x - nodeX;
        float cellOffsetY = drop->pos_y - nodeY;

        // Calculate droplet's height and direction of flow with bilinear interpolation of surrounding heights
        struct interp_result heightAndGradient = { 0 };
        interpolate(height_map, map_size, drop->pos_x, drop->pos_y, &heightAndGradient);
        
        // Update the droplet's direction and position (move position 1 unit regardless of speed)
        drop->dir_x = (drop->dir_x * INERTIA - heightAndGradient.gradient_x * (1 - INERTIA));
        drop->dir_y = (drop->dir_y * INERTIA - heightAndGradient.gradient_y * (1 - INERTIA));
        // Normalize direction
        float len = sqrtf(drop->dir_x * drop->dir_x + drop->dir_y * drop->dir_y);
        if (len != 0) {
            drop->dir_x /= len;
            drop->dir_y /= len;
        }
        // Change position based on dir and velocity
        drop->pos_x += drop->dir_x;
        drop->pos_y += drop->dir_y;

        // Stop simulating droplet if it's not moving or has flowed over edge of map
        if ((drop->dir_x == 0 && drop->dir_y == 0) 
            || drop->pos_x < 0 || drop->pos_x >= map_size - 1 
            || drop->pos_y < 0 || drop->pos_y >= map_size - 1) {
            break;
        }

        // Find the droplet's new height and calculate the deltaHeight
        struct interp_result new_result = { 0 };
        interpolate(height_map, map_size, drop->pos_x, drop->pos_y, &new_result);
        float newHeight = new_result.height;
        float deltaHeight = newHeight - heightAndGradient.height;

        float sedimentCapacity = fmaxf(-deltaHeight * drop->velocity * drop->water * sedimentCapacityFactor, minSedimentCapacity);
        // If carrying more sediment than capacity, or if flowing uphill:
        if (drop->sediment > sedimentCapacity || deltaHeight > 0) {
            // If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
            float amountToDeposit = (deltaHeight > 0) ? fminf(deltaHeight, drop->sediment) 
                                                      : (drop->sediment - sedimentCapacity) * depositSpeed;
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
            float amountToErode = fminf((sedimentCapacity - drop->sediment) * erodeSpeed, -deltaHeight);

            // Use erosion brush to erode from all nodes inside the droplet's erosion radius
            for (int i = 0; i < brushLength; i ++) {
                int erodeIndex = dropletIndex + brushIndices[i];

                float weightedErodeAmount = amountToErode * brushWeights[i];
                float deltaSediment = (height_map[erodeIndex] < weightedErodeAmount) ? height_map[erodeIndex] : weightedErodeAmount;
                height_map[erodeIndex] -= deltaSediment;
                drop->sediment += deltaSediment;
            }
        }

        // Update droplet's speed and water content
        drop->velocity = sqrtf(fmaxf(0,drop->velocity * drop->velocity + deltaHeight * GRAVITY));
        drop->water *= (1 - evaporateSpeed);
    }
}





// TESTING
int main() {
    printf("Hello world!\n");
}