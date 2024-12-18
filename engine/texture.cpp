#include "wrapper.hpp"
#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::unordered_map<std::string, Texture *> Texture::cache_{};

Texture::Texture()
{
}

Texture::~Texture()
{
    if (texture_)
        glDeleteBuffers(1, &texture_);
}

Texture *Texture::CreateByFilename(const std::string &path)
{
    if (cache_.contains(path))
        return cache_[path];

    auto texture = new Texture();
    texture->InitByFilename(path);

    cache_[path] = texture;
    return texture;
}

Texture *Texture::CreateByMemoryImage(const void *image_data, int size, const std::string &cache_key)
{
    if (cache_.contains(cache_key))
        return cache_[cache_key];

    auto texture = new Texture();
    texture->InitByMemoryImage(image_data, size);

    cache_[cache_key] = texture;
    return texture;
}

Texture *Texture::CreateByMemoryRGBA(const void *rgba_data, int width, int height, const std::string &cache_key)
{
    if (cache_.contains(cache_key))
        return cache_[cache_key];

    auto texture = new Texture();
    texture->InitByMemoryRGBA(rgba_data, width, height);

    cache_[cache_key] = texture;
    return texture;
}

bool Texture::InitByFilename(const std::string &path)
{
    stbi_set_flip_vertically_on_load(1);
    int channels;
    stbi_uc *rgba_data = stbi_load(path.c_str(), &width_, &height_, &channels, STBI_rgb_alpha);
    if (!rgba_data)
    {
        SPDLOG_ERROR("stbi_load {} failed: {}", path, stbi_failure_reason());
        return false;
    }

    InitByMemoryRGBA(rgba_data, width_, height_);

    stbi_image_free(rgba_data);

    return true;
}

bool Texture::InitCubeMapByFilename(const std::vector<std::string> &paths)
{
    texture_target_ = GL_TEXTURE_CUBE_MAP;

    GL_CALL(glGenTextures(1, &texture_));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture_));
    stbi_set_flip_vertically_on_load(1);

    for (int i = 0; i < paths.size(); ++i)
    {
        int channels, width, height;
        stbi_uc *data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
        {
            SPDLOG_ERROR("stbi_load {} failed: {}", paths[i], stbi_failure_reason());
            continue;
        }

        GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

        stbi_image_free(data);
    }
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT));

    return true;
}

bool Texture::InitByMemoryImage(const void *image_data, int size)
{
    stbi_set_flip_vertically_on_load(1);
    int channels;
    stbi_uc *rgba_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(image_data), size, &width_, &height_, &channels, STBI_rgb_alpha);

    InitByMemoryRGBA(rgba_data, width_, height_);

    stbi_image_free(rgba_data);

    return true;
}

bool Texture::InitByMemoryRGBA(const void *rgba_data, int width, int height)
{
    width_ = width;
    height_ = height;

    GL_CALL(glGenTextures(1, &texture_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_data));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    return true;
}

bool Texture::InitColorAttachment(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;

    GL_CALL(glGenTextures(1, &texture_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    return true;
}

bool Texture::InitDepthStencilAttachment(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;

    GL_CALL(glGenTextures(1, &texture_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width_, height_, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    return true;
}

void Texture::SetUnit(int texture_unit)
{
    texture_unit_ = texture_unit;
}

int Texture::GetUnit() const
{
    return texture_unit_;
}

void Texture::Bind()
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + texture_unit_));
    GL_CALL(glBindTexture(texture_target_, texture_));
}

GLuint Texture::GetTexture() const
{
    return texture_;
}
