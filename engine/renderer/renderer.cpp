#include <algorithm>
#include "renderer.hpp"
#include "../object/material/phong_material.hpp"
#include "../object/material/phong_opacity_mask_material.hpp"
#include "../object/material/color_material.hpp"
#include "../object/material/depth_material.hpp"
#include "../object/material/screen_plane_material.hpp"
#include "../wrapper.hpp"

Renderer::Renderer()
{
    // [todo] lazy load
    auto phong_shader = new Shader();
    phong_shader->InitByFilename("assets/shaders/phong.vs", "assets/shaders/phong.fs");
    shader_map_[MaterialType::Phong] = phong_shader;
    auto phong_opacity_mask_shader = new Shader();
    phong_opacity_mask_shader->InitByFilename("assets/shaders/phong_opacity_mask.vs", "assets/shaders/phong_opacity_mask.fs");
    shader_map_[MaterialType::PhongOpacityMask] = phong_opacity_mask_shader;
    auto color_shader = new Shader();
    color_shader->InitByFilename("assets/shaders/color.vs", "assets/shaders/color.fs");
    shader_map_[MaterialType::Color] = color_shader;
    auto depth_shader = new Shader();
    depth_shader->InitByFilename("assets/shaders/depth.vs", "assets/shaders/depth.fs");
    shader_map_[MaterialType::Depth] = depth_shader;
    auto screen_plane_shader = new Shader();
    screen_plane_shader->InitByFilename("assets/shaders/screen_plane.vs", "assets/shaders/screen_plane.fs");
    shader_map_[MaterialType::ScreenPlane] = screen_plane_shader;
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
    AmbientLight *ambient_light,
    GLuint fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // default depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    // default polygon offset
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    // default stencil test
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);
    // default color blend
    glDisable(GL_BLEND);
    // default face culling
    glDisable(GL_CULL_FACE);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    std::vector<Mesh *> opacity_objects{};
    std::vector<Mesh *> transparent_objects{};
    // push opacity objects and transparent objects
    ProjectObjects(scene, opacity_objects, transparent_objects);
    // sort transpraent objects, render from far to near
    std::sort(
        transparent_objects.begin(),
        transparent_objects.end(),
        [&](const Mesh *a, const Mesh *b) -> bool {
            auto a_position_in_camera = camera->GetViewMatrix() * a->GetModelMatrix() * glm::vec4(0.0, 0.0, 0.0, 1.0);
            auto b_position_in_camera = camera->GetViewMatrix() * b->GetModelMatrix() * glm::vec4(0.0, 0.0, 0.0, 1.0);
            return a_position_in_camera.z < b_position_in_camera.z;
        });

    for (auto object : opacity_objects)
        RenderObject(object, camera, spot_light, directional_light, point_lights, ambient_light);
    for (auto object : transparent_objects)
        RenderObject(object, camera, spot_light, directional_light, point_lights, ambient_light);
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
        SetBlendState(mesh->material);
        SetFaceCullingState(mesh->material);

        Shader *shader = shader_map_[mesh->material->type];
        if (!shader)
        {
            SPDLOG_ERROR("cannot find shader");
            return;
        }
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
            shader->SetUniform("unif_opacity", material->opacity);
            shader->SetUniform("unif_ambient_color", ambient_light->color);
            //   camera
            shader->SetUniform("unif_camera_position", camera->position);
        }
        break;
        case MaterialType::PhongOpacityMask:
        {
            auto material = dynamic_cast<PhongOpacityMaskMaterial *>(mesh->material);

            //   texture
            material->diffuse->Bind();                                                // bind texture to texture unit
            shader->SetUniform("unif_diffuse_sampler", material->diffuse->GetUnit()); // bind texture sampler to texture unit
            material->opacity_mask->Bind();
            shader->SetUniform("unif_opacity_mask_sampler", material->opacity_mask->GetUnit());
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
            shader->SetUniform("unif_opacity", material->opacity);
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
            shader->SetUniform("unif_opacity", material->opacity);
            //   mvp
            shader->SetUniform("unif_model", mesh->GetModelMatrix());
            shader->SetUniform("unif_view", camera->GetViewMatrix());
            shader->SetUniform("unif_projection", camera->GetProjectionMatrix());
        }
        break;
        case MaterialType::Depth:
        {
            shader->SetUniform("unif_near", camera->near_);
            shader->SetUniform("unif_far", camera->far_);
            shader->SetUniform("unif_model", mesh->GetModelMatrix());
            shader->SetUniform("unif_view", camera->GetViewMatrix());
            shader->SetUniform("unif_projection", camera->GetProjectionMatrix());
        }
        break;
        case MaterialType::ScreenPlane:
        {
            auto material = dynamic_cast<ScreenPlaneMaterial *>(mesh->material);
            material->screen->Bind();
            shader->SetUniform("unif_screen_sampler", material->screen->GetUnit());
            shader->SetUniform("unif_screen_width", material->screen_width_);
            shader->SetUniform("unif_screen_height", material->screen_height_);
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

void Renderer::SetBlendState(Material *material)
{
    if (material->enable_blend)
    {
        glEnable(GL_BLEND);
        glBlendFunc(material->blend_src_factor, material->blend_dst_factor);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void Renderer::SetFaceCullingState(Material *material)
{
    if (material->enable_face_cull)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(material->face_front);
        glCullFace(material->face_cull);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::ProjectObjects(Object *object, std::vector<Mesh *> &opacity_objects, std::vector<Mesh *> &transparent_objects)
{
    if (object->GetType() == ObjectType::Mesh)
    {
        auto mesh = dynamic_cast<Mesh *>(object);
        if (mesh->material->enable_blend)
            transparent_objects.push_back(mesh);
        else
            opacity_objects.push_back(mesh);
    }

    for (auto child : object->GetChildren())
    {
        ProjectObjects(child, opacity_objects, transparent_objects);
    }
}
