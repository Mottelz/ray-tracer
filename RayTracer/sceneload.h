#pragma once
#include "light.h"
#include "camera.h"
#include "sphere.h"
#include "plane.h"

bool load_scene(const std::string& filepath);
bool load_scene(const std::string& filepath, std::vector<Object*> & things, std::vector<Light*> & lights, Camera* & cam);
