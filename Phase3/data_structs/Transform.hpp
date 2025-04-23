#ifndef TRANSFORM
#define TRANSFORM
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "Point.hpp"


using namespace std;

typedef struct transform* Transform;

Transform newTransformation(char, Point, float);

Transform newTimeBasedTransformation (char, Point, float, float, bool, std::vector<Point> );

char getType(Transform);

Point getTransVal(Transform);

float getAngle(Transform);

float getTime(Transform);

bool getAlign(Transform);

std::vector<Point> getControlPoints (Transform);

#endif //TRANSFORM