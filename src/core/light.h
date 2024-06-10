#ifndef LIGHT_H
#define LIGHT_H

#include "surfel.h"
#include "ray.h"
#include "commons.h"
#include "paramset.h"

namespace rt3 {
    
enum class light_flag_e : int {
	point = 1,
	directional = 2,
	area = 4,
	ambient = 8,
	spot = 16
};

// Verifica se há oclusão entre dois pontos de contato.

class VisibilityTester {
public:
    VisibilityTester()=default;
    VisibilityTester( const Surfel& p0, const Surfel& p1) : p0{p0}, p1{p1} {};
    bool unoccluded( const Scene& scene );
public:
    Surfel p0, p1; //p0 is the point of origin (or surface hitting point) and p1 is the point of destination (or light origin point).
};

class Light {
public:
  light_flag_e flags;
  Spectrum intensity;
public:
  virtual ~Light(){};
  Light( int flags, const Spectrum& intensity ) : flags{static_cast<light_flag_e>(flags)}, intensity{intensity} {};
  /// Retorna a intensidade da luz, direção e o teste oclusão.
  virtual Spectrum sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ ) = 0;
  virtual void preprocess( const Scene & ) {};
  bool is_ambient();
};

Light *create_light(const ParamSet &ps);

} // namespace rt3

#endif //LIGHT_H