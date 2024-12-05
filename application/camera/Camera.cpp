#include "Camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + glm::cross(up, right), up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::identity<glm::mat4>();
}