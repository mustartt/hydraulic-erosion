#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "erosion.h"
#include "export.h"
#include "heightmap_gen.h"

int main(int argc, char** argv) {
  int size, iter;
  if (argc > 1) {
    size = 512;
    iter = size * size;
  } else {
    printf("Enter size: \n");
    scanf("%d", &size);
    iter = size * size;
    printf("Enter iterations (recommends %d): \n", iter);
    scanf("%d", &iter);
  }

  float* map = malloc(size * size * sizeof(float));

  struct setting example = {
    .seed = 12345,
    .octaves = 6,
    .persistence = 0.65f,
    .height = 0.75,
    .scale = 0.5
  };

  compute_weights_matrix(2);

  gen_heightmap(map, size, &example);
  export_png(map, size, "output_0.png");

  for (int i = 0; i < iter; i++) {
    struct droplet drop = {
      .pos_x = rand() % size,
      .pos_y = rand() % size,
      .dir_x = 0,
      .dir_y = 0,
      .speed = 1,
      .water = 1,
      .sediment = 0
    };

    erode(map, size, &drop);
  }

  export_png(map, size, "output_1.png");
  export_obj(map, size, 256, "output_1.obj");

  free_weights_matrix();
  free(map);
  return 0;
}