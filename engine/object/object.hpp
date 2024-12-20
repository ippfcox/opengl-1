#pragma once

#include <vector>
#include "../common.hpp"

enum class ObjectType
{
    Object,
    Mesh,
    Scene,
};

class Object
{
public:
    Object();
    virtual ~Object();

    void SetPosition(glm::vec3 world_position);
    glm::vec3 GetPosition() const;
    void SetRotateX(float angle_delta);
    void SetRotateY(float angle_delta);
    void SetRotateZ(float angle_delta);
    void SetAngleX(float angle);
    void SetAngleY(float angle);
    void SetAngleZ(float angle);

    void SetScale(glm::vec3 scale);
    glm::mat4 GetModelMatrix() const;

    ObjectType GetType() const;

    void AddChild(Object *object);
    std::vector<Object *> GetChildren();
    Object *GetParent();

protected:
    glm::vec3 world_position_{0.0f};
    float angle_x_{0.0f};
    float angle_y_{0.0f};
    float angle_z_{0.0f};
    glm::vec3 scale_{1.0f};

    ObjectType type_;

    std::vector<Object *> children_{};
    Object *parent_{nullptr};
};