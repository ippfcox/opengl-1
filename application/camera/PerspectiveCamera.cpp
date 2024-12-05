#include "PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
    : fovy_(fovy), aspect_(aspect), near_(near), far_(far)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fovy_), aspect_, near_, far_);
}