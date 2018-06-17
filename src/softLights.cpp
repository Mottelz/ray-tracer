#include "softLights.h"

//Basic constructor
SoftLight::SoftLight(glm::dvec3 pos, glm::dvec3 colour, int sample, double rad) : Light(pos, colour) {
    m_rad = rad;
    for (int i = 0; i <  sample; i++) {
        m_pos_range.push_back(generatePosition());
    }
}

//Return position range
std::vector<glm::dvec3> SoftLight::getPositionRange() {
    return m_pos_range;
}

//Get a random position
glm::dvec3 SoftLight::generatePosition() {
    
    std::random_device r; //The random seed.
    std::default_random_engine e1(r()); //The random device
    std::uniform_int_distribution<int> uniform_dist1(0, m_rad); //Random range, between 0 and radius.
    std::uniform_int_distribution<int> uniform_dist2(0, 360); //Random range, between 0 and 360. (All possibly degrees of a circle.)
    
    int within_radius = uniform_dist1(e1); //Get a random point within the radius.
    int random_degree1 = uniform_dist2(e1); //Get a random degree.
    int random_degree2 = uniform_dist2(e1); //Get another random degree.
    
    glm::dvec3 centre = m_pos; //Get the centre of our sephrical light.
    
    //Convert the random degrees to radians
    double alpha = glm::radians((double)random_degree1);
    double beta = glm::radians((double)random_degree2);
    
    //Get random Cartesian coordinates (see http://mathworld.wolfram.com/SphericalCoordinates.html for details).
    
    glm::dvec3 toRet(0.0); //The empty position.
    toRet.x = within_radius*cos(beta)*sin(alpha) + centre.x; //Get the random x.
    toRet.y = within_radius*sin(beta)*sin(alpha) + centre.y; //Get the random y.
    toRet.z = within_radius*cos(alpha) + centre.z; //Get the random z.
    
    return toRet;
}
