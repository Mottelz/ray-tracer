#include "sceneload.h"


//Load the data of a file
bool load_scene(const std::string& filepath) {
    FILE * file = fopen(filepath.c_str(), "r");
    int num_objects;
    if (file == NULL) {
        std::cout << "Scene failed to load!" <<std::endl;
        return false;
    }
    
    //Read in the numbed of objects as the first line
    fscanf(file, "%i\n", &num_objects);
    if (num_objects == 0) {
        std::cout << "Missing number of objects in the scene!" << std::endl;
        return false;
    } else {
        printf("Number of objects in scene: %i\n", num_objects);
    }
    
    
    
    //Read in the rest of the scene
    for (int i = 0; i < num_objects; i++) {
        //read in the line header
        char lineHeader[128];
        fscanf(file, "%s\n", lineHeader); //read in object header
        
        //Read in a camera
        if (strcmp(lineHeader, "camera") == 0) {
            glm::dvec3 temp_pos(-1.0f);
            int temp_fov = 0;
            int temp_f = 0;
            double temp_a = 0.0;
            for (int i = 0; i < 4; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "fov:") == 0) {
                    fscanf(file, "%i\n", &temp_fov);
                } else if (strcmp(lineHeader, "f:") == 0) {
                    fscanf(file, "%i\n", &temp_f);
                } else if (strcmp(lineHeader, "a:") == 0) {
                    fscanf(file, "%lf\n", &temp_a);
                } else {
                    std::cout << lineHeader << "\nCamera is missing info!" << std::endl;
                    return false;
                }
            }
            printf("Camera Specs: \n  Position: %s\n  FoV: %i\n  Aspect Ratio: %lf\n  Focal Length: %i\n", glm::to_string(temp_pos).c_str(), temp_fov, temp_a, temp_f);
        }
        
        //Read in the plane
        else if (strcmp(lineHeader, "plane") == 0) {
            glm::dvec3 temp_norm;
            glm::dvec3 temp_pos;
            glm::dvec3 temp_amb;
            glm::dvec3 temp_dif;
            glm::dvec3 temp_spe;
            double temp_shi = 0.0;
            for (int i = 0; i < 6; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "nor:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_norm.x, &temp_norm.y, &temp_norm.z);
                } else if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "amb:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_amb.x, &temp_amb.y, &temp_amb.z);
                } else if (strcmp(lineHeader, "dif:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_dif.x, &temp_dif.y, &temp_dif.z);
                } else if (strcmp(lineHeader, "spe:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_spe.x, &temp_spe.y, &temp_spe.z);
                } else if (strcmp(lineHeader, "shi:") == 0) {
                    fscanf(file, "%lf\n", &temp_shi);
                } else {
                    std::cout << "Plane is missing info" << std::endl;
                    return false;
                }
            }
            printf("Plane Specs: \n  Norm: %s\n  Position: %s\n  Ambient: %s\n  Diffuse: %s\n  Specular: %s\n  Shine: %lf\n",
                   glm::to_string(temp_norm).c_str(),
                   glm::to_string(temp_pos).c_str(),
                   glm::to_string(temp_amb).c_str(),
                   glm::to_string(temp_dif).c_str(),
                   glm::to_string(temp_spe).c_str(),
                   temp_shi);
        }
        
        //Read in a sphere
        else if (strcmp(lineHeader, "sphere") == 0) {
            glm::dvec3 temp_pos;
            int temp_rad = 0;
            glm::dvec3 temp_amb;
            glm::dvec3 temp_dif;
            glm::dvec3 temp_spe;
            double temp_shi = 0.0;
            for (int i = 0; i < 6; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "rad:") == 0) {
                    fscanf(file, "%i\n", &temp_rad);
                } else if (strcmp(lineHeader, "amb:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_amb.x, &temp_amb.y, &temp_amb.z);
                } else if (strcmp(lineHeader, "dif:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_dif.x, &temp_dif.y, &temp_dif.z);
                } else if (strcmp(lineHeader, "spe:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_spe.x, &temp_spe.y, &temp_spe.z);
                } else if (strcmp(lineHeader, "shi:") == 0) {
                    fscanf(file, "%lf\n", &temp_shi);
                } else {
                    std::cout << "Sphere is missing info" << std::endl;
                    return false;
                }
            }
            printf("Sphere Specs: \n  Position: %s\n  Rad: %i\n  Ambient:%s\n  Diffuse: %s\n  Specular: %s\n  Shine: %lf\n",
                   glm::to_string(temp_pos).c_str(),
                   temp_rad,
                   glm::to_string(temp_amb).c_str(),
                   glm::to_string(temp_dif).c_str(),
                   glm::to_string(temp_spe).c_str(),
                   temp_shi);
        }
        
        //Read in light
        else if (strcmp(lineHeader, "light") == 0) {
            glm::dvec3 temp_pos;
            glm::dvec3 temp_col;
            for (int i = 0; i < 2; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "col:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_col.x, &temp_col.y, &temp_col.z);
                } else {
                    std::cout << "Light is missing info" << std::endl;
                    return false;
                }
            }
            printf("Light Specs: \n  Position: %s\n  Colour: %s\n",
                   glm::to_string(temp_pos).c_str(),
                   glm::to_string(temp_col).c_str());
        }
        
        //If something has gone horribly wrong
        else {
            std::cout << "We're reading the scene wrong!" << std::endl;
        }
    }
    return true;
};

//Load Data and return it
bool load_scene(const std::string& filepath, std::vector<Object*> & things, std::vector<Light*> & lights, Camera* & cam) {
    FILE * file = fopen(filepath.c_str(), "r");
    int num_objects;
    if (file == NULL) {
        std::cout << "Scene failed to load!" <<std::endl;
        return false;
    }
    
    //Read in the numbed of objects as the first line
    fscanf(file, "%i\n", &num_objects);
    if (num_objects == 0) {
        std::cout << "Missing number of objects in the scene!" << std::endl;
        return false;
    } else {
        printf("Number of objects in scene: %i\n", num_objects);
    }
    
    
    
    //Read in the rest of the scene
    for (int i = 0; i < num_objects; i++) {
        //read in the line header
        char lineHeader[128];
        fscanf(file, "%s\n", lineHeader); //read in object header
        
        //Read in a camera
        if (strcmp(lineHeader, "camera") == 0) {
            glm::dvec3 temp_pos(-1.0f);
            int temp_fov = 0;
            int temp_f = 0;
            double temp_a = 0.0;
            for (int i = 0; i < 4; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "fov:") == 0) {
                    fscanf(file, "%i\n", &temp_fov);
                } else if (strcmp(lineHeader, "f:") == 0) {
                    fscanf(file, "%i\n", &temp_f);
                } else if (strcmp(lineHeader, "a:") == 0) {
                    fscanf(file, "%lf\n", &temp_a);
                } else {
                    std::cout << lineHeader << "\nCamera is missing info!" << std::endl;
                    return false;
                }
            }
            cam = new Camera(temp_pos, temp_fov, temp_a, temp_f);
        }
        
        //Read in the plane
        else if (strcmp(lineHeader, "plane") == 0) {
            glm::dvec3 temp_norm;
            glm::dvec3 temp_pos;
            glm::dvec3 temp_amb;
            glm::dvec3 temp_dif;
            glm::dvec3 temp_spe;
            double temp_shi = 0.0;
            for (int i = 0; i < 6; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "nor:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_norm.x, &temp_norm.y, &temp_norm.z);
                } else if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "amb:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_amb.x, &temp_amb.y, &temp_amb.z);
                } else if (strcmp(lineHeader, "dif:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_dif.x, &temp_dif.y, &temp_dif.z);
                } else if (strcmp(lineHeader, "spe:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_spe.x, &temp_spe.y, &temp_spe.z);
                } else if (strcmp(lineHeader, "shi:") == 0) {
                    fscanf(file, "%lf\n", &temp_shi);
                } else {
                    std::cout << "Plane is missing info" << std::endl;
                    return false;
                }
            }
            Plane* temp_plane = new Plane(temp_norm, temp_pos, temp_amb, temp_dif, temp_spe, temp_shi);
            things.push_back(temp_plane);
        }
        
        //Read in a sphere
        else if (strcmp(lineHeader, "sphere") == 0) {
            glm::dvec3 temp_pos;
            double temp_rad = 0.0;
            glm::dvec3 temp_amb;
            glm::dvec3 temp_dif;
            glm::dvec3 temp_spe;
            double temp_shi = 0.0;
            for (int i = 0; i < 6; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "rad:") == 0) {
                    fscanf(file, "%lf\n", &temp_rad);
                } else if (strcmp(lineHeader, "amb:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_amb.x, &temp_amb.y, &temp_amb.z);
                } else if (strcmp(lineHeader, "dif:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_dif.x, &temp_dif.y, &temp_dif.z);
                } else if (strcmp(lineHeader, "spe:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_spe.x, &temp_spe.y, &temp_spe.z);
                } else if (strcmp(lineHeader, "shi:") == 0) {
                    fscanf(file, "%lf\n", &temp_shi);
                } else {
                    std::cout << "Sphere is missing info" << std::endl;
                    return false;
                }
            }
            Sphere* temp_sphere = new Sphere(temp_pos, temp_rad, temp_amb, temp_dif, temp_spe, temp_shi);
            things.push_back(temp_sphere);
        }
        
        //Read in light
        else if (strcmp(lineHeader, "light") == 0) {
            glm::dvec3 temp_pos;
            glm::dvec3 temp_col;
            for (int i = 0; i < 2; i++) {
                fscanf(file, "%s: ", lineHeader);
                if (strcmp(lineHeader, "pos:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_pos.x, &temp_pos.y, &temp_pos.z);
                } else if (strcmp(lineHeader, "col:") == 0) {
                    fscanf(file, "%lf %lf %lf\n", &temp_col.x, &temp_col.y, &temp_col.z);
                } else {
                    std::cout << "Light is missing info" << std::endl;
                    return false;
                }
            }
            Light* temp_light = new Light(temp_pos, temp_col);
            lights.push_back(temp_light);
        }
        
        //If something has gone horribly wrong
        else {
            std::cout << "We're reading the scene wrong!" << std::endl;
            return false;
        }
    }
    return true;
};
