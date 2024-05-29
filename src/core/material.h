#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.h"
#include "commons.h"
#include "paramset.h"
#include "ray.h"
#include "surfel.h"
namespace rt3{
class Material {
    public:
        // Constructor
        Material() {/*empty*/}

        virtual void scatter(const Ray& r_in, const Surfel& sf) const = 0;
};
Material *create_material(const ParamSet &ps);
}
#endif //MATERIAL_H