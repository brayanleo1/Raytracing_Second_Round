#ifndef COMMONS_H
#define COMMONS_H

#include <cstdint>
#include "math.h"

namespace rt3 {


struct RGBAColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	bool hit = false;
	
};

struct Bounds3f { // if it is a bounding box, it should be a box, not a sphere
	//so it should have 2 points, min and max
	Point3f pMin = {INFINITY, INFINITY, INFINITY};
	Point3f pMax = {-INFINITY, -INFINITY, -INFINITY};
  // ... (members to store bounding box data)
};

constexpr RGBAColor red{255,0,0,255};
constexpr RGBAColor green{0,255,0,255};
constexpr RGBAColor blue{0,0,255,255};
constexpr RGBAColor yeallow{255,255,0,255};
constexpr RGBAColor white{255,255,255,255};
constexpr RGBAColor black{0,0,0,255};
constexpr RGBAColor none{0,0,0,0};


inline bool operator>(const RGBAColor &v1, const RGBAColor &v2)
{
	return v1.r > v2.r || v1.g > v2.g || v1.b > v2.b;
}

inline bool operator>(const Spectrum &s1, const Spectrum &s2)
{
	return s1.r() > s2.r() || s1.g() > s2.g() || s1.b() > s2.b();
}

} // namespace rt3

#endif