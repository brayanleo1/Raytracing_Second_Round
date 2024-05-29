#include "primitive.h"
namespace rt3 {
    GeometricPrimitive *create_geometric_primitive(const ParamSet &ps, const std::shared_ptr<Material> &mtr) {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(create_shape(ps));
        
        return new GeometricPrimitive(s, mtr);
    }
}