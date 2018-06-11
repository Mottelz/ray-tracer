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
    glm::dvec3 m_norm;
    glm::dvec3 m_pos;
    glm::dvec3 m_amb;
    glm::dvec3 m_dif;
    glm::dvec3 m_spe;
    double m_shi;
public:
    Plane(glm::dvec3 norm, glm::dvec3 pos, glm::dvec3 amb, glm::dvec3 dif, glm::dvec3 spe, double shine); /**< @brief Just the basic constructor. */
    inline glm::dvec3 getPostion() override { return m_pos; }; /**< @brief Returns the position of the plane. */
    Intersect intersect(const Ray &r, double bias) override; /**< @brief Checks if a ray intersects with the plane. Returns an Intersect with contact true or false. */
    void print() override; /**< @brief Prints a nicely formatted string with all the plane's stats. */
    glm::dvec3 getColour() override; /**< @brief Returns the ambient colour. */
    glm::dvec3 getColour(const Light &light, const Intersect& hit, const glm::dvec3& camPos) override; /**< @brief Runs #calculate_colour and returns the result. */
};
