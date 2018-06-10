#include "camera.h"

Camera::Camera(glm::vec3 pos, int fov, float asp, int focal) {
    m_pos = pos;
    m_fov = glm::radians((float) fov);
    m_asp = asp;
    m_focal = focal;
    m_height = floor(2*m_focal*glm::tan(m_fov/2));
    m_width = floor(m_height*m_asp);
}

void Camera::print() {
    printf("Camera Specs: \n  Position: %s\n  FoV: %f\n  Aspect Ratio: %f\n  Focal Length: %i\n  Dimensions (WxH): %fx%f\n",
           glm::to_string(m_pos).c_str(),
           m_fov,
           m_asp,
           m_focal,
           m_width,
           m_height);
}

Camera::~Camera() {
    std::cout << "Deleting camera..." << std::endl;
}

void Camera::setAll(glm::vec3 pos, int fov, float asp, int focal) { 
    m_pos = pos;
    m_fov = glm::radians((float) fov);
    m_asp = asp;
    m_focal = focal;
    m_height = 2*m_focal*glm::tan(m_fov/2);
    m_width = m_height*m_asp;
}

int Camera::getHeight() {
    return ((int) m_height);
}

int Camera::getWidth() {
    return ((int) m_width);
}

glm::vec3 Camera::getPosition() { 
    return m_pos;
}

