#include "light.h"
#include "scene.h"
#include "pointLight.h"


namespace rt3 {

Light *create_light(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("none"));
  if(type == "point") {
    return rt3::create_point_light(ps);
  } else {
    std::cerr << "Light type " << type << " not recognized." << std::endl;
    return nullptr;
  }

}


bool VisibilityTester::unoccluded(const Scene& scene) {
  Ray r{p0.p, p1.p - p0.p, 0.001f, 0.999f};
  return !scene.intersect_p(r);
}


bool Light::is_ambient() {
  return static_cast<int>(flags) & static_cast<int>(light_flag_e::ambient);
}

} // namespace rt3