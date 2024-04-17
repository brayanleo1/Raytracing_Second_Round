#include "material.h"

namespace rt3 {

Material *create_material(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("flat"));
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
  std::cout<<"r:"<<std::to_string(c.r)<<" g:"<<std::to_string(c.g)<<" b:"<<std::to_string(c.b)<<"\n";
  return new Material(type, c);
}

}