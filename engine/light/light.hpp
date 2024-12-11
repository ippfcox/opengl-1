#pragma once

#include "../common.hpp"

class Light
{
public:
    Light();
    ~Light();

    glm::vec3 color{1.0f};
    float specular_intensity{1.0f};
};