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
void draw_square(cimg_library::CImg<double> &image, int width, int height) {
    //Draw a pink square in the centre
    glm::dvec3 color(64, 0, 77);
    for (int j = (width/2)-75; j < (width/2)+75; j++) {
        for (int k = (height/2)-75; k < (height/2)+75; k++) {
            image(j, k, 0) = color.x;
            image(j, k, 1) = color.y;
            image(j, k, 2) = color.z;
        }
    }
}

//Set the colour of a given pixel
void draw(cimg_library::CImg<double> &image, int w, int h, glm::dvec3 color) {
    //Color in a given pixel
    image(w, h, 0) = color.r*255;
    image(w, h, 1) = color.g*255;
    image(w, h, 2) = color.b*255;
}

//clips a vec3
glm::dvec3 clip(glm::dvec3 vec, double lo, double hi) {
    vec.x = (vec.x > hi) ? hi : vec.x;
    vec.y = (vec.y > hi) ? hi : vec.y;
    vec.z = (vec.z > hi) ? hi : vec.z;
    
    vec.x = (vec.x < lo) ? lo : vec.x;
    vec.y = (vec.y < lo) ? lo : vec.y;
    vec.z = (vec.z < lo) ? lo : vec.z;
    
    return vec;
};



//returns true if p1 is closer to p0
bool is_closer(const glm::dvec3 p0, const glm::dvec3 p1, const glm::dvec3 p2) {
    if ((glm::distance(p0, p1) > glm::distance(p0, p2))) {
        return false;
    }
    return true;
};



//general colour calculation function. here to save time during tweaking.
glm::dvec3 calculate_colour(glm::dvec3 lightPos, glm::dvec3 lightCol, glm::dvec3 camPos, glm::dvec3 p0, glm::dvec3 norm, glm::dvec3 diffIn, glm::dvec3 specIn, glm::dvec3 ambIn, double shineIn) {
    
    //Get missing directions and reflections
    glm::dvec3 view_dir = glm::normalize(camPos - p0);
    glm::dvec3 light_dir = glm::normalize(lightPos - p0);
    glm::dvec3 reflection = glm::reflect(norm, view_dir);
    
    //Get alpha and theta
    double alpha = glm::dot(view_dir, reflection);
    double theta = glm::dot(norm, light_dir);
    
    //actually calculate specular and diffuse
    glm::dvec3 spec_fin = glm::pow(glm::max(alpha, 0.0), shineIn) * specIn;
    glm::dvec3 diff_fin = glm::max(theta, 0.0) * diffIn;
    
    //finally calculate and return the colour
    glm::dvec3 colour = (ambIn + diff_fin + spec_fin) * lightCol;
    return colour;
};






