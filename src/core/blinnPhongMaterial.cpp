#include "blinnPhongMaterial.h"

namespace rt3 {

BlinnPhongMaterial *create_blinn_phong_material(const ParamSet &ps) {
    auto ka = retrieve(ps, "ambient", Spectrum{0.0f, 0.0f, 0.0f});
    auto kd = retrieve(ps, "diffuse", Spectrum{0.0f, 0.0f, 0.0f});
    auto ks = retrieve(ps, "specular", Spectrum{0.0f, 0.0f, 0.0f});
    auto km = retrieve(ps, "mirror", Spectrum{0.0f, 0.0f, 0.0f});
    float glossiness = retrieve(ps, "glossiness", 0.0f);
        
    return new BlinnPhongMaterial(ka, kd, ks, km, glossiness);
}

} // namespace rt3