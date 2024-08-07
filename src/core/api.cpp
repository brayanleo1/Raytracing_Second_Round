#include "api.h"
#include "background.h"
#include "camera.h"
#include "triangle.h"

#include <chrono>
#include <memory>

#include <fstream>

namespace rt3 {

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
  std::cout << ">>> Inside API::make_background()\n";
  BackgroundColor *bkg{nullptr};
  bkg = create_color_background(ps);

  // Return the newly created background.
  return bkg;
}

Material *API::make_material(const std::string &name, const ParamSet &ps) {
  std::cout << ">>> Inside API::make_material()\n";
  Material *mtr{nullptr};

  mtr = create_material(ps);

  // Return the newly created material.
  return mtr;
}

Primitive *API::make_primitive(const std::string &name, const ParamSet &ps, std::shared_ptr<Material>& mtr) {
  std::cout << ">>> Inside API::make_primitive()\n";
  Primitive *pm{nullptr};
  pm = create_primitive(ps, mtr); //Actually, this will be create_primitive(ps, mtr);
  
  

  // Return the newly created primitive.
  return pm;
}

Integrator *API::make_integrator(const std::string &name, const ParamSet &ps, std::unique_ptr<Camera>& cam) {
  std::cout << ">>> Inside API::make_integrator()\n";
  Integrator *igt{nullptr};
  // Convert unique_ptr to shared_ptr
  std::shared_ptr<Camera> shared_cam = std::move(cam);

  igt = create_integrator(ps, shared_cam);
  //igt = create_integrator(ps, cam);

  // Return the newly created integrator.
  return igt;
}

Scene *API::make_scene(const std::vector<std::shared_ptr<Primitive>> &prims, std::unique_ptr<BackgroundColor> &bkg, std::vector<std::shared_ptr<Light>> &lts) {
  std::cout << ">>> Inside API::make_scene()\n";
  Scene *scene{nullptr};
  std::shared_ptr<BackgroundColor> shared_bkg = std::move(bkg);
  //auto agg = create_primitive_aggregate(prims);
  std::shared_ptr<PrimitiveAggregate> ag = std::shared_ptr<PrimitiveAggregate>(create_primitive_aggregate(prims));
  scene = create_scene(ag, shared_bkg, lts);

  // Return the newly created scene.
  return scene;
}

Light *API::make_light(const std::string &name, const ParamSet &ps) {
  std::cout << ">>> Inside API::make_light()\n";
  Light *light{nullptr};
  light = create_light(ps);

  // Return the newly created light.
  return light;
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
  
  std::unique_ptr<BackgroundColor> the_background{
      make_background(render_opt->bkg_type, render_opt->bkg_ps)};

  std::unique_ptr<Film> the_film{
      make_film(render_opt->film_type, render_opt->film_ps)};

  std::unique_ptr<Camera> the_camera{
      make_camera(render_opt->camera_type, render_opt->camera_ps, render_opt->look_at)};

  //Add film to the camera
  std::cout << "Adding film to camera\n";
  the_camera->add_film(the_film.get());
  std::cout << "Film added to camera\n";

  //Add material
  for(long unsigned int i = 0; i < render_opt->material_ps.size(); i++)
  {
    std::shared_ptr<Material> the_material{
      make_material(render_opt->material_type[i], render_opt->material_ps[i])};
    render_opt->materials_list.push_back(the_material);
  }

  //Add primitives
  for(long unsigned int i = 0; i < render_opt->primitives_ps.size(); i++)
 {
   std::shared_ptr<Primitive> the_primitives{
     make_primitive(render_opt->primitives_type[i], render_opt->primitives_ps[i], render_opt->materials_list[render_opt->material_index[i]])};
   if(render_opt->primitives_type[i] == "trianglemesh") {
     const PrimitiveAggregate* pg = dynamic_cast<const PrimitiveAggregate*>(the_primitives.get());
     auto q = pg->get_primitives(); //PrimitiveAggregate
     for(long unsigned int j = 0; j < q.size(); j++) {
       render_opt->primitives_list.push_back(q[j]);
     }
   } else {
     render_opt->primitives_list.push_back(the_primitives);
   }
 }

  //Add lights
  for(long unsigned int i = 0; i < render_opt->light_ps.size(); i++)
  {
    std::shared_ptr<Light> the_light{
      make_light(render_opt->light_type[i], render_opt->light_ps[i])};
    render_opt->lights_list.push_back(the_light);
  }
  
  std::unique_ptr<Integrator> the_integrator{
      make_integrator(render_opt->igt_type, render_opt->igt_ps, the_camera)};

  std::unique_ptr<Scene> the_scene{
      make_scene(render_opt->primitives_list, the_background, render_opt->lights_list)};

  

  // Run only if we got film and background.
  if (the_integrator && the_scene) {
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
    //render(*the_background, the_camera.get(), render_opt->primitives_list); // TODO: This is the ray tracer's  main loop.
    the_integrator->render(the_scene);
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

    // Log the time elapsed in the render process on a file. If there is more than one, then append.
    std::ofstream log_file;
    log_file.open("render_log.txt", std::ios_base::app);
    log_file << "Time elapsed: " << diff_sec.count() << " seconds ("
             << std::chrono::duration<double, std::milli>(diff).count() << " ms) \n";
    log_file.close();
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

void API::integrator(const ParamSet &ps) {
  std::cout << ">>> Inside API::integrator()\n";
  VERIFY_SETUP_BLOCK("API::integrator");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->igt_type = type;
  // Store current integrator object.
  render_opt->igt_ps = ps;
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
  render_opt->material_type.push_back(type);
  render_opt->material_ps.push_back(ps);
  
}

void API::make_named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::make_named_material()\n";
  VERIFY_WORLD_BLOCK("API::make_named_material");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  // I need to create and store in the named_materials map.
  render_opt->named_materials.push_back(std::pair<string, ParamSet>(retrieve(ps, "name", string{"unknown"}), ps));
}

void API::named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::named_material()\n";
  VERIFY_WORLD_BLOCK("API::named_material");

  // retrieve name from ps.
  std::string name = retrieve(ps, "name", string{"unknown"});
  // I need to get the name and push on material_type and material_ps
  render_opt->material_type.push_back(name);
  render_opt->material_ps.push_back(render_opt->named_materials[ //Need to find the index based on the name
    std::find_if(render_opt->named_materials.begin(), render_opt->named_materials.end(),
      [name](const std::pair<string, ParamSet>& element) { return element.first == name; }
    ) - render_opt->named_materials.begin()
    ].second);
}

void API::primitives(const ParamSet &ps) {
  std::cout << ">>> Inside API::primitives()\n";
  VERIFY_WORLD_BLOCK("API::primitives");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->primitives_type.push_back(type);
  render_opt->primitives_ps.push_back(ps);
  render_opt->material_index.push_back(render_opt->material_type.size()-2);

}

void API::light_source(const ParamSet &ps) {
  std::cout << ">>> Inside API::light_source()\n";
  VERIFY_WORLD_BLOCK("API::light_source");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{"unknown"});
  render_opt->light_type.push_back(type);
  render_opt->light_ps.push_back(ps);
}

} // namespace rt3
