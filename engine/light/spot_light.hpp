#pragma once

#include "light.hpp"
#include "../object/object.hpp"

class SpotLight : public Light, public Object
{
public:
    SpotLight();
    ~SpotLight();

    glm::vec3 direction{-1.0f};
    float inner_angle;
    float outer_angle;
};