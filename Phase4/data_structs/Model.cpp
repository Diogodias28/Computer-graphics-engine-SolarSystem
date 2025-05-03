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
    m->color = color;
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
    std::vector<Point> points;
    std::string line;

    fileName = "../3d/" + fileName;
    std::ifstream file(fileName);

    if (file.is_open()) {
        std::getline(file, line); // Lê o número de pontos (ignora-os)

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string xStr, yStr, zStr;

            if (std::getline(iss, xStr, ',') && 
                std::getline(iss, yStr, ',') && 
                std::getline(iss, zStr, ',')) {
                
                float x = std::stof(xStr);
                float y = std::stof(yStr);
                float z = std::stof(zStr);

                points.push_back(makePoint(x, y, z));
            }
        }
        file.close();
    } else {
        printf("Error opening file: %s\n", fileName.c_str());
    }

    return points;
}

void writeToFile(std::vector<Point> controlPoints, string fileName) {
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
        file << getX(controlPoints[i]) << ", " << getY(controlPoints[i]) << ", " << getZ(controlPoints[i]) << std::endl;
    }

    file.close();
}

const char* getTextureFile(Model m) {
    return m->textureFile;
}

// ============================================================
// ============================================================

void printModelsInfo(Group group) {
    // Processa modelos deste grupo
    std::vector<Model> models = getGroupModels(group);
    for (size_t i = 0; i < models.size(); i++) {
        printf("Model %zu:\n", i+1);
        
        // Informações de cor
        Color c = getColor(models[i]);
        printf("  Color Components:\n");
        printf("    Diffuse:  (%.2f, %.2f, %.2f)\n", c.diffuse.r, c.diffuse.g, c.diffuse.b);
        printf("    Ambient:  (%.2f, %.2f, %.2f)\n", c.ambient.r, c.ambient.g, c.ambient.b);
        printf("    Specular: (%.2f, %.2f, %.2f)\n", c.specular.r, c.specular.g, c.specular.b);
        printf("    Emissive: (%.2f, %.2f, %.2f)\n", c.emissive.r, c.emissive.g, c.emissive.b);
        printf("    Shininess: %.2f\n", c.shininess);
        
        // Informação de textura
        const char* tex = getTextureFile(models[i]);
        printf("  Texture: %s\n\n", tex ? tex : "None");
    }

    // Processa subgrupos recursivamente
    std::vector<Group> subgroups = getGroupSubgroups(group);
    for (const auto& subgroup : subgroups) {
        printModelsInfo(subgroup);
    }
}

// ============================================================
// ============================================================
