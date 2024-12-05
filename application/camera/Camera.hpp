#pragma once

#include "common.hpp"

class Camera
{
public:
    Camera();
    ~Camera();

    glm::mat4 GetViewMatrix();
    virtual glm::mat4 GetProjectionMatrix();

    glm::vec3 position{0.0f, 0.0f, 1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};

private:
};