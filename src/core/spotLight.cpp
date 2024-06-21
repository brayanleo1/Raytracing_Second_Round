#include "spotLight.h"

namespace rt3 {

Spectrum SpotLight::sample_Li(const Surfel& hit, Vector3f* wi, VisibilityTester* vis) {
    
    *wi = from - hit.p;
    wi->make_unit_vector();
    auto q = hit.p - from;
    q.make_unit_vector();

    *vis = VisibilityTester{hit, Surfel{from, Vector3f{0,0,0}, *wi, 0, Point2f{0,0}, nullptr}}; //Okay

    float angle = acos( dot(*wi, direction));
    angle = angle * 180 / M_PI;

    if (angle > cutoff) {
        return Spectrum{0, 0, 0};
    } else if (angle <= falloff) {
        return intensity;
    } else {
        return intensity * pow((angle - cutoff) / (falloff - cutoff), 2);
    }


    
}

Light *create_spot_light(const ParamSet &ps) {
    Point3f from = retrieve(ps, "from", Vector3f{0, 0, 0});
    Point3f to = retrieve(ps, "to", Vector3f{0, 0, 1});
    Vector3f direction = from - to; direction.make_unit_vector();
    Spectrum intensity = retrieve(ps, "I", Spectrum{0, 0, 0});
    if (intensity[0] == 0 && intensity[1] == 0 && intensity[2] == 0) {
        intensity = retrieve(ps, "L", Spectrum{0, 0, 0});
    }
    float cutoff = retrieve(ps, "cutoff", 30.0f);
    float falloff = retrieve(ps, "falloff", 15.0f);
    return new SpotLight(direction, from, intensity, 16, cutoff, falloff);
}

} // namespace rt3