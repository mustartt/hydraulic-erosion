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
*                    float evaporate_speed, float gravity )
*       void use_default_erosion_params( unsigned int seed, 
                                         int octaves, float persistence, 
                                         float scale, float map_height )
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
 * @param sim_size  the size of the heightmap
 */
void initialize( int sim_size );

/**
 * @brief Returns the current heightmap
 */
float* get_heightmap( void );

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
                                float scale, float map_height );

/**
 * @brief Generates noise onto the heightmap 
 */
void generate_noise( void );

/**
 * @brief Samples the height map at @param x and @param y
 */
float sample(int x, int y);

/**
 * @brief Performs n @param iterations on the heightmap 
 */
void erode_iter( int iterations, int radius );

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
 * @brief Frees the allocated heightmap
 */
void free_heightmap( void );
