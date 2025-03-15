#ifndef POINT
#define POINT
#include <stdlib.h>
#include <math.h>
#include <vector>

typedef struct point* Point;

Point createPoint();

Point makePoint(float, float, float);

float getX(Point);

float getY(Point);

float getZ(Point);

#endif // POINT