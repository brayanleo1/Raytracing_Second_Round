#include "directionalLight.h"
#include "scene.h"

namespace rt3 {

void DirectionalLight::preprocess(const Scene &scene) {
    //find the bounding box of the scene
    Bounds3f bounds = scene.world_bound();
    //find the diagonal of the bounding box
    Vector3f diagonal = bounds.pMax - bounds.pMin;
    //set the world_radius to the length of the diagonal
    world_radius = diagonal.length();
}

Spectrum DirectionalLight::sample_Li(const Surfel& hit, Vector3f* wi, VisibilityTester* vis) {
    //set the direction of the light to the direction of the light
    *wi = -direction;
    //set the visibility tester to the direction of the light
    //*vis = VisibilityTester{hit, Surfel{hit.p + *wi * world_radius, Vector3f{0,0,0}, *wi, 0, Point2f{0,0}, nullptr}};
    *vis = VisibilityTester{hit, Surfel{hit.p + *wi * world_radius, Vector3f{0,0,0}, *wi, 0, Point2f{0,0}, nullptr}};
    //return the intensity of the light
    return intensity;
}

Light* create_directional_light(const ParamSet& ps) {
    Point3f from = retrieve(ps, "from", Vector3f{0, 0, 0});
    Point3f to = retrieve(ps, "to", Vector3f{0, 0, 1});
    Vector3f direction = to - from; direction.make_unit_vector();
    Spectrum intensity = retrieve(ps, "L", Spectrum{0, 0, 0});
    if (intensity[0] == 0 && intensity[1] == 0 && intensity[2] == 0) {
        intensity = retrieve(ps, "I", Spectrum{0, 0, 0});
    }
    return new DirectionalLight(direction, intensity);
}

} // namespace rt3