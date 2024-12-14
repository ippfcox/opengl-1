#include "object.hpp"

Object::Object()
    : type_(ObjectType::Object)
{
}

Object::~Object()
{
}

void Object::SetPosition(glm::vec3 world_position)
{
    world_position_ = world_position;
}

glm::vec3 Object::GetPosition() const
{
    return world_position_;
}

void Object::SetRotateX(float angle_delta)
{
    angle_x_ += angle_delta;
}

void Object::SetRotateY(float angle_delta)
{
    angle_y_ += angle_delta;
}

void Object::SetRotateZ(float angle_delta)
{
    angle_z_ += angle_delta;
}

void Object::SetScale(glm::vec3 scale)
{
    scale_ = scale;
}

glm::mat4 Object::GetModelMatrix()
{
    // scale -> rotate (x(pitch) -> y(yaw) -> z(roll)) -> translate(world coordinate)

    glm::mat4 transform(1.0f);

    transform = glm::scale(transform, scale_);
    transform = glm::rotate(transform, glm::radians(angle_x_), glm::vec3{1.0f, 0.0f, 0.0f});
    transform = glm::rotate(transform, glm::radians(angle_y_), glm::vec3{0.0f, 1.0f, 0.0f});
    transform = glm::rotate(transform, glm::radians(angle_z_), glm::vec3{0.0f, 0.0f, 1.0f});
    transform = glm::translate(glm::mat4(1.0f), world_position_) * transform;

    if (parent_)
        return parent_->GetModelMatrix() * transform;
    return transform;
}

ObjectType Object::GetType() const
{
    return type_;
}

void Object::AddChild(Object *object)
{
    auto iter = std::find(children_.begin(), children_.end(), object);
    if (iter != children_.end())
    {
        SPDLOG_WARN("add duplicate object");
        return;
    }

    children_.push_back(object);
    object->parent_ = this;
}

std::vector<Object *> Object::GetChildren()
{
    return children_;
}

Object *Object::GetParent()
{
    return parent_;
}
