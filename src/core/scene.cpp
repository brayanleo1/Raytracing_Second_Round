#include "scene.h"
#include "primitive.h"

namespace rt3 {

bool Scene::intersect(const Ray& r, Surfel *isect) const {
    return aggregate->intersect(r, isect);
}

bool Scene::intersect_p(const Ray& r) const {
    return aggregate->intersect_p(r);
}

Scene *create_scene(const std::shared_ptr<Primitive> &ag, const std::shared_ptr< Background > &bkg, std::vector<std::shared_ptr<Light>> lts) {
    return new Scene(ag, bkg, lts);
}

bool Scene::have_ambient_light() const {
    for (const auto &light : lights) {
        if (light.get()->is_ambient()) {
            return true;
        }
    }
    return false;
}

Bounds3f Scene::world_bound() const {
    return aggregate.get()->world_bounds();
}

} // namespace rt3