#pragma once

#include "camera.hpp"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspect, float near, float far);
    ~PerspectiveCamera();

    glm::mat4 GetProjectionMatrix() override;

    void Scale(float scale) override;

private:
    float fovy_ = 0.0f;
    float aspect_ = 0.0f;
};