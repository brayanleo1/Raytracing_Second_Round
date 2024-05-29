#ifndef FLAT_MATERIAL_H
#define FLAT_MATERIAL_H

#include "material.h"
namespace rt3{
class FlatMaterial : public Material {
    public:
        // Constructor
        FlatMaterial(RGBAColor col) : color{col} {// Empty constructor 
        }
        RGBAColor kd() const {
            return color;
        }
        void scatter(const Ray& r_in, const Surfel& sf) const override {
            // Empty
        }
    private:
        RGBAColor color;
};
FlatMaterial *create_flat_material(const ParamSet &ps);
} // namespace rt3
#endif // FLAT_MATERIAL_H