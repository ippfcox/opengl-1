#include "material.hpp"

class ColorMaterial : public Material
{
public:
    ColorMaterial();
    ~ColorMaterial();

    glm::vec3 color;
};