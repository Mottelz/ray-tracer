#pragma once
#include "libs.h"
#include "structs.h"
#include "light.h"

class Object {
public:
    virtual glm::vec3 getPostion() = 0;
    virtual Intersect intersect(const Ray& r) = 0;
    virtual void print() = 0;
    virtual glm::vec3 getColour() = 0;
    virtual glm::vec3 getColour(const Light& light, const Intersect& hit, const glm::vec3& camPos) = 0;
};
