#include "light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    ~DirectionalLight();

    glm::vec3 direction{-1.0f};
};