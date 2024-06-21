#include "light.h"
#include "scene.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "ambientLight.h"


namespace rt3 {

Light *create_light(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("none"));
  if(type == "point") {
    return rt3::create_point_light(ps);
  } else if(type == "directional") {
    return rt3::create_directional_light(ps);
  } else if(type == "spot") {
    return rt3::create_spot_light(ps);
  } else if (type == "ambient") {
    return rt3::create_ambient_light(ps);
  } else {
    std::cerr << "Light type " << type << " not recognized." << std::endl;
    return nullptr;
  }

}


bool VisibilityTester::unoccluded(const Scene& scene) {
  Ray r{p0.p, p1.p - p0.p, 0.0f, 1.0f};
  return !scene.intersect_p(r);
}


bool Light::is_ambient() {
  return static_cast<int>(flags) & static_cast<int>(light_flag_e::ambient);
}

} // namespace rt3