/**
 * @file softLights.
 * @author Mottel Zirkind
 * @brief The header for the softLights class.
 **/

#pragma once
#include "libs.h"
#include "util.h"
#include "light.h"

/**
 * @brief SoftLight class for an area light.
 * Child of #Light. Creates a range of random positions.
 **/

class SoftLight : public Light {
    double m_rad; /**< @brief The radius of our area light. */
    std::vector<glm::dvec3> m_pos_range; /**< @brief The sample range. */
public:
    SoftLight(glm::dvec3 pos, glm::dvec3 colour, int samples,  double rad = 5.0); /**< @brief The constructor. Uses #generatePosition for the number of samples. */
    glm::dvec3 generatePosition(); /**< @brief Returns a random poision */
    std::vector<glm::dvec3> getPositionRange(); /**< @brief Returns the position range. */
};
