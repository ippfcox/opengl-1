#pragma once

#include "camera_control.hpp"

class TrackballCameraControl : public CameraControl
{
public:
    TrackballCameraControl();
    ~TrackballCameraControl();

    void OnCursor(float xpos, float ypos) override;

private:
    float move_speed_ = 0.01f;
};