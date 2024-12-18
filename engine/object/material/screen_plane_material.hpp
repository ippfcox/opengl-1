#pragma once

#include "material.hpp"
#include "../../texture.hpp"

class ScreenPlaneMaterial : public Material
{
public:
    ScreenPlaneMaterial(int screen_width, int screen_height);
    ~ScreenPlaneMaterial();

    Texture *screen{nullptr};
    int screen_width_{0};
    int screen_height_{0};
};