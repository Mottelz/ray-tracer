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
    glm::vec3 m_pos; 
    int m_rad;
    glm::vec3 m_amb;
    glm::vec3 m_dif;
    glm::vec3 m_spe;
    float m_shi;
public:
    Sphere(glm::vec3 pos, int rad, glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi); /**< Just the basic constructor. */
    inline glm::vec3 getPostion() override { return m_pos; }; /**< Returns the position of the sphere's centre. */
    Intersect intersect(const Ray &r) override; /**< Checks if a ray intersects with the sphere. Returns an Intersect with contact true or false. */
    void print() override; /**< Prints a nicely formatted string with all the sphere's stats. */
    glm::vec3 getColour() override; /**< Returns the ambient colour. */
    glm::vec3 getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) override; /**< Calculates the norm for the point of contact then runs #calculate_colour */
};
