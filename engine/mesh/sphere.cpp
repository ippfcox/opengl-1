#include "sphere.hpp"

Sphere::Sphere(float radius)
{
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};

    int latitude_count = 10;
    int longtitude_count = 10;

    float delta_phi = glm::pi<float>() / latitude_count;
    float delta_theta = 2 * glm::pi<float>() / longtitude_count;
    float delta_u = 1.0f / longtitude_count;
    float delta_v = 1.0f / latitude_count;

    for (int i = 0; i <= latitude_count; ++i)
    {
        for (int j = 0; j <= longtitude_count; ++j)
        {
            float x = radius * std::sin(i * delta_phi) * std::cos(j * delta_theta);
            float y = radius * std::cos(i * delta_phi);
            float z = radius * std::sin(i * delta_phi) * std::sin(j * delta_theta);
            vertices.push_back(Vertex{glm::vec3{x, y, z}, glm::vec2{j * delta_u, i * delta_v}});

            if (i == latitude_count || j == longtitude_count)
                continue;

            unsigned int p1 = i * (longtitude_count + 1) + j;
            unsigned int p2 = (i + 1) * (longtitude_count + 1) + j;
            unsigned int p3 = p1 + 1;
            unsigned int p4 = p2 + 1;
            indices.insert(indices.end(), {p1, p3, p2, p2, p3, p4});
        }
    }

    SetupVertices(vertices, indices);
}

Sphere::~Sphere()
{
}