#pragma once

#include "camera_control.hpp"

class GameCameraControl : public CameraControl
{
public:
    GameCameraControl();
    ~GameCameraControl();

    void SetMoveSpeed(float move_speed);

    void Update() override;
    void OnMouseCursor(double xpos, double ypos) override;

private:
    float pitch_ = 0.0f;
    float move_speed_ = 0.001f;
};