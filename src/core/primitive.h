#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "ray.h"
#include "surfel.h"
#include "material.h"
namespace rt3
{
class Primitive {
	public:
		virtual ~Primitive(){};
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		//virtual const Material * get_material(void) const = { return material; }
		virtual const Material* get_material() const { return material.get(); }
	private:
		std::shared_ptr<Material> material;
};

}

#endif  // PRIMITIVE_H