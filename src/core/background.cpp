#include "background.h"
#include <vector>

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Spectrum Background::sampleXYZ(const Point2f &pixel_ndc) const {
  // TODO
  return Spectrum{0, 0, 0};
}

BackgroundColor *create_color_background(const ParamSet &ps) {
  std::vector<Spectrum> colors;
  Spectrum noValue = {0,0,0};
  Spectrum noExists = {300,300,300};
  if(rt3::retrieve(ps, "bl", noExists)[0] != 300) {
    colors.push_back(rt3::retrieve(ps, "bl", noValue));
    colors.push_back(rt3::retrieve(ps, "tl", noValue));
    colors.push_back(rt3::retrieve(ps, "tr", noValue));
    colors.push_back(rt3::retrieve(ps, "br", noValue));
  } else {
    colors.push_back(rt3::retrieve(ps, "color", noValue));
  }
  std::vector<RGBAColor> rgbColors;
  bool flnum = false;
  for(auto s : colors) {
    if((s[0] > 0 && s[0] < 1) || (s[1] > 0 && s[1] < 1) || (s[2] > 0 && s[2] < 1)){
      flnum = true;
      break;
    }
  }
  for(auto s : colors) {
      RGBAColor c;
      if(flnum) {
        c.r = int(s[0]*255);
        c.g = int(s[1]*255);
        c.b = int(s[2]*255);
        c.a = 255;
      } else {
        c.r = s[0];
        c.g = s[1];
        c.b = s[2];
        c.a = 255;
      }
    rgbColors.push_back(c);
  }
  
  return new BackgroundColor(rgbColors);
}

RGBAColor BackgroundColor::sample(const RGBAColor &A, const RGBAColor &B, float t) const
{
  RGBAColor R;
  //cout<<std::to_string(A.r)<<" xr " << std::to_string(B.r)<<"\n";
  R.r = rt3::Lerp(t, float(A.r), float(B.r));
  R.g = rt3::Lerp(t, float(A.g), float(B.g));
	R.b = rt3::Lerp(t, float(A.b), float(B.b));
  R.a = 255;
	return R;
}

RGBAColor BackgroundColor::sample(const float &A, const float &B) const
{
  /*_sleep(100);
  for(auto s : corners) {
    cout<<"r:"<<std::to_string(s.r)<<" g:"<<std::to_string(s.g)<<" b:"<<std::to_string(s.b)<<"para um\n";
  }*/
  auto iL = sample(corners[tl], corners[bl], B);
  auto iR = sample(corners[tr], corners[br], B);
  RGBAColor R = sample(iL, iR, A);  
  //cout<<std::to_string(B)<<" "<<std::to_string(A)<<"\n";
  //cout<<"r:"<<std::to_string(R.r)<<" g:"<<std::to_string(R.g)<<" b:"<<std::to_string(R.b)<<"para um\n";
	return R;
}

BackgroundColor::BackgroundColor(const std::vector<RGBAColor> &colors)
{
  corners[bl] = colors[bl];
  if(colors.size() > 1) {
    corners[tl] = colors[tl];
    corners[tr] = colors[tr];
    corners[br] = colors[br];
  } else {
    corners[tl] = colors[0];
    corners[tr] = colors[0];
    corners[br] = colors[0];
  }
}
} // namespace rt3
