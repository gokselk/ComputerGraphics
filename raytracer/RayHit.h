//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_RAYHIT_H
#define RAYTRACER_RAYHIT_H

#include "Vec3f.h"
#include "Material.h"

class Ray;

class RayHit {
public:
    RayHit(const Ray& ray, int material_id);
    RayHit(const RayHit& ray_hit);
    RayHit& operator=(const RayHit& ray_hit);
    ~RayHit();
    float t;
    Vec3f normal;
    Vec3f hit_point;
    Ray* ray;
    int material_id;
    void setHit(float t, const Vec3f& _hit_point, const Vec3f& _normal);

    Ray get_reflected_ray(float shadow_ray_epsilon) const;
};

#endif //RAYTRACER_RAYHIT_H
