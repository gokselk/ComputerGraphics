//
// Created by goksel on 11/27/21.
//

#include "RayHit.h"

#include <utility>
#include "Ray.h"

RayHit::RayHit(const Ray& ray, int material_id) : material_id(material_id) {
    this->ray = new Ray(ray);
    t = -9999999;
}

void RayHit::setHit(float t, const Vec3f& _hit_point, const Vec3f& _normal) {
    this->t = t;
    hit_point = _hit_point;
    normal = _normal;
}

RayHit::RayHit(const RayHit &ray_hit){
    t = ray_hit.t;
    hit_point = ray_hit.hit_point;
    normal = ray_hit.normal;
    material_id = ray_hit.material_id;
    this->ray = new Ray(*ray_hit.ray);
}

RayHit::~RayHit() {
    delete ray;
    ray = nullptr;
}

RayHit& RayHit::operator=(const RayHit &ray_hit){
    t = ray_hit.t;
    hit_point = ray_hit.hit_point;
    normal = ray_hit.normal;
    material_id = ray_hit.material_id;
    *this->ray = *ray_hit.ray;
    return *this;
}

Ray RayHit::get_reflected_ray(float shadow_ray_epsilon) const {
    Vec3f ref_vec = ray->direction - normal * ray->direction.dot(normal) * 2;
    return {hit_point + ref_vec * shadow_ray_epsilon, ref_vec.normalize()};
}