#include "Settings.hpp"
#include <vector>
#include <string>

using namespace std;

struct settings{
    // Câmara
    int window[2];
    float poscam[3];
    float lookAt[3];
    float up[3];
    float projection[3]; // fov, near, far
    Light light;
    Group group;
    bool showAxes;
};

Settings newSettings(){
    Settings newSettings = new settings();
    newSettings->light = createLight();
    newSettings->group = createGroup();
    newSettings->showAxes = true;
    return newSettings;
}

Light parseLight(TiXmlElement* lightsElement) {
    char type = 'N';
    std::vector<float> dir;
    std::vector<Point> positions = std::vector<Point>();
    float cutoff = 0.0f;

    TiXmlElement* firstLight = lightsElement->FirstChildElement("light");
    if(firstLight) {

        std::string lightType = firstLight->Attribute("type");

        if (lightType == "directional") {
            type = 'D';
            dir.push_back(atof(firstLight->Attribute("dirx")));
            dir.push_back(atof(firstLight->Attribute("diry")));
            dir.push_back(atof(firstLight->Attribute("dirz")));
        }
        else if (lightType == "spot") {
            type = 'S';
            positions.push_back(makePoint(
                atof(firstLight->Attribute("posx")),
                atof(firstLight->Attribute("posy")),
                atof(firstLight->Attribute("posz"))
            ));
            
            dir.push_back(atof(firstLight->Attribute("dirx")));
            dir.push_back(atof(firstLight->Attribute("diry")));
            dir.push_back(atof(firstLight->Attribute("dirz")));

            cutoff = atof(firstLight->Attribute("cutoff"));
        }
        else if (lightType == "point") {
            type = 'P';
            positions.push_back(makePoint(
                atof(firstLight->Attribute("posx")),
                atof(firstLight->Attribute("posy")),
                atof(firstLight->Attribute("posz"))
            ));

            for (TiXmlElement* lightElem = firstLight->NextSiblingElement("light");
                lightElem && std::string(lightElem->Attribute("type")) == "point";
                lightElem = lightElem->NextSiblingElement("light")) {
            
                positions.push_back(makePoint(
                    atof(lightElem->Attribute("posx")),
                    atof(lightElem->Attribute("posy")),
                    atof(lightElem->Attribute("posz"))
                ));
            }
        }
    }
    return makeLight(type,positions,dir,cutoff);
}

Group parseGroup(TiXmlElement* groupElement) {
    std::vector<Model> models = std::vector<Model>();
    std::vector<Transform> transformations = std::vector<Transform>();
    std::vector<Group> subgroups = std::vector<Group>();
    bool showOrbit = true;

    float currentR = 1.0f, currentG = 0.6f, currentB = 0.7f;
    
    // Parsing das transformações
    TiXmlElement* transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        for (TiXmlElement* transType = transformElement->FirstChildElement(); 
             transType; 
             transType = transType->NextSiblingElement()) {
            
            std::string elementName = transType->Value();
            Transform transform = nullptr;

            if (elementName == "translate") {
                // Verificar se é uma translação temporal
                if (transType->Attribute("time")) {
                    float time = atof(transType->Attribute("time"));
                    bool align = transType->Attribute("align") ? 
                                (strcasecmp(transType->Attribute("align"), "true") == 0) : false;
                    bool showOrbit = transType->Attribute("showOrbit") ? 
                                (strcasecmp(transType->Attribute("showOrbit"), "true") == 0) : true;

                    // Coletar pontos
                    std::vector<Point> points;
                    for (TiXmlElement* p = transType->FirstChildElement("point"); 
                         p; 
                         p = p->NextSiblingElement("point")) {
                        points.push_back(makePoint(
                            atof(p->Attribute("x")),
                            atof(p->Attribute("y")),
                            atof(p->Attribute("z"))
                        ));
                    }

                    // Validar mínimo de 4 pontos
                    if (points.size() < 4) {
                        std::cerr << "Erro: Curva Catmull-Rom necessita de pelo menos 4 pontos ("
                                  << points.size() << " fornecidos). Transformação ignorada.\n";
                        continue;
                    }

                    transform = newTransformation('T', makePoint(0,0,0), 0, time, points, align, showOrbit);
                }
                else { // Translação estática
                    transform = newTransformation('T', 
                        makePoint(
                            atof(transType->Attribute("x")),
                            atof(transType->Attribute("y")),
                            atof(transType->Attribute("z"))
                        ), 
                        0, 0, {}, false);
                }
            }
            else if (elementName == "rotate") {
                float angle = 0, time = 0;
                bool hasTime = false;

                // Priorizar tempo se existir
                if (transType->Attribute("time")) {
                    time = atof(transType->Attribute("time"));
                    hasTime = true;
                } 
                else if (transType->Attribute("angle")) {
                    angle = atof(transType->Attribute("angle"));
                }

                transform = newTransformation('R',
                    makePoint(
                        atof(transType->Attribute("x")),
                        atof(transType->Attribute("y")),
                        atof(transType->Attribute("z"))
                    ),
                    angle,
                    time
                );
            }
            else if (elementName == "scale") {
                transform = newTransformation('S',
                    makePoint(
                        atof(transType->Attribute("x")),
                        atof(transType->Attribute("y")),
                        atof(transType->Attribute("z"))
                    ),
                    0, 0, {}, false);
            }

            if (transform) {
                transformations.push_back(transform);
            }
        }
    }

    
    // Parsing dos models
    TiXmlElement* modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        TiXmlElement* modelElement = modelsElement->FirstChildElement("model");
        if (modelElement) {
            const char* filename = modelElement->Attribute("file");
            std::vector<Point> points = readFromFile(filename)[0];
            std::vector<Point> normals = readFromFile(filename)[1];
            std::vector<Point> texCoord = readFromFile(filename)[2];

            // Textures
            const char* textureFile = nullptr;
            TiXmlElement* textureElem = modelElement->FirstChildElement("texture");
            if(textureElem) {
                textureFile = textureElem->Attribute("file");
            }

            //Colors
            Color currentColor = createDefaultColor();
            TiXmlElement* colorElem = modelElement->FirstChildElement("color");
            if (colorElem) {
                // Update cor
                TiXmlElement* diffuseElem = colorElem->FirstChildElement("diffuse");
                TiXmlElement* ambientElem = colorElem->FirstChildElement("ambient");
                TiXmlElement* specularElem = colorElem->FirstChildElement("specular");
                TiXmlElement* emissiveElem = colorElem->FirstChildElement("emissive");
                TiXmlElement* shininessElem = colorElem->FirstChildElement("shininess");

                if(diffuseElem) {
                    setDiffuse(currentColor, createRGB(
                        atoi(diffuseElem->Attribute("R")),
                        atoi(diffuseElem->Attribute("G")),
                        atoi(diffuseElem->Attribute("B"))
                    ));
                }
                if(ambientElem) {
                    setAmbient(currentColor,createRGB(
                        atoi(ambientElem->Attribute("R")),
                        atoi(ambientElem->Attribute("G")),
                        atoi(ambientElem->Attribute("B"))
                    ));
                }
                if(specularElem) {
                    setSpecular(currentColor, createRGB(
                        atoi(specularElem->Attribute("R")),
                        atoi(specularElem->Attribute("G")),
                        atoi(specularElem->Attribute("B"))
                    ));
                }
                if(emissiveElem) {
                    setEmissive(currentColor, createRGB(
                        atoi(emissiveElem->Attribute("R")),
                        atoi(emissiveElem->Attribute("G")),
                        atoi(emissiveElem->Attribute("B"))
                    ));
                }
                if(shininessElem) {
                    setShininess(currentColor, atof(shininessElem->Attribute("value")));
                }
            }

            Model model = makeModel(points, normals, texCoord, currentColor, textureFile); // ????????????????????????
            models.push_back(model);
        }

    }
    
    // Parsing dos subgrupos
    for (TiXmlElement* subgroupElement = groupElement->FirstChildElement("group");
         subgroupElement;
         subgroupElement = subgroupElement->NextSiblingElement("group")) {
        
        Group subgroup = parseGroup(subgroupElement);
        subgroups.push_back(subgroup);
    }
    
    return makeGroup(models, transformations, subgroups);
}

Settings xmlToSettings(const char* filePath){
    Settings result = newSettings();
    if(result != NULL){
        TiXmlDocument doc;
        if(doc.LoadFile(filePath)){
            TiXmlElement* root = doc.FirstChildElement("world"); // todo o conteúdo do ficheiro
            // Obtenção dos dados da câmara
            TiXmlElement* window = root->FirstChildElement("window"); // parâmetros da janela
            TiXmlElement* camera = root->FirstChildElement("camera"); // parâmetros da câmara
            TiXmlElement* posCamera = camera->FirstChildElement("position"); // posição da câmara
            TiXmlElement* lookAtCamera = camera->FirstChildElement("lookAt"); // lookAt da câmara
            TiXmlElement* upCamera = camera->FirstChildElement("up"); // up da câmara
            TiXmlElement* projectionCamera = camera->FirstChildElement("projection"); // projections
            TiXmlElement* showAxes = root->FirstChildElement("showAxes"); //exibir eixos
            result->window[0] = atoi(window->Attribute("width")); //largura da janela
            result->window[1] = atoi(window->Attribute("height")); //altura da janela
            result->poscam[0] = atof(posCamera->Attribute("x")); // coordenada x da posição da câmara
            result->poscam[1] = atof(posCamera->Attribute("y")); // coordenada y da posição da câmara
            result->poscam[2] = atof(posCamera->Attribute("z")); // coordenada z da posição da câmara
            result->lookAt[0] = atof(lookAtCamera->Attribute("x")); // coordenada x da posição lookAt da câmara
            result->lookAt[1] = atof(lookAtCamera->Attribute("y")); // coordenada y da posição lookAt da câmara
            result->lookAt[2] = atof(lookAtCamera->Attribute("z")); // coordenada z da posição lookAt da câmara
            result->up[0] = atof(upCamera->Attribute("x")); // coordenada x do up da câmara
            result->up[1] = atof(upCamera->Attribute("y")); // coordenada y do up da câmara
            result->up[2] = atof(upCamera->Attribute("z")); // coordenada z do up da câmara
            result->projection[0] = atof(projectionCamera->Attribute("fov")); // parâmetro fov do xml
            result->projection[1] = atof(projectionCamera->Attribute("near")); // parâmetro near do xml
            result->projection[2] = atof(projectionCamera->Attribute("far")); // parâmetro far do xml 
            result->showAxes = (strcmp(showAxes->GetText(), "true") == 0);
            
            TiXmlElement* lights = root->FirstChildElement("lights");
            if(lights) {
                result->light = parseLight(lights);
            } else {
                result->light = createLight();
            }

            TiXmlElement* group = root->FirstChildElement("group"); // obtenção do group do ficheiro
            if(group){
                result->group = parseGroup(group);
            }
        } else{
            printf("Error: Could not load XML file %s\n", filePath);
            return NULL;
        }
    }
    return result;
}


void setCamPosition(Settings set, float x, float y, float z){
    set->poscam[0] = x;
    set->poscam[1] = y;
    set->poscam[2] = z;
}

int getHeight(Settings set){
    return set->window[1];
}

int getWidth(Settings set){
    return set->window[0];
}

float getXPosCam(Settings set){
    return set->poscam[0];
}

float getYPosCam(Settings set){
    return set->poscam[1];
}

float getZPosCam(Settings set){
    return set->poscam[2];
}

float getXLookAt(Settings set){
    return set->lookAt[0];
}

float getYLookAt(Settings set){
    return set->lookAt[1];
}

float getZLookAt(Settings set){
    return set->lookAt[2];
}

float getXUp(Settings set){
    return set->up[0];
}

float getYUp(Settings set){
    return set->up[1];
}

float getZUp(Settings set){
    return set->up[2];
}

float getFov(Settings set){
    return set->projection[0];
}

float getNear(Settings set){
    return set->projection[1];
}

float getFar(Settings set){
    return set->projection[2];
}

bool getShowAxes(Settings set){
    return set->showAxes;
}

void setShowAxes(Settings set, bool showAxes){
    set->showAxes = showAxes;
}

Group getGroup(Settings set){
    return set->group;
}

Light getLight(Settings set){
    return set->light;
}