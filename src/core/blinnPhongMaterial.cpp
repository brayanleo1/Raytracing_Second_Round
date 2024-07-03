#include "blinnPhongMaterial.h"

namespace rt3 {

BlinnPhongMaterial *create_blinn_phong_material(const ParamSet &ps) {
    auto ka = retrieve(ps, "ambient", Spectrum{0.0f, 0.0f, 0.0f});
    if (ka[0] > 1 || ka[1] > 1 || ka[2] > 1) {
        ka = ka/255;
    }
    auto kd = retrieve(ps, "diffuse", Spectrum{0.0f, 0.0f, 0.0f});
    if (kd[0] > 1 || kd[1] > 1 || kd[2] > 1) {
        kd = kd/255;
    }
    auto ks = retrieve(ps, "specular", Spectrum{0.0f, 0.0f, 0.0f});
    if (ks[0] > 1 || ks[1] > 1 || ks[2] > 1) {
        ks = ks/255;
    }
    auto km = retrieve(ps, "mirror", Spectrum{0.0f, 0.0f, 0.0f});
    if (km[0] > 1 || km[1] > 1 || km[2] > 1) {
        km = km/255;
    }
    float glossiness = retrieve(ps, "glossiness", 0.0f);
        
    return new BlinnPhongMaterial(ka, kd, ks, km, glossiness);
}

} // namespace rt3