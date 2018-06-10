/**
 * @file light.h
 * @author Mottel Zirkind
 * @brief The header for the light class
 **/

#pragma once
#include "libs.h"
#include "util.h"
#include "structs.h"

/**
 * @brief Light class could basically be a struct.
 * This is a really minimal class that just store all the light's data. 
 **/

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
