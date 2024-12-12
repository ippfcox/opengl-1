#pragma once

#include "light.hpp"
#include "../object/object.hpp"

class PointLight : public Light, public Object
{
public:
    PointLight();
    ~PointLight();
};