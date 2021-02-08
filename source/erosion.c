
/***********************************************************************
* FILENAME :        erosion.h   erosion.c
*
* DESCRIPTION :
*       Implements iterative hydraulic erosion on height map 
*
* PUBLIC FUNCTIONS :
*       void    erode( float* height_map, int map_size, int iters, int seed )
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


#define GRAVITY             4
#define INITIAL_SPEED       1
#define INITIAL_VOLUME      1

#define DROPLET_LIFETIME    30



/** 
 * @struct interp_result
 * @brief  stores the result of bilinear interpolation on the heightmap
 * 
 * @var    interp_result::height
 *         height is the height of the interpolated result
 * @var    interp_result::gradient_x
 *         gradient_x is the gradient of the heightmap in the x direction
 * @var    interp_result::gradient_y
 *         gradient_y is the gradient of the heightmap in the y direction
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
 * @param height_map  heightmap
 * @param map_size    size of the heightmap
 * @param pos_x       x coordinate to sample
 * @param pos_y       y coordinate to sample
 * @param[out] result struct of type interp_result where the information is 
 *                    stored
 */
void interpolate( float height_map[], int map_size, float pos_x, float pos_y, 
                  struct interp_result* result ) {
    int coord_x = (int) pos_x;
    int coord_y = (int) pos_y;

    // Calculate droplet's offset inside the cell (0,0) to (1,1) 
    float x = pos_x - coord_x;
    float y = pos_y - coord_y;

    // Calculate heights of the four nodes of the droplet's cell
    int map_index = coord_y * map_size + coord_x;
    float height_tl = height_map[map_index];
    float height_tr = height_map[map_index + 1];
    float height_bl = height_map[map_index + map_size];
    float height_sr = height_map[map_index + map_size + 1];

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
        // calculate path 
        // calculate gradient
        // calculate sediment pickup / erosion
        // calculate sediment deposition
    }
}



// TESTING
int main() {
    printf("Hello world!\n");
}