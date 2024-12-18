#include "cube.hpp"

Cube::Cube(float edge_length)
{
    float l = edge_length / 2.0f;
    std::vector<Vertex> vertices = {
        // Front face (z = 0.5)
        {glm::vec3{-l, -l, +l}, glm::vec2{0.0f, 0.0f}, glm::vec3{+0.0f, +0.0f, +1.0f}},
        {glm::vec3{+l, -l, +l}, glm::vec2{1.0f, 0.0f}, glm::vec3{+0.0f, +0.0f, +1.0f}},
        {glm::vec3{+l, +l, +l}, glm::vec2{1.0f, 1.0f}, glm::vec3{+0.0f, +0.0f, +1.0f}},
        {glm::vec3{-l, +l, +l}, glm::vec2{0.0f, 1.0f}, glm::vec3{+0.0f, +0.0f, +1.0f}},
        // Back face (z = -0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{1.0f, 0.0f}, glm::vec3{+0.0f, +0.0f, -1.0f}},
        {glm::vec3{+l, -l, -l}, glm::vec2{0.0f, 0.0f}, glm::vec3{+0.0f, +0.0f, -1.0f}},
        {glm::vec3{+l, +l, -l}, glm::vec2{0.0f, 1.0f}, glm::vec3{+0.0f, +0.0f, -1.0f}},
        {glm::vec3{-l, +l, -l}, glm::vec2{1.0f, 1.0f}, glm::vec3{+0.0f, +0.0f, -1.0f}},
        // Left face (x = -0.5)
        {glm::vec3{-l, +l, -l}, glm::vec2{0.0f, 1.0f}, glm::vec3{+0.0f, +1.0f, +0.0f}},
        {glm::vec3{+l, +l, -l}, glm::vec2{1.0f, 1.0f}, glm::vec3{+0.0f, +1.0f, +0.0f}},
        {glm::vec3{+l, +l, +l}, glm::vec2{1.0f, 0.0f}, glm::vec3{+0.0f, +1.0f, +0.0f}},
        {glm::vec3{-l, +l, +l}, glm::vec2{0.0f, 0.0f}, glm::vec3{+0.0f, +1.0f, +0.0f}},
        // Right face (x = 0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{1.0f, 1.0f}, glm::vec3{+0.0f, -1.0f, +0.0f}},
        {glm::vec3{+l, -l, -l}, glm::vec2{0.0f, 1.0f}, glm::vec3{+0.0f, -1.0f, +0.0f}},
        {glm::vec3{+l, -l, +l}, glm::vec2{0.0f, 0.0f}, glm::vec3{+0.0f, -1.0f, +0.0f}},
        {glm::vec3{-l, -l, +l}, glm::vec2{1.0f, 0.0f}, glm::vec3{+0.0f, -1.0f, +0.0f}},
        // Top face (y = 0.5)
        {glm::vec3{+l, -l, -l}, glm::vec2{1.0f, 0.0f}, glm::vec3{+1.0f, +0.0f, +0.0f}},
        {glm::vec3{+l, +l, -l}, glm::vec2{1.0f, 1.0f}, glm::vec3{+1.0f, +0.0f, +0.0f}},
        {glm::vec3{+l, +l, +l}, glm::vec2{0.0f, 1.0f}, glm::vec3{+1.0f, +0.0f, +0.0f}},
        {glm::vec3{+l, -l, +l}, glm::vec2{0.0f, 0.0f}, glm::vec3{+1.0f, +0.0f, +0.0f}},
        // Bottom face (y = -0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{0.0f, 0.0f}, glm::vec3{-1.0f, +0.0f, +0.0f}},
        {glm::vec3{-l, +l, -l}, glm::vec2{1.0f, 0.0f}, glm::vec3{-1.0f, +0.0f, +0.0f}},
        {glm::vec3{-l, +l, +l}, glm::vec2{1.0f, 1.0f}, glm::vec3{-1.0f, +0.0f, +0.0f}},
        {glm::vec3{-l, -l, +l}, glm::vec2{0.0f, 1.0f}, glm::vec3{-1.0f, +0.0f, +0.0f}},
    };

    // maybe not all correct, culling back face by ccw works not correctly
    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,        // Front face
        4, 5, 6, 6, 7, 4,        // Back face
        8, 9, 10, 10, 11, 8,     // Left face
        12, 13, 14, 14, 15, 12,  // Right face
        16, 17, 18, 18, 19, 16,  // Top face
        20, 21, 22, 22, 23, 20}; // Bottom face

    SetupVertices(vertices, indices);
}

Cube::~Cube()
{
}