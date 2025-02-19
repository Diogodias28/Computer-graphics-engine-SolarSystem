#include <string>
#include <string.h> 
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

string fileName;

void genPlane(int length, int divisions) {

}

void genCone(int radius, int height, int slices, int stacks) {
    
}

void genSphere(int radius, int slices, int stacks) {
    
}

void genBox(int length, int grid) {
    
}

int main(int argc, char *argv[]) {

    if (argc >= 5) { // no minimo tem sempre 5 argumentos

        // plane
        if(strcmp(argv[1], "plane") == 0) {
            int length = atoi(argv[2]);
            int divisions = atoi(argv[3]);
            fileName = argv[4];
            genPlane(length, divisions);
        }

        // cone
        else if(strcmp(argv[1], "cone") == 0) {
            int radius = atoi(argv[2]);
            int height = atoi(argv[3]);
            int slices = atoi(argv[4]);
            int stacks = atoi(argv[5]);
            fileName = argv[6];
            genCone(radius, height, slices, stacks);
        }

        // sphere
        else if(strcmp(argv[1], "sphere") == 0) {
            int radius = atoi(argv[2]);
            int slices = atoi(argv[3]);
            int stacks = atoi(argv[4]);
            fileName = argv[5];
            genSphere(radius, slices, stacks);
        }

        // box
        else if(strcmp(argv[1], "box") == 0) {
            int length = atoi(argv[2]);
            int grid = atoi(argv[3]);
            fileName = argv[4];
            genBox(length, grid);
        }

        else {
            printf("Figura inválida.");
            return 1;
        }
    }
    
    else {
        printf("Número de argumentos inválido.");
    }

    return 0;
}