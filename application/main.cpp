#include "application.hpp"
#include "shader.hpp"
#include "wrapper.hpp"

constexpr int width = 800;
constexpr int height = 600;
constexpr char title[] = "hy";

GLuint vao;
Shader shader;

void prepare_vao()
{
    float vertices[] = {
        -0.5f, -0.5f, +0.0f, +1.0f, +0.0f, +0.0f,
        +0.5f, -0.5f, +0.0f, +0.0f, +1.0f, +0.0f,
        +0.0f, +0.5f, +0.0f, +0.0f, +0.0f, +1.0f};

    unsigned int indices[] = {0, 1, 2};

    GLuint vbo;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLuint ebo;
    GL_CALL(glGenBuffers(1, &ebo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))));
    GL_CALL(glBindVertexArray(GL_NONE));
}

void prepare_shader()
{
    shader.InitByFilename("assets/shaders/1.vs", "assets/shaders/1.fs");
}

void render()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    shader.Use();
    shader.SetUniform("time", (float)glfwGetTime());
    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
    GL_CALL(glBindVertexArray(GL_NONE));
    shader.End();
}

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

    prepare_vao();
    prepare_shader();

    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glClearColor(0.0f, 0.8f, 0.8f, 1.0f));

    while (app->Update())
    {
        render();
    }

    app->Destroy();
}