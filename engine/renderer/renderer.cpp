#include "renderer.hpp"
#include "../object/material/phong_material.hpp"
#include "../object/material/pure_material.hpp"
#include "../object/material/depth_material.hpp"
#include "../wrapper.hpp"

Renderer::Renderer()
{
    // [todo] lazy load
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/shaders/phong.vs", "assets/shaders/phong.fs");
    shader_map_[MaterialType::Phong] = phong_shader;
    auto pure_shader = new Shader();
    pure_shader->InitByFilename("assets/shaders/pure.vs", "assets/shaders/pure.fs");
    shader_map_[MaterialType::Pure] = pure_shader;
    auto depth_shader = new Shader();
    depth_shader->InitByFilename("assets/shaders/depth.vs", "assets/shaders/depth.fs");
    shader_map_[MaterialType::Depth] = depth_shader;
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
    Scene *scene,
    Camera *camera,
    SpotLight *spot_light,
    DirectionalLight *directional_light,
    const std::vector<PointLight *> point_lights,
    AmbientLight *ambient_light)
{
    // set state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderObject(scene, camera, spot_light, directional_light, point_lights, ambient_light);
}

void Renderer::RenderObject(
    Object *object,
    Camera *camera,
    SpotLight *spot_light,
    DirectionalLight *directional_light,
    const std::vector<PointLight *> point_lights,
    AmbientLight *ambient_light)
{
    if (object->GetType() == ObjectType::Mesh)
    {
        auto mesh = dynamic_cast<Mesh *>(object);
        // depth
        if (mesh->material->enable_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(mesh->material->depth_func);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        if (mesh->material->enable_depth_write)
        {
            glDepthMask(GL_TRUE);
        }
        else
        {
            glDepthMask(GL_FALSE);
        }
        // polygon_offset
        if (mesh->material->enable_polygon_offset)
        {
            glEnable(mesh->material->polygen_offset_type);
            glPolygonOffset(mesh->material->polygon_offset_factor, mesh->material->polygon_offset_unit);
        }
        else
        {
            glDisable(GL_POLYGON_OFFSET_FILL);
            glDisable(GL_POLYGON_OFFSET_LINE);
        }

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
            material->diffuse->Bind();                                                // bind texture to texture unit
            shader->SetUniform("unif_diffuse_sampler", material->diffuse->GetUnit()); // bind texture sampler to texture unit
            // material->specular_mask->Bind(1);
            // shader->SetUniform("unif_specular_mask_sampler", 1);
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
        case MaterialType::Depth:
        {
            auto material = dynamic_cast<DepthMaterial *>(mesh->material);
            shader->SetUniform("unif_near", camera->near_);
            shader->SetUniform("unif_far", camera->far_);
            shader->SetUniform("unif_model", mesh->GetModelMatrix());
            shader->SetUniform("unif_view", camera->GetViewMatrix());
            shader->SetUniform("unif_projection", camera->GetProjectionMatrix());
        }
        break;
        default:
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

    for (auto child : object->GetChildren())
    {
        RenderObject(child, camera, spot_light, directional_light, point_lights, ambient_light);
    }
}
