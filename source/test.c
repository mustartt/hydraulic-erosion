#include <stdio.h>
#include <stdlib.h>

#include "api.h"

int main(int argc, char** argv) {

  initialize(512);
  set_parameters(1, 4, 0.45, 1, 1, 30, 0.05, 4, 0.01, 0.3, 0.3, 0.01, 4);
  generate_noise();
  save_png("output.png");
  erode_iter(512 * 512 * 3, 3);
  save_png("output1.png");

  return 0;
}
