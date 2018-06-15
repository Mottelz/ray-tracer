/**
 * @file sceneload.h
 * @author Mottel Zirkind
 * @brief The scene loader. This version assumes a vec3 setup.
 **/


#pragma once
#include "light.h"
#include "camera.h"
#include "sphere.h"
#include "plane.h"
#include "softLights.h"

bool load_scene(const std::string& filepath); /**< @brief Loads the scene with a vec3 config and prints the read in results to terminal. Used for testing. */
bool load_scene(const std::string& filepath, std::vector<Object*> & things, std::vector<Light*> & lights, Camera* & cam); /**< @brief Loads the scene with a vec3 config. */
