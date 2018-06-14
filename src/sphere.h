/**
 * @file sphere.h
 * @author Mottel Zirkind
 * @brief The header for the sphere class
 **/

#pragma once
#include "object.h"
#include "util.h"

/**
 * @brief Child of Object.
 * This class represents Spheres that can be placed in the scene.
 **/

class Sphere: public Object {
    glm::dvec3 m_pos;
    int m_rad;
    glm::dvec3 m_amb;
    glm::dvec3 m_dif;
    glm::dvec3 m_spe;
    double m_shi;
public:
    Sphere(glm::dvec3 pos, int rad, glm::dvec3 amb, glm::dvec3 dif, glm::dvec3 spe, double shi); /**< @brief Just the basic constructor. */
    inline glm::dvec3 getPostion() override { return m_pos; }; /**< @brief Returns the position of the sphere's centre. */
    Intersect intersect(const Ray &r) override; /**< @brief Checks if a ray intersects with the sphere. Returns an Intersect with contact true or false. */
    void print() override; /**< @brief Prints a nicely formatted string with all the sphere's stats. */
    glm::dvec3 getColour() override; /**< @brief Returns the ambient colour. */
    glm::dvec3 getColour(const Light &light, const Intersect& hit, const glm::dvec3& camPos) override; /**< @brief Calculates the norm for the point of contact then runs #calculate_colour */
};
