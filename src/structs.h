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
    glm::vec3 org; /**< @brief The origin of the ray. #org. */
    glm::vec3 dir; /**< @brief The direction of the ray. Make sure to normalize!!! #dir. */
};

/**
 * @brief A struct that tracks the point of intersection.
 **/
struct Intersect {
    glm::vec3 pos; /**< @brief The point of the intersection. #pos. */
    bool contact; /**< @brief Whether or not there's been contact. #contact. */
    int thing; /**< @brief The index of the in scene object that was hit. #thing. */
};
