#ifndef COMMONS_H
#define COMMONS_H

#include <cstdint>

struct RGBAColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Bounds3f {
  // ... (members to store bounding box data)
};

constexpr RGBAColor red{255,0,0,255};
constexpr RGBAColor green{0,255,0,255};
constexpr RGBAColor blue{0,0,255,255};
constexpr RGBAColor yeallow{255,255,0,255};
constexpr RGBAColor white{255,255,255,255};
constexpr RGBAColor black{0,0,0,255};
constexpr RGBAColor none{0,0,0,0};

#endif