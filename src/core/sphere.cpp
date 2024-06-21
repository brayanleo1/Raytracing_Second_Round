#include "sphere.h"

namespace rt3 {

bool Sphere::intersect(const Ray& r, Surfel* sf) const {
  // Implement the intersection between the ray and sphere
  // Calculate the intersection point, surface normal, and other required information
  // Assign the values to the Surfel object (sf)
  // Return true if intersection found, false otherwise
  Vector3f oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;

  if (discriminant > 0) {
    float t = (-b - sqrt(discriminant)) / (2.0 * a);

    if (t > 0 && t < r.t_max) {
      r.t_max = t;
      sf->p = r.origin() + t * r.direction();
      sf->n = (sf->p - center); // Unnormalized normal?
      sf->n.make_unit_vector(); // Normalized normal
      sf->wo = -r.direction(); // Outgoing direction of light, which is -ray
      sf->uv = Point2f{0, 0}; // Set the parametric coordinate to (0, 0) for now
      sf->time = t; // Set the time of the hit to t

      return true;
    }
  }
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
  if(discriminant < 0) {
    return false;
  } else {
    float t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t > 0 && t < r.t_max) {
      r.t_max = t;
      return true;
      }
    return false;
  }
}

Bounds3f Sphere::world_bounds() const {
  // Implement the bounding box of the sphere
  // Return the bounding box
  Point3f radius3 = Point3f{radius, radius, radius};
  return Bounds3f{center - radius3, center + radius3};
}


Sphere *create_sphere(const ParamSet &ps)
{
  Point3f center = retrieve(ps, "center", Point3f(0,0,0));
  float radius = retrieve(ps, "radius", float(0.0));
  
  return new Sphere(center, radius);
}

}