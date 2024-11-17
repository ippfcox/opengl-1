#pragma once

#include "common.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    bool Use();
    void End();

private:
    GLuint program_;
};