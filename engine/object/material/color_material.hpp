#include "material.hpp"

class ColorMaterial : public Material
{
public:
    ColorMaterial();
    ColorMaterial(glm::vec4 color);
    ~ColorMaterial();

    glm::vec4 color;
};