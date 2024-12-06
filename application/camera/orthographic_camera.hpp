#pragma once

#include "camera.hpp"

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
    ~OrthographicCamera();

    glm::mat4 GetProjectionMatrix() override;

private:
    float left_ = 0.0f;
    float right_ = 0.0f;
    float bottom_ = 0.0f;
    float top_ = 0.0f;
    float near_ = 0.0f;
    float far_ = 0.0f;
};