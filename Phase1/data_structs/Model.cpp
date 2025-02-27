#include "Model.hpp"

struct model{
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

    return m;
}

std::vector<Point> getPoints(Model m){
    return m->points;
}

void addPoint(Model m, Point p){
    m->points.push_back(p);
}

std::vector<Point> readFromFile(std::string fileName) {
    std::vector<Point> points = std::vector<Point>();
    std::string path =  "../outputs/3d/" + fileName;
    
    int fd = open(path.c_str(), O_RDONLY);

    if(fd == -1){
        printf("Unable to open file: %s\n", fileName.c_str());
        return points;
    }
    
    char buffer[4096];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer)-1);
    close(fd);

    if(bytesRead>=0){
        buffer[bytesRead] = '\0';

        char* line = strtok(buffer, "\n");
        while (line) {
            char* xStr = strtok(line, ",");
            char* yStr = strtok(nullptr, ",");
            char* zStr = strtok(nullptr, ",");
    
            if (xStr && yStr && zStr) {
                float x = atof(xStr);
                float y = atof(yStr);
                float z = atof(zStr);
                points.push_back(makePoint(x, y, z));
            }
    
            line = strtok(nullptr, "\n");  // Move to next line
        }
    }
    return points;
}

void writeToFile(std::vector<Point> controlPoints, string fileName) {
    std::string path = "../outputs/3d/" + fileName;

    int fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);  // Open file for writing

    if (fd == -1) {
        printf("Error opening file: %s\n", fileName.c_str());
        return;
    }

    char buffer[128];

    // Write number of points
    int len = sprintf(buffer, "%lu\n", controlPoints.size());
    write(fd, buffer, len);

    // Write each point
    for (const auto& p : controlPoints) {
        len = sprintf(buffer, "%f,%f,%f\n", getX(p), getY(p), getZ(p));
        write(fd, buffer, len);
    }

    close(fd);
}