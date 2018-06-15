#include "softLights.h"


SoftLight::SoftLight(glm::dvec3 pos, glm::dvec3 colour, int sample, double rad) : Light(pos, colour) {
    m_rad = rad;
    for (int i = 0; i <  sample; i++) {
        m_pos_range.push_back(getPosition());
    }
}

std::vector<glm::dvec3> SoftLight::getPositionRange() {
    return m_pos_range;
}


glm::dvec3 SoftLight::getPosition() {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist1(0, m_rad);
    std::uniform_int_distribution<int> uniform_dist2(0, 360);
    int within_radius = uniform_dist1(e1);
    int random_degree1 = uniform_dist2(e1);
    int random_degree2 = uniform_dist2(e1);
    
    glm::dvec3 centre = m_pos;
    glm::dvec3 toRet(0.0);
    
    double alpha = glm::radians((double)random_degree1);
    double beta = glm::radians((double)random_degree2);
    
    toRet.x = within_radius*cos(beta)*sin(alpha) + centre.x;
    toRet.y = within_radius*sin(beta)*sin(alpha) + centre.y;
    toRet.z = within_radius*cos(alpha) + centre.z;
    
    return toRet;
}
