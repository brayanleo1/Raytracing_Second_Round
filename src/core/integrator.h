#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "rt3.h"
#include "commons.h"
#include "scene.h"
#include "paramset.h"

namespace rt3 {
class Integrator {
	public:
		virtual ~Integrator(){};
		virtual void render( const std::unique_ptr<Scene>& scene ) = 0;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator(){};
		SamplerIntegrator( std::shared_ptr<Camera> cam ) : camera{cam} {/*empty*/}
		//SamplerIntegrator() {}

		virtual RGBAColor Li( const Ray& ray, const std::unique_ptr<Scene>& scene ) const = 0;
		virtual void render( const std::unique_ptr<Scene>& scene );
        virtual void preprocess(const Scene &scene) const = 0;

    protected:
		std::shared_ptr<Camera> camera;
};

SamplerIntegrator *create_integrator(const ParamSet &ps, std::shared_ptr<Camera> cam);

} // namespace rt3
#endif //INTEGRATOR_H