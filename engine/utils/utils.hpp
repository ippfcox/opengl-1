#include "../common.hpp"

class Utils
{
public:
    Utils() = delete;
    
    static void Decompose(glm::mat4 model_matrix, glm::vec3 &position, glm::vec3 &eular_angle, glm::vec3 &scale);
};