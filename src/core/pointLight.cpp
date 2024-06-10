#include "pointLight.h"

namespace rt3 {


Spectrum PointLight::sample_Li( const Surfel& hit, Vector3f* wi, VisibilityTester* vis ) {
    *wi = (position - hit.p);
    wi->make_unit_vector();
    *vis = VisibilityTester( hit, Surfel{position, Vector3f{0,0,0}, *wi, wi->length(), Point2f{0,0}, nullptr} );
    return intensity;
}

Light *create_point_light(const ParamSet &ps) {
    Point3f position = retrieve(ps, "from", Point3f{0,0,0});
    Spectrum intensity = retrieve(ps, "I", Spectrum{0,0,0});
    if (intensity[0] == 0 && intensity[1] == 0 && intensity[2] == 0) {
        intensity = retrieve(ps, "L", Spectrum{0,0,0});
    }
    //RGBAColor i = RGBAColor{ static_cast<unsigned char>(intensity[0]*255), static_cast<unsigned char>(intensity[1]*255), static_cast<unsigned char>(intensity[2]*255), 255};
    Point3f scale = retrieve(ps, "scale", Point3f{1,1,1});
    return new PointLight(position, intensity, scale);
}

} // namespace rt3