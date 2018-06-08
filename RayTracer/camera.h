#pragma once
#include "libs.h"

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
    void setAll(glm::vec3 pos, int fov, float asp, int focal);
    ~Camera();
    int getHeight();
    int getWidth();
    glm::vec3 getPosition();
    inline int getFocal() { return m_focal; };
};
