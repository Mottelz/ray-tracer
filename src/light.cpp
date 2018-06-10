#include "light.h"

Light::Light(glm::vec3 pos, glm::vec3 colour) { 
    m_pos = pos;
    m_colour = colour;
}

//If none given, colour set to white.
Light::Light(glm::vec3 pos) { 
    m_pos = pos;
    m_colour = glm::vec3(1.0f);
}

Light::~Light() {
    tell_user("Deleting Light...");
};

void Light::print() {
    printf("Light specs:\n  Colour: %s\n  Position: %s\n", glm::to_string(m_colour).c_str(), glm::to_string(m_pos).c_str());
};

