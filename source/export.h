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

/**
 * @brief Export heightmap as an .obj file with filename
 * 
 * Exports the @param heightmap with @param map_size with filename
 * @param filename. The exported file in the Virtual file system is 
 * the .obj format with only verticies and faces exported [does not include
 * the noramls or UVs]. The exported mesh has dimension [1, 1, 1].
 * 
 * @param heightmap the heightmap to export
 * @param map_size  the heightmap size
 * @param filename  the filename [include extension]
 */
void export_obj( float* heightmap, int map_size, int export_size, char* filename );


/**
 * @brief Export heightmap as an .png file with filename
 * 
 * Exports the @param heightmap with @param map_size with filename
 * @param filename. The exported file in the Virtual file system is 
 * in the .png image format [RGBA format]
 * 
 * @param heightmap the heightmap to export
 * @param map_size  the heightmap size
 * @param filename  the filename [include extension]
 */
void export_png( float* heightmap, int map_size, char* filename );