/**
 * @file util.h
 * @author Mottel Zirkind
 * @brief Header file for functions without a home. These get used by the program, but don't belong to any particular class.
 **/

#pragma once
#include "libs.h"


const char* get_name(std::string name, std::string ext); /**< @brief Adds the date and time to a file name in the following format 1970-01-01 12h00m00s.*/

void draw_square(cimg_library::CImg<double> &image, int width, int height); /**< @brief Draws a 150x150 pink square in the centre of the image.*/

void draw(cimg_library::CImg<double> &image, int w, int h, glm::dvec3 color); /**< @brief Colours in the given pixel. Given vec3 should be clipped between [0,1] the function converts it.*/

void tell_user(std::string message); /**< @brief A simple print function. Here so that all messages can be disabled by commenting out one line.*/

glm::dvec3 clip(glm::dvec3 vec, double lo, double hi); /**< @brief Clip a vec3 between the given values. */

double clip(double in_val, double lo, double hi); /**< @brief Clip a double between the given values. */


bool is_closer(glm::dvec3 p0, glm::dvec3 p1, glm::dvec3 p2); /**< @brief True if p1 is closer to p0. False if p2 is closer to p0. */

glm::dvec3 calculate_colour(glm::dvec3 lightPos, glm::dvec3 lightCol, glm::dvec3 camPos, glm::dvec3 p0, glm::dvec3 norm, glm::dvec3 diffIn, glm::dvec3 specIn, glm::dvec3 ambIn, double shineIn); /**< @brief Calculates the colour using the Phong lighting model. Called by Sphere and Plane.*/

glm::dvec3 merge_colours(std::vector<glm::dvec3> colours); /**< @brief Merges colours using the squaring method described here: https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5 */

void downsize(cimg_library::CImg<double> &src, cimg_library::CImg<double> &down, int width, int height, int aa_multi); /**< @brief  Downsizes from src to down. Expects the width and height of down. aa_multi is the difference in size between src and down (i.e. src.width = down.width * aa_multi). */

glm::dvec3 gammify(glm::dvec3 colour, double gamma); /**< @brief Does that gamma thing. */

bool are_equal(glm::dvec3 v0, glm::dvec3 v1); /**< @brief Compare vecs. */
