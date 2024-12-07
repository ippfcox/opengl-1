#pragma once

#include "../common.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
};

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    GLuint GetVAO();

    virtual unsigned int GetIndicesCount();

protected:
    bool SetupVertices(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

private:
    GLuint vao_{0};
    GLuint vbo_{0};
    GLuint ebo_{0};

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
};