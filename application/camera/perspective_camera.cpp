#include "perspective_camera.hpp"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
    : fovy_(fovy), aspect_(aspect)
{
    near_ = near;
    far_ = far;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fovy_), aspect_, near_, far_);
}

void PerspectiveCamera::Scale(float scale)
{
    position += scale * glm::cross(up, right);
}