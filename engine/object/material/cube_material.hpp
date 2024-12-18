#pragma once

#include "material.hpp"
#include "../../texture.hpp"

class CubeMaterial : public Material
{
public:
    CubeMaterial();
    ~CubeMaterial();

    Texture *diffuse;
};