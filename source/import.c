/***********************************************************************
* FILENAME :        import.h   import.c
*
* DESCRIPTION :
*       Utility Functions for import PNG heightmaps using libpng
*
* PUBLIC FUNCTIONS :
*       float* import_heightmap( const char* filename, int* dimension )
*
* AUTHOR :    Henry Jiang         DATE :    Feb 13, 2021
*/

#include "import.h"

#include <png.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* produces fatal error messages and exits */
static void fatal_error(const char * message, ...) {
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

float* import_heightmap( const char* filename, int* dimension ) {
  png_structp png_ptr;
  png_infop info_ptr;
  FILE * fp;
  png_uint_32 width;
  png_uint_32 height;
  int bit_depth;
  int color_type;
  int interlace_method;
  int compression_method;
  int filter_method;

  /* reading PNG header metadata */
  fp = fopen(filename, "rb");
  if (!fp) {
    fatal_error("Cannot open '%s': %s\n", filename, strerror(errno));
  }
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fatal_error("Cannot create PNG read structure");
  }
  info_ptr = png_create_info_struct(png_ptr);
  if (!png_ptr) {
    fatal_error("Cannot create PNG info structure");
  }
  png_set_palette_to_rgb(png_ptr);
  png_init_io(png_ptr, fp);
  png_read_png(png_ptr, info_ptr, 0, 0);
  png_get_IHDR(png_ptr, info_ptr, & width, & height, & bit_depth, &
    color_type, & interlace_method, & compression_method, &
    filter_method);

  if (width != height) {
    printf("Image is not the same dimension.\n");
    exit(EXIT_FAILURE);
  }

  /* create heightmap */
  float* heightmap = (float*) malloc(width * height * sizeof(float));

  if (heightmap == NULL) {
    printf("Failed to create heightmap.\n");
    exit(EXIT_FAILURE);
  }

  /* read image data */
  png_bytepp rows = png_get_rows(png_ptr, info_ptr);
  int i, j;

  for (i = 0; i < height; i++) {
    for (j = 0; j < width * 3; j += 3) {
      int pixel_sum = rows[i][j] + rows[i][j + 1] + rows[i][j + 2];
      float val = (float) pixel_sum / (255 * 3) ;
      heightmap[i * width + j / 3] = val;
    }   
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(fp);

  *dimension = width;
  return heightmap;
}

