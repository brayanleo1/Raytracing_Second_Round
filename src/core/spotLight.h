//<light_source type="spot" I="0.5 0.5 0.4" from="1.5 5 -8" to="1.5 -2 -8" cutoff="30" falloff="15" />
#include "light.h"

namespace rt3 {

class SpotLight : public Light {

public:
    SpotLight( const Vector3f& direction, const Point3f& from, const Spectrum& intensity, int flags, float cutoff, float falloff ) : Light{flags, intensity}, direction{direction}, from{from}, cutoff{cutoff}, falloff{falloff} {};

    Spectrum sample_Li ( const Surfel& hit, Vector3f* wi, VisibilityTester* vis ) override;

    void preprocess( const Scene& scene ) override {}

    private:
        Vector3f direction;
        Point3f from;
        float cutoff, falloff;
};

Light* create_spot_light( const ParamSet& ps );

} // namespace rt3