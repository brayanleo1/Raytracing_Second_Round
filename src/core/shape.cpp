#include "shape.h"
#include "sphere.h"

namespace rt3{

Shape *create_shape(const ParamSet &ps) {
    if(retrieve(ps, "radius", float(-999.0)) != -999.0) {//It is a Sphere
        return create_sphere(ps);
    }
}

} // namespace rt3