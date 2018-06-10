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
    Light(glm::vec3 pos); /**< Constructor for when you only know the position. This makes the light white and fully bright. */
    Light(glm::vec3 pos, glm::vec3 colour); /**< Constructor for when you know everything about the light. */
    inline glm::vec3 getPosition() const { return m_pos; }; /**< Returns light's position. */
    inline glm::vec3 getColour() const { return m_colour; }; /**< Returns light's colour. */
    void print(); /**< Prints the light's stats in a nicely formatted string. */
};
