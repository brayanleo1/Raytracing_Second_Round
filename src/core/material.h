#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.h"
#include "commons.h"
#include "paramset.h"
namespace rt3{
class Material {
    private:
        std::string type;
        RGBAColor color;
    public:
        Material(std::string typ, RGBAColor col ) : type{typ}, color{col} {};
        RGBAColor get_color() const {
            return color;
        };
        std::string get_type() const {
            return type;
        };
};
Material *create_material(const ParamSet &ps);
}
#endif //MATERIAL_H