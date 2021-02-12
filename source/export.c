/***********************************************************************
* FILENAME :        export.h   export.c
*
* DESCRIPTION :
*       Utility Functions for exporting heightmap as .obj files
*       or png heightmaps
*
* PUBLIC FUNCTIONS :
*       
*
* NOTES :
*       This export utils function export file to the virtual file system
*       under the Webassembly VFS, which has to be streamed into a url link
*       the js module: example code snippet
*       https://motley-coder.com/2019/04/01/download-files-emscripten/
*
* AUTHOR :    Henry Jiang         DATE :    Feb 06, 2021
*H*/

#include "export.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void export_obj(float* heightmap, int map_size, char* filename) {

}



void export_png(float* heightmap, int map_size, char* filename) {

}
