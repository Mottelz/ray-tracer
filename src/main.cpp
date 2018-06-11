#define show_pic false //Display the image with CImg when done
#define mot_log false //Create a log (these get pretty freaking big)
#include "libs.h" //basic libraries (glm, iostrem, etc.)
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene

std::string scene = "scene1.txt";
//Based on testing, these are the optimal biases to avoid acne and get proper distribution.
float ray_bias = 0.0f;
float shadow_bias = 0.005f;

//Main function
int main(int argc, const char * argv[]) {
    std::vector<Object*> things; //init empty vector of physical objects/things in the scene.
    std::vector<Light*> lights; //init empty vector of lights
    Camera* cam; //init camera pointer

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
    for (int j = 0; j < width; j++) { //loop through width
        for (int k = 0; k < height; k++) { //loop through height
            Intersect hit; //The empty intersect object we're starting with.
            hit.contact = false; //Since it hasn't made cactually hit anything yet, set it to false.
            Ray r; //The ray we are firing.
            r.org = cam -> getPosition(); //The origin point of the ray is the camera's position.
            r.dir.x = r.org.x - (j-(width/2)); //X = origin minus current pixel's width.
            r.dir.y = r.org.y - (k-(height/2)); //Y = origin minus current pixel's height.
            r.dir.z = r.org.z - cam -> getFocal(); //Z = origin minus the focal length.
            r.dir = glm::normalize(r.dir); //Normalize the direction.

            //for every object. check if there's a hit and if it's close.
            for (int i = 0; i < things.size(); i++) { //Loop through obejcts.
                Intersect t_hit = things[i] -> intersect(r, ray_bias); //Check for intersection.
                t_hit.thing = i; //Set the intersection object index to the current object. Used when calulating colour.
                if (t_hit.contact) { //If we made contact with this object the check.
                    if (!hit.contact) { //If the previous Intersect wasn't storing contact,
                        hit = t_hit; //switch to this one.
                    } else { //If the current hit and the new one are both storing hits,
                        if (is_closer(r.org, t_hit.pos, hit.pos)) { //and if the new one is closer
                            hit = t_hit; //then replace the old with the new
                        }
                    }
                }
            } //Once we've looped through all the objects in the scene
            
            if(hit.contact){//If we made contact calulate the colour/shadow.
#if mot_log
                log << "HIT! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n"; //register a hit in the log.
#endif
                glm::vec3 colour(0.0f); //Start with black.
                for (int i = 0; i < lights.size(); i++) { //Loop through every light in the scene.
                    //Create the shadow ray.
                    glm::vec3 light_pos = lights[i] -> getPosition(); //get the light's position stored in a variable for readablity.
                    Ray sr; //Create the shadow ray.
                    sr.org = light_pos; //Set shadow ray origin to the light.
                    sr.dir = hit.pos - light_pos; //Set the shadow ray direction by light's position minus point of contact.
                    sr.dir = glm::normalize(sr.dir); //Normalize the direction.
                    bool in_shadow = false; //Store if this is in shadow. Starts off in light.

                    //Loop through objects and check if one is closer.
                    for (int m = 0; m < things.size(); m++) {
                        Intersect shadow_hit = things[m] -> intersect(sr, shadow_bias); //Get the shadow ray intersection
                        if (shadow_hit.contact) { //If there is an intersection
                            if (is_closer(light_pos, shadow_hit.pos, hit.pos)) { //and that object is closer,
                                in_shadow = true; //then we are in shadow.
                            }
                        }
                    } //After looping through all of the objects for a given light.

                    if (!in_shadow) { //If we're not in shadow,
                        glm::vec3 new_colour = things[hit.thing] -> getColour(*lights[i], hit, cam ->getPosition()); //The new colour.
                        colour.x = glm::sqrt(glm::pow(colour.x, 2)+glm::pow(new_colour.x, 2)); //Colour merging done via squareroot of the combined squares.
                        colour.y = glm::sqrt(glm::pow(colour.y, 2)+glm::pow(new_colour.y, 2)); //Colour merging done via squareroot of the combined squares.
                        colour.z = glm::sqrt(glm::pow(colour.z, 2)+glm::pow(new_colour.z, 2)); //Colour merging done via squareroot of the combined squares.
                    }
                } //After looping through all of the lights.
                
                if (colour == glm::vec3(0.0f)) { //If the colur is still black,
                    colour = things[hit.thing] -> getColour(); //set it to the ambient colour for this object.
                }
                
                clip(colour, 0.0f, 1.0f); //Clip the colour to [0,1]
                draw(image, j, k, colour); //Draw the pixel. The X is inverted because of a quirk in CImg.
            } else { //If the initial ray did not intersect with any object.
#if mot_log
                log << "MISS! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n"; //Log a miss
#endif
                draw(image, j, k, glm::vec3(0.1f, 0.0f, 0.1f)); //And fill the pixel with Burgundy, so we know that something was added to the pixel.
            }
        }
    }

    cam -> print(); //Print out the camera's stats so our user gets some feedback between the writing and the saving.

    //Save out the image in PNG format.
    image.save(get_name("/Users/mottelzirkind/Desktop/results/render ",".png")); //get_name used so that the image includes a timestamp.

#if show_pic //If we want to see the picture displayed then display it with CImg.
    //Display the rendered image on screen
        cimg_library::CImgDisplay main_disp(image,"Render");
        while (!main_disp.is_closed()) {
            main_disp.wait();
        }
#endif

#if mot_log //If we're using a log
    log.close(); //Close the log's write stream.
#endif
    return 0; //And end the program.
}




