#include "Group.hpp"

struct group{
    std::vector<Model> models;
    std::vector<Transform> transformations;
    std::vector<Group> subgroups;
};

Group createGroup(){ //cria grupo vazio
    Group g = new group();

    g->models = std::vector<Model>();
    g->transformations = std::vector<Transform>();
    g->subgroups = std::vector<Group>();

    return g;
}

Group makeGroup(std::vector<Model> models, std::vector<Transform> transformations, std::vector<Group> subgroups){
    Group g = createGroup();

    g->models = models;
    g->transformations = transformations;
    g->subgroups = subgroups;

    return g;
}

std::vector<Model> getModels(Group g){
    return g->models;
};

std::vector<Transform> getTransformations(Group g){
    return g->transformations;
};

std::vector<Group> getSubgroup(Group g){
    return g->subgroups;
};

void loadTexture(Model m) {
    unsigned int t, tw, th;
    unsigned char *texData;
    unsigned int texture;
    const char* texFile = getTextureFile(m);
    ilGenImages(1, &t);
    ilBindImage(t);
    
    // Check if the texture file exists
    if (ilLoadImage((ILstring)texFile) == IL_FALSE) {
        printf("Failed to load texture: %s\n", texFile);
        return;
    }
    
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    
    // Check if glGenerateMipmap is available
    #ifndef __APPLE__    
    if (GLEW_VERSION_3_0) {
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        // Fall back for older OpenGL versions
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tw, th, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    }
    #else
    glGenerateMipmap(GL_TEXTURE_2D);
    #endif
    
    // Clean up
    ilDeleteImages(1, &t);
    setTexture(m, texture);
}


void initGroupBuffers(Group g) {
    std::vector<Model> models = getModels(g);
    for (Model &model : models) {
        loadTexture(model); //é suposto ser aqui?
        initModelBuffers(model);
    }

    std::vector<Group> subgroups = getSubgroup(g);
    for (Group &subgroup : subgroups) {
        initGroupBuffers(subgroup);
    }
}