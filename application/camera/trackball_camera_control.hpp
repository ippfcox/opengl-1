#pragma once

#include "camera_control.hpp"

class TrackballCameraControl : public CameraControl
{
public:
    TrackballCameraControl();
    ~TrackballCameraControl();

    void OnMouseScroll(double yoffseet) override;
    void OnMouseCursor(double xpos, double ypos) override;

private:
    float move_speed_ = 0.01f;
};