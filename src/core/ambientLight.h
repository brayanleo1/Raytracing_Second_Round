#include "light.h"

namespace rt3 {

class AmbientLight : public Light {

public:
    AmbientLight(const Spectrum &intensity) : Light{8, intensity} {}

    Spectrum sample_Li ( const Surfel& hit, Vector3f* wi, VisibilityTester* vis ) override;

    void preprocess(const Scene &scene) override {}

};

Light *create_ambient_light(const ParamSet &ps);

} // namespace rt3