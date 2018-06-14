#include "plane.h"

Plane::Plane(glm::dvec3 norm, glm::dvec3 pos, glm::dvec3 amb, glm::dvec3 dif, glm::dvec3 spe, double shine) {
    m_norm = norm;
    m_pos = pos;
    m_amb = amb;
    m_dif = dif;
    m_spe = spe;
    m_shi = shine;
}

Intersect Plane::intersect(const Ray &r) {
    //The formula
    //t = a-b/c
    //a = n.p0
    //b = n.org
    //c = n.d
    Intersect toRet;
    
    //caculate a, b, & c
    double a = glm::dot(m_norm, m_pos);
    double b = glm::dot(m_norm, r.org);
    double c = glm::dot(m_norm, r.dir);
    
    //If plane is behind the camera
    if(c == 0) {
        toRet.contact = false;
        return toRet;
    }
    
    double t = (a-b)/c;
    
    if (t < ZERO) {
        toRet.contact = false;
        return toRet;
    }
    
    //compare to zero
    toRet.contact = true;
    toRet.pos = r.org + r.dir*(t);
    return toRet;
}



void Plane::print() { 
    printf("Plane Specs: \n  Norm: %s\n  Position: %s\n  Ambient: %s\n  Diffuse: %s\n  Specular: %s\n  Shine: %f\n",
           glm::to_string(m_norm).c_str(),
           glm::to_string(m_pos).c_str(),
           glm::to_string(m_amb).c_str(),
           glm::to_string(m_dif).c_str(),
           glm::to_string(m_spe).c_str(),
           m_shi);
}

glm::dvec3 Plane::getColour() {
    return m_amb;
}


glm::dvec3 Plane::getColour(const Light &light, const Intersect& hit, const glm::dvec3& camPos) {
    glm::dvec3 colour = calculate_colour(light.getPosition(), light.getColour(), camPos, hit.pos, m_norm, m_dif, m_spe, m_amb, m_shi);
    return colour;
}

