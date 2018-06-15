#include "libs.h" //basic libraries (glm, iostrem, etc.)
#include "util.h" //stray functions that would clutter the main (draw_square, draw, etc.)
#include "sceneload.h" //The function that loads the scene
#include "softLights.h"

std::string scene = "scenes/scene5.txt";
double colour_bias = 1.0;
double shadow_colour_bias = 0.75;
double gamma_val = 1.0;
double ray_org_bias = 1.0;
int num_samples = 5;
#if antialiasing
int aa_rad = 1;
int aa_multi = aa_rad+2;
#endif


//Main function
int main(int argc, const char * argv[]) {
    std::vector<Object*> things; //init empty vector of physical objects/things in the scene.
    std::vector<Light*> lights; //init empty vector of lights
    Camera* cam; //init camera pointer
    std::string message;

#if mot_log
    //A log. Stores points and whether or not they hit.
    std::ofstream log;
    log.open(get_name("/Users/mottelzirkind/Desktop/results/log-",".txt"));
#endif

    //Load the scene
    if(!load_scene(scene, things, lights, cam)) {
        tell_user("Failed to load scene!");
    } else {
        message = get_name("Loaded scene at ", "");
#if mot_log
        log << "Scene:" + scene + "\n" +message + "\n";
#endif
        tell_user(message);
    };

#if antialiasing
    //Get the image width & height
    int width = cam->getWidth()*aa_multi;
    int height = cam->getHeight()*aa_multi;
#else
    //Get the image width & height
    int width = cam->getWidth();
    int height = cam->getHeight();
#endif

#if mot_log
    log << "Grid size (WxH): " + std::to_string(width) + "x" + std::to_string(height) + "\n";
#endif


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
#if antialiasing
            r.dir.z = -(cam -> getFocal()*aa_multi - r.org.z); //Z = negative the focal length minus origin.
#else
            r.dir.z = -(cam -> getFocal() - r.org.z); //Z = negative the focal length minus origin.
#endif
            r.dir = glm::normalize(r.dir); //Normalize the direction.

            //for every object. check if there's a hit and if it's close.
            for (int i = 0; i < things.size(); i++) { //Loop through obejcts.
                Intersect t_hit = things[i] -> intersect(r); //Check for intersection.
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

            if(hit.contact) {//If we made contact calulate the colour/shadow.
                std::vector<glm::dvec3> colours;
                colours.push_back(things[hit.thing] -> getColour());
                int light_intensity = 0;
                for (int i = 0; i < lights.size(); i++) { //Loop through every light in the scene.
#if soft_shadow
                    for (int s = 0; s < num_samples; s++) {
                        Ray shadow_ray; //Create the shadow ray.
                        shadow_ray.org = hit.pos; //Set shadow ray origin to the light.
                        shadow_ray.dir = lights[i] -> getPosition() - hit.pos; //Set the shadow ray direction by light's position minus point of contact.
                        shadow_ray.dir = glm::normalize(shadow_ray.dir); //Normalize the direction.
                        shadow_ray.org = shadow_ray.org+(shadow_ray.dir*ray_org_bias);
                        bool in_shadow = false;
                        for (int m = 0; m < things.size() && !in_shadow; m++) { //Loop through objects and check if one is closer.
                            Intersect shadow_hit = things[m] -> intersect(shadow_ray); //Get the shadow ray intersection
                            if (!shadow_hit.contact) { //If there is an intersection
                                light_intensity += 1;
                            } else {
                                in_shadow = true;
                            }
                        } //After looping through all of the objects for a given light.
                        glm::dvec3 new_colour(0.0);
                        new_colour = things[hit.thing] -> getColour(lights[i]->getColour(), lights[i] -> getPosition(), hit, cam ->getPosition()); //The new colour.
//                        tell_user("Light Intensity: " + std::to_string(light_intensity));
                        new_colour = clip(new_colour, ZERO, 1.0);
                        colours.push_back(new_colour);
                    }
#else
                    Ray shadow_ray; //Create the shadow ray.
                    shadow_ray.org = hit.pos; //Set shadow ray origin to the light.
                    shadow_ray.dir = lights[i] -> getPosition() - hit.pos; //Set the shadow ray direction by light's position minus point of contact.
                    shadow_ray.dir = glm::normalize(shadow_ray.dir); //Normalize the direction.
                    shadow_ray.org = shadow_ray.org+(shadow_ray.dir*ray_org_bias);
                    bool in_shadow = false; //Store if this is in shadow. Starts off in light.

                    for (int m = 0; m < things.size(); m++) { //Loop through objects and check if one is closer.
                        Intersect shadow_hit = things[m] -> intersect(shadow_ray); //Get the shadow ray intersection
                        if (shadow_hit.contact) { //If there is an intersection
                            in_shadow = true; //then we are in shadow.
                            break;
                        }
                    } //After looping through all of the objects for a given light.
                    glm::dvec3 new_colour(0.0);
                    if (!in_shadow || typeid(*things[hit.thing]) == typeid(Sphere)) { //If we're not in shadow,
                        new_colour = things[hit.thing] -> getColour(lights[i]->getColour(), lights[i] -> getPosition(), hit, cam ->getPosition()) * colour_bias; //The new colour.
                    } else {
                        new_colour = things[hit.thing] -> getColour() * lights[i] -> getColour() * shadow_colour_bias; //The new colour.
                    }
                    new_colour = clip(new_colour, ZERO, 1.0);
                    colours.push_back(new_colour);
#endif
                }
                glm::dvec3 colour = merge_colours(colours);
#if soft_shadow
                colour = colour*((double)light_intensity/((double)num_samples*lights.size()));
#endif
                if(gamma_val != 1.0) {
                    colour = gammify(colour, gamma_val);
                }
                colour = clip(colour, ZERO, 1.0);
                draw(image, j, k, colour); //Draw the pixel.
            } else { //If the initial ray did not intersect with any object.
                draw(image, j, k, glm::dvec3(0)); //And fill the pixel with Burgundy, so we know that something was added to the pixel.
            }
        }
    }
#if antialiasing
    width = cam -> getWidth();
    height = cam -> getHeight();
    cimg_library::CImg<double> final_image(width, height, 1, 3, 0);
    downsize(image, final_image, width, height, aa_multi);
    std::string filename = get_name("/Users/mottelzirkind/Desktop/results/render-down",".bmp");
    final_image.save(filename.c_str()); //get_name used so that the image includes a timestamp.
#else
    std::string filename = get_name("/Users/mottelzirkind/Desktop/results/render-",".bmp"); //get_name used so that the image includes a timestamp.
    image.save(filename.c_str());
#endif

    message = get_name("Saved scene at ", "");
    tell_user(message);
#if mot_log
    log << "Saved as: " + filename + "\n"
    + "Image size (WxH): " + std::to_string(width) + "x" + std::to_string(height) + "\n"
    +  message + "\n"
    + "Configuration: \n"
    + "  Colour Bias: " + std::to_string(colour_bias) + "\n"
    + "  Shadow Colour Bias: " + std::to_string(shadow_colour_bias) + "\n"
    + "  Gamma Value: " + std::to_string(gamma_val) + "\n"
    + "  Shadow Ray Origin Bias: " + std::to_string(ray_org_bias) + "\n";
#endif

    //If we want to see the picture displayed then display it with CImg.
    //Display the rendered image on screen
#if show_pic
    std::string command = "open -a Preview " + filename;
    system(command.c_str());
#endif
    return 0; //And end the program.
}
