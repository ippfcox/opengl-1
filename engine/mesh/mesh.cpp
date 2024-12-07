#include "mesh.hpp"
#include "../wrapper.hpp"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

GLuint Mesh::GetVAO()
{
    return vao_;
}

bool Mesh::SetupVertices(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
{
    vertices_ = vertices;
    indices_ = indices;

    GL_CALL(glGenBuffers(1, &vbo_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

    GL_CALL(glGenBuffers(1, &ebo_));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GL_CALL(glGenVertexArrays(1, &vao_));
    GL_CALL(glBindVertexArray(vao_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position))));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, uv))));
    GL_CALL(glBindVertexArray(GL_NONE));

    return true;
}

unsigned int Mesh::GetIndicesCount()
{
    return indices_.size();
}
