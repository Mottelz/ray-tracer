/**
 * @file structs.h
 * @author Mottel Zirkind
 * @brief Home to the structs.
 **/

#pragma once
#include "libs.h"

/**
 * @brief A Ray that can be fired from things.
 **/

struct Ray {
    glm::vec3 org; /**< The origin of the ray. #org. */
    glm::vec3 dir; /**< The direction of the ray. Make sure to normalize!!! #dir. */
};

/**
 * @brief A struct that tracks the point of intersection.
 **/
struct Intersect {
    glm::vec3 pos; /**< The point of the intersection. #pos. */
    bool contact; /**< Whether or not there's been contact. #contact. */
    int thing; /**< The index of the in scene object that was hit. #thing. */
};
