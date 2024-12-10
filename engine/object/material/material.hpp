#pragma once

#include "../../common.hpp"

enum class MaterialType
{
    Phong,
};

class Material
{
public:
    Material();
    virtual ~Material();

    MaterialType type;
};
