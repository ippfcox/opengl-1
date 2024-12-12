#pragma once

#include "../../common.hpp"

enum class MaterialType
{
    Phong,
    Pure,
};

class Material
{
public:
    Material();
    virtual ~Material();

    MaterialType type;
};
