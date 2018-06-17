/**
 * @file libs.h
 * @author Mottel Zirkind
 * @brief The header for external libraries and feature configuration
 **/
#pragma once //Libraries loaded here
#pragma GCC system_header //Surpress warnings

//Configuration
#define mot_log true //Create a log to store the stats for testing
#define show_pic false //Display the image with Preview when done
#define antialiasing true //Apply antialiasing
#define soft_shadow true
#define ZERO 0.0001 //Need this because C is weird.

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
