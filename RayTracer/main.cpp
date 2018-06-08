#include "libs.h" //basic libraries (glm, iostrem, etc.)
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene

std::string scene = "scene1.txt";


////Main function
int main(int argc, const char * argv[]) {
    std::vector<Object*> things; //init empty vector of objects in the scene.
    std::vector<Light*> lights; //init empty vector of lights
    Camera* cam; //init camera
    std::ofstream log;
    log.open(get_name("/Users/mottelzirkind/Desktop/log ",".txt"));
    
    
    //Load the scene
    if(!load_scene(scene, things, lights, cam)) {
        tell_user("Failed to load scene!");
    };
    
    int width = cam->getWidth();
    int height = cam->getHeight();
    
    //Creates an image with three channels and sets it to black
    cimg_library::CImg<float> image(width, height, 1, 3, 0);
    
    //The main loop
    for (int j = 0; j < width; j++) {
        for (int k = 0; k < height; k++) {
            Intersect hit;
            hit.contact = false;
            Ray r;
            r.org = cam -> getPosition();
            //direction is difference between two points
            r.dir.x = r.org.x - (j-(width/2));
            r.dir.y = r.org.y - (k-(height/2));
            r.dir.z = r.org.z - cam -> getFocal();
            r.dir = glm::normalize(r.dir);
            
            //for every object. check if there's a hit and if it's close.
            for (int i = 0; i < things.size(); i++) {
                Intersect t_hit = things[i] -> intersect(r);
                t_hit.thing = i;
                if (t_hit.contact) {
                    if (!hit.contact) {
                        hit = t_hit;
                    } else {
                        glm::vec3 distA, distB;
                        distA = hit.pos - r.org;
                        distB = t_hit.pos - r.org;
                        if (glm::length(distA) > glm::length(distB)) {
                            hit = t_hit;
                        }
                    }
                }
            }
            
            
            if(hit.contact){
                log << "HIT! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n";
                for (auto &light : lights) {
                    //Create the shadow ray (points from object to light).
                    Ray sr;
                    glm::vec3 light_pos = light -> getPosition();
                    sr.org = hit.pos;
                    sr.dir = hit.pos - light_pos;
                    sr.dir = glm::normalize(sr.dir);
                    bool in_shadow = false;
                    
                    
                    //Loop through objects and check if one is closer.
                    for (auto &thing : things) {
                        Intersect temp_hit = thing -> intersect(sr);
                        if (temp_hit.contact) {
                            glm::vec3 distA, distB;
                            distA = sr.org - light_pos;
                            distB = temp_hit.pos - light_pos;
                            if (glm::length(distA) > glm::length(distB)) {
                                in_shadow = true;
                            }
                        }
                    }
                    glm::vec3 colour(0.5f);
                    if (in_shadow) {
                        colour = things[hit.thing] -> getColour();
                    } else {
                        colour = things[hit.thing] -> getColour(*light, hit, cam ->getPosition());
                    }
                    draw(image, j, k, colour);
                }
            } else {
                log << "MISS! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n";
                draw(image, j, k, glm::vec3(0.0f));
            }
        }
    }
    
    cam -> print();
    
    //Save out the image in PNG format.
    image.save(get_name("render ",".png"));
    //Display the rendered image on screen
    cimg_library::CImgDisplay main_disp(image,"Render");
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
    
    log.close();
    return 0;
}



//int main(int argc, const char * argv[]) {
//    return 0;
//}
