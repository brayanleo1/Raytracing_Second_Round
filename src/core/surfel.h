#ifndef SURFEL_H
#define SURFEL_H

#include "rt3.h"

namespace rt3
{
class Surfel{
	public:
		Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, float time, 
			const Point2f& uv, const Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {/* empty */};

		Surfel() = default;

		Point3f p;        //!< Contact point.
		Vector3f n;       //!< The surface normal.
		Vector3f wo;      //!< Outgoing direction of light, which is -ray.
		Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
		const Primitive *primitive=nullptr; //!< Pointer to the primitive.
        float time; //!< Time of the hit.
};
}
#endif  // SURFEL_H
