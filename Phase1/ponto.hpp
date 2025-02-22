#ifndef PONTO
#define PONTO
#include <stdlib.h>
#include <math.h>

typedef struct ponto* Point;
std::vector<Point> readFromFile(const std::string& filename);

#endif // PONTO