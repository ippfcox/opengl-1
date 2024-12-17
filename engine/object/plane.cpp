#include "plane.hpp"

Plane::Plane(float width, float height)
{
    float w2 = width / 2;
    float h2 = height / 2;
    std::vector<Vertex> vertices = {
        {glm::vec3{-w2, -h2, 0}, glm::vec2{0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}},
        {glm::vec3{+w2, -h2, 0}, glm::vec2{1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}},
        {glm::vec3{+w2, +h2, 0}, glm::vec2{1.0f, 1.0f}, glm::vec3{0.0f, 0.0f, 1.0f}},
        {glm::vec3{-w2, +h2, 0}, glm::vec2{0.0f, 1.0f}, glm::vec3{0.0f, 0.0f, 1.0f}},
    };

    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    SetupVertices(vertices, indices);
}

Plane::~Plane()
{
}
