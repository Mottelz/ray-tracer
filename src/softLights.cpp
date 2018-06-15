#include "softLights.h"


SoftLight::SoftLight(glm::dvec3 pos, glm::dvec3 colour, int rad) : Light(pos, colour) {
    m_rad = rad;
}

glm::dvec3 SoftLight::getPosition() const {
    glm::dvec3 centre = Light::getPosition();
    glm::dvec3 toRet(0.0);
    
    int r = m_rad*(std::rand()/RAND_MAX);
    double alpha = ZERO * rand();
    double beta = ZERO * rand();
    
    toRet.x = r*cos(beta)*sin(alpha) - centre.x;
    toRet.y = r*sin(beta)*sin(alpha) - centre.y;
    toRet.z = r*cos(alpha) - centre.z;
    
    return toRet;
}
