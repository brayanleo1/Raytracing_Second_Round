#include "flatMaterial.h"

namespace rt3 {

FlatMaterial *create_flat_material(const ParamSet &ps) {
  Spectrum noValue = {0,0,0};
  Spectrum color = retrieve(ps, "color", noValue);
  bool flnum = false;
  if((color[0] > 0 && color[0] < 1) || (color[1] > 0 && color[1] < 1) || (color[2] > 0 && color[2] < 1)){
    flnum = true;
  }
  RGBAColor c;
  c.a = 255;
  if(flnum) {
    c.r = color[0]*255;
    c.g = color[1]*255;
    c.b = color[2]*255;
  } else {
    c.r = color[0];
    c.g = color[1];
    c.b = color[2];
  }
  return new FlatMaterial(c);
}

} // namespace rt3