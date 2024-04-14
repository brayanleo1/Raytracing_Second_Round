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
    int fovy;
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

/*

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Point3& position, const Point3& target, const Vector3& up,
                      float vertical_fov_degrees, float aspect_ratio)
        : position(position), aspect_ratio(aspect_ratio)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(cross(forward, up));
        Vector3 up_vec = cross(right, forward);

        float half_fov_radians = radians(vertical_fov_degrees) / 2.f;
        float half_height = std::tan(half_fov_radians);
        float half_width = aspect_ratio * half_height;

        top_left = position + forward - half_width * right + half_height * up_vec;
        horizontal = 2 * half_width * right;
        vertical = -2 * half_height * up_vec;
    }

    Ray generate_ray(int x, int y) const override {
        float u = static_cast<float>(x) / static_cast<float>(width);
        float v = static_cast<float>(y) / static_cast<float>(height);
        Point3 pixel = top_left + u * horizontal + v * vertical;
        return Ray(position, normalize(pixel - position));
    }

private:
    Point3 position;
    Point3 top_left;
    Vector3 horizontal;
    Vector3 vertical;
    float aspect_ratio;
    int width;
    int height;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const Point3& position, const Point3& target, const Vector3& up,
                       float height, float aspect_ratio)
        : position(position), height(height), aspect_ratio(aspect_ratio)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(cross(forward, up));
        Vector3 up_vec = cross(right, forward);

        half_width = height * aspect_ratio / 2.f;
        top_left = position + forward - half_width * right + height / 2.f * up_vec;
        horizontal = 2 * half_width * right;
        vertical = -height * up_vec;
    }

    Ray generate_ray(int x, int y) const override {
        float u = static_cast<float>(x) / static_cast<float>(width);
        float v = static_cast<float>(y) / static_cast<float>(height);
        Point3 pixel = top_left + u * horizontal + v * vertical;
        return Ray(pixel, normalize(-position));
    }

private:
    Point3 position;
    Point3 top_left;
    Vector3 horizontal;
    Vector3 vertical;
    float half_width;
    float height;
    float aspect_ratio;
    int width;
    int height;
};



*/