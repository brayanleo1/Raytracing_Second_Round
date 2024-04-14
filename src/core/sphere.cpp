#include "sphere.h"

namespace rt3 {

bool Sphere::intersect(const Ray& r, Surfel* sf) const {
  // Implement the intersection between the ray and sphere
  // Calculate the intersection point, surface normal, and other required information
  // Assign the values to the Surfel object (sf)
  // Return true if intersection found, false otherwise
  return false;
}

bool Sphere::intersect_p(const Ray& r) const {
  // Implement the simplified intersection check between the ray and sphere
  // Return true if intersection found, false otherwise
  Vector3f oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  return (discriminant > 0);

  return false;
}

Sphere *create_sphere(const ParamSet &ps) {
  Point3f center = retrieve(ps, "center", Point3f(0,0,0));
  float radius = retrieve(ps, "radius", float(0.0));
  
  return new Sphere(center, radius);
}

}