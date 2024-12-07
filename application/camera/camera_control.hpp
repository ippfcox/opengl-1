#pragma once

#include "camera.hpp"
#include <map>

class CameraControl
{
public:
    CameraControl();
    virtual ~CameraControl();

    void SetCamera(Camera *camera);
    void SetSensitivity(float sensitivity);
    void SetScaleSpeed(float scale_speed);

    virtual void Update();
    virtual void OnKeyboard(int key, int action, int mods);
    virtual void OnMouseButton(int button, int action, double xpos, double ypos);
    virtual void OnMouseScroll(double yoffset) = 0;
    virtual void OnMouseCursor(double xpos, double ypos) = 0;

protected:
    bool mouse_left_down_ = false;
    bool mouse_right_down_ = false;
    bool mouse_middle_down_ = false;
    float cursor_current_x_ = 0.0f;
    float cursor_current_y_ = 0.0f;
    float sensitivity_ = 0.2f;
    float scale_speed_ = 0.2f;
    std::map<int, bool> key_map_;
    Camera *camera_ = nullptr;
};