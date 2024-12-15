#include "renderer.hpp"
#include "../object/material/phong_material.hpp"
#include "../object/material/color_material.hpp"
#include "../object/material/depth_material.hpp"
#include "../wrapper.hpp"

Renderer::Renderer()
{
    // [todo] lazy load
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/shaders/phong.vs", "assets/shaders/phong.fs");
    shader_map_[MaterialType::Phong] = phong_shader;
    auto color_shader = new Shader();
    color_shader->InitByFilename("assets/shaders/color.vs", "assets/shaders/color.fs");
    shader_map_[MaterialType::Color] = color_shader;
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

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
        SetDepthState(mesh->material);
        SetPolygonOffsetState(mesh->material);
        SetStencilState(mesh->material);

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
        case MaterialType::Color:
        {
            auto material = dynamic_cast<ColorMaterial *>(mesh->material);
            //   color
            shader->SetUniform("unif_color", material->color);
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

    int index = 0;
    for (auto child : object->GetChildren())
    {
        RenderObject(child, camera, spot_light, directional_light, point_lights, ambient_light);
        index++;
    }
}

void Renderer::SetDepthState(Material *material)
{
    if (material->enable_depth_test)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(material->depth_func);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    if (material->enable_depth_write)
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
}

void Renderer::SetPolygonOffsetState(Material *material)
{
    if (material->enable_polygon_offset)
    {
        glEnable(material->polygen_offset_type);
        glPolygonOffset(material->polygon_offset_factor, material->polygon_offset_unit);
    }
    else
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}

void Renderer::SetStencilState(Material *material)
{
    if (material->enable_stencil_test)
    {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(material->stencil_op_s_fail, material->stencil_op_s_pass_z_fail, material->stencil_op_s_pass_z_pass);
        glStencilMask(material->stencil_mask);
        glStencilFunc(material->stencil_func_func, material->stencil_func_ref, material->stencil_func_mask);
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
}
