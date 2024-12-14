#include "utils.hpp"

void Utils::Decompose(glm::mat4 model_matrix, glm::vec3 &position, glm::vec3 &eular_angle, glm::vec3 &scale)
{
    glm::quat orientation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model_matrix, scale, orientation, position, skew, perspective);

    auto rotation = glm::toMat4(orientation);
    glm::extractEulerAngleXYZ(rotation, eular_angle.x, eular_angle.y, eular_angle.z);
    eular_angle = glm::degrees(eular_angle);
}
