#include "scene.h"
#include "primitive.h"

namespace rt3 {

bool Scene::intersect(const Ray& r, Surfel *isect) const {
    return aggregate->intersect(r, isect);
}

bool Scene::intersect_p(const Ray& r) const {
    return aggregate->intersect_p(r);
}

Scene *create_scene(const std::shared_ptr<Primitive> &ag, const std::shared_ptr< Background > &bkg) {
    return new Scene(ag, bkg);
}

} // namespace rt3