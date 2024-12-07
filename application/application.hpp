#pragma once

#include "common.hpp"

class Application
{
public:
    using OnResizeCallback = std::function<void(int width, int height)>;
    using OnKeyboardCallback = std::function<void(int key, int action, int mods)>;
    using OnMouseButtonCallback = std::function<void(int button, int action, int mods)>;
    using OnMouseScrollCallback = std::function<void(double yoffset)>;
    using OnMouseCursorCallback = std::function<void(double xpos, double ypos)>;

    ~Application();
    static Application *Instance();
    bool Init(const int &width, const int &height, const std::string &title);
    bool Update();
    void Destroy();

    bool SetOnResize(OnResizeCallback on_resize);
    bool SetOnKeyboard(OnKeyboardCallback on_keyboard);
    bool SetOnMouseButton(OnMouseButtonCallback on_mouse_button);
    bool SetOnMouseScroll(OnMouseScrollCallback on_mouse_scroll);
    bool SetOnMouseCursor(OnMouseCursorCallback on_mouse_cursor);

    std::string GetTitle() const;
    int GetWidth() const;
    int GetHeight() const;
    void GetCursorPosition(double *xpos, double *ypos) const;

private:
    Application();

    int width_;
    int height_;
    std::string title_;
    GLFWwindow *glfw_window_;
    OnResizeCallback on_resize_;
    OnKeyboardCallback on_keyboard_;
    OnMouseButtonCallback on_mouse_button_;
    OnMouseScrollCallback on_mouse_scroll_;
    OnMouseCursorCallback on_mouse_cursor_;
};