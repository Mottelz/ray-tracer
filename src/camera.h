/**
 * @file camera.h
 * @author Mottel Zirkind
 * @brief The header for the camera class
 **/

#pragma once
#include "libs.h"

/**
 * @brief Camera stores all of the camera related informaion.
 * This class is loaded first and defies all of the scene related info.
 **/
class Camera {
    float m_height;
    float m_width;
    glm::vec3 m_pos;
    float m_fov;
    int m_focal;
    float m_asp;
public:
    Camera(glm::vec3 pos, int fov, float asp, int focal); /**< @brief The only constructor. Calculates the width and height based on the given info. */
    void print(); /**< @brief Prints the camera's details as a nicely formatted string. */
    int getHeight(); /**< @brief Returns canvas's height as an int. */
    int getWidth(); /**< @brief Returns canvas's width as an int. */
    glm::vec3 getPosition(); /**< @brief Returns camera's position. */
    inline int getFocal() { return m_focal; }; /**< @brief Returns camera's focal length. */
};
