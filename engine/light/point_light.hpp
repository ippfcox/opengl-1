#pragma once

#include "light.hpp"
#include "../object/object.hpp"

class PointLight : public Light, public Object
{
public:
    PointLight();
    ~PointLight();

    float k2, k1, kc;
};