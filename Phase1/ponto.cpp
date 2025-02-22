#include "ponto.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

struct Point {
    float x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

std::vector<Point> readFromFile(const std::string& filename) {
    std::vector<Point> pontos;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Point p;
            if (iss >> p.x >> p.y >> p.z) {
                pontos.push_back(p);
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return pontos;
}