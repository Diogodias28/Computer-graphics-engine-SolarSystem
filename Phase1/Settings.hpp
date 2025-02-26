#ifndef SETTINGS
#define SETTINGS
#include "../TinyXML/tinyxml.h"
#include <stdlib.h>
#include "Model.hpp"

typedef struct Settings* Settings;

Settings newSettings();

Settings xmlToSettings(const char*);

std::vector<std::string> getModels(Settings conf);

void setCamPosition(Settings, float, float, float);

float getXPosCam(Settings);

float getYPosCam(Settings);

float getZPosCam(Settings);

float getXLookAt(Settings conf);

float getYLookAt(Settings conf);

float getZLookAt(Settings conf);

float getXUp(Settings conf);

float getYUp(Settings conf);

float getZUp(Settings conf);

void deleteSettings(Settings);

#endif // Settings