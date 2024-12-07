#include "trackball_camera_control.hpp"

TrackballCameraControl::TrackballCameraControl()
{
}

TrackballCameraControl::~TrackballCameraControl()
{
}

void TrackballCameraControl::OnMouseScroll(double yoffset)
{
    camera_->Scale(yoffset * scale_speed_);
}

void TrackballCameraControl::OnMouseCursor(double xpos, double ypos)
{
    if (mouse_left_down_)
    {
        auto pitch = static_cast<float>((ypos - cursor_current_y_) * sensitivity_);
        auto yaw = static_cast<float>((xpos - cursor_current_x_) * sensitivity_);

        auto pitch_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), camera_->right);
        camera_->up = pitch_mat * glm::vec4(camera_->up, 0.0f);
        camera_->position = pitch_mat * glm::vec4(camera_->position, 1.0f);

        auto yaw_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        camera_->up = yaw_mat * glm::vec4(camera_->up, 0.0f);
        camera_->right = yaw_mat * glm::vec4(camera_->right, 0.0f);
        camera_->position = yaw_mat * glm::vec4(camera_->position, 1.0f);
    }

    if (mouse_middle_down_)
    {
        camera_->position += (ypos - cursor_current_y_) * move_speed_ * camera_->up - (xpos - cursor_current_x_) * move_speed_ * camera_->right;
    }

    cursor_current_x_ = xpos;
    cursor_current_y_ = ypos;
}