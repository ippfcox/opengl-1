#include "material.hpp"

class PureMaterial : public Material
{
public:
    PureMaterial();
    ~PureMaterial();

    glm::vec3 color;
};