#include "primitive.h"
#include "triangle.h"

namespace rt3 {

Primitive *create_primitive(const ParamSet &ps, const std::shared_ptr<Material> &mtr) {
    if(retrieve(ps, "type", std::string("trianglemesh")) == "trianglemesh") {
        // if it is a trianglemesh, we need to create a PrimitiveAggregate
        // but to do that, we need to create a GeometricPrimitive for each triangle
        // and then create the PrimitiveAggregate with all the GeometricPrimitives
        std::vector<std::shared_ptr<Shape>> trimesh = create_triangle_mesh_shape(retrieve(ps, "flip_normals", false), ps);
        std::vector<std::shared_ptr<Primitive>> primitives;
        for (const auto &triangle : trimesh) {
            primitives.push_back(std::shared_ptr<Primitive>(new GeometricPrimitive(triangle, mtr)));
        }
        return new PrimitiveAggregate(primitives);
    }
    else if(retrieve(ps, "type", std::string("sphere")) == "sphere") {
        return create_geometric_primitive(ps, mtr);
    }
    else {
        std::cerr << "Primitive type not recognized." << std::endl;
        return nullptr;
    }
}

}