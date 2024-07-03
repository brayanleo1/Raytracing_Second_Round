#include "blinnPhongIntegrator.h"
#include "scene.h"
#include "light.h"
#include "primitive.h"
#include "blinnPhongMaterial.h"

#include <iostream>
#include <thread>
#include <chrono>

namespace rt3 {

/*
    // [0] FIRST STEP TO INITIATE `L` WITH THE COLOR VALUE TO BE RETURNED.
	// [1] FIND CLOSEST RAY INTERSECTION OR RETURN BACKGROUND RADIANCE
	// [2] SPECIAL SITUATION: IF THE RAY HITS THE SURFACE FROM "BEHIND" (INSIDE), WE DO NOT COLOR.
	// [3] GET THE MATERIAL ASSOCIATED WITH THE HIT SURFACE
	// [4] INITIALIZE COMMON VARIABLES FOR BLINNPHONG INTEGRATOR (COEFFICIENTS AND VECTORS L, N, V, ETC.)
	// [5] CALCULATE & ADD CONTRIBUTION FROM EACH LIGHT SOURCE
	// [6] ADD AMBIENT CONTRIBUTION HERE (if there is any).
	// [7] ADD MIRROR REFLECTION CONTRIBUTION
	return L;

*/
std::pair<Spectrum,bool> BlinnPhongIntegrator::Li(const Ray& ray, const Scene& scene, int depth) const {

    // [0] FIRST STEP TO INITIATE `L` WITH THE COLOR VALUE TO BE RETURNED.
    Spectrum L{0, 0, 0};
    
    // [1] FIND CLOSEST RAY INTERSECTION OR RETURN BACKGROUND RADIANCE
    Surfel surfel;
    if (!scene.intersect(ray, &surfel)) {
        return {L,false};
    }
    
    // [2] SPECIAL SITUATION: IF THE RAY HITS THE SURFACE FROM "BEHIND" (INSIDE), WE DO NOT COLOR.
    if (dot(surfel.n, ray.direction()) > 0) {
        return {L,false};
    }
    
    // [3] GET THE MATERIAL ASSOCIATED WITH THE HIT SURFACE
    const BlinnPhongMaterial* material = dynamic_cast<const BlinnPhongMaterial*>(surfel.primitive->get_material().get());
    
    // [4] INITIALIZE COMMON VARIABLES FOR BLINNPHONG INTEGRATOR (COEFFICIENTS AND VECTORS Le, N, V, ETC.)

    Vector3f N = surfel.n;
    Vector3f V = -ray.direction();
    
    // [5] CALCULATE & ADD CONTRIBUTION FROM EACH LIGHT SOURCE
    for (const auto& light : scene.lights) {

        Vector3f wi;
        VisibilityTester vis;
        Spectrum Lig = light->sample_Li(surfel, &wi, &vis);

        if (light->is_ambient()) {
            L += material->get_ka() * Lig;
            continue;
        }


        if (!vis.unoccluded(scene)) {
            continue;
        }

        // Diffuse term
        float cos_theta = dot(N, wi);
        if (cos_theta > 0) {
            L += material->get_kd() * Lig * cos_theta;
            
        }

        
        Vector3f H = (wi + V);
        H.make_unit_vector();
        
        // Specular term
        float cos_alpha = dot(N, H);
        
        if (cos_alpha > 0 && material->get_glossiness() > 0){
            L += material->get_ks() * Lig * pow(cos_alpha, material->get_glossiness());
        }
        
    }
    
    
    // [7] ADD MIRROR REFLECTION CONTRIBUTION
    
    if (depth > 0 && material->get_km() > Spectrum{0,0,0}) {        

        Vector3f R = ray.direction() - 2 * dot(ray.direction(), N) * N;
        R.make_unit_vector();
        Ray reflected_ray(surfel.p + 0.001 * R, R);
        auto [reflected_L, hit] = Li(reflected_ray, scene, depth-1);
        reflected_L = material->get_km() * reflected_L;

        L += reflected_L;
    }

    return {L,true};
    
}

BlinnPhongIntegrator *create_blinn_phong_integrator(const ParamSet &ps, const std::shared_ptr<Camera> &cam) {
    auto depth = retrieve(ps, "depth", 0);
    return new BlinnPhongIntegrator(cam, depth);
}

void BlinnPhongIntegrator::preprocess(const Scene &scene) const {
    // Utilize the preprocess method of the light sources from the scene.
    for (const auto& light : scene.lights) {
        light->preprocess(scene);
    }
}


} // namespace rt3