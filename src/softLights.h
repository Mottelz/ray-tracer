#pragma once
#include "libs.h"
#include "util.h"
#include "light.h"

class SoftLight : public Light {
    double m_rad;
    std::vector<glm::dvec3> m_pos_range;
public:
    SoftLight(glm::dvec3 pos, glm::dvec3 colour, int samples,  double rad = 3.0);
    glm::dvec3 generatePosition();
    std::vector<glm::dvec3> getPositionRange();
};
