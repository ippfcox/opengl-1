#include "wrapper.hpp"
#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture()
{
}

Texture::~Texture()
{
    if (texture_)
        glDeleteBuffers(1, &texture_);
}

bool Texture::InitByFilename(const std::string &path)
{
    stbi_set_flip_vertically_on_load(1);
    int channels;
    stbi_uc *data = stbi_load(path.c_str(), &width_, &height_, &channels, STBI_rgb_alpha);

    GL_CALL(glGenTextures(1, &texture_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    stbi_image_free(data);

    return true;
}

void Texture::Bind(int texture_unit)
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + texture_unit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
}