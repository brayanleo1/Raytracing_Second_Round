#ifndef FLAT_INTEGRATOR_H
#define FLAT_INTEGRATOR_H

#include "integrator.h"
namespace rt3 {
class FlatIntegrator : public SamplerIntegrator {
public:
    virtual ~FlatIntegrator(){};
    FlatIntegrator( std::shared_ptr<Camera> cam ) : SamplerIntegrator{cam} {};

    RGBAColor Li(const Ray& ray, const std::unique_ptr<Scene>& scene) const override;
    /*void preprocess(conststd::unique_ptr<Scene>& scene) {
        // Empty for now.
    };*/

    void add_camera(std::shared_ptr<Camera> cam) {
        camera = cam;
    }

};
    
FlatIntegrator *create_flat_integrator(const std::shared_ptr<Camera> &cam);

} // namespace rt3
#endif // FLAT_INTEGRATOR_H