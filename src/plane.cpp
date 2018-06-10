#include "plane.h"

Plane::Plane(glm::vec3 norm, glm::vec3 pos, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shine) {
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
    float a = glm::dot(m_norm, m_pos);
    float b = glm::dot(m_norm, r.org);
    float c = glm::dot(m_norm, r.dir);
    
    //If plane is behind the camera
    if(c == 0) {
        toRet.contact = false;
        return toRet;
    }
    
    float t = (a-b)/c;
    
    if (t < 0) {
        toRet.contact = false;
        return toRet;
    }
    
    //compare to zero
    toRet.contact = true;
    toRet.pos = r.org + r.dir*(t+T_BIAS);
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

glm::vec3 Plane::getColour() {
    return m_amb;
}


glm::vec3 Plane::getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) {
    glm::vec3 colour = calculate_colour(light.getPosition(), light.getColour(), camPos, hit.pos, m_norm, m_dif, m_spe, m_amb, m_shi);
    return colour;
}

