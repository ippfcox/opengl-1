#include "application.hpp"

Application::Application()
{
}

Application::~Application()
{
}

Application *Application::Instance()
{
    static Application instance;
    return &instance;
}

bool Application::SetOnResize(OnResizeCallback on_resize)
{
    if (!glfw_window_)
    {
        return false;
    }

    on_resize_ = on_resize;

    glfwSetFramebufferSizeCallback(glfw_window_, [](GLFWwindow *window, int width, int height) {
        auto *self = (Application *)glfwGetWindowUserPointer(window);
        if (!self || !self->on_resize_)
        {
            return;
        }

        self->on_resize_(width, height);
    });

    return true;
}

bool Application::SetOnKeyboard(OnKeyboardCallback on_keyboard)
{
    if (!glfw_window_)
    {
        return false;
    }

    on_keyboard_ = on_keyboard;

    glfwSetKeyCallback(glfw_window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *self = (Application *)glfwGetWindowUserPointer(window);
        if (!self || !self->on_resize_)
        {
            return;
        }

        self->on_keyboard_(key, action, mods);
    });

    return true;
}

bool Application::Init(const int &width, const int &height, const std::string &title)
{
    width_ = width;
    height_ = height;
    title_ = title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfw_window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(glfw_window_);

    glfwSetWindowUserPointer(glfw_window_, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("gladLoadGLLoader failed");
        return false;
    }

    return true;
}

bool Application::Update()
{
    if (glfwWindowShouldClose(glfw_window_))
        return false;

    glfwPollEvents();
    glfwSwapBuffers(glfw_window_);

    return true;
}

void Application::Destroy()
{
    glfwTerminate();
}