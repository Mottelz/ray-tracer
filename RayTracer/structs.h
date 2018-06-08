#pragma once
#include "libs.h"

struct Ray {
    glm::vec3 org;
    glm::vec3 dir;
};

struct Intersect {
    glm::vec3 pos;
    bool contact;
    int thing;
};
