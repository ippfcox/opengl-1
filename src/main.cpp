#include "glwrapper.hpp"

constexpr int width = 800;
constexpr int height = 600;
constexpr char title[] = "hy";

int main()
{
    spdlog::set_level(spdlog::level::debug);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(glfw_window);

    glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow *window, int width, int height) {
        GL_CALL(glViewport(0, 0, width, height));
    });

    glfwSetKeyCallback(glfw_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("gladLoadGLLoader failed");
        return -1;
    }

    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glClearColor(0.0f, 0.8f, 0.8f, 1.0f));

    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwPollEvents();

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        glfwSwapBuffers(glfw_window);
    }

    glfwTerminate();
}