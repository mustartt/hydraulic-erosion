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
*       void use_default_erosion_params( unsigned int seed, 
                                         int octaves, float persistence, 
                                         float scale, float map_height,
                                         int radius )
*       void override_heightmap( float* new_heightmap )
*       void erode_iter( int iterations )
*       void save_obj( char* filename, int size ) 
*       void save_png( char* filename )
*       void save_stl(char* filename)
*
* AUTHOR :    Henry Jiang         DATE :    Feb 13, 2021
*/

/**
 * @brief Initializes the internal states for the simulator
 * 
 * @param map       passing a pre-existing heightmap. calloc if map is NULL
 * @param sim_size  the size of the heightmap
 */
float* initialize( float* map, int sim_size );

/**
 * @brief Initalizes the simulation parameters
 */ 
void set_parameters( unsigned int seed, 
                     int octaves, float persistence, 
                     float scale, float map_height,
                     int droplet_life, float inertia,
                     float sediment_capacity, float min_sediment_capacity,
                     float deposit_speed, float erode_speed,
                     float evaporate_speed, float gravity,
                     int radius );

/**
 * @brief Only initializes the noise generator settings and compute
 *        and computes the erosion weights matrix
 */
void use_default_erosion_params(unsigned int seed, 
                                int octaves, float persistence, 
                                float scale, float map_height,
                                int radius);

/**
 * @brief Generates noise onto the heightmap 
 */
void generate_noise( void );

/**
 * @brief Samples the height map at @param x and @param y
 */
float sample(int x, int y);

/**
 * @brief overrides the original heightmap and frees the old heightmap if alloced
 */
void override_heightmap( float* new_heightmap );

/**
 * @brief Performs n @param iterations on the heightmap 
 */
void erode_iter( int iterations );

/**
 * @brief Export the obj file 
 */
void save_obj( char* filename, int size );

/**
 * @brief Export the png file 
 */
void save_png( char* filename );

/**
 * @brief Export the stl file 
 */
void save_stl( char* filename );

/**
 * @brief frees allocated memory
 */
void teardown( void );