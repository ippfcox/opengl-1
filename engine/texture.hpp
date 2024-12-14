#pragma once

#include "common.hpp"

class Texture
{
public:
    Texture();
    ~Texture();

    // cached
    static Texture *CreateByFilename(const std::string &path);
    static Texture *CreateByMemoryImage(const void *image_data, int size, const std::string &cache_key);
    static Texture *CreateByMemoryRGBA(const void *rgba_data, int width, int height, const std::string &cache_key);

    bool InitByFilename(const std::string &path);
    bool InitByMemoryImage(const void *image_data, int size);
    bool InitByMemoryRGBA(const void *rgba_data, int width, int height);

    void SetUnit(int texture_unit);
    int GetUnit();
    void Bind();

private:
    GLuint texture_;
    int width_;
    int height_;
    int texture_unit_;

    static std::unordered_map<std::string, Texture *> cache_;
};