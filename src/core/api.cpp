#include "api.h"
#include "background.h"
#include "camera.h"
#include "primitive.h"
#include "sphere.h"
#include "material.h"

#include <chrono>
#include <memory>

namespace rt3 {

void render(BackgroundColor backgroundb, const Camera *camera) {
  // Perform objects initialization here.
  // The Film object holds the memory for the image.
  // ...

  auto background = backgroundb;
  auto w = camera->film->get_resolution()[0];
  auto h = camera->film->get_resolution()[1];

  // [4] Create spheres
  auto s1 = Sphere({-1, 0.5, 5}, 0.4); // ps is the ParamSet with all information regarding a sphere.
  auto s2 = Sphere({1, -0.5, 8}, 0.4);
  auto s3 = Sphere({-1, -1.5, 3.5}, 0.4);
  // [5] Add spheres to list of primitives.
  std::vector< Sphere > obj_list;
  obj_list.push_back( s1 );
  obj_list.push_back( s2 );
  obj_list.push_back( s3 );

  // Traverse all pixels to shoot rays from.
  for ( int j = 0 ; j < h ; j++ ) {
      for( int i = 0 ; i < w ; i++ ) {
          Ray ray = camera->generate_ray( i, j );
          // Not shooting rays just yet; so let us sample the background.
          auto colorDef = background.sample( float(i)/float(w), float(j)/float(h) );
          // Get the backgound color in case the ray hits nothing.
          if ( background.mapping_type == Background::mapping_t::screen )
            colorDef = background.sample( i/w, j/h ); // screen mapping needs a normalized pixel coord.
            // Traverse each object of the scene.
          for ( const Primitive& p : obj_list) {
            // Each time the ray hits something, max_t parameter of the ray must be updated.
            if ( p.intersect_p( ray ) ) // Does the ray hit any sphere in the scene?
              colorDef = red;  // Just paint it red.
            }
          camera->film->add_sample( {i,j},  colorDef ); // set image buffer at position (i,j), accordingly.

      }
  }
  // send image color buffer to the output file.
  camera->film->write_image();
}

//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
std::unique_ptr<RenderOptions> API::render_opt;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Film *API::make_film(const std::string &name , const ParamSet &ps) {
  std::cout << ">>> Inside API::make_film()\n";
  Film *film{nullptr};
  film = create_film(ps);

  // Return the newly created film.
  return film;
}

Camera *API::make_camera(const std::string &name , const ParamSet &ps, const ParamSet &ps_lkat) {
  std::cout << ">>> Inside API::make_camera()\n";
  Camera *camera{nullptr};
  camera = create_camera(ps, ps_lkat);

  // Return the newly created camera.
  return camera;
}

BackgroundColor *API::make_background(const std::string &name, const ParamSet &ps) {
  std::cout << ">>> Inside API::background()\n";
  BackgroundColor *bkg{nullptr};
  bkg = create_color_background(ps);

  // Return the newly created background.
  return bkg;
}

Material *API::make_material(const std::string &name, const ParamSet &ps) {
  std::cout << ">>> Inside API::material()\n";
  Material *mtr{nullptr};
  mtr = create_material(ps);

  // Return the newly created material.
  return mtr;
}

Primitive *API::make_primitive(const std::string &name, const ParamSet &ps, std::shared_ptr<Material>& mtr) {
  std::cout << ">>> Inside API::primitive()\n";
  Primitive *pm{nullptr};
  //pm = create_sphere(ps, mtr);

  // Return the newly created primitive.
  return pm;
}

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ
// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine(const RunningOptions &opt) {
  // Save running option sent from the main().
  curr_run_opt = opt;
  // Check current machine state.
  if (curr_state != APIState::Uninitialized) {
    RT3_ERROR("API::init_engine() has already been called! ");
  }
  // Set proper machine state
  curr_state = APIState::SetupBlock;
  // Preprare render infrastructure for a new scene.
  render_opt = std::make_unique<RenderOptions>();
  // Create a new initial GS
  // curr_GS = GraphicsState();
  RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_up() {
  // Check for correct machine state
  if (curr_state == APIState::Uninitialized) {
    RT3_ERROR("API::clean_up() called before engine initialization.");
  } else if (curr_state == APIState::WorldBlock) {
    RT3_ERROR("API::clean_up() called inside world definition section.");
  }
  curr_state = APIState::Uninitialized;

  RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run() {
  // Try to load and parse the scene from a file.
  RT3_MESSAGE("[2] Beginning scene file parsing...\n");
  // Recall that the file name comes from the running option struct.
  parse(curr_run_opt.filename.c_str());
}

void API::world_begin() {
  VERIFY_SETUP_BLOCK("API::world_begin"); // check for correct machine state.
  curr_state = APIState::WorldBlock;      // correct machine state.
}

void API::world_end() {
  VERIFY_WORLD_BLOCK("API::world_end");
  // The scene has been properly set up and the scene has
  // already been parsed. It's time to render the scene.

  // At this point, we have the background as a solitary pointer here.
  // In the future, the background will be parte of the scene object.
  std::unique_ptr<BackgroundColor> the_background{
      make_background(render_opt->bkg_type, render_opt->bkg_ps)};
  // Same with the film, that later on will belong to a camera object.
  std::unique_ptr<Film> the_film{
      make_film(render_opt->film_type, render_opt->film_ps)};

  std::unique_ptr<Camera> the_camera{
      make_camera(render_opt->camera_type, render_opt->camera_ps, render_opt->look_at)};

  //Add material
  std::shared_ptr<Material> the_material{
      make_material(render_opt->material_type, render_opt->material_ps)};

  std::shared_ptr<Material> flatRedMaterial = std::make_shared<Material>("Flat", red);
  std::unique_ptr<Primitive> the_primitive{
      make_primitive(render_opt->primitives_type, render_opt->primitives_ps, flatRedMaterial)};

  /*
  //Add primitive
  std::unique_ptr<Primitive> the_primitive{
      make_primitive(render_opt->primitives_type, render_opt->primitives_ps, the_material)};
    */

  //Add objects to the list
  std::vector<std::shared_ptr<Primitive> > objects;

  //Add film to the camera
  the_camera->add_film(the_film.get());

  // Run only if we got film and background.
  if (the_film && the_background) {
    RT3_MESSAGE("    Parsing scene successfuly done!\n");
    RT3_MESSAGE("[2] Starting ray tracing progress.\n");

    // Structure biding, c++17.
    auto res = the_film->get_resolution();
    size_t w = res[0];
    size_t h = res[1];
    RT3_MESSAGE("    Image dimensions in pixels (W x H): " + std::to_string(w) +
                " x " + std::to_string(h) + ".\n");
    RT3_MESSAGE(
        "    Ray tracing is usually a slow process, please be patient: \n");

    //================================================================================
    auto start = std::chrono::steady_clock::now();
    render(*the_background, the_camera.get()); // TODO: This is the ray tracer's  main loop.
    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start; // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) +
                " seconds (" +
                std::to_string(
                    std::chrono::duration<double, std::milli>(diff).count()) +
                " ms) \n");
  }
  // [4] Basic clean up
  curr_state = APIState::SetupBlock; // correct machine state.
  reset_engine();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine() {
  // curr_GS = GraphicsState();
  // This will delete all information on integrator, cameras, filters,
  // acceleration structures, etc., that has been set previously.
  render_opt = std::make_unique<RenderOptions>();
}

void API::background(const ParamSet &ps) {
  std::cout << ">>> Inside API::background()\n";
  VERIFY_WORLD_BLOCK("API::background");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->bkg_type = type;
  // Store current background object.
  render_opt->bkg_ps = ps;
}

void API::film(const ParamSet &ps) {
  std::cout << ">>> Inside API::film()\n";
  VERIFY_SETUP_BLOCK("API::film");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->film_type = type;
  render_opt->film_ps = ps;
}

void API::camera(const ParamSet &ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::camera");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->camera_type = type;
  render_opt->camera_ps = ps;
}

void API::look_at(const ParamSet &ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::look_at");

  //pass the parameters
  render_opt->look_at = ps;
}

void API::material(const ParamSet &ps) {
  std::cout << ">>> Inside API::material()\n";
  VERIFY_WORLD_BLOCK("API::material");

  
  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->material_type = type;
  render_opt->material_ps = ps;
  
}

void API::primitives(const ParamSet &ps) {
  std::cout << ">>> Inside API::primitives()\n";
  VERIFY_WORLD_BLOCK("API::primitives");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->primitives_type = type;
  render_opt->primitives_ps = ps;
}

} // namespace rt3
