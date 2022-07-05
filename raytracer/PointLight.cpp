//
// Created by goksel on 11/27/21.
//

#include "PointLight.h"

PointLight::PointLight(const Vec3f &position, const Vec3f &intensity) {
    this->position = position;
    this->intensity = intensity;
}

PointLight::PointLight(const PointLight &pl) {
    this->position = pl.position;
    this->intensity = pl.intensity;
}

PointLight::PointLight(const parser::PointLight &pl) {
    this->position = Vec3f(pl.position);
    this->intensity = Vec3f(pl.intensity);
}

Vec3f PointLight::get_irradiance(const Vec3f &point) const {
    float attenuation = 1.0f / position.distanceSquared(point);
    return intensity * attenuation;
}