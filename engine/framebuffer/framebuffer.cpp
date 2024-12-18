#include "framebuffer.hpp"
#include "../wrapper.hpp"

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
    : width_(width), height_(height)
{
    color_attachment_ = new Texture();
    color_attachment_->InitColorAttachment(width_, height_);
    depth_stencil_attachment_ = new Texture();
    depth_stencil_attachment_->InitDepthStencilAttachment(width_, height_);

    GL_CALL(glGenFramebuffers(1, &fbo_));
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
    GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_->GetTexture(), 0));
    GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_attachment_->GetTexture(), 0));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SPDLOG_ERROR("framebuffer is not complete");
        throw std::runtime_error("framebuffer init failed");
    }

    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));
}

Framebuffer::~Framebuffer()
{
}

Texture *Framebuffer::GetColorAttachment() const
{
    return color_attachment_;
}

Texture *Framebuffer::GetDepthStencilAttachment() const
{
    return depth_stencil_attachment_;
}

GLuint Framebuffer::GetFramebuffer() const
{
    return fbo_;
}