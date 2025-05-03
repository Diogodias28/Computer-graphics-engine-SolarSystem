#ifndef LIGHT
#define LIGHT

#include <stdlib.h>
#include <vector>
#include "Point.hpp"


using namespace std;

typedef struct light* Light;

Light makeLight(char, std::vector<Point>, std::vector<float>, float);

Light createLight();

char getType(Light);

std::vector<Point> getPositions(Light);

std::vector<float> getDirections(Light);

float getCutoof(Light);

#endif
