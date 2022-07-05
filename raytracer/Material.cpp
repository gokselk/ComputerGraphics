#include "Material.h"

Material::Material(bool is_mirror, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, const Vec3f& mirror, float phong_exponent) {
    this->is_mirror = is_mirror;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->mirror = mirror;
    this->phong_exponent = phong_exponent;
}

Material::Material(const Material &material) {
    this->is_mirror = material.is_mirror;
    this->ambient = material.ambient;
    this->diffuse = material.diffuse;
    this->specular = material.specular;
    this->mirror = material.mirror;
    this->phong_exponent = material.phong_exponent;
}

Material::Material(const parser::Material &material) {
    this->is_mirror = material.is_mirror;
    this->ambient = Vec3f(material.ambient);
    this->diffuse = Vec3f(material.diffuse);
    this->specular = Vec3f(material.specular);
    this->mirror = Vec3f(material.mirror);
    this->phong_exponent = material.phong_exponent;
}