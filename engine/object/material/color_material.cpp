#include "color_material.hpp"

ColorMaterial::ColorMaterial()
{
    type = MaterialType::Color;
}

ColorMaterial::ColorMaterial(glm::vec4 color)
    : color(color)
{
    type = MaterialType::Color;
}

ColorMaterial::~ColorMaterial()
{
}