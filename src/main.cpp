#define show_pic false //Display the image with CImg when done
#define mot_log false //Create a log (these get pretty freaking big)
#include "libs.h" //basic libraries (glm, iostrem, etc.)
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene

std::string scene = "scenes/scene5.txt";
//Based on testing, these are the optimal biases to avoid acne and get proper distribution.
double ray_bias = 0.00;
double shadow_bias = 0.0085;

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
    cimg_library::CImg<double> image(width, height, 1, 3, 0);

    //Loop through every pixel
    for (int j = 0; j < width; j++) { //loop through width
        for (int k = 0; k < height; k++) { //loop through height
            Intersect hit; //The empty intersect object we're starting with.
            hit.contact = false; //Since it hasn't made cactually hit anything yet, set it to false.
            Ray r; //The ray we are firing.
            r.org = cam -> getPosition(); //The origin point of the ray is the camera's position.
            r.dir.x = (j-(width/2)) - r.org.x; //current pixel's width (starts at furthest negative x).
            r.dir.y = ((height/2)-k) - r.org.y; //current pixel's height (starts at positive y).
            r.dir.z = -(cam -> getFocal() - r.org.z); //Z = negative the focal length minus origin.
            
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
                std::vector<glm::dvec3> colours;
                colours.push_back(things[hit.thing] -> getColour());
                for (int i = 0; i < lights.size(); i++) { //Loop through every light in the scene.
                    //Create the shadow ray.
                    glm::dvec3 light_pos = lights[i] -> getPosition(); //get the light's position stored in a variable for readablity.
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
                                break;
                            }
                        }
                    } //After looping through all of the objects for a given light.

                    if (!in_shadow) { //If we're not in shadow,
                        glm::dvec3 new_colour = things[hit.thing] -> getColour(*lights[i], hit, cam ->getPosition()); //The new colour.
                        colours.push_back(new_colour);
                    }
                } //After looping through all of the lights.
                
                glm::dvec3 colour;
                
                if (colours.size() == 0) { //If the colur is still black,
                    colour = things[hit.thing] -> getColour(); //set it to the ambient colour for this object.
                    colour = clip(colour, 0.0, 1.0); //Clip the colour to [0,1]
                } else {
                    colour = merge_colours(colours);
                }
                draw(image, j, k, colour); //Draw the pixel. The X is inverted because of a quirk in CImg.
            } else { //If the initial ray did not intersect with any object.
#if mot_log
                log << "MISS! pixel: " + std::to_string(j) + " x " + std::to_string(k) + " at pos: " + glm::to_string(r.dir) + "\n"; //Log a miss
#endif
                draw(image, j, k, glm::dvec3(0)); //And fill the pixel with Burgundy, so we know that something was added to the pixel.
            }
        }
    }

    cam -> print(); //Print out the camera's stats so our user gets some feedback between the writing and the saving.
    image.save(get_name("/Users/mottelzirkind/Desktop/results/render ",".bmp")); //get_name used so that the image includes a timestamp.
    
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




