#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "commons.h"
#include "ray.h"
#include "film.h"
#include "paramset.h"

namespace rt3
{
class Camera{
    public:
    virtual Ray generate_ray(int x, int y) const = 0;
    void add_film(Film* f);
    Point3f look_at;
    Point3f look_from;
    Point3f vup;
    Film* film;
    Camera(Point3f lk_at, Point3f lk_fr, Point3f up) : look_at{lk_at}, look_from{lk_fr}, vup{up} {/*empty*/}
    
};
class PerspectiveCamera : public Camera {
    public:
    Ray generate_ray(int x, int y) const override;
    PerspectiveCamera(Point3f lk_at, Point3f lk_fr, Point3f up, int fov) : Camera(lk_at, lk_fr, up), fovy{fov} {};
    float fovy;
	// ...
};
class OrthographicCamera : public Camera {
    public:
    Ray generate_ray(int x, int y) const override;
    OrthographicCamera(Point3f lk_at, Point3f lk_fr, Point3f up, std::vector<real_type> sw) : Camera(lk_at, lk_fr, up), screen_window{sw} {};
    std::vector<real_type> screen_window;
	// ...
};

Camera *create_camera(const ParamSet &ps, const ParamSet &ps_lkat); 
}

#endif  // CAMERA_H