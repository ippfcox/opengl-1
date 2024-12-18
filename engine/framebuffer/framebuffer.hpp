#pragma once

#include "../common.hpp"
#include "../texture.hpp"

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    Texture *GetColorAttachment() const;
    Texture *GetDepthStencilAttachment() const;
    GLuint GetFramebuffer() const;

private:
    int width_{0};
    int height_{0};
    GLuint fbo_{GL_NONE};
    Texture *color_attachment_{nullptr};
    Texture *depth_stencil_attachment_{nullptr};
};