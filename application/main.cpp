#include "application.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera/orthographic_camera.hpp"
#include "camera/perspective_camera.hpp"
#include "camera/trackball_camera_control.hpp"
#include "camera/game_camera_control.hpp"
#include "mesh/cube.hpp"
#include "mesh/sphere.hpp"
#include "mesh/plane.hpp"
#include "wrapper.hpp"

constexpr int width = 80;
constexpr int height = 60;
constexpr char title[] = "hy";

glm::vec3 diffuse_light_direction{-1.0f, -0.3f, -3.0f};
glm::vec3 diffuse_light_color{0.7f, 0.6, 0.0f};
float specular_intensity = 2.0f;

Mesh *mesh = nullptr;
Shader shader;
Texture textuer_earth;

Camera *camera = nullptr;
CameraControl *camera_control = nullptr;

void prepare_vao()
{
    mesh = new Cube(10);
    // mesh = new Sphere(2.0);
    // mesh = new Plane(2, 3);
}

void prepare_shader()
{
    shader.InitByFilename("assets/shaders/1.vs", "assets/shaders/1.fs");
}

void prepare_texture()
{
    textuer_earth.InitByFilename("assets/textures/earthmap1k.jpg");
    textuer_earth.Bind(0);
}

void prepare_camera()
{
    camera = new PerspectiveCamera(60.0f, (float)width / height, 0.1f, 1000.0f);
    // camera = new OrthographicCamera(-6.0f, 6.0f, -6.0f, 6.0f, -6.0f, 6.0f);
    // camera_control = new TrackballCameraControl();
    camera_control = new GameCameraControl();
    camera_control->SetCamera(camera);
    dynamic_cast<GameCameraControl *>(camera_control)->SetMoveSpeed(0.02f);
}

void render()
{
    // glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto transform = glm::mat4(1.0f);

    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    shader.Use();
    shader.SetUniform("unif_texture_earth", 0);
    shader.SetUniform("unif_model", transform);
    shader.SetUniform("unif_view", camera->GetViewMatrix());
    shader.SetUniform("unif_projection", camera->GetProjectionMatrix());
    shader.SetUniform("unif_light_direction", diffuse_light_direction);
    shader.SetUniform("unif_light_color", diffuse_light_color);
    shader.SetUniform("unif_specular_intensity", specular_intensity);
    shader.SetUniform("unif_camera_position", camera->position);
    GL_CALL(glBindVertexArray(mesh->GetVAO()));
    GL_CALL(glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));
    GL_CALL(glBindVertexArray(GL_NONE));
    shader.End();
}

int main()
{
    spdlog::set_level(spdlog::level::debug);

    auto app = Application::Instance();
    app->Init(width, height, title);
    app->SetOnResize([&](int width, int height) {
        GL_CALL(glViewport(0, 0, width, height));
    });
    app->SetOnKeyboard([](int key, int action, int mods) {
        camera_control->OnKeyboard(key, action, mods);
    });
    app->SetOnMouseButton([&](int button, int action, int mods) {
        double xpos, ypos;
        app->GetCursorPosition(&xpos, &ypos);
        camera_control->OnMouseButton(button, action, xpos, ypos);
    });
    app->SetOnMouseScroll([&](double yoffset) {
        camera_control->OnMouseScroll(yoffset);
    });
    app->SetOnMouseCursor([](double xpos, double ypos) {
        camera_control->OnMouseCursor(xpos, ypos);
    });

    prepare_vao();
    prepare_shader();
    prepare_texture();
    prepare_camera();

    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glDepthFunc(GL_LESS));
    // GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

    while (app->Update())
    {
        camera_control->Update();
        render();
    }

    app->Destroy();
}