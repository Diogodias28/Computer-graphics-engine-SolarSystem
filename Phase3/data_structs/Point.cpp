#include "Point.hpp"

Point createPoint() {
    return makePoint(0.0f, 0.0f, 0.0f);
}

Point makePoint(float x, float y, float z){
    Point p = (Point) malloc (sizeof(struct point));

    if (p != NULL) {
        p->x = x;
        p->y = y;
        p->z = z;
    }

    return p;
}

float getX(Point p){
    return p->x;
}

float getY(Point p){
    return p->y;
}

float getZ(Point p){
    return p->z;
}

Point crossProduct(Point a, Point b) {
    float x = getY(a) * getZ(b) - getZ(a) * getY(b);
    float y = getZ(a) * getX(b) - getX(a) * getZ(b);
    float z = getX(a) * getY(b) - getY(a) * getX(b);

    return makePoint(x, y, z);
}