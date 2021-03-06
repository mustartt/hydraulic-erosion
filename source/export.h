/***********************************************************************
* FILENAME :        export.h   export.c
*
* DESCRIPTION :
*       Utility Functions for exporting heightmap as .obj files
*       or png heightmaps
*
* PUBLIC FUNCTIONS :
*       void export_obj( float* heightmap, int map_size, char* filename )
*       void export_png( float* heightmap, int map_size, char* filename )
*
* NOTES :
*       This export utils function export file to the virtual file system
*       under the Webassembly VFS, which has to be streamed into a url link
*       the js module: example code snippet
*       https://motley-coder.com/2019/04/01/download-files-emscripten/
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*H*/

#ifndef EXPORT_H_
#define EXPORT_H_

/**
 * @brief Export heightmap as an .obj file with filename
 * 
 * Exports the @param heightmap with @param map_size with filename
 * @param filename. The exported file in the Virtual file system is 
 * the .obj format with only verticies and faces exported [does not include
 * the noramls or UVs]. The exported mesh has dimension [1, 1, 1].
 * The obj file can be exported in lower quality to reduce file size 
 * controlled by @param export_size. and @param export_size must be 
 * less and an interger multiple of @param map_size.
 * 
 * @param heightmap   the heightmap to export
 * @param map_size    the heightmap size
 * @param export_size the desired export size
 * @param filename    the filename [include extension]
 */
void export_obj( float* heightmap, int map_size, int export_size, char* filename );


/**
 * @brief Export heightmap as an .png file with filename
 * 
 * Exports the @param heightmap with @param map_size with filename
 * @param filename. The exported file in the Virtual file system is 
 * in the .png image format [RGB format]
 * 
 * @param heightmap the heightmap to export
 * @param map_size  the heightmap size
 * @param filename  the filename [include extension]
 */
void export_png( float* heightmap, int map_size, char* filename );


/**
 * @brief Export heightmap as an .stl file with filename
 * 
 * Exports the @param heightmap with @param map_size with filename
 * @param filename. The exported file in the Virtual file system is 
 * in the .stl binary fileformat
 * note: see https://en.wikipedia.org/wiki/STL_(file_format)
 * 
 * @param heightmap the heightmap to export
 * @param map_size  the heightmap size
 * @param filename  the filename [include extension]
 */
void export_stl( float* heightmap, int map_size, char* filename );


// DEBUGGING FUNCTIONS
float* read_map( char* filename, int size );
void   write_map( float* height_map, int size, char* filename );

#endif