#pragma once

#include "common.hpp"

class Texture
{
public:
    Texture();
    ~Texture();

    bool InitByFilename(const std::string &path);

    void Bind(int texture_unit);

private:
    GLuint texture_;
    int width_;
    int height_;
};