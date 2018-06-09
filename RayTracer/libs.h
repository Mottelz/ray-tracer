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

//Math
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/epsilon.hpp>


//CImg
#define cimg_use_png //Allows import of png
#define cimg_use_jpeg //Allows import of jpg
#include <CImg.h>

#define T_BIAS 0.025f
