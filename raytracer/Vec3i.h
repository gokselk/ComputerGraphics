//
// Created by goksel on 11/26/21.
//

#ifndef RAYTRACER_VEC3I_H
#define RAYTRACER_VEC3I_H

#include <cmath>
#include "parser.h"

class Vec3i : public parser::Vec3i {
public:
    Vec3i();
    Vec3i(int x, int y, int z);
    explicit Vec3i(const parser::Vec3i &v);
    explicit Vec3i(const parser::Vec3f &v);

    Vec3i operator+(const Vec3i &v) const;
    Vec3i operator-(const Vec3i &v) const;
    Vec3i operator*(const Vec3i &v) const;
    Vec3i operator*(const int &v) const;
    Vec3i operator/(const Vec3i &v) const;
    Vec3i operator/(const int &v) const;

    Vec3i &operator+=(const Vec3i &v);
    Vec3i &operator-=(const Vec3i &v);
    Vec3i &operator*=(const Vec3i &v);
    Vec3i &operator*=(const int &v);
    Vec3i &operator/=(const Vec3i &v);
    Vec3i &operator/=(const int &v);

    bool operator==(const Vec3i &v) const;
    bool operator!=(const Vec3i &v) const;

    int &operator[](const int &i);
    const int &operator[](const int &i) const;

    float length() const;
    float lengthSquared() const;
    Vec3i normalize() const;
    Vec3i cross(const Vec3i &v) const;
    int dot(const Vec3i &v) const;
    float angle(const Vec3i &v) const;
    float distance(const Vec3i &v) const;
    float distanceSquared(const Vec3i &v) const;
};



#endif //RAYTRACER_VEC3I_H
