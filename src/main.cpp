#include "libs.h" //Basic libraries (glm, iostrem, etc.) & configuration.
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene. (This one includes all of the other classes.)

//Set a few parameters
std::string path = "/Users/mottelzirkind/Desktop/results/basic/"; /**< @brief Path to where files are saved. This will be created if it does not exist. */
double shadow_colour_bias = 0.25; /**< @brief Shadow bias helps tone down colours in shadow. */
double gamma_val = 1.0; /**< @brief If not set to 1.0 applies a gamma effect which helps with contrast. See #Gammify. */

//If anti aliasing is turned on,
//Set the radius around the pixel.
//(Because AA is done by downsizing.)
#if antialiasing
int aa_rad = 1;
int aa_multi = aa_rad+2;
#endif


/**< @brief The function that does all the work. Not the main so that multiple scenes and sample sizes can be easily set to run. */
#if soft_shadow
void render_scene(const char* scene_name, int num_samples) {  //If soft shadows are tunred on, then get the number of samples.
#else
void render_scene(const char* scene_name) { //If soft shadows are off, just give the scene.
#endif
    std::string scene(scene_name); //Convert scene to a string.
    std::vector<Object*> things; /**< @brief Empty vector of physical objects/things in the scene. */
    std::vector<Light*> lights; /**< @brief Empty vector of lights */
    Camera* cam; /**< @brief Camera pointer */
    std::string message; /**< @brief The string where messages are stored so that they can be used for both log and terminal output. */
    
    
#if mot_log //If the log is enabled, create it.
    std::ofstream log;
    log.open(get_name(path+"log-",".txt"));
#endif
    
    //Load the scene and note the time.
    if(!load_scene(scene, things, lights, cam)) {
        tell_user("Failed to load scene!");
    } else {
        message = get_name("Loaded scene at ", "");
#if mot_log
        log << "Scene:" + scene + "\n" +message + "\n";
#endif
        tell_user(message);
    };
    
    
#if antialiasing //If anti-alising is enabled
             //Set grid to base size times multiplier.
    int width = cam->getWidth()*aa_multi;
    int height = cam->getHeight()*aa_multi;
#else //If anti-alising is off
  //Set grid to base size.
    int width = cam->getWidth();
    int height = cam->getHeight();
#endif
    
    //Log the size of the grid.
#if mot_log
    log << "Grid size (WxH): " + std::to_string(width) + "x" + std::to_string(height) + "\n";
#endif
    
    //If soft shadows are enabled convert the lights to area lights.
#if soft_shadow
    std::vector<SoftLight*> soft_lights;
    for (int l = 0; l < lights.size(); l++) {
        soft_lights.push_back(new SoftLight(lights[l] -> getPosition(), lights[l] -> getColour(), num_samples));
    }
#endif
    
    //Creates an image with three channels and sets it to black
    cimg_library::CImg<double> image(width, height, 1, 3, 0);
    
    //If anti-alising is on, the z calculation requires using the aa multiplier.
    //Z is calculated here, because it's consistent per grid. (The lens never moves.)
#if antialiasing
    double grid_z = -(cam -> getFocal()*aa_multi - cam -> getPosition()); //Z = negative the focal length minus origin.
#else
    double grid_z = -(cam -> getFocal() - cam -> getPosition().z); //Z = negative the focal length minus origin.
#endif
    
    //Loop through every pixel
    for (int j = 0; j < width; j++) { //loop through width
        for (int k = 0; k < height; k++) { //loop through height
            Intersect hit; //The empty intersect object we're starting with.
            hit.contact = false; //Since it hasn't made contact with anything yet, set it to false.
            Ray r; //The ray we are firing.
            r.org = cam -> getPosition(); //The origin point of the ray is the camera's position.
                                          //Set the direction. It's the point on the grid minus the origin.
            r.dir.x = (j-(width/2)) - r.org.x; //current pixel's x value (starts at furthest negative x).
            r.dir.y = ((height/2)-k) - r.org.y; //current pixel's y value (starts at positive y).
            r.dir.z = grid_z; //Set Z based on the calculation above.
            r.dir = glm::normalize(r.dir); //Normalize the direction.
            
            //for every object. check if there's a hit and if it's close.
            for (int i = 0; i < things.size(); i++) { //Loop through obejcts.
                Intersect t_hit = things[i] -> intersect(r); //Check for intersection.
                t_hit.thing = i; //Set the intersection object index to the current object. Used when calulating colour.
                if (t_hit.contact) { //If we made contact with this object during the check.
                    if (!hit.contact) { //If the previous Intersect wasn't storing contact,
                        hit = t_hit; //switch to this one.
                    } else { //If the current hit and the new one are both storing hits,
                        if (is_closer(r.org, t_hit.pos, hit.pos)) { //and if the new one is closer
                            hit = t_hit; //then replace the old with the new
                        }
                    }
                }
            } //Once we've looped through all the objects in the scene
            
            if(hit.contact) {//If we made contact calulate the colour/shadow.
                std::vector<glm::dvec3> colours; //Start with a vector of colours.
                colours.push_back(things[hit.thing] -> getColour()); //Add the base ambient (helps smooth things out).
                
#if soft_shadow //If soft shadows are active we calculate using area lights
                for (int i = 0; i < soft_lights.size(); i++) { //Loop through every light in the scene.
                    int light_intensity = 0; //Start with a light intensity at 0.
                    bool in_shadow; //track if we're in shadow
                    for (int s = 0; s < soft_lights[i] -> getPositionRange().size(); s++) {//loop though the samples.
                        in_shadow = false; //for this point, we're not in shadow.
                        Ray shadow_ray; //Create the shadow ray.
                        shadow_ray.org = hit.pos; //Set shadow ray origin to the point of contact
                        shadow_ray.dir = soft_lights[i] -> getPositionRange()[s] - hit.pos; //Set the shadow ray direction by point of contact minus light position.
                        shadow_ray.dir = glm::normalize(shadow_ray.dir); //Normalize the direction.
                        for (int m = 0; m < things.size(); m++) { //Loop through objects and check if one is closer.
                            Intersect shadow_hit = things[m] -> intersect(shadow_ray); //Get the shadow ray intersection
                            if (shadow_hit.contact) { //If there is an intersection,
                                in_shadow = true; //we're in shadow
                                break; //stop checking if this sample point is in shadow.
                            }
                        } //after looping through all the object in the scene
                        if (!in_shadow) { //If this sample point isn't in shadow,
                            light_intensity += 1; //increase the light intensity
                        }
                    } //after looping though all of the sample points
                    double light_multi = ((double)light_intensity/((double)num_samples)); //Calculate the light intensity multiplier (samples in light/total samples)
                    glm::dvec3 new_colour = things[hit.thing] -> getColour(soft_lights[i]->getColour(), soft_lights[i] -> getPositionRange()[0], hit, cam ->getPosition()) * light_multi; //Get the colour and multiply it by the light intensity multiplier.
                    new_colour = clip(new_colour, ZERO, 1.0); //clamp the colour.
                    colours.push_back(new_colour); //add the colour from this light to our list of colours.
                } //After looping through all the lights
#else
                //if soft shadow is turned off.
                for (int i = 0; i < lights.size(); i++) { //Loop through every light in the scene.
                    Ray shadow_ray; //Create the shadow ray.
                    shadow_ray.org = hit.pos; //Set shadow ray origin to the point of intersetion.
                    shadow_ray.dir = lights[i] -> getPosition() - hit.pos; //Set the shadow ray direction by point of contact minus light position.
                    shadow_ray.dir = glm::normalize(shadow_ray.dir); //Normalize the direction.
                    bool in_shadow = false; //Store if this is in shadow. Starts off in light.
                    
                    for (int m = 0; m < things.size(); m++) { //Loop through objects and check if one is closer.
                        Intersect shadow_hit = things[m] -> intersect(shadow_ray); //Get the shadow ray intersection
                        if (shadow_hit.contact) { //If there is an intersection
                            in_shadow = true; //then we are in shadow.
                            break;
                        }
                    } //After looping through all of the objects for a given light.
                    glm::dvec3 new_colour(0.0); //The basic colour created outside the if statement.
                    if (!in_shadow || typeid(*things[hit.thing]) == typeid(Sphere)) { //If we're not in shadow or this object is a sphere,
                        new_colour = things[hit.thing] -> getColour(lights[i]->getColour(), lights[i] -> getPosition(), hit, cam ->getPosition()); //Calcualte colour.
                    } else {
                        new_colour = things[hit.thing] -> getColour() * lights[i] -> getColour() * shadow_colour_bias; //Colour is ambient times light colour (with a shadow bias.)
                    }
                    new_colour = clip(new_colour, ZERO, 1.0); //clamp the colour
                    colours.push_back(new_colour); //add it to the list of colours
                    
                }//After looping through all the lights
#endif
                glm::dvec3 colour = merge_colours(colours); //merge the colour that we calculated for each light
                if(gamma_val != 1.0) { //gammify colours
                    colour = gammify(colour, gamma_val);
                }
                colour = clip(colour, ZERO, 1.0); //clamp the colour
                draw(image, j, k, colour); //Fill in the pixel.
            } else { //If the initial ray did not intersect with any object.
                draw(image, j, k, glm::dvec3(0)); //fill the pixel with black.
            }
        }
    } //After colouring in the entire grid.

#if antialiasing
    //If anti aliasing is on,
    width = cam -> getWidth();
    height = cam -> getHeight();
    cimg_library::CImg<double> final_image(width, height, 1, 3, 0); //create an image of the final size
    downsize(image, final_image, width, height, aa_multi); //downsize the image
    std::string filename = get_name(path+"render-down",".bmp"); //Get the image's timestamped name.
    final_image.save(filename.c_str()); //Save the image
#else
    //If anti aliasing is off,
    std::string filename = get_name(path+"render-",".bmp"); //Get the image's timestamped name.
    image.save(filename.c_str()); //Save the image
#endif
    //Notify the user and save the final details of the log.
    message = get_name("Saved scene at ", "");
    tell_user(message);
#if mot_log
    log << "Saved as: " + filename + "\n"
    + "Image size (WxH): " + std::to_string(width) + "x" + std::to_string(height) + "\n"
    +  message + "\n"
    + "Configuration: \n"
    + "  Shadow Colour Bias: " + std::to_string(shadow_colour_bias) + "\n"
    + "  Gamma Value: " + std::to_string(gamma_val) + "\n";
#endif
    
#if (soft_shadow && mot_log)
    log << + "  Number of Samples: " + std::to_string(num_samples) + "\n";
#endif
    
    
#if show_pic //if show_pic is on
    std::string command = "open -a Preview " + filename;
    system(command.c_str()); //Open the image with Preview (macOS).
#endif
}
    
int main (int argc, char *argv[]) {
    std::string command = "mkdir " + path;
    system(command.c_str());
    
    
    //        std::vector<std::string> scenes = {"scenes/scene1.txt", "scenes/scene2.txt", "scenes/scene3.txt", "scenes/scene4.txt", "scenes/scene5.txt"};
    std::vector<std::string> scenes = {"scenes/scene5.txt"};
    std::vector<int> sample_range = {5, 25};
    
#if soft_shadow
    for (int j = 0; j < sample_range.size(); j++) {
        for (int i = 0; i < scenes.size(); i++) {
            tell_user("Drawing scene " + scenes[i] + " with " + std::to_string(sample_range[j]) + " samples.");
            render_scene(scenes[i].c_str(), sample_range[j]);
        }
    }
#else
    for (int i = 0; i < scenes.size(); i++) {
        tell_user("Drawing scene " + scenes[i]);
        render_scene(scenes[i].c_str());
    }
#endif
    return 0;
}
