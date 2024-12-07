#include "camera_control.hpp"

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

void CameraControl::SetCamera(Camera *camera)
{
    camera_ = camera;
}

void CameraControl::SetSensitivity(float sensitivity)
{
    sensitivity_ = sensitivity;
}

void CameraControl ::SetScaleSpeed(float scale_speed)
{
    scale_speed_ = scale_speed;
}

void CameraControl::Update()
{
}

void CameraControl::OnKeyboard(int key, int action, int mods)
{
    if (action == GLFW_REPEAT)
        return;

    bool pressed = action == GLFW_PRESS;
    key_map_[key] = pressed;
}

void CameraControl::OnMouseButton(int button, int action, double xpos, double ypos)
{
    bool pressed = action == GLFW_PRESS;
    if (pressed)
    {
        cursor_current_x_ = xpos;
        cursor_current_y_ = ypos;
    }

    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouse_left_down_ = pressed;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouse_right_down_ = pressed;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouse_middle_down_ = pressed;
        break;
    }
}
