#include "sphere.h"

Sphere::Sphere(glm::vec3 pos, int rad, glm::vec3 amb, float dif, float spe, float shi) {
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
    float c = (r.org.x-m_pos.x)*(r.org.x-m_pos.x)+(r.org.y-m_pos.y)*(r.org.y-m_pos.y)+(r.org.z-m_pos.z)*(r.org.z-m_pos.z)-glm::pow(m_rad, 2);
    
    //the bit that goes under the sqrt. If this is negative we can stop.
    float under = glm::pow(b, 2) - 4*c;
    if (under < 0) {
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
    toRet.pos = r.org + r.dir*v;
    toRet.contact = true;
    return toRet;
}

void Sphere::print() { 
    printf("Sphere Specs: \n  Position: %s\n  Rad: %i\n  Ambient:%s\n  Diffuse: %f\n  Specular: %f\n  Shine: %f\n",
           glm::to_string(m_pos).c_str(),
           m_rad,
           glm::to_string(m_amb).c_str(),
           m_dif,
           m_spe,
           m_shi);
}

glm::vec3 Sphere::getColour() {
    return m_amb;
}


glm::vec3 Sphere::getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) {
    glm::vec3 colour(1.0f);
    glm::vec3 light_vec = glm::normalize(hit.pos-light.getPosition());
    glm::vec3 norm = glm::normalize(hit.pos-m_pos);
    
    
    
    //Diffuse calculation
    float theta = glm::dot(light_vec,norm);
    float diffuse = glm::max(theta, 0.0f) * m_dif;
    
    //Specular calculation
    glm::vec3 veiw_dir = glm::normalize(hit.pos - camPos);
    glm::vec3 reflect = glm::reflect(veiw_dir, norm);
    float alpha = glm::dot(reflect, veiw_dir);
    float specular = glm::pow(glm::max(alpha, 0.0f), m_shi);
    
    colour = ((diffuse+specular) * m_amb * light.getColour()) + m_amb;
    
    //clip to [1,0]
    colour.x = (colour.x > 1) ? 1 : colour.x;
    colour.y = (colour.y > 1) ? 1 : colour.y;
    colour.z = (colour.z > 1) ? 1 : colour.z;
    
    colour.x = (colour.x < 0) ? 0 : colour.x;
    colour.y = (colour.y < 0) ? 0 : colour.y;
    colour.z = (colour.z < 0) ? 0 : colour.z;
    
    return colour;
}
