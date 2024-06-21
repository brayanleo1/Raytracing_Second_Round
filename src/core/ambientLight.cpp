#include "ambientLight.h"

namespace rt3 {

Spectrum AmbientLight::sample_Li(const Surfel &hit, Vector3f *wi, VisibilityTester *vis) {
    return intensity;
}

Light *create_ambient_light(const ParamSet &ps) {
    Spectrum intensity = retrieve(ps, "L", Spectrum{0.0, 0.0, 0.0});
    if(intensity[0] == 0 && intensity[1] == 0 && intensity[2] == 0) {
        intensity = retrieve(ps, "I", Spectrum{0.0, 0.0, 0.0});
    }
    return new AmbientLight(intensity);
}

} // namespace rt3