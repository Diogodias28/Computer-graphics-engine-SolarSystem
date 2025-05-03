#include "Light.hpp"

struct light{
    char type; // P = point, D = directional, S = spot, N = null
    std::vector<Point> positions;
    Vector directions;
    float cutoof;
};

Light makeLight(char type, std::vector<Point> positions, Vector direction, float cutoff) {
    Light l = (Light)malloc(sizeof(struct light));

    l->type = type;
    l->positions = positions;
    l->directions = direction;
    l->cutoof = cutoff;
    return l;
}

Light createLight() {
    Vector nullVector = {0.0f,0.0f,0.0f};
    return makeLight ('N', std::vector<Point>(), nullVector, 0.0f);
}

char getType(Light l) {
    return l->type;
}

std::vector<Point> getPositions(Light l) {
    return l->positions;
}

Vector getDirections(Light l) {
    return l->directions;
}

float getCutoof(Light l) {
    return l->cutoof;
}