#include "sphere.h"

Sphere::Sphere(glm::vec3 pos, int rad, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi) {
    m_pos = pos;
    m_rad = rad;
    m_amb = amb;
    m_dif = dif;
    m_spe = spe;
    m_shi = shi;
}

Intersect Sphere::intersect(const Ray &r) { 
    float t0 = 0, t1 = 0; //The intersection point
    Intersect toRet;
    toRet.contact = false;
    float v;
    //Calculate A, B, C before finding the roots
    float a = glm::length(r.dir);
    
    if(a > 1.1f || a < 0.9f ) {
        tell_user("Something is wrong with a");
    }
    
    float b = 2*(r.dir.x*(r.org.x-m_pos.x)+r.dir.y*(r.org.y-m_pos.y)+r.dir.z*(r.org.z-m_pos.z));
    float c = glm::pow((r.org.x-m_pos.x), 2)+glm::pow((r.org.y-m_pos.y), 2)+glm::pow((r.org.z-m_pos.z), 2)-glm::pow(m_rad, 2);
    
    //the bit that goes under the sqrt. If this is negative we can stop.
    float under = glm::pow(b, 2) - 4*c;
    if (under < glm::epsilon<float>()) {
        toRet.contact = false;
        return toRet;
    }
    //only do the sqrt calculation once
    float sq = glm::sqrt(under);
    
    //get t0 and t1
    t0 = (-b-sq)/2;
    t1 = (-b+sq)/2;
    
    //decide which to returns
    if (t0 < glm::epsilon<float>() && t1 < glm::epsilon<float>()) {
        toRet.contact = false;
        return toRet;
    } else if (t0 > glm::epsilon<float>() && t1 > glm::epsilon<float>()) {
        v = (t0 > t1) ? t1 : t0;
    } else {
        v = (t0 > t1) ? t0 : t1;
    }
    
    //Calculate and return point.
    toRet.pos = r.org + r.dir*(v+T_BIAS);
    toRet.contact = true;
    return toRet;
}

void Sphere::print() { 
    printf("Sphere Specs: \n  Position: %s\n  Rad: %i\n  Ambient:%s\n  Diffuse: %s\n  Specular: %s\n  Shine: %f\n",
           glm::to_string(m_pos).c_str(),
           m_rad,
           glm::to_string(m_amb).c_str(),
           glm::to_string(m_dif).c_str(),
           glm::to_string(m_spe).c_str(),
           m_shi);
}

glm::vec3 Sphere::getColour() {
    return m_amb;
}


glm::vec3 Sphere::getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) {
    glm::vec3 norm = glm::normalize(hit.pos-m_pos);
    glm::vec3 colour = calculate_colour(light.getPosition(), light.getColour(), camPos, hit.pos, norm, m_dif, m_spe, m_amb, m_shi);
    return colour;
}
