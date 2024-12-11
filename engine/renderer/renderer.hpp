#pragma once

#include <vector>
#include <unordered_map>
#include "../object/mesh.hpp"
#include "../shader.hpp"
#include "../../application/camera/camera.hpp" // [fixme] refactor needed
#include "../light/directional_light.hpp"
#include "../light/ambient_light.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Render(
        const std::vector<Mesh *> &meshes,
        Camera *camera,
        DirectionalLight *directional_light,
        AmbientLight *ambient_light);

private:
    std::unordered_map<MaterialType, Shader *> shader_map_;
};