#pragma once

#include "../../common.hpp"

enum class MaterialType
{
    Phong,
    PhongOpacityMask,
    Color,
    Depth,
    ScreenPlane,
    Cube,
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
    GLfloat polygon_offset_factor{0.0f};
    GLfloat polygon_offset_unit{0.0f};

    bool enable_stencil_test{false};
    GLenum stencil_op_s_fail{GL_KEEP};
    GLenum stencil_op_s_pass_z_fail{GL_KEEP};
    GLenum stencil_op_s_pass_z_pass{GL_KEEP};
    GLuint stencil_mask{0xFF};
    GLenum stencil_func_func{GL_ALWAYS};
    GLenum stencil_func_ref{0};
    GLenum stencil_func_mask{0xFF};

    bool enable_blend{false};
    GLenum blend_src_factor{GL_SRC_ALPHA};
    GLenum blend_dst_factor{GL_ONE_MINUS_SRC_ALPHA};
    float opacity{1.0f};

    bool enable_face_cull{false};
    GLenum face_front{GL_CCW};
    GLenum face_cull{GL_BACK};
};
