#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

namespace rt3
{
class Sphere : public Primitive {
    public: 
        Sphere(const Point3f& center, float radius)
            : center(center), radius(radius) {}

        ~Sphere(){}
        
        /*
         * Implementation of the intersection between the ray and sphere
         * Calculate the intersection point, surface normal, and other required information
         * Assign the values to the Surfel object (sf)
         * Return true if intersection found, false otherwise
        */
        bool intersect(const Ray& r, Surfel* sf) const override;

            
        bool intersect_p(const Ray& r) const override;

        Sphere *create_sphere(const ParamSet &ps);


    private:
        Point3f center;
        float radius;
        std::shared_ptr<Material> material;
};
//Sphere *create_primitive(const ParamSet &ps, const std::shared_ptr<Material>& mtr);

}
#endif  // SPHERE_H