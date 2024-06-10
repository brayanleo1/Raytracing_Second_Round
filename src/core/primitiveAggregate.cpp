#include "primitive.h"

namespace rt3 {

bool PrimitiveAggregate::intersect( const Ray& r, Surfel *sf ) const {
	bool hit = false;
	for (const auto &primitive : primitives) {
		hit |= primitive.get()->intersect(r, sf);
	}
	return hit;
	}

bool PrimitiveAggregate::intersect_p(const Ray &r) const {
	for (const auto &primitive : primitives) {
		if (primitive.get()->intersect_p(r)) {
			return true;
		}
	}
	return false;
}

Bounds3f PrimitiveAggregate::world_bounds() const {
    Bounds3f bounds;
    for (const auto &primitive : primitives) {
        bounds = PrimitiveAggregate::union_bounds(bounds, primitive.get()->world_bounds());
    }
    return bounds;
}

Bounds3f PrimitiveAggregate::union_bounds(const Bounds3f &b1, const Bounds3f &b2) const {
    return Bounds3f{PrimitiveAggregate::min(b1.pMin, b2.pMin), PrimitiveAggregate::max(b1.pMax, b2.pMax)};
}

Point3f PrimitiveAggregate::min(const Point3f &p1, const Point3f &p2) const {
    return Point3f{std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()), std::min(p1.z(), p2.z())};
}

Point3f PrimitiveAggregate::max(const Point3f &p1, const Point3f &p2) const {
    return Point3f{std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()), std::max(p1.z(), p2.z())};
}

PrimitiveAggregate *create_primitive_aggregate(const std::vector<std::shared_ptr<Primitive>> &primitives) {
    return new PrimitiveAggregate(primitives);
}


} // namespace rt3