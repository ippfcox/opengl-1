#include "material.hpp"
#include "../../texture.hpp"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    ~PhongMaterial();

    Texture *diffuse;
    float shiness;
};