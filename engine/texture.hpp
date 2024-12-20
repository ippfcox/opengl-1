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
    bool InitCubeMapByFilename(const std::vector<std::string> &paths); // right, left, top, bottom, back, front
    bool InitByMemoryImage(const void *image_data, int size);
    bool InitByMemoryRGBA(const void *rgba_data, int width, int height);
    bool InitColorAttachment(unsigned int width, unsigned int height);
    bool InitDepthStencilAttachment(unsigned int width, unsigned int height);

    void SetUnit(int texture_unit);
    int GetUnit() const;
    void Bind();
    GLuint GetTexture() const;

private:
    GLuint texture_{0};
    int width_{0};
    int height_{0};
    int texture_unit_{0};
    GLenum texture_target_{GL_TEXTURE_2D};

    static std::unordered_map<std::string, Texture *> cache_;
};