#include "light.h"

Light::Light(glm::dvec3 pos, glm::dvec3 colour) { 
    m_pos = pos;
    m_colour = colour;
}

//If none given, colour set to white.
Light::Light(glm::dvec3 pos) { 
    m_pos = pos;
    m_colour = glm::dvec3(1.0);
}

void Light::print() {
    printf("Light specs:\n  Colour: %s\n  Position: %s\n", glm::to_string(m_colour).c_str(), glm::to_string(m_pos).c_str());
};

glm::dvec3 Light::getPosition() { 
    return m_pos;
}


