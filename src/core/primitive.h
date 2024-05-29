#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "ray.h"
#include "surfel.h"
#include "material.h"
#include "shape.h"
#include "commons.h"
namespace rt3
{
class Primitive {
	public:
		virtual ~Primitive(){};
		virtual Bounds3f world_bounds() const = 0;
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		//virtual const std::shared_ptr<Material> get_material(void) const = { return material; }
		virtual const std::shared_ptr<Material> get_material(void) const = 0;
};

class GeometricPrimitive : public Primitive {
	public:
		GeometricPrimitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material): shape{shape}, material{material} {}
		Bounds3f world_bounds() const override {return Bounds3f();}
		bool intersect ( const Ray& r, Surfel *sf ) const override {bool hit = shape.get()->intersect(r, sf);
		if(hit) {sf->primitive = this;} return hit;}
		bool intersect_p ( const Ray& r) const override {return shape.get()->intersect_p(r);}
		const std::shared_ptr<Material> get_material(void) const override {return material;}
	private:
		std::shared_ptr<Shape> shape;
		std::shared_ptr<Material> material;
};

class PrimitiveAggregate : public Primitive {
	public:
		PrimitiveAggregate(std::vector<std::shared_ptr<Primitive>> primitives): primitives{primitives} {}
		Bounds3f world_bounds() const override {return Bounds3f();}
		bool intersect ( const Ray& r, Surfel *sf ) const override {
			bool hit = false;
			for (const auto &primitive : primitives) {
				hit |= primitive.get()->intersect(r, sf);
			}
			/*For test*/ //std::cout << "hitted this color " << sf->primitive->get_material().get() << "\n";
			return hit;
		}
		bool intersect_p ( const Ray& r) const override {
			for (const auto &primitive : primitives) {
				if (primitive.get()->intersect_p(r)) {
					return true;
				}
			}
			return false;
		}
		const std::shared_ptr<Material> get_material(void) const override {return nullptr;}
	private:
		std::vector<std::shared_ptr<Primitive>> primitives;
	};

PrimitiveAggregate *create_primitive_aggregate(const std::vector<std::shared_ptr<Primitive>> &primitives);
GeometricPrimitive *create_geometric_primitive(const ParamSet &ps, const std::shared_ptr<Material> &mtr);
}
#endif  // PRIMITIVE_H