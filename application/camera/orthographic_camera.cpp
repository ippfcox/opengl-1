#include "orthographic_camera.hpp"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
    : left_(left), right_(right), bottom_(bottom), top_(top)
{
    near_ = near;
    far_ = far;
}

OrthographicCamera::~OrthographicCamera()
{
}

glm::mat4 OrthographicCamera::GetProjectionMatrix()
{
    float scale = std::pow(2.0f, scale_);
    return glm::ortho(left_ * scale_, right_ * scale_, bottom_ * scale_, top_ * scale_, near_, far_);
}

void OrthographicCamera::Scale(float scale)
{
    scale_ += scale;
}