#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

#include "parser.h"
#include "Vec3f.h"

class PointLight {
public:
    PointLight(const Vec3f &position, const Vec3f &intensity);
    PointLight(const PointLight &pl);
    explicit PointLight(const parser::PointLight &pl);

    Vec3f get_irradiance(const Vec3f &point) const;

    Vec3f position;
    Vec3f intensity;
};

#endif //RAYTRACER_POINTLIGHT_H

