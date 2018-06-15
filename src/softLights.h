#pragma once
#include "libs.h"
#include "util.h"
#include "light.h"

class SoftLight : public Light {
    int m_rad;
public:
    SoftLight(glm::dvec3 pos, glm::dvec3 colour, int rad = 10);
    glm::dvec3 getPosition() const;
};
