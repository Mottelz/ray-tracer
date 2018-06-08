#pragma once
#include "libs.h"


const char* get_name(std::string name, std::string ext);
void draw_square(cimg_library::CImg<float> &image, int width, int height);
void draw(cimg_library::CImg<float> &image, int w, int h, glm::vec3 color);
void tell_user(std::string message);
glm::vec3 clip(glm::vec3 vec, float lo, float hi);
