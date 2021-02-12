/***********************************************************************
* FILENAME :        heightmap_gen.h   heightmap_gen.c
*
* DESCRIPTION :
*       Implements the main noise and pre erosion heightmap generation
*       from simplex noise
*
* PUBLIC FUNCTIONS :
*       void gen_heightmap(float* height_map, int map_size, setting_t setting);
*       
*
* PRIVATE FUNCTIONS :
*
* NOTES :
*       Generates heightmap based on seed and number of octaves and 
*       persistence with modifier scale and height
*       note: makesure height_map is zero'd before calling
*
* AUTHOR :    Henry Jiang         DATE :    Feb 11, 2021
*/

struct setting {
    unsigned int seed;
    int octaves;
    float persistence;
    float scale;
    float height;
}; 
/**
 * @brief The settings for the height map generator
 * 
 * Example Settings struct:
 */
/*
struct setting example = {
    .seed = 12345,
    .octaves = 6,
    .persistence = 0.65f,
    .height = 1,
    .scale = 1
};
*/

typedef struct setting* setting_t;

/**
 * @brief Generates height map with simplex noise 
 * 
 * @param height_map    the height map to be filled
 * @param map_size      the map size
 * @param setting       settings struct with settings for the 
 *                      simplex noise generator
 */
void gen_heightmap( float* height_map, int map_size, setting_t setting );