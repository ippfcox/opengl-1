#pragma once

#include "../../common.hpp"

enum class MaterialType
{
    Phong,
    Color,
    Depth,
};

class Material
{
public:
    Material();
    virtual ~Material();

    MaterialType type;

    bool enable_depth_test{true};
    GLenum depth_func{GL_LESS};
    bool enable_depth_write{true};

    bool enable_polygon_offset{false};
    GLenum polygen_offset_type{GL_POLYGON_OFFSET_FILL};
    float polygon_offset_factor{0.0f};
    float polygon_offset_unit{0.0f};
};
