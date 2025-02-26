#ifndef MODEL
#define MODEL
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Point.hpp"

using namespace std;

typedef struct Model* Model;

Model createModel();

Model makeModel(std::vector<Point> points);

std::vector<Point> getPoints(Model m);

void addPoint(Model m, Point p);

std::vector<Point> readFromFile(std::string filename);

void writeToFile(std::vector<Point> controlPoints, string fileName);

#endif //MODEL