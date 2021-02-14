#include <stdio.h>
#include <stdlib.h>

#include "api.h"

int main(int argc, char** argv) {

  initialize(NULL, 512);
  use_default_erosion_params(12345, 5, 0.45, 3, 1, 2);
  generate_noise();

  save_png("output_0.png");

  erode_iter(512 * 512 * 5);

  save_png("output_1.png");
  save_obj("output_1.obj", 512);

  teardown();

  return 0;
}
