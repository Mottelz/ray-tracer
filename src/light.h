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
    glm::dvec3 m_pos;
    glm::dvec3 m_colour;
public:
    Light(glm::dvec3 pos); /**< @brief Constructor for when you only know the position. This makes the light white and fully bright. */
    Light(glm::dvec3 pos, glm::dvec3 colour); /**< @brief Constructor for when you know everything about the light. */
    inline glm::dvec3 getPosition() const { return m_pos; }; /**< @brief Returns light's position. */
    inline glm::dvec3 getColour() const { return m_colour; }; /**< @brief Returns light's colour. */
    void print(); /**< @brief Prints the light's stats in a nicely formatted string. */
};
