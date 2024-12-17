#pragma once

#include "material.hpp"
#include "../../texture.hpp"

class ScreenPlaneMaterial : public Material
{
public:
    ScreenPlaneMaterial();
    ~ScreenPlaneMaterial();

    Texture *screen{nullptr};
};