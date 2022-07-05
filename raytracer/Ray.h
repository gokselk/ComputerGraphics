//
// Created by goksel on 11/26/21.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Vec3f.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Matrix3x3f.h"

class Scene; // forward declaration
class RayHit; // forward declaration

class Ray {
public:

    Vec3f position;
    Vec3f direction;
    Ray(const Ray& ray);
    Ray(const Vec3f& position, const Vec3f& direction);
    Ray& operator=(const Ray& ray);
    Vec3f get_color(const Scene& scene, int depth);
    RayHit get_ray_hit(const Scene& scene, const Triangle& triangle) const;
    RayHit get_ray_hit(const Scene& scene, const Sphere& sphere) const;
    RayHit get_ray_hit(const Scene& scene, const Mesh& mesh) const;

    RayHit get_first_hit(const Scene &scene);

    RayHit get_shadow_ray(const Scene &scene, float light_distance);

    RayHit get_ray_hit(const Scene &scene, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, int material_id) const;
};

#endif //RAYTRACER_RAY_H
