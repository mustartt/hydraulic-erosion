/***********************************************************************
* FILENAME :        erosion.h   erosion.c
*
* DESCRIPTION :
*       Implements iterative hydraulic erosion on height map 
*
* PUBLIC FUNCTIONS :
*       void    erode( float* height_map, int map_size, struct droplet* drop )
*
* NOTES :
*       Implements basic algorithm from Hans Theobald Beyer
*       https://www.firespark.de/resources/downloads/implementation
*       %20of%20a%20methode%20for%20hydraulic%20erosion.pdf
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*H*/

#ifndef EROSION_H_
#define EROSION_H_

struct droplet {
    float posX, posY;
    float dirX, dirY;
    float velocity;
    float water;
    float volume;
};

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


#endif