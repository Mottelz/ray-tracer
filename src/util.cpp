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
    image(w, h, 0) = color.r*256;
    image(w, h, 1) = color.g*256;
    image(w, h, 2) = color.b*256;
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

//clips a double
double clip(double in_val, double lo, double hi) {
    in_val = (in_val < lo) ? lo : in_val;
    in_val = (in_val > hi) ? hi : in_val;
    return in_val;
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
    double alpha = glm::dot(light_dir, reflection);
    double theta = glm::dot(norm, light_dir);
    alpha = clip(alpha, 0.0, 1.0);
    theta = clip(theta, 0.0, 1.0);
    
    
    //actually calculate specular and diffuse
    glm::dvec3 spec_fin = glm::pow(glm::max(alpha, 0.0), shineIn) * specIn;
    glm::dvec3 diff_fin = glm::max(theta, 0.0) * diffIn;
    
    //finally calculate, clip and return the colour
    glm::dvec3 colour = (ambIn + diff_fin + spec_fin) * lightCol;
    
    return clip(colour, 0.0, 1.0);;
};


//Calculate a merging colour
glm::dvec3 merge_colours(std::vector<glm::dvec3> colours) {
    glm::dvec3 colour(0.0); //start with an empty colour
    
    //sum the squares
    for (int c = 0; c < colours.size(); c++) {
        colour = colour + (colours[c]*colours[c]);
    }
    
    //divide the sum and get its square root
    colour.x = glm::sqrt(colour.x/colours.size());
    colour.y = glm::sqrt(colour.y/colours.size());
    colour.z = glm::sqrt(colour.z/colours.size());
    
    //clip and return
    return clip(colour, 0.0, 1.0);
};

void downsize(cimg_library::CImg<double> &src, cimg_library::CImg<double> &down, int width, int height, int aa_multi) {
    for(int w = 0; w < width; w++) {
        for(int h = 0; h < height; h++) {
            std::vector<glm::dvec3> to_merge;
            glm::dvec3 pixel(0);
            int temp_w = aa_multi*w;
            int temp_h = aa_multi*h;
            for(int j = 0; j < aa_multi; j++) {
                for (int k = 0; k < aa_multi; k++) {
                    pixel.r = src(temp_w+k, temp_h+j, 0)/256;
                    pixel.g = src(temp_w+k, temp_h+j, 1)/256;
                    pixel.b = src(temp_w+k, temp_h+j, 2)/256;
                    to_merge.push_back(pixel);
                }
            }
            draw(down, w, h, merge_colours(to_merge));
        }
    }
};

glm::dvec3 gammify(glm::dvec3 colour, double gamma) {
    glm::dvec3 toRet(0);
    
    toRet.r = pow(colour.r, gamma);
    toRet.g = pow(colour.g, gamma);
    toRet.b = pow(colour.b, gamma);
    
    return toRet;
};
