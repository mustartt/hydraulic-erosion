#include <stdio.h>
#include <stdlib.h>

#include "api.h"

int main(int argc, char** argv) {

  initialize(512);
  use_default_erosion_params(1, 6, 0.5, 1, 1);
  generate_noise();
  save_png("output.png");

  erode_iter(512 * 512 * 5, 3);

  save_png("output1.png");

  return 0;
}
