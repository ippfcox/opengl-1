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
    glm::vec3 world_position_;
    float angle_x_;
    float angle_y_;
    float angle_z_;
    glm::vec3 scale_;
};