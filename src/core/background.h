#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "rt3-base.h"
#include "rt3.h"
#include "commons.h"

namespace rt3 {
// TODO: Create a virtual class Background and derive BackgroundColor,
// BackgroundSphereImage, BackgroundSkyBoxImage.
/*!
 * A background is basically a rectangle, have a color associated to each
 * corner. A background might be sampled based on a normalized coordinate in
 * \f$[0,1]^2.\f$. We generate samples with bilinear interpolation of the corner
 * colors. We consider the corners in a circular clockwise order, starting at
 * the lower-left corner.
 *
 * Alternatively, a background might be a regular image.
 */
class Background {
 public:
  /// Types of color mapping scheme
  enum class mapping_t {
    screen = 0,  //!< background is assigned to the image screen
    spherical    //!< background is assigne to a sphere surrounding the scene.
  };

 public:
  mapping_t mapping_type;  //!< sampling type we use to pick a color.

  /// Ctro receiving a single color for the entire background.
  Background(mapping_t mt = mapping_t::spherical)
      : mapping_type{mt} { /* empty */
  }

  virtual ~Background(){/* empty */};
  Spectrum sampleXYZ(const Point2f &pixel_ndc) const;

  virtual RGBAColor sample ( const RGBAColor &A, const RGBAColor &B, float t ) const = 0;
  virtual RGBAColor sample ( const float &A, const float &B) const = 0;
};

class BackgroundColor : public Background {
 private:
  /// Each corner has a color associated with.
  // Spectrum corners[4] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  /// Corner indices.

  RGBAColor corners[4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

  enum Corners_e {
    bl = 0,  //!< Bottom left corner.
    tl,      //!< Top left corner.
    tr,      //!< Top right corner.
    br       //!< Bottom right corner.
  };

 public:
  /// Ctro receives a list of four colors, for each corner.
  BackgroundColor() {
    // TODO
  }

  RGBAColor sample ( const RGBAColor &A, const RGBAColor &B, float t ) const override;

  RGBAColor sample ( const float &A, const float &B) const override;

  BackgroundColor( const std::vector< RGBAColor >& colors );

  virtual ~BackgroundColor(){};
};

// factory pattern functions.
BackgroundColor *create_color_background(const ParamSet &ps);
}  // namespace rt3
#endif
