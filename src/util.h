/**
 * @file util.h
 * @author Mottel Zirkind
 * @brief Header file for functions without a home. These get used by the program, but don't belong to any particular class.
 **/

#pragma once
#include "libs.h"


const char* get_name(std::string name, std::string ext);
void draw_square(cimg_library::CImg<float> &image, int width, int height);
void draw(cimg_library::CImg<float> &image, int w, int h, glm::vec3 color);
void tell_user(std::string message);
glm::vec3 clip(glm::vec3 vec, float lo, float hi);
bool is_closer(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
glm::vec3 calculate_colour(glm::vec3 lightPos, glm::vec3 lightCol, glm::vec3 camPos, glm::vec3 p0, glm::vec3 norm, glm::vec3 diffIn, glm::vec3 specIn, glm::vec3 ambIn, float shineIn);
