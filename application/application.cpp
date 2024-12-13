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

void Application::Destroy()
{
    glfwTerminate();
}

bool Application::Update()
{
    if (glfwWindowShouldClose(glfw_window_))
        return false;

    glfwPollEvents();
    glfwSwapBuffers(glfw_window_);

    return true;
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
        if (!self || !self->on_keyboard_)
        {
            return;
        }

        self->on_keyboard_(key, action, mods);
    });

    return true;
}

bool Application::SetOnMouseScroll(OnMouseScrollCallback on_mouse_scroll)
{
    if (!glfw_window_)
    {
        return false;
    }

    on_mouse_scroll_ = on_mouse_scroll;

    glfwSetScrollCallback(glfw_window_, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto *self = (Application *)glfwGetWindowUserPointer(window);
        if (!self || !self->on_mouse_button_)
        {
            return;
        }

        self->on_mouse_scroll_(yoffset);
    });

    return true;
}

bool Application::SetOnMouseButton(OnMouseButtonCallback on_mouse_button)
{
    if (!glfw_window_)
    {
        return false;
    }

    on_mouse_button_ = on_mouse_button;

    glfwSetMouseButtonCallback(glfw_window_, [](GLFWwindow *window, int button, int action, int mods) {
        auto *self = (Application *)glfwGetWindowUserPointer(window);
        if (!self || !self->on_mouse_button_)
        {
            return;
        }

        self->on_mouse_button_(button, action, mods);
    });

    return true;
}

bool Application::SetOnMouseCursor(OnMouseCursorCallback on_mouse_cursor)
{
    if (!glfw_window_)
    {
        return false;
    }

    on_mouse_cursor_ = on_mouse_cursor;

    glfwSetCursorPosCallback(glfw_window_, [](GLFWwindow *window, double xpos, double ypos) {
        auto *self = (Application *)glfwGetWindowUserPointer(window);
        if (!self || !self->on_mouse_cursor_)
        {
            return;
        }

        self->on_mouse_cursor_(xpos, ypos);
    });

    return true;
}

GLFWwindow *Application::GetWindow() const
{
    return glfw_window_;
}

std::string Application::GetTitle() const
{
    return title_;
}

int Application::GetWidth() const
{
    return width_;
}

int Application::GetHeight() const
{
    return height_;
}

void Application::GetCursorPosition(double *xpos, double *ypos) const
{
    glfwGetCursorPos(glfw_window_, xpos, ypos);
}