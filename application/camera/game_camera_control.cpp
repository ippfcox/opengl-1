#include "game_camera_control.hpp"

GameCameraControl::GameCameraControl()
{
}

GameCameraControl::~GameCameraControl()
{
}

void GameCameraControl::SetMoveSpeed(float move_speed)
{
    move_speed_ = move_speed;
}

void GameCameraControl::Update()
{
    glm::vec3 direction(0.0f);

    auto front = glm::cross(camera_->up, camera_->right);
    auto right = camera_->right;

    if (key_map_[GLFW_KEY_W])
        direction += front;
    if (key_map_[GLFW_KEY_S])
        direction -= front;
    if (key_map_[GLFW_KEY_D])
        direction += right;
    if (key_map_[GLFW_KEY_A])
        direction -= right;

    if (glm::length(direction) > 0)
        direction = glm::normalize(direction);

    camera_->position += direction * move_speed_;
}

void GameCameraControl::OnMouseCursor(double xpos, double ypos)
{
    if (mouse_right_down_)
    {
        auto pitch_delta = static_cast<float>((ypos - cursor_current_y_) * sensitivity_);
        auto yaw = static_cast<float>((xpos - cursor_current_x_) * sensitivity_);

        if (pitch_ + pitch_delta > 89.0f || pitch_ + pitch_delta < -89.0f)
            return;

        pitch_ += pitch_delta;

        auto pitch_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-pitch_delta), camera_->right);
        camera_->up = pitch_mat * glm::vec4(camera_->up, 0.0f);

        auto yaw_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        camera_->up = yaw_mat * glm::vec4(camera_->up, 0.0f);
        camera_->right = yaw_mat * glm::vec4(camera_->right, 0.0f);
    }

    cursor_current_x_ = xpos;
    cursor_current_y_ = ypos;
}
