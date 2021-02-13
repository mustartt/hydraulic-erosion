#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "erosion.h"
#include "export.h"
#include "heightmap_gen.h"

int main() {
  struct droplet drop = {
    .pos_x = 32,
    .pos_y = 32,
    .dir_x = 0,
    .dir_y = 0,
    .speed = 1,
    .water = 1,
    .sediment = 0
  };

  int size = 128;
  float* map = malloc(size * size * sizeof(float));

  

}