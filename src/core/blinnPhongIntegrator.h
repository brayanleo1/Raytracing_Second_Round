#include "integrator.h"

namespace rt3 {

class BlinnPhongIntegrator : public SamplerIntegrator {

public:
    BlinnPhongIntegrator() = default;
    BlinnPhongIntegrator(std::shared_ptr<Camera> cam, int depth) : SamplerIntegrator{cam}, depth{depth} {/* empty */};

    std::pair<Spectrum,bool> Li(const Ray& ray, const Scene& scene, int depth) const;

    RGBAColor Li(const Ray& ray, const std::unique_ptr<Scene>& scene) const override  {
        auto l = Li(ray, *scene, depth);
        auto r = l.first;
        auto b = l.second;

        if (r.r() > 1 || r.g() > 1 || r.b() > 1) { //Gambierra
            //Now i correct the color
            if(r[0] > 1) r[0] = 1;
            if(r[1] > 1) r[1] = 1;
            if(r[2] > 1) r[2] = 1;
        }
        RGBAColor c{static_cast<unsigned char>(r.r() * 255), static_cast<unsigned char>(r.g() * 255), static_cast<unsigned char>(r.b() * 255), 255, b};
        return c;
    };

    void preprocess(const Scene &scene) const;

private:
    int depth;

};

BlinnPhongIntegrator *create_blinn_phong_integrator(const ParamSet &ps, const std::shared_ptr<Camera> &cam);

} // namespace rt3