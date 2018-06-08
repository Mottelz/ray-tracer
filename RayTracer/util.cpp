#include "util.h"

void tell_user(std::string message){
    std::cout << message << std::endl;
};

//Create filename with timestamp
const char* get_name(std::string name, std::string ext) {
    //Get time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    //Convert time to stream
    std::ostringstream oss;
    oss << std::put_time(&tm, "%y-%m-%d %Hh%Mm%Ss");
    //Create the string and return it
    std::string filename = name + oss.str() + ext;
    return filename.c_str();
}

//Draw a 150x150 pink square in the centre of an image
void draw_square(cimg_library::CImg<float> &image, int width, int height) {
    //Draw a pink square in the centre
    glm::vec3 color(64, 0, 77);
    for (int j = (width/2)-75; j < (width/2)+75; j++) {
        for (int k = (height/2)-75; k < (height/2)+75; k++) {
            image(j, k, 0) = color.x;
            image(j, k, 1) = color.y;
            image(j, k, 2) = color.z;
        }
    }
}

//Set the colour of a given pixel
void draw(cimg_library::CImg<float> &image, int w, int h, glm::vec3 color) {
    //Color in a given pixel
    image(w, h, 0) = color.r*255;
    image(w, h, 1) = color.g*255;
    image(w, h, 2) = color.b*255;
}

glm::vec3 clip(glm::vec3 vec, float lo, float hi) {
    vec.x = (vec.x > hi) ? hi : vec.x;
    vec.y = (vec.y > hi) ? hi : vec.y;
    vec.z = (vec.z > hi) ? hi : vec.z;
    
    vec.x = (vec.x < lo) ? lo : vec.x;
    vec.y = (vec.y < lo) ? lo : vec.y;
    vec.z = (vec.z < lo) ? lo : vec.z;
    
    return vec;
};
