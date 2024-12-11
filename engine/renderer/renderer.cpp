#include "renderer.hpp"
#include "../object/material/phong_material.hpp"
#include "../wrapper.hpp"

Renderer::Renderer()
{
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/shaders/phong.vs", "assets/shaders/phong.fs");
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
        switch (mesh->material->type)
        {
        case MaterialType::Phong:
        {
            auto material = dynamic_cast<PhongMaterial *>(mesh->material);
            //   texture
            material->diffuse->Bind(0);                    // bind texture to texture unit
            shader->SetUniform("unif_diffuse_sampler", 0); // bind texture sampler to texture unit
            material->specular_mask->Bind(1);
            shader->SetUniform("unif_specular_mask_sampler", 1);
            //   mvp
            shader->SetUniform("unif_model", mesh->GetModelMatrix());
            shader->SetUniform("unif_view", camera->GetViewMatrix());
            shader->SetUniform("unif_projection", camera->GetProjectionMatrix());
            auto normal_matrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
            shader->SetUniform("unif_normal_matrix", normal_matrix);
            //   light
            shader->SetUniform("unif_light_direction", directional_light->direction);
            shader->SetUniform("unif_light_color", directional_light->color);
            shader->SetUniform("unif_specular_intensity", directional_light->specular_intensity);
            shader->SetUniform("unif_specular_shiness", material->shiness);
            shader->SetUniform("unif_ambient_color", ambient_light->color);
            //   camera
            shader->SetUniform("unif_camera_position", camera->position);
        }
        break;
        default:
            continue;
            break;
        }

        // bind vao
        GL_CALL(glBindVertexArray(mesh->geometry->GetVAO()));

        // render
        GL_CALL(glDrawElements(GL_TRIANGLES, mesh->geometry->GetIndicesCount(), GL_UNSIGNED_INT, 0));

        // finish
        // GL_CALL(glBindVertexArray(GL_NONE));
        // shader->End();
    }
}