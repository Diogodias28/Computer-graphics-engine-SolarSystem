#ifndef LIGHT
#define LIGHT

#include <stdlib.h>
#include <vector>
#include "Point.hpp"

struct Vector{
    float x;
    float y;
    float z;
};

using namespace std;

typedef struct light* Light;

Light makeLight(char, std::vector<Point>, float, float);

Light createLight();

char getType(Light);

std::vector<Point> getPositions(Light);

Vector getDirections(Light);

float getCutoof(Light);

#endif
