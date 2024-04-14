#ifndef RAY_H
#define RAY_H

#include "rt3.h"

namespace rt3 {
class Ray {
public:
  Ray(const Point3f &o, const Vector3f &d, real_type start = 0,
      real_type end = INFINITY)
      : o{o}, d{d}, t_min{0.F}, t_max{INFINITY} { /*empty*/
  }
  Ray() : t_min{0.F}, t_max{INFINITY} { /*empty*/
  }
  Point3f origin() const { return o; }
  Vector3f direction() const { return d; }
  Point3f operator()(real_type t) const { return o + d * t; }

private:
  Point3f o;                      //!< origin
  Vector3f d;                     //!< direction
  mutable real_type t_min, t_max; //!< parameters
};
} // namespace rt3

#endif // RAY_H
