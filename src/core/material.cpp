#include "material.h"
#include "flatMaterial.h"

namespace rt3 {

Material *create_material(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("none"));
  if(type == "flat") {
    return create_flat_material(ps);
  } else {
    std::cerr << "Material type " << type << " not recognized." << std::endl;
    return nullptr;
  }
}

}