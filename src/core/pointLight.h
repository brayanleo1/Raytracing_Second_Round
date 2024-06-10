#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"

namespace rt3 {

class PointLight : public Light {
public:
    PointLight(const Point3f& position, const Spectrum& intensity, const Point3f& scale) : position{position}, Light{1, intensity}, scale{scale} {}

    Point3f getPosition() const { return position; }

    Spectrum sample_Li( const Surfel& hit, Vector3f* wi, VisibilityTester* vis ) override;

private:
    Point3f position;
    Point3f scale;
};

Light *create_point_light(const ParamSet &ps);

}
#endif //POINT_LIGHT_H