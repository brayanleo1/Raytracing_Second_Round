#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "light.h"

namespace rt3 {

class DirectionalLight : public Light {
public:
    DirectionalLight(const Vector3f& direction, const Spectrum& intensity) : direction{direction}, Light{2, intensity} {}

    Vector3f getDirection() const { return direction; }

    Spectrum sample_Li( const Surfel& hit, Vector3f* wi, VisibilityTester* vis ) override;

    void preprocess( const Scene &scene ) override;

private:
    Vector3f direction;
    float world_radius;
};

Light *create_directional_light(const ParamSet &ps);

} // namespace rt3

#endif //DIRECTIONAL_LIGHT_H