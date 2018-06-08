#pragma once
#include "object.h"
#include "structs.h"
#include "util.h"

class Plane: public Object {
    glm::vec3 m_norm;
    glm::vec3 m_pos;
    glm::vec3 m_amb;
    float m_dif;
    float m_spe;
    float m_shi;
public:
    Plane(glm::vec3 norm, glm::vec3 pos, glm::vec3 amb, float dif, float spe, float shine);
    inline glm::vec3 getPostion() override { return m_pos; };
    Intersect intersect(const Ray &r) override;
    void print() override;
    glm::vec3 getColour() override;
    glm::vec3 getColour(const Light &light, const Intersect& hit, const glm::vec3& camPos) override;
};
