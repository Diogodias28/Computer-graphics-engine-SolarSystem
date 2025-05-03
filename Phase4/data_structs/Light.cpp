#include "Light.hpp"

struct light{
    char type; // P = point, D = directional, S = spot, N = null
    std::vector<Point> positions;
    std::vector<float> directions;
    float cutoof;
};

Light makeLight(char type, std::vector<Point> positions, std::vector<float> direction, float cutoff) {
    Light l = new light();

    if (l != NULL) {
        l->type = type;
        l->positions = positions;
        l->directions = direction;
        l->cutoof = cutoff;
    }

    return l;
}

Light createLight() {
    std::vector<float> nullVector = {0.0f,0.0f,0.0f};
    return makeLight ('N', std::vector<Point>(), nullVector, 0.0f);
}

char getType(Light l) {
    return l->type;
}

std::vector<Point> getPositions(Light l) {
    return l->positions;
}

std::vector<float> getDirections(Light l) {
    return l->directions;
}

float getCutoof(Light l) {
    return l->cutoof;
}