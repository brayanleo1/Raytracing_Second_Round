#ifndef SHAPE_H
#define SHAPE_H

#include "rt3.h"
#include "paramset.h"
#include "surfel.h"
#include "ray.h"
#include "commons.h"

namespace rt3 {
class Shape {
    public:
        ~Shape(){};
        virtual Bounds3f world_bounds() const = 0;
        virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
        // Simpler & faster version of intersection that only return true/false.
        // It does not compute the hit point information.
        virtual bool intersect_p( const Ray& r ) const = 0;

};
Shape *create_shape(const ParamSet &ps);
}
#endif //SHAPE_H