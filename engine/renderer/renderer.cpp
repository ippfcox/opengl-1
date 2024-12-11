#include "renderer.hpp"

Renderer::Renderer()
{
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/phong.vs", "assets/phong.fs");
    shader_map_[MaterialType::Phong] = phong_shader;
}

Renderer::~Renderer()
{
}

void Renderer::Render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    DirectionalLight *directional_light,
    AmbientLight *ambient_light)
{
    // set state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    for (auto &mesh : meshes)
    {
        Shader *shader = shader_map_[mesh->material->type]; // checkerror
        // use shader
        shader->Use();

        // update uniform

        // bind vao

        // render
        shader->End();
    }
}