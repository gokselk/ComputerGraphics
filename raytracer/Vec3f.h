#ifndef RAYTRACER_VEC3F_H
#define RAYTRACER_VEC3F_H

#include <cmath>
#include "parser.h"

class Vec3f : public parser::Vec3f {
public:
    Vec3f();

    Vec3f(float x, float y, float z);

    explicit Vec3f(const parser::Vec3f &v);

    Vec3f(const Vec3f &v);

    Vec3f& operator=(const Vec3f &v);

    Vec3f operator+(const Vec3f &v) const;

    Vec3f operator-(const Vec3f &v) const;

    Vec3f operator*(const Vec3f &v) const;

    Vec3f operator*(float f) const;

    Vec3f operator/(float f) const;

    Vec3f &operator+=(const Vec3f &v);

    Vec3f &operator-=(const Vec3f &v);

    Vec3f &operator*=(const Vec3f &v);

    Vec3f &operator*=(float f);

    Vec3f &operator/=(float f);

    Vec3f operator-();

    float dot(const Vec3f &v) const;

    Vec3f cross(const Vec3f &v) const;

    float length() const;

    Vec3f normalize();

    float distance(const Vec3f &v) const;

    float distanceSquared(const Vec3f &v) const;

    Vec3f clamp(const Vec3f &min, const Vec3f &max);
};

#endif //RAYTRACER_VEC3F_H
