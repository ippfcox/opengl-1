#include "orthographic_camera.hpp"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
    : left_(left), right_(right), bottom_(bottom), top_(top), near_(near), far_(far)
{
}

OrthographicCamera::~OrthographicCamera()
{
}

glm::mat4 OrthographicCamera::GetProjectionMatrix()
{
    return glm::ortho(left_, right_, bottom_, top_, near_, far_);
}