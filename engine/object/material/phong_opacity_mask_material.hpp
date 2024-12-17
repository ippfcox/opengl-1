#include "material.hpp"
#include "../../texture.hpp"

class PhongOpacityMaskMaterial : public Material
{
public:
    PhongOpacityMaskMaterial();
    ~PhongOpacityMaskMaterial();

    Texture *diffuse{nullptr};
    Texture *opacity_mask{nullptr};
    float shiness{1.0f};
};