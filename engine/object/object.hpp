#pragma once

#include "../common.hpp"

class Object
{
public:
    Object();
    virtual ~Object();

    void SetPosition(glm::vec3 world_position);
    void SetRotateX(float angle_delta);
    void SetRotateY(float angle_delta);
    void SetRotateZ(float angle_delta);
    void SetScale(glm::vec3 scale);
    glm::mat4 GetModelMatrix();

protected:
    glm::vec3 world_position_{0.0f};
    float angle_x_{0.0f};
    float angle_y_{0.0f};
    float angle_z_{0.0f};
    glm::vec3 scale_{1.0f};
};