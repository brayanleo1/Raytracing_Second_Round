#include "material.h"

namespace rt3 {

Material *create_material(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("flat"));
  RGBAColor color = retrieve(ps, "color", RGBAColor{0, 0, 0, 255});
  
  return new Material(type, color);
}

}