#include "integrator.h"
#include "camera.h"
#include "background.h"
#include "flatIntegrator.h"

namespace rt3 {
    void SamplerIntegrator::render(const std::unique_ptr<Scene>& scene) {
    // Always call the preprocess() before doing any rendering.
    // This might be just an empty method, or not, depending on the integrator's needs.
    //preprocess(scene);
    std::cout << "Rendering scene, the start" << std::endl;
    auto w = camera->film->get_resolution()[0];
    auto h = camera->film->get_resolution()[1];


    // Traverse all pixels to shoot rays from.
    for ( int j = 0 ; j < h ; j++ ) {
        for( int i = 0 ; i < w ; i++ ) {
            Ray ray = camera->generate_ray( i, j );
            auto colorDef = scene.get()->background.get()->sample( float(i)/float(w), float(j)/float(h) );
            // Get the backgound color in case the ray hits nothing.
            if ( scene.get()->background.get()->mapping_type == Background::mapping_t::screen )
                colorDef = scene.get()->background.get()->sample( i/w, j/h ); // screen mapping needs a normalized pixel coord.
                
            auto temp_L =  Li( ray, scene );
            if(temp_L.r != none.r || temp_L.g != none.g || temp_L.b != none.b || temp_L.a != none.a){
                colorDef = temp_L;
            }
            camera->film->add_sample( {i,j},  colorDef ); // set image buffer at position (i,j), accordingly.

        }
    }
    // send image color buffer to the output file.
    camera->film->write_image();
    }

    SamplerIntegrator *create_integrator(const ParamSet &ps, std::shared_ptr<Camera> cam) {
        auto type = retrieve<std::string>(ps, "type", "none");
        std::cout << "Integrator type: " << type << std::endl;
        if (type == "flat") {
            return rt3::create_flat_integrator(cam);
        } else {
            std::cerr << "Integrator type not recognized." << std::endl;
            return nullptr;
        }
    }

} // namespace rt3