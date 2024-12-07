#include "cube.hpp"

Cube::Cube(float edge_length)
{
    float l = edge_length / 2.0f;
    std::vector<Vertex> vertices = {
        // Front face (z = 0.5)
        {glm::vec3{-l, -l, l}, glm::vec2{0.0f, 0.0f}}, // 0
        {glm::vec3{l, -l, l}, glm::vec2{1.0f, 0.0f}},  // 1
        {glm::vec3{l, l, l}, glm::vec2{1.0f, 1.0f}},   // 2
        {glm::vec3{-l, l, l}, glm::vec2{0.0f, 1.0f}},  // 3

        // Back face (z = -0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{1.0f, 0.0f}}, // 4
        {glm::vec3{l, -l, -l}, glm::vec2{0.0f, 0.0f}},  // 5
        {glm::vec3{l, l, -l}, glm::vec2{0.0f, 1.0f}},   // 6
        {glm::vec3{-l, l, -l}, glm::vec2{1.0f, 1.0f}},  // 7

        // Left face (x = -0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{0.0f, 0.0f}}, // 4
        {glm::vec3{-l, -l, l}, glm::vec2{1.0f, 0.0f}},  // 0
        {glm::vec3{-l, l, l}, glm::vec2{1.0f, 1.0f}},   // 3
        {glm::vec3{-l, l, -l}, glm::vec2{0.0f, 1.0f}},  // 7

        // Right face (x = 0.5)
        {glm::vec3{l, -l, -l}, glm::vec2{1.0f, 0.0f}}, // 5
        {glm::vec3{l, -l, l}, glm::vec2{0.0f, 0.0f}},  // 1
        {glm::vec3{l, l, l}, glm::vec2{0.0f, 1.0f}},   // 2
        {glm::vec3{l, l, -l}, glm::vec2{1.0f, 1.0f}},  // 6

        // Top face (y = 0.5)
        {glm::vec3{-l, l, -l}, glm::vec2{0.0f, 1.0f}}, // 7
        {glm::vec3{l, l, -l}, glm::vec2{1.0f, 1.0f}},  // 6
        {glm::vec3{l, l, l}, glm::vec2{1.0f, 0.0f}},   // 2
        {glm::vec3{-l, l, l}, glm::vec2{0.0f, 0.0f}},  // 3

        // Bottom face (y = -0.5)
        {glm::vec3{-l, -l, -l}, glm::vec2{0.0f, 0.0f}}, // 4
        {glm::vec3{l, -l, -l}, glm::vec2{1.0f, 0.0f}},  // 5
        {glm::vec3{l, -l, l}, glm::vec2{1.0f, 1.0f}},   // 1
        {glm::vec3{-l, -l, l}, glm::vec2{0.0f, 1.0f}},  // 0
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Top face
        16, 17, 18, 16, 18, 19,
        // Bottom face
        20, 21, 22, 20, 22, 23};

    SetupVertices(vertices, indices);
}

Cube::~Cube()
{
}