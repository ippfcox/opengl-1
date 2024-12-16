#include "material.hpp"
#include "../../texture.hpp"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    ~PhongMaterial();

    Texture *diffuse{nullptr};
    Texture *specular_mask{nullptr};
    float shiness{1.0f};
};