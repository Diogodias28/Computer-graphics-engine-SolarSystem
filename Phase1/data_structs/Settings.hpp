#ifndef SETTINGS
#define SETTINGS
#include "../TinyXML/tinyxml.h"
#include <stdlib.h>
#include "Model.hpp"

typedef struct settings* Settings;

Settings newSettings();

Settings xmlToSettings(const char*);

std::vector<std::string> getModels(Settings);

void setCamPosition(Settings, float, float, float);

float getXPosCam(Settings);

float getYPosCam(Settings);

float getZPosCam(Settings);

float getXLookAt(Settings);

float getYLookAt(Settings);

float getZLookAt(Settings);

float getXUp(Settings);

float getYUp(Settings);

float getZUp(Settings);

void deleteSettings(Settings);

#endif // Settings