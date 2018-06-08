#pragma once
#include "libs.h"
#include "util.h"
#include "structs.h"

class Light {
    glm::vec3 m_pos;
    glm::vec3 m_colour;
public:
    Light(glm::vec3 pos);
    Light(glm::vec3 pos, glm::vec3 colour);
    ~Light();
    inline glm::vec3 getPosition() const { return m_pos; };
    inline glm::vec3 getColour() const { return m_colour; };
    void print();
};
