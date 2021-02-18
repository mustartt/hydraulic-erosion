/***********************************************************************
* FILENAME :        erosion.h   erosion.c
*
* DESCRIPTION :
*       Implements iterative hydraulic erosion on height map 
*
* PUBLIC FUNCTIONS :
*       void    erode( float* height_map, int map_size, struct droplet* drop )
*       void    compute_weights_matrix( int radius )
*       void    free_weights_matrix( void )
*          
* NOTES :
*       Call create_brush before using erode with the same map_size
*       Implements basic algorithm from Hans Theobald Beyer
*       https://www.firespark.de/resources/downloads/implementation
*       %20of%20a%20methode%20for%20hydraulic%20erosion.pdf
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*H*/

#ifndef EROSION_H_
#define EROSION_H_

struct droplet {
    float pos_x, pos_y;
    float dir_x, dir_y;
    float speed;
    float water;
    float sediment;
};


typedef struct {
  int   DROPLET_LIFETIME;
  float INERTA;
  float SEDIMENT_CAPACITY_FACTOR;
  float MIN_SEDIMENT_CAPACITY;
  float DEPOSIT_SPEED;
  float ERODE_SPEED;
  float EVAPORATE_SPEED;
  float GRAVITY;
} erosion_setting_t;



/**
 * @brief Calculates the erosion of a single droplet on the height map
 * 
 * @param height_map array of floats representing the heightmap
 *                   have dimension of map_size = n x n
 *                   values ranges from [0, 1]
 * @param map_size   the size of the height_map buffer which is
 *                   the width of the heightmap
 * @param drop       pre-initalized structure representing the droplet
 * 
 * Modifies height_map in place.
 * height_map is a buffer of size map_size^2
 * Erosion parameters and inital parameters for the simulation is in the 
 * source file.
 */ 
void erode( float* height_map, int map_size, struct droplet* drop );


/**
 * @brief Computes the weight matrix for simulating the droplet size
 * 
 * Creates a weights matrix of radius @param radius with the 
 * all entires in the matrix normalized w_i = w_i / w_sum 
 * 
 * @param radius the radius of the weights matrix to be allocated
 * 
 * note: Computation time increase significantly with larger weighted matrix
 *       size. Recommended to use size between 1 - 3     
 *       assumes that caller calls @param free_weights_matrix
 */
void compute_weights_matrix( int radius );


/**
 * @brief Frees the weights matrix and set pts to NULL
 */
void free_weights_matrix( void );

/**
 * @brief Writes the erosion settings
 */
void write_settings( erosion_setting_t setting );

#endif