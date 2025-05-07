#include "Model.hpp"

struct model{
    //std::vector<Point> points;
    Color color;
    const char* textureFile;
    float* vertexB;
    GLuint vertexCount;
    GLuint buffer[2];
    bool buffersInitialized; // Track estado OpenGL
};

Model createModel(){
    Model m = new model();
    if(!m){
        std::cerr << "Failed to allocate memory for model" << std::endl;
        exit(1);
    }

    //m->points = std::vector<Point>();
    m->color = createDefaultColor();
    m->textureFile = nullptr;
    m->vertexB = nullptr;
    m->vertexCount = 0;
    m->vertexB = nullptr;
    m->buffersInitialized = false;

    return m;
}

Model makeModel(std::vector<Point> points, Color color, const char* textureFile){
    Model m = new model();

    if (m->vertexB != nullptr) {
        free(m->vertexB);
    }
    
    m->vertexB = (float *)malloc(points.size() * 3 * sizeof(float));
    

    for(int i=0; i<points.size(); i++){
        m->vertexB[i*3 + 0] = getX(points[i]);
		m->vertexB[i*3 + 1] = getY(points[i]);
		m->vertexB[i*3 + 2] = getZ(points[i]);
    }

    m->vertexCount = points.size();

    m->color = color;
    m->textureFile = textureFile;

    return m;
}


void initModelBuffers(Model m) {
    if (!m->buffersInitialized && m->vertexB) {
        glGenBuffers(2, m->buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m->buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, 
                    sizeof(float) * m->vertexCount * 3,
                    m->vertexB, 
                    GL_STATIC_DRAW);
        
        // Free CPU memory after uploading to GPU
        free(m->vertexB);
        m->vertexB = nullptr;
        
        m->buffersInitialized = true;
    }
}

void setColor(Model m, Color c) {
    m->color = c;
}

Color getColor(Model m) {
    return(m->color);
}

float* getVertexB(Model m){
    return m->vertexB;
}

GLuint getVertexCount(Model m){
    return m->vertexCount;
}

GLuint getBuffer_0(Model m){
    return m->buffer[0];
}

GLuint getBuffer_1(Model m){
    return m->buffer[1];
}

std::vector<Point> readFromFile(std::string fileName) {
    std::vector<Point> points, normals, texCoords;
    std::string line;

    fileName = "../3d/" + fileName;
    std::ifstream file(fileName);

    if (file.is_open()) {
        std::getline(file, line); // Lê o número de pontos (ignora-os)

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string xCPstr, yCPstr, zCPstr, xNstr, yNstr, zNstr, xTCstr, yTCstr;

            if (std::getline(iss, xCPstr, ',') && 
                std::getline(iss, yCPstr, ',') && 
                std::getline(iss, zCPstr, ';') &&
                std::getline(iss, xNstr, ',') && 
                std::getline(iss, yNstr, ',') && 
                std::getline(iss, zNstr, ';') &&
                std::getline(iss, xTCstr, ',') && 
                std::getline(iss, yTCstr, ',') && 
                std::getline(iss, zCPstr)) {
                
                float xP = std::stof(xCPstr);
                float yP = std::stof(yCPstr);
                float zP = std::stof(zCPstr);
                points.push_back(makePoint(xP, yP, zP));

                float xN = std::stof(xNstr);
                float yN = std::stof(yNstr);
                float zN = std::stof(zNstr);
                normals.push_back(makePoint(xN, yN, zN));

                float xT = std::stof(xTCstr);
                float yT = std::stof(yTCstr);
                texCoords.push_back(makeTexturePoint(xT, yT));
            }
        }
        file.close();
    } else {
        printf("Error opening file: %s\n", fileName.c_str());
    }

    return points;
}

void writeToFile(std::vector<Point> controlPoints, std::vector<Point> normals, std::vector<Point> texCoords, string fileName) {
    fileName = "../3d/" + fileName;
    std::ofstream file(fileName);

    if (!file.is_open()) {
        printf("Error opening file: %s\n", fileName.c_str());
        return;
    }

    // Número de pontos de controlo
    file << controlPoints.size() << std::endl;

    // Escreve os pontos de controlo no formato "x, y, z"
    file << std::fixed << std::setprecision(6);
    for (int i = 0; i<controlPoints.size(); i++) {
        file << getX(controlPoints[i]) << ", " << getY(controlPoints[i]) << ", " << getZ(controlPoints[i]) << ";" << getX(normals[i]) << ", " << getY(normals[i]) << ", " << getZ(normals[i]) << ";" << getX(texCoords[i]) << ", " << getY(texCoords[i]) << std::endl;
    }

    file.close();
}

const char* getTextureFile(Model m) {
    return m->textureFile;
}