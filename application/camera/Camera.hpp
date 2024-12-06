#pragma once

#include "common.hpp"

class Camera
{
public:
    Camera();
    virtual ~Camera();

    glm::mat4 GetViewMatrix();
    virtual glm::mat4 GetProjectionMatrix() = 0;

    glm::vec3 position{0.0f, 0.0f, 1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};

private:
};