/***********************************************************************
* FILENAME :        erosion.h   erosion.c
*
* DESCRIPTION :
*       Implements iterative hydraulic erosion on height map 
*
* PUBLIC FUNCTIONS :
*       void    erode( float* height_map, int map_size, int iters, int seed )
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

#include <stdbool.h>


struct droplet {
    float posX, posY;
    float dirX, dirY;
    float velocity;
    float water;
    float volume;
};


void erode(float* height_map, int map_size, int iters, int seed);


#endif