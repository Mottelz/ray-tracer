/**
 * @file plane.h
 * @author Mottel Zirkind
 * @brief The header for the plane class.
 **/

#pragma once
#include "object.h"
#include "structs.h"
#include "util.h"
/**
 * @brief Child of Object.
 * This class represents infinte planes that can be placed in the scene.
 **/
class Plane: public Object {
    glm::vec3 m_norm;
    glm::vec3 m_pos;
    glm::vec3 m_amb;
    glm::vec3 m_dif;
    glm::vec3 m_spe;
    float m_shi;
public:
    Plane(glm::vec3 norm, glm::vec3 pos, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shine); /**< @brief Just the basic constructor. */
    inline glm::vec3 getPostion() override { return m_pos; }; /**< @brief Returns the position of the plane. */
    Intersect intersect(const Ray &r, float bias) override; /**< @brief Checks if a ray intersects with the plane. Returns an Intersect with contact true or false. */
    void print() override; /**< @brief Prints a nicely formatted string with all the plane's stats. */
    glm::vec3 getColour() override; /**< @brief Returns the ambient colour. */
    glm::vec3 getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) override; /**< @brief Runs #calculate_colour and returns the result. */
};
