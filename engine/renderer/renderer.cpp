#include "renderer.hpp"
#include "../object/material/phong_material.hpp"
#include "../object/material/pure_material.hpp"
#include "../wrapper.hpp"

Renderer::Renderer()
{
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/shaders/phong.vs", "assets/shaders/phong.fs");
    shader_map_[MaterialType::Phong] = phong_shader;
    auto pure_shader = new Shader();
    pure_shader->InitByFilename("assets/shaders/pure.vs", "assets/shaders/pure.fs");
    shader_map_[MaterialType::Pure] = pure_shader;
}

Renderer::~Renderer()
{
}

void Renderer::SetClearColor(glm::vec3 clear_color)
{
    clear_color_ = clear_color;
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
}

void Renderer::Render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    SpotLight *spot_light,
    DirectionalLight *directional_light,
    const std::vector<PointLight *> point_lights,
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
            shader->SetUniform("unif_spot_light.color", spot_light->color);
            shader->SetUniform("unif_spot_light.specular_intensity", spot_light->specular_intensity);
            shader->SetUniform("unif_spot_light.position", spot_light->GetPosition());
            shader->SetUniform("unif_spot_light.direction", spot_light->direction);
            shader->SetUniform("unif_spot_light.inner_cone", glm::cos(glm::radians(spot_light->inner_angle)));
            shader->SetUniform("unif_spot_light.outer_cone", glm::cos(glm::radians(spot_light->outer_angle)));
            for (int i = 0; i < point_lights.size(); ++i)
            {
                std::string name = "unif_point_lights[" + std::to_string(i) + "].";
                shader->SetUniform(name + "color", point_lights[i]->color);
                shader->SetUniform(name + "specular_intensity", point_lights[i]->specular_intensity);
                shader->SetUniform(name + "position", point_lights[i]->GetPosition());
                shader->SetUniform(name + "k2", point_lights[i]->k2);
                shader->SetUniform(name + "k1", point_lights[i]->k1);
                shader->SetUniform(name + "kc", point_lights[i]->kc);
            }
            shader->SetUniform("unif_directional_light.color", directional_light->color);
            shader->SetUniform("unif_directional_light.specular_intensity", directional_light->specular_intensity);
            shader->SetUniform("unif_directional_light.direction", directional_light->direction);
            shader->SetUniform("unif_specular_shiness", material->shiness);
            shader->SetUniform("unif_ambient_color", ambient_light->color);
            //   camera
            shader->SetUniform("unif_camera_position", camera->position);
        }
        break;
        case MaterialType::Pure:
        {
            auto material = dynamic_cast<PureMaterial *>(mesh->material);
            //   color
            shader->SetUniform("unif_point_light_color", material->color);
            //   mvp
            shader->SetUniform("unif_model", mesh->GetModelMatrix());
            shader->SetUniform("unif_view", camera->GetViewMatrix());
            shader->SetUniform("unif_projection", camera->GetProjectionMatrix());
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