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

struct Bounds3f { 
	Point3f pMin = {INFINITY, INFINITY, INFINITY};
	Point3f pMax = {-INFINITY, -INFINITY, -INFINITY};
};

constexpr RGBAColor red{255,0,0,255};
constexpr RGBAColor green{0,255,0,255};
constexpr RGBAColor blue{0,0,255,255};
constexpr RGBAColor yeallow{255,255,0,255};
constexpr RGBAColor white{255,255,255,255};
constexpr RGBAColor black{0,0,0,255};
constexpr RGBAColor none{0,0,0,0};



inline RGBAColor operator*(float t, const RGBAColor &v)
{	
    return RGBAColor{ static_cast<unsigned char>(t * v.r), static_cast<unsigned char>(t * v.g), static_cast<unsigned char>(t * v.b), v.a};
}

inline RGBAColor operator*(const RGBAColor &v, float t)
{
	return t * v;
}

inline RGBAColor operator*(const RGBAColor &v1, const Spectrum &s) {
	return RGBAColor{ static_cast<unsigned char>(v1.r * s.r(), static_cast<unsigned char>(v1.g * s.g(), static_cast<unsigned char>(v1.b * s.b()), v1.a))};
}

inline RGBAColor operator*(const Spectrum &s, const RGBAColor &v1) {
	return v1 * s;
}

inline RGBAColor operator+(const RGBAColor &v1, const RGBAColor &v2)
{
	return RGBAColor{ static_cast<unsigned char>(v1.r + v2.r), static_cast<unsigned char>(v1.g + v2.g), static_cast<unsigned char>(v1.b + v2.b), v1.a};
}

inline RGBAColor operator+=(RGBAColor &v1, const RGBAColor &v2)
{
	v1.r += v2.r;
	v1.g += v2.g;
	v1.b += v2.b;
	return v1;
}

inline RGBAColor operator+=(RGBAColor &v1, const Spectrum &s)
{
	v1.r += s.r()*255;
	v1.g += s.g()*255;
	v1.b += s.b()*255;
	return v1;
}

inline bool operator>(const RGBAColor &v1, const RGBAColor &v2)
{
	return v1.r > v2.r || v1.g > v2.g || v1.b > v2.b;
}

inline bool operator>(const Spectrum &s1, const Spectrum &s2)
{
	return s1.r() > s2.r() || s1.g() > s2.g() || s1.b() > s2.b();
}

//Now i create declarations for min and max functions
inline Point3f min(const Point3f &p1, const Point3f &p2) {
	return Point3f{std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()), std::min(p1.z(), p2.z())};
}
inline Point3f max(const Point3f &p1, const Point3f &p2) {
	return Point3f{std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()), std::max(p1.z(), p2.z())};
}

} // namespace rt3

#endif