#include "glwrapper.hpp"
#include "application.hpp"

constexpr int width = 800;
constexpr int height = 600;
constexpr char title[] = "hy";

int main()
{
    spdlog::set_level(spdlog::level::debug);

    auto app = Application::Instance();
    app->Init(width, height, title);
    app->SetOnResize([](int width, int height) {
        GL_CALL(glViewport(0, 0, width, height));
        SPDLOG_INFO("resize");
    });
    app->SetOnKeyboard([](int key, int action, int mods) {
        SPDLOG_INFO("{}", key);
    });

    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glClearColor(0.0f, 0.8f, 0.8f, 1.0f));

    while (app->Update())
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    app->Destroy();
}