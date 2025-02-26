#include "Model.hpp"

struct Model{
    std::vector<Point> points;
};

Model createModel(){
    Model m;
    m->points = std::vector<Point>();

    return m;
}

Model makeModel(std::vector<Point> points){
    Model m;
    for(int i=0; i<points.size(); i++){
        addPoint(m, points[i]);
    }
}

std::vector<Point> getPoints(Model m){
    return m->points;
}

void addPoint(Model m, Point p){
    m->points.push_back(p);
}

std::vector<Point> readFromFile(std::string filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            float x, y ,z;
            if (iss >> x >> y >> z) {
                points.push_back(makePoint(x, y, z));
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return points;
}

void writeToFile(std::vector<Point> controlPoints, string fileName) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        printf("Error opening file: %s\n", fileName.c_str());
        return;
    }

    // Number of control points
    file << controlPoints.size() << std::endl;

    // Write control points
    for (const auto& p : controlPoints) {
        file << getX(p) << ", " << getY(p) << ", " << getZ(p) << std::endl;
    }

    file.close();
}