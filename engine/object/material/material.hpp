#pragma once

#include "../../common.hpp"

enum class MaterialType
{
    Phong,
    Pure,
    Depth,
};

class Material
{
public:
    Material();
    virtual ~Material();

    MaterialType type;

    bool depth_test{true};
    GLenum depth_func{GL_LESS};
    bool depth_mask{true};
};
