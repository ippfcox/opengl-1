#include "material.hpp"
#include "../../texture.hpp"

class PhongEnvMaterial : public Material
{
public:
    PhongEnvMaterial();
    ~PhongEnvMaterial();

    Texture *diffuse{nullptr};
    Texture *specular_mask{nullptr};
    Texture *env{nullptr};
    float shiness{1.0f};
};