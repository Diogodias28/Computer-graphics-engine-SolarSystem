#ifndef COLOR
#define COLOR

#include <stdlib.h>

struct RGB {
    float r, g, b;
};

typedef struct color* Color;

Color createDefaultColor();

Color makeColor (RGB, RGB, RGB, RGB, float);

RGB createRGB(int, int, int);

float normalizeColor (int);

RGB getDiffuse (Color);

RGB getAmbient (Color);

RGB getSpecular (Color);

RGB getEmissive (Color);

float getShininess(Color);

#endif