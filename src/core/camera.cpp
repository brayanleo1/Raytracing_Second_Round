#include "camera.h"
#include "paramset.h"

namespace rt3 {

    void Camera::add_film(Film* f) {
    film = f;
    }

    Ray PerspectiveCamera::generate_ray(int x, int y) const {

       Vector3f gaze = look_at - look_from;
       Vector3f w = gaze; w.make_unit_vector(); // left-hand orientation
       Vector3f u = cross( vup, w ); u.make_unit_vector(); // The order inside cross matters. Can you guess why?
       Vector3f v = cross( w, u ); v.make_unit_vector();
       Point3f e = look_from;
    
       auto a = film->get_resolution()[0]/film->get_resolution()[1];
       auto h = tan(fovy/2);
       auto t = h;
       auto b = -h;
       auto l = -a * h;
       auto r = a * h;

       auto un = l+(r-l)*(x+0.5)/film->get_resolution()[0] /*column*/;
       auto vn = b+(t-b)*(y+0.5)/film->get_resolution()[1] /*row*/;

       return Ray(e, w+un*u+vn*v);
    }

    Ray OrthographicCamera::generate_ray(int x, int y) const{

       Vector3f gaze = look_at - look_from;
       Vector3f w = gaze; w.make_unit_vector(); // left-hand orientation
       Vector3f u = cross( vup, w ); u.make_unit_vector(); // The order inside cross matters. Can you guess why?
       Vector3f v = cross( w, u ); v.make_unit_vector();
       Point3f e = look_from;
       
       auto t = screen_window[3];
       auto b = screen_window[2];
       auto l = screen_window[0];
       auto r = screen_window[1];

       auto un = l+(r-l)*(x+0.5)/film->get_resolution()[0] /*column*/;
       auto vn = b+(t-b)*(y+0.5)/film->get_resolution()[1] /*row*/;

       return Ray(e+un*u+vn*v, w);
    }

    Camera *create_camera(const ParamSet &ps, const ParamSet &ps_lkat){
        std::cout << ">>> Inside create_camera()\n";
        Point3f lk_at = retrieve(ps_lkat, "look_at", Point3f(0, 0, 0));
        Point3f lk_fr = retrieve(ps_lkat, "look_from", Point3f(0, 0, 0));
        Point3f up = retrieve(ps_lkat, "up", Point3f(0, 0, 0));
        float fov = retrieve(ps, "fovy", float(0));
        std::vector<real_type> sw = retrieve(ps, "screen_window", std::vector<real_type>{0, 0, 0, 0});
        if(fov == 0) {
            return new OrthographicCamera(lk_at, lk_fr, up, sw);
        } else {
            return new PerspectiveCamera(lk_at, lk_fr, up, fov);
        }
        
    }

}