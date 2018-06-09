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
    glm::vec3 colour(1.0f);
    glm::vec3 light_vec = glm::normalize(light.getPosition()-hit.pos);
    
    //Diffuse calculation
    float theta = glm::dot(light_vec,m_norm);
    glm::vec3 diffuse = glm::max(theta, 0.0f) * m_dif;
    
    //Specular calculation
    glm::vec3 veiw_dir = glm::normalize(hit.pos-camPos);
    glm::vec3 reflect = glm::reflect(veiw_dir, m_norm);
    float alpha = glm::dot(reflect, veiw_dir);
    glm::vec3 specular = glm::pow(glm::max(alpha, 0.0f), m_shi) * m_spe;
    
    //Get calc colour
    colour = (diffuse+specular) * light.getColour();
    
    //clip [0,1]
//    colour = clip(colour, 0.0f, 1.0f);
    
    return colour;
}

