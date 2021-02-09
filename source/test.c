#include <stdio.h>
#include <stdlib.h>

#include "erosion.h"

float* read_map(int size) {
    FILE* file = fopen("input.txt", "r");

    float* height_map = (float*) malloc(size * size * sizeof(float));
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int val;
            fscanf(file, "%d", &val);
            height_map[x * size + y] = (float) val / 255.0f;
        }
    }

    fclose(file);
    return height_map;
}

void write_map(float* height_map, int size) {
    FILE* file = fopen("output.txt", "w");

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int val = (int) (height_map[x * size + y] * 255);
            fprintf(file, "%d ", val);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


void process_test() {
    create_brush(128, 3);

    float* height_map = read_map(128);

    // do stuff
    for (int i = 0; i < 1000000; i++) {
        struct droplet drop = {
            .pos_x = rand() % 64,
            .pos_y = rand() % 64,
            .dir_x = 0,
            .dir_y = 0,
            .velocity = 1,
            .water = 1,
            .sediment = 0
        };

        erode(height_map, 64, &drop);
    }
    
    // write output
    write_map(height_map, 128);


    // free stuff
    free_brush();
    free(height_map);
}


int main( void ) {
    process_test();
}