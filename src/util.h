/**
 * @file util.h
 * @author Mottel Zirkind
 * @brief Header file for functions without a home. These get used by the program, but don't belong to any particular class.
 **/

#pragma once
#include "libs.h"


const char* get_name(std::string name, std::string ext); /**< @brief Adds the date and time to a file name in the following format 1970-01-01 12h00m00s.*/
void draw_square(cimg_library::CImg<float> &image, int width, int height); /**< @brief Draws a 150x150 pink square in the centre of the image.*/
void draw(cimg_library::CImg<float> &image, int w, int h, glm::vec3 color); /**< @brief Colours in the given pixel. Given vec3 should be clipped between [0,1] the function converts it.*/
void tell_user(std::string message); /**< @brief A simple print function. Here so that all messages can be disabled by commenting out one line.*/
glm::vec3 clip(glm::vec3 vec, float lo, float hi); /**< @brief Clip a vec3 between the given values. */
bool is_closer(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2); /**< @brief True if p1 is closer to p0. False if p2 is closer to p0. */
glm::vec3 calculate_colour(glm::vec3 lightPos, glm::vec3 lightCol, glm::vec3 camPos, glm::vec3 p0, glm::vec3 norm, glm::vec3 diffIn, glm::vec3 specIn, glm::vec3 ambIn, float shineIn); /**< @brief Calculates the colour using the Phong lighting model. Called by Sphere and Plane.*/
