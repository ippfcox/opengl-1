#include "application.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera/orthographic_camera.hpp"
#include "camera/perspective_camera.hpp"
#include "camera/trackball_camera_control.hpp"
#include "camera/game_camera_control.hpp"
#include "assimp_loader.hpp"
#include "object/object.hpp"
#include "object/mesh.hpp"
#include "object/sphere.hpp"
#include "object/cube.hpp"
#include "object/scene.hpp"
#include "object/material/phong_material.hpp"
#include "object/material/color_material.hpp"
#include "object/material/depth_material.hpp"
#include "renderer/renderer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "wrapper.hpp"

constexpr int width = 800;
constexpr int height = 600;
constexpr char title[] = "hy";

Renderer *renderer = nullptr;
Scene *scene = nullptr;
DirectionalLight *directional_light = nullptr;
std::vector<PointLight *> point_lights{};
AmbientLight *ambient_light = nullptr;
SpotLight *spot_light = nullptr;

Camera *camera = nullptr;
CameraControl *camera_control = nullptr;

glm::vec3 clear_color{0.0f};

void prepare()
{

    // renderer
    renderer = new Renderer();

    // meshes
    auto geometry = new Cube(5.0f);

    auto material = new DepthMaterial();
    // auto material = new PhongMaterial();
    // material->shiness = 40.0f;
    // material->diffuse = new Texture();
    // material->diffuse->InitByFilename("assets/textures/box.png");
    // material->specular_mask = new Texture();
    // material->specular_mask->InitByFilename("assets/textures/box_specular.png");

    auto mesh = new Mesh(geometry, material);

    auto geometry2 = new Sphere(0.1f);
    auto material2 = new ColorMaterial();
    material2->color = {1.0f, 1.0f, 0.0f};
    auto mesh2 = new Mesh(geometry2, material2);
    mesh2->SetPosition({2.3f, 0.0f, 0.0f});

    mesh->AddChild(mesh2);

    scene = new Scene();
    // auto test = AssimpLoader::Load("assets/fbx/phoenix-bird/fly.fbx");
    // test->SetScale(glm::vec3(0.1f));
    // scene->AddChild(test);

    scene->AddChild(mesh);

    // light
    directional_light = new DirectionalLight();
    directional_light->direction = {-1.0f, -0.3f, -3.0f};
    directional_light->color = {0.5f, 0.5f, 0.5f};
    directional_light->specular_intensity = 1.5f;
    auto point_light = new PointLight();
    point_light->SetPosition({0.5f, 1.5f, 0.5f});
    point_light->color = {1.0f, 0.0f, 0.0f};
    point_light->specular_intensity = 2.0f;
    point_light->k2 = 0.17f;
    point_light->k1 = 0.07;
    point_light->kc = 1.0f;
    point_lights.push_back(point_light);
    point_light = new PointLight();
    point_light->SetPosition({0.5f, 1.5f, -0.5f});
    point_light->color = {1.0f, 0.0f, 0.0f};
    point_light->specular_intensity = 2.0f;
    point_light->k2 = 0.17f;
    point_light->k1 = 0.07;
    point_light->kc = 1.0f;
    point_lights.push_back(point_light);
    point_light = new PointLight();
    point_light->SetPosition({-0.5f, 1.5f, 0.5f});
    point_light->color = {1.0f, 0.0f, 0.0f};
    point_light->specular_intensity = 2.0f;
    point_light->k2 = 0.17f;
    point_light->k1 = 0.07;
    point_light->kc = 1.0f;
    point_lights.push_back(point_light);
    point_light = new PointLight();
    point_light->SetPosition({-0.5f, 1.5f, -0.5f});
    point_light->color = {1.0f, 0.0f, 0.0f};
    point_light->specular_intensity = 2.0f;
    point_light->k2 = 0.17f;
    point_light->k1 = 0.07;
    point_light->kc = 1.0f;
    point_lights.push_back(point_light);

    ambient_light = new AmbientLight();
    ambient_light->color = {0.2f, 0.2f, 0.2f};
    spot_light = new SpotLight();
    // spot_light->SetPosition(mesh2->GetPosition());
    spot_light->direction = {-1.0f, 0.0f, 0.0f};
    spot_light->inner_angle = 20.0f;
    spot_light->outer_angle = 30.0f;
    spot_light->color = {1.0f, 1.0f, 0.0f};
    spot_light->specular_intensity = 0.8f;
}

void prepare_camera()
{
    camera = new PerspectiveCamera(60.0f, (float)width / height, 0.1f, 50.0f);
    // camera = new OrthographicCamera(-6.0f, 6.0f, -6.0f, 6.0f, -6.0f, 6.0f);
    // camera_control = new TrackballCameraControl();
    camera_control = new GameCameraControl();
    camera_control->SetCamera(camera);
    dynamic_cast<GameCameraControl *>(camera_control)->SetMoveSpeed(0.02f);
}

void init_imgui(Application *app)
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui_ImplGlfw_InitForOpenGL(app->GetWindow(), true);
}

void render_imgui(Application *app)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("debug");
    ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));
    ImGui::End();

    ImGui::Render();
    int width, height;
    glfwGetFramebufferSize(app->GetWindow(), &width, &height);
    glViewport(0, 0, width, height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

    prepare();
    prepare_camera();
    init_imgui(app);

    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    // GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

    while (app->Update())
    {
        scene->SetRotateY(0.1f);

        camera_control->Update();
        renderer->SetClearColor(clear_color);
        renderer->Render(scene, camera, spot_light, directional_light, point_lights, ambient_light);
        render_imgui(app);
    }

    app->Destroy();
}