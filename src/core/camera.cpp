#include "camera.h"
#include "paramset.h"

namespace rt3 {

    void Camera::add_film(Film* f) {
    film = f;
    }

    Ray PerspectiveCamera::generate_ray(int x, int y) const {
        /*std::clog<<"pers"<<"\n";*/
        /*
        *Vector3f gaze = look_at - look_from; Vector3 w = normalize(gaze); // left-hand orientation
        *Vector3f u = normalize( cross( vup, w ) ); // The order inside cross matters. Can you guess why?
        *Vector3f v = normalize( cross ( w, u ) );
        *Point3f e = look_from;
        */
       Vector3f gaze = look_at - look_from;
       Vector3f w = gaze; w.make_unit_vector(); // left-hand orientation
       Vector3f u = cross( vup, w ); u.make_unit_vector(); // The order inside cross matters. Can you guess why?
       Vector3f v = cross( w, u ); v.make_unit_vector();
       Point3f e = look_from;
       //l, r, b, t are dimensions, like from 200 x 100: l = 0; r = 199, t = 0; b = 99 ?
       //i,j are x,y
       //fd = 1,(focal distance)
       auto a = film->get_resolution()[0]/film->get_resolution()[1];
       auto h = tan(fovy/2);
       auto t = h;
       auto b = -h;
       auto l = -a * h;
       auto r = a * h;

       auto un = l+(r-l)*(x+0.5)/film->get_resolution()[0] /*column*/;
       auto vn = b+(t-b)*(y+0.5)/film->get_resolution()[1] /*row*/;


       //Cada ^ indica que é um vetor
       /*u = l+(r−l)(i+0.5)/(n_x)
       **v​ = b+(t−b)(j+0.5)/(n_y) */
       /*
       *ray.direction ← (fd)w^+uu^+vv^
        *ray.origin ←e
       */
       return Ray(e, w+un*u+vn*v);
    }

    Ray OrthographicCamera::generate_ray(int x, int y) const{
       /*std::clog<<"orto"<<"\n";
       std::clog<< look_at << "\n";
       std::clog<< look_from << "\n";*/
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

       /*
       *ray.direction ← w^
        *ray.origin ←e +uu^+vv^
       */
       return Ray(w, e+un*u+vn*v);
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