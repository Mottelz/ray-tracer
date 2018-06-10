#define show_pic false //Display the image with CImg when done
#define mot_log false //Create a log (these get pretty freaking big)
#include "libs.h" //basic libraries (glm, iostrem, etc.)
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene


std::string scene = "scene5.txt";


//Main function
int main(int argc, const char * argv[]) {
    std::vector<Object*> things; //init empty vector of objects in the scene.
    std::vector<Light*> lights; //init empty vector of lights
    Camera* cam; //init camera

//A log. Stores points and whether or not they hit.
#if mot_log
    std::ofstream log;
    log.open(get_name("/Users/mottelzirkind/Desktop/results/log ",".txt"));
#endif


    //Load the scene
    if(!load_scene(scene, things, lights, cam)) {
        tell_user("Failed to load scene!");
    };
    
    //Get the image width & height
    int width = cam->getWidth();
    int height = cam->getHeight();

    //Creates an image with three channels and sets it to black
    cimg_library::CImg<float> image(width, height, 1, 3, 0);

    //Loop through every pixel
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
                        if (is_closer(r.org, t_hit.pos, hit.pos)) {
                            hit = t_hit;
                        }
                    }
                }
            }


            if(hit.contact){
#if mot_log
                log << "HIT! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n";
#endif
                glm::vec3 colour(0.0f);
                for (int i = 0; i < lights.size(); i++) {
                    //Create the shadow ray.
                    glm::vec3 light_pos = lights[i] -> getPosition();
                    Ray sr;
                    sr.org = light_pos;
                    sr.dir = hit.pos - light_pos;
                    sr.dir = glm::normalize(sr.dir);
                    bool in_shadow = false;

                    //Loop through objects and check if one is closer.
                    for (int m = 0; m < things.size(); m++) {
                        Intersect temp_hit = things[m] -> intersect(sr);
                        if (temp_hit.contact) {
                            if (is_closer(light_pos, temp_hit.pos, hit.pos)) {
                                in_shadow = true;
                            }
                        }
                    }

                    if (!in_shadow) {
                        colour += things[hit.thing] -> getColour(*lights[i], hit, cam ->getPosition());
                    }
                }
                if (colour == glm::vec3(0.0f)) {
                    colour = things[hit.thing] -> getColour();
                }
                clip(colour, 0.0f, 1.0f);
                draw(image, j, k, colour);
            } else {
#if mot_log
                log << "MISS! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n";
#endif
                draw(image, j, k, glm::vec3(0.1f, 0.0f, 0.1f));
            }
        }
    }

    cam -> print();

    //Save out the image in PNG format.
    image.save(get_name("/Users/mottelzirkind/Desktop/results/render ",".bmp"));
#if show_pic
    //Display the rendered image on screen
        cimg_library::CImgDisplay main_disp(image,"Render");
        while (!main_disp.is_closed()) {
            main_disp.wait();
        }
#endif
#if mot_log
    log.close();
#endif
    return 0;
}




