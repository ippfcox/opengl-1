#pragma once

#include "common.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    int InitBySrc(const std::string &vertex_shader_src, const std::string &fragment_shader_src);
    int InitByFilename(const std::string &vertex_shader_filename, const std::string &fragment_shader_filename);

    void Use();
    void End();

    void SetUniform(const std::string &uniform_name, int i0);
    void SetUniform(const std::string &uniform_name, float f0);
    void SetUniform(const std::string &uniform_name, float f0, float f1, float f2);
    void SetUniform(const std::string &uniform_name, glm::vec3 v);
    void SetUniform(const std::string &uniform_name, float f0, float f1, float f2, float f3);
    void SetUniform(const std::string &uniform_name, glm::vec4 v);
    void SetUniform(const std::string &uniform_name, const float *matrix4fv);
    void SetUniform(const std::string &uniform_name, glm::mat4 m);
    void SetUniform(const std::string &uniform_name, bool b);

private:
    GLuint program_;
};