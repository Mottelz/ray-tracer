/**
 * @file object.h
 * @author Mottel Zirkind
 * @brief The header for the object class.
 **/

#pragma once
#include "libs.h"
#include "structs.h"
#include "light.h"


/**
 * @brief A pure virtual parent class for any physical object in this scene.
 **/
class Object {
public:
    virtual glm::dvec3 getPostion() = 0;
    virtual Intersect intersect(const Ray& r) = 0;
    virtual void print() = 0;
    virtual glm::dvec3 getColour() = 0;
    virtual glm::dvec3 getColour(const glm::dvec3 lightColour, const glm::dvec3 lightPos, const Intersect& hit, const glm::dvec3& camPos) = 0;
};
