#include <vector>
#include <fstream>
#include <string>
#include <string.h> 
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../data_structs/Point.hpp"
#include "../data_structs/Model.hpp"

using namespace std;

string fileName;

/*
void genPlane(int length, int divisions) { //está com o Point anterior, mudar para que fique com o point e o model (aka figura) de agora.
                                            //NÃO FAZER OS PATCHES
    std::vector<std::vector<int>> patches; // To store the patches' indices
    std::vector<Point> controlPoints; // To store the control points

    float step = (float)length / divisions;
    float halfLength = length / 2.0f;

    // Generate control points (divisions + 1) x (divisions + 1)
    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            float x = -halfLength + i * step;
            float z = -halfLength + j * step;
            controlPoints.push_back(Point(x, 0.0f, z));
        }
    }

    // Create patches (each patch is a 4x4 grid of points)
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            int idx = i * (divisions + 1) + j; // The top-left corner of the patch

            // Adding the 16 indices for this patch
            patches.push_back({
                idx, idx + 1, idx + 2, idx + 3,
                idx + (divisions + 1), idx + (divisions + 1) + 1, idx + (divisions + 1) + 2, idx + (divisions + 1) + 3,
                idx + 2 * (divisions + 1), idx + 2 * (divisions + 1) + 1, idx + 2 * (divisions + 1) + 2, idx + 2 * (divisions + 1) + 3,
                idx + 3 * (divisions + 1), idx + 3 * (divisions + 1) + 1, idx + 3 * (divisions + 1) + 2, idx + 3 * (divisions + 1) + 3
            });
        }
    }

    // Write the patch data to file (indices and control points)
    writeToFile(controlPoints, fileName);
}
*/

void genBox(int length, int grid, std::string fileName) {

    std::vector<Point> controlPoints;
    float part = (float)length / grid; 
    float half = length / 2.0f;

    for (int i = 0; i < grid; i++) {
        for (int j = 0; j < grid; j++) {
            float x = -half + i * part;
            float y = -half + j * part;

            // Front Face
            controlPoints.push_back(makePoint(x, y, half));
            controlPoints.push_back(makePoint(x + part, y, half));
            controlPoints.push_back(makePoint(x, y + part, half));

            controlPoints.push_back(makePoint(x + part, y, half));
            controlPoints.push_back(makePoint(x + part, y + part, half));
            controlPoints.push_back(makePoint(x, y + part, half));

			// Back Face
			controlPoints.push_back(makePoint(x, y, -half));
			controlPoints.push_back(makePoint(x, y + part, -half));
			controlPoints.push_back(makePoint(x + part, y, -half));
	  
			controlPoints.push_back(makePoint(x, y + part, -half));
			controlPoints.push_back(makePoint(x + part, y + part, -half));
			controlPoints.push_back(makePoint(x + part, y, -half));
	  
			// Left Face
			controlPoints.push_back(makePoint(-half, x, y));
			controlPoints.push_back(makePoint(-half, x, y + part));
			controlPoints.push_back(makePoint(-half, x + part, y));
	  
			controlPoints.push_back(makePoint(-half, x, y + part));
			controlPoints.push_back(makePoint(-half, x + part, y + part));
			controlPoints.push_back(makePoint(-half, x + part, y));
	  
			// Right Face done
			controlPoints.push_back(makePoint(half, x + part, y + part));
			controlPoints.push_back(makePoint(half, x, y + part));
			controlPoints.push_back(makePoint(half, x, y));
	  
			controlPoints.push_back(makePoint(half, x + part, y + part));
			controlPoints.push_back(makePoint(half, x, y));
			controlPoints.push_back(makePoint(half, x + part, y));
	  
			// Top Face
			controlPoints.push_back(makePoint(x, half, y));
			controlPoints.push_back(makePoint(x, half, y + part));
			controlPoints.push_back(makePoint(x + part, half, y));
	  
			controlPoints.push_back(makePoint(x, half, y + part));
			controlPoints.push_back(makePoint(x + part, half, y + part));
			controlPoints.push_back(makePoint(x + part, half, y));
	  
			// Bottom Face
			controlPoints.push_back(makePoint(x, -half, y));
			controlPoints.push_back(makePoint(x + part, -half, y));
			controlPoints.push_back(makePoint(x, -half, y + part));
	  
			controlPoints.push_back(makePoint(x, -half, y + part));
			controlPoints.push_back(makePoint(x + part, -half, y));
			controlPoints.push_back(makePoint(x + part, -half, y + part));
			
        }
    }
    writeToFile(controlPoints, fileName);
}

void genSphere(float radius, int slices, int stacks, std::string fileName) {
    std::vector<Point> controlPoints;

    // divide o circulo de 360 graus em slices iguais
    float anglePerSlice = (float)(2 * M_PI) / slices;
    // divide o arco de 180 graus em stacks iguais
    float anglePerStack = (float)M_PI / stacks;

    float alpha, beta;
    Point p1, p2, p3, p4;

    for (int j = 0; j < stacks; j++) {
        // ângulo de latitude para a stack atual e seguinte
        beta = j * anglePerStack;
        float nextBeta = (j + 1) * anglePerStack;

        for (int i = 0; i < slices; i++) {
            // ângulo de atitude para o slice atual e seguinte
            alpha = i * anglePerSlice;
            float nextAlpha = (i + 1) * anglePerSlice;

            // pontos na stack atual
            p1 = makePoint(radius * sinf(beta) * sinf(alpha), radius * cosf(beta), radius * sinf(beta) * cosf(alpha));
            p2 = makePoint(radius * sinf(beta) * sinf(nextAlpha), radius * cosf(beta), radius * sinf(beta) * cosf(nextAlpha));

            // pontos na próxima stack
            p3 = makePoint(radius * sinf(nextBeta) * sinf(alpha), radius * cosf(nextBeta), radius * sinf(nextBeta) * cosf(alpha));
            p4 = makePoint(radius * sinf(nextBeta) * sinf(nextAlpha), radius * cosf(nextBeta), radius * sinf(nextBeta) * cosf(nextAlpha));

            // Vamos fazer os triângulos
            // triângulo de cima
            controlPoints.push_back(p1);
            controlPoints.push_back(p4);
            controlPoints.push_back(p2);

            // triângulo de baixo
            controlPoints.push_back(p1);
            controlPoints.push_back(p3);
            controlPoints.push_back(p4);
        }
    }

    writeToFile(controlPoints, fileName);
}

void genCone(int radius, int height, int slices, int stacks) {
    std::vector<Point> controlPoints;

    float anglePerSlice = (float) 2 * M_PI / slices;
    float stackHeight = height / stacks;

    float currRadius = radius;
    float prevRadius; 

    float alpha;
    float beta;

    float currHeight;
    float prevHeight;

    for (int j = 1; j <= stacks; j++) {

        currHeight = stackHeight * j;
        currRadius = currRadius - (radius / stacks);

        for (int i = 1; i <= slices; i++) {

            alpha = anglePerSlice * i;
            beta = alpha + anglePerSlice;

            // Stack 1

            if (j == 1) {

                // Base

                controlPoints.push_back(makePoint(0.0f,0.0f,0.0f));
                controlPoints.push_back(makePoint(radius * sinf(beta),0.0f,radius * cosf(beta)));
                controlPoints.push_back(makePoint(radius * sinf(alpha),0.0f,radius * cosf(alpha)));

                controlPoints.push_back(makePoint(radius * sinf(beta), 0.0f, radius * cosf(beta)));
                controlPoints.push_back(makePoint(currRadius * sinf(beta), currHeight, currRadius * cosf(beta)));
                controlPoints.push_back(makePoint(radius * sinf(alpha), 0.0f, radius * cosf(alpha)));

                controlPoints.push_back(makePoint(radius * sinf(alpha), 0.0f, radius * cosf(alpha)));
                controlPoints.push_back(makePoint(currRadius * sinf(beta), currHeight, currRadius * cosf(beta)));
                controlPoints.push_back(makePoint(currRadius * sinf(alpha), currHeight, currRadius * cosf(alpha)));

            }
            
            else if (j != stacks) {
                controlPoints.push_back(makePoint(prevRadius * sinf(beta), prevHeight, prevRadius * cosf(beta)));
                controlPoints.push_back(makePoint(currRadius * sinf(beta), currHeight, currRadius * cosf(beta)));
                controlPoints.push_back(makePoint(prevRadius * sinf(alpha), prevHeight, prevRadius * cosf(alpha)));

                controlPoints.push_back(makePoint(prevRadius * sinf(alpha), prevHeight, prevRadius * cosf(alpha)));
                controlPoints.push_back(makePoint(currRadius * sinf(beta), currHeight, currRadius * cosf(beta)));
                controlPoints.push_back(makePoint(currRadius * sinf(alpha), currHeight, currRadius * cosf(alpha)));

            }

            else {
                controlPoints.push_back(makePoint(prevRadius * sinf(beta), prevHeight, prevRadius * cosf(beta)));
                controlPoints.push_back(makePoint(0.0f, currHeight, 0.0f));
                controlPoints.push_back(makePoint(prevRadius * sinf(alpha), prevHeight, prevRadius * cosf(alpha)));
            }
        }

        prevHeight = currHeight;
        prevRadius = currRadius;
    }
    // Write to file
    writeToFile(controlPoints, fileName);
}


int main(int argc, char *argv[]) {

    if (argc >= 5) { // no minimo tem sempre 5 argumentos

        /*
        // plane
        if(strcmp(argv[1], "plane") == 0) {
            int length = atoi(argv[2]);
            int divisions = atoi(argv[3]);
            fileName = argv[4];
            genPlane(length, divisions);
        }
        */

        // cone
        if(strcmp(argv[1], "cone") == 0) {
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
            genSphere(radius, slices, stacks, fileName);
        }

        // box
        else if(strcmp(argv[1], "box") == 0) {
            int length = atoi(argv[2]);
            int grid = atoi(argv[3]);
            fileName = argv[4];
            genBox(length, grid, fileName);
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