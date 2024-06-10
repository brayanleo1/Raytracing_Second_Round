#include "material.h"

namespace rt3 {

class BlinnPhongMaterial : public Material {

public:
    BlinnPhongMaterial(const Spectrum& ka, const Spectrum& kd, const Spectrum& ks, const Spectrum& km, float glossiness) : ka{ka}, kd{kd}, ks{ks}, km{km}, glossiness{glossiness} {}

    void scatter(const Ray& r_in, const Surfel& sf) const override {
        // Do nothing
    }
    
    Spectrum get_ka() const { return ka; }
    Spectrum get_kd() const { return kd; }
    Spectrum get_ks() const { return ks; }
    Spectrum get_km() const { return km; }
    float get_glossiness() const { return glossiness; }

private:
    Spectrum ka, kd, ks, km;
    float glossiness;

};

BlinnPhongMaterial *create_blinn_phong_material(const ParamSet &ps);

} // namespace rt3