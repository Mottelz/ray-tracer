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
    Camera(glm::vec3 pos, int fov, float asp, int focal);
    void print();
    ~Camera();
    int getHeight();
    int getWidth();
    glm::vec3 getPosition();
    inline int getFocal() { return m_focal; };
};
