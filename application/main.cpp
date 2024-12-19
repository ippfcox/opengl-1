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
#include "object/plane.hpp"
#include "object/screen_plane.hpp"
#include "object/scene.hpp"
#include "object/material/phong_material.hpp"
#include "object/material/phong_opacity_mask_material.hpp"
#include "object/material/phong_env_material.hpp"
#include "object/material/color_material.hpp"
#include "object/material/depth_material.hpp"
#include "object/material/screen_plane_material.hpp"
#include "object/material/cube_material.hpp"
#include "renderer/renderer.hpp"
#include "framebuffer/framebuffer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "wrapper.hpp"

constexpr int width = 1600;
constexpr int height = 900;
constexpr char title[] = "hy";

Framebuffer *fb = nullptr;
Renderer *renderer = nullptr;
Scene *scene_offscreen = nullptr;
Scene *scene_inscreen = nullptr;
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
    fb = new Framebuffer(width, height);

    // off screen
    scene_offscreen = new Scene();

    auto skybox_geometry = new Cube(1.0);
    auto skybox_material = new CubeMaterial();
    skybox_material->diffuse = new Texture();
    skybox_material->diffuse->InitByFilename("assets/textures/8081_earthmap10k.jpg");
    // skybox_material->diffuse->InitCubeMapByFilename({
    //     "assets/textures/skybox/right.jpg",
    //     "assets/textures/skybox/left.jpg",
    //     "assets/textures/skybox/top.jpg",
    //     "assets/textures/skybox/bottom.jpg",
    //     "assets/textures/skybox/front.jpg", // [fixme] maybe not correct...
    //     "assets/textures/skybox/back.jpg",
    // });
    auto skybox_mesh = new Mesh(skybox_geometry, skybox_material);
    scene_offscreen->AddChild(skybox_mesh);

    auto sphere_geometry = new Sphere(4.0f);
    auto sphere_material = new PhongMaterial();
    sphere_material->diffuse = new Texture();
    sphere_material->diffuse->InitByFilename("assets/textures/earthmap1k.jpg");
    sphere_material->diffuse->SetUnit(0);
    sphere_material->shiness = 4.0f;
    auto sphere_mesh = new Mesh(sphere_geometry, sphere_material);
    scene_offscreen->AddChild(sphere_mesh);

    // auto geometry = new Plane(5, 5);
    // auto material = new PhongMaterial();
    // material->diffuse = new Texture();
    // material->diffuse->InitByFilename("assets/textures/earthmap1k.jpg");
    // auto plane = new Mesh(geometry, material);
    // scene_offscreen->AddChild(plane);

    // in screen
    scene_inscreen = new Scene();
    auto g2 = new ScreenPlane();
    auto m2 = new ScreenPlaneMaterial(width, height);
    m2->screen = fb->GetColorAttachment();
    m2->screen->SetUnit(0);
    auto screen = new Mesh(g2, m2);
    scene_inscreen->AddChild(screen);

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
        // scene->SetRotateY(0.1f);

        camera_control->Update();
        renderer->SetClearColor(clear_color);
        renderer->Render(scene_offscreen, camera, spot_light, directional_light, point_lights, ambient_light, fb->GetFramebuffer());
        renderer->Render(scene_inscreen, camera, spot_light, directional_light, point_lights, ambient_light);
        render_imgui(app);
    }

    app->Destroy();
}