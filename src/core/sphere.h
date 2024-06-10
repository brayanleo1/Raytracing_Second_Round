#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

namespace rt3
{
class Sphere : public Shape {
    public: 
        ~Sphere(){}

        Sphere(const Point3f& center, float radius)
            : center(center), radius(radius) {}
        
        /*
         * Implementation of the intersection between the ray and sphere
         * Calculate the intersection point, surface normal, and other required information
         * Assign the values to the Surfel object (sf)
         * Return true if intersection found, false otherwise
        */
        bool intersect(const Ray& r, Surfel* sf) const override;

            
        bool intersect_p(const Ray& r) const override;

        Bounds3f world_bounds() const;

    private:
        Point3f center;
        float radius;
};
Sphere *create_sphere(const ParamSet &ps);

}
#endif  // SPHERE_H