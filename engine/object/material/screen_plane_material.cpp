#include "screen_plane_material.hpp"

ScreenPlaneMaterial::ScreenPlaneMaterial(int screen_width, int screen_height)
    : screen_width_(screen_width), screen_height_(screen_height)
{
    type = MaterialType::ScreenPlane;
}

ScreenPlaneMaterial::~ScreenPlaneMaterial()
{
}