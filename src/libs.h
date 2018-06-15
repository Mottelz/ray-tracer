/**
 * @file libs.h
 * @author Mottel Zirkind
 * @brief The header for external libraries that are used everywhere. Also defines T_BIAS which helps with acne.
 **/

#pragma once //Libraries loaded here
#pragma GCC system_header //Surpress warnings

//Standard libs
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>

//Math
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>


//CImg
#define cimg_use_png //Allows import of png
#define cimg_use_jpeg //Allows import of jpg
#include <CImg.h>

//Configuration
#define show_pic true //Display the image with Preview when done
#define antialiasing false //Apply antialiasing
#define mot_log true //Create a log to store the stats for testing
#define soft_shadow true
#define ZERO 0.0001 //Need this because C is weird.
