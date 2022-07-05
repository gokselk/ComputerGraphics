//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "parser.h"
#include "Vec3f.h"

#include "Ray.h"
#include "RayHit.h"
#include "PointLight.h"

class Material {
public:
    bool is_mirror;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    Vec3f mirror;
    float phong_exponent;
    Material(bool is_mirror, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, const Vec3f& mirror, float phong_exponent);
    Material(const Material &material);
    explicit Material(const parser::Material &material);
};

#endif //RAYTRACER_MATERIAL_H
