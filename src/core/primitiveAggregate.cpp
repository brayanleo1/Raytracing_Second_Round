#include "primitive.h"

namespace rt3 {

PrimitiveAggregate *create_primitive_aggregate(const std::vector<std::shared_ptr<Primitive>> &primitives) {
    return new PrimitiveAggregate(primitives);
}
} // namespace rt3