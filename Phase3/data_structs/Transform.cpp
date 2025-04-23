#include "Transform.hpp"

struct transform{
    char type; //T translation, S scale, R rotation
    Point transformation;
    float angle; 
    float time;
    bool align;
    std::vector<Point> controlPoints;
};

Transform newTransformation(char type, Point transformation, float angle){
    Transform t = (Transform)malloc(sizeof(struct transform));

    t->type = type;
    t->transformation = transformation;
    t->angle = angle;
    t->time = 0.0f;
    t->align = false;
    
    return t;
}

Transform newTimeBasedTransformation(char type, Point transformation, float time, float angle, bool align, std::vector<Point> controlPoints) {
    Transform t = (Transform) malloc (sizeof(struct transform));

    t->type = type;
    t->transformation = transformation;
    t->angle = angle;
    t->time = time;
    t->align = align;
    t->controlPoints = controlPoints;

    return t;
}

char getType(Transform t){
    return t->type;
}

Point getTransVal(Transform t){
    return t->transformation;
}

float getAngle(Transform t){
    return t->angle;
}

float getTime(Transform t) {
    return t->time;
}

bool getAlign(Transform t) {
    return t->align;
}

std::vector<Point> getControlPoints (Transform t) {
    return t->controlPoints;
}
