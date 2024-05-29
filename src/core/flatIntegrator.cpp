#include "flatIntegrator.h"
#include "flatMaterial.h"
#include "primitive.h"

namespace rt3 {
// This method must be overridden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
RGBAColor FlatIntegrator::Li(const Ray &ray, const std::unique_ptr<Scene>& scene) const
{
    RGBAColor L{0,0,0,0}; // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!scene.get()->intersect(ray, &isect)) {
        return L; // empty object.
    }
    // Some form of determining the incoming radiance at the ray's origin.
    // Polymorphism in action.
    const FlatMaterial *fm = dynamic_cast<const FlatMaterial *>(isect.primitive->get_material().get());
    // Assign diffuse color to L.
    L = fm->kd(); // Call a method present only in FlatMaterial.
    return L;
}

FlatIntegrator *create_flat_integrator(const std::shared_ptr<Camera> &cam)
{
    return new FlatIntegrator(cam);
}
} // namespace rt3