#pragma once

#include "common.hpp"

class Application
{
public:
    using OnResizeCallback = std::function<void(int width, int height)>;
    using OnKeyboardCallback = std::function<void(int key, int action, int modes)>;
    using OnMouseCallback = std::function<void(int button, int action, int mods)>;
    using OnCursorCallback = std::function<void(double xpos, double ypos)>;

    ~Application();
    static Application *Instance();
    bool Init(const int &width, const int &height, const std::string &title);
    bool Update();
    void Destroy();

    bool SetOnResize(OnResizeCallback on_resize);
    bool SetOnKeyboard(OnKeyboardCallback on_keyboard);
    bool SetOnMouse(OnMouseCallback on_mouse);
    bool SetOnCursor(OnCursorCallback on_cursor);

    int Width() const;
    int Height() const;
    std::string Title() const;

private:
    Application();

    int width_;
    int height_;
    std::string title_;
    GLFWwindow *glfw_window_;
    OnResizeCallback on_resize_;
    OnKeyboardCallback on_keyboard_;
    OnMouseCallback on_mouse_;
    OnCursorCallback on_cursor_;
};