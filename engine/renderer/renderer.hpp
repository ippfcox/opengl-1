#pragma once

#include <vector>
#include <unordered_map>
#include "../object/scene.hpp"
#include "../object/mesh.hpp"
#include "../shader.hpp"
#include "../../application/camera/camera.hpp" // [fixme] refactor needed
#include "../light/directional_light.hpp"
#include "../light/point_light.hpp"
#include "../light/ambient_light.hpp"
#include "../light/spot_light.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void SetClearColor(glm::vec3 clear_color);

    void Render(
        const std::vector<Mesh *> &meshes,
        Camera *camera,
        SpotLight *spot_light,
        DirectionalLight *directional_light,
        const std::vector<PointLight *> point_lights,
        AmbientLight *ambient_light);

    void Render(
        Scene *scene,
        Camera *camera,
        SpotLight *spot_light,
        DirectionalLight *directional_light,
        const std::vector<PointLight *> point_lights,
        AmbientLight *ambient_light);

    void RenderObject(
        Object *object,
        Camera *camera,
        SpotLight *spot_light,
        DirectionalLight *directional_light,
        const std::vector<PointLight *> point_lights,
        AmbientLight *ambient_light);

private:
    std::unordered_map<MaterialType, Shader *> shader_map_;
    glm::vec3 clear_color_;
};