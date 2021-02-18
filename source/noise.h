/***********************************************************************
* FILENAME :        noise.h   noise.c
*
* DESCRIPTION :
*       Implements a version of OpenSimplex Noise
*
* PUBLIC FUNCTIONS :
*       double    noise( double xin, double yin ) 
*       void      init_perm( void )
*       int       random( void )
*       void      set_seed( unsigned int seed )
* PRIVATE FUNCTIONS :
*       int       fastfloor( double x )
*       double    dot_2( int g[], double x, double y )
*
* NOTES :
*       Implements basic simplex noise algorithm from 
*       http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
*
* AUTHOR :    Henry Jiang         DATE :    Feb 11, 2021
*/

#ifndef NOISE_H_
#define NOISE_H_

/** 
 * @brief initialize the perm permutation array before using simplex noise
 */
void init_perm( void );


/**
 * @brief get the Simplex Noise at position xin and yin
 * 
 * Implments 2D simplex noise algorithm with values ranging from [-1, 1].
 * call init_perm( void ) before using the algorithm and it has to be called
 * only once
 * 
 * @param xin input x coordinate
 * @param yin input y coordinate
 */
double noise( double xin, double yin );


/**
 * @brief Pseudo random function based on the undefined behavior of long overflow
 */
int defined_random( void );

/**
 * @brief Sets the random seed based on @param seed
 */
void set_random_seed( unsigned int seed );



#endif