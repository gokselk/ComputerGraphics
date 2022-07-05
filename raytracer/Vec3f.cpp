//
// Created by goksel on 11/29/21.
//

#include "Vec3f.h"

Vec3f::Vec3f() : parser::Vec3f{0, 0, 0} {

}

Vec3f::Vec3f(float x, float y, float z) : parser::Vec3f{x, y, z} {

}

Vec3f::Vec3f(const Vec3f &v) : parser::Vec3f{v.x, v.y, v.z} {
}

Vec3f Vec3f::operator+(const Vec3f &v) const {
    return {x + v.x, y + v.y, z + v.z};
}

Vec3f Vec3f::operator-(const Vec3f &v) const {
    return {x - v.x, y - v.y, z - v.z};
}

Vec3f Vec3f::operator*(const Vec3f &v) const {
    return {x * v.x, y * v.y, z * v.z};
}

Vec3f Vec3f::operator*(float f) const {
    return {x * f, y * f, z * f};
}

Vec3f Vec3f::operator/(float f) const {
    return {x / f, y / f, z / f};
}

Vec3f &Vec3f::operator+=(const Vec3f &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3f &Vec3f::operator-=(const Vec3f &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vec3f &Vec3f::operator*=(const Vec3f &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

Vec3f &Vec3f::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

Vec3f &Vec3f::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

float Vec3f::dot(const Vec3f &v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3f Vec3f::cross(const Vec3f &v) const {
    return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

float Vec3f::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3f Vec3f::normalize() {
    float len = length();
    return {x / len, y / len, z / len};
}

float Vec3f::distance(const Vec3f &v) const {
    return std::sqrt(distanceSquared(v));
}

float Vec3f::distanceSquared(const Vec3f &v) const {
    return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z);
}

Vec3f::Vec3f(const parser::Vec3f &v) : parser::Vec3f{v} {
}

Vec3f& Vec3f::operator=(const Vec3f &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vec3f Vec3f::clamp(const Vec3f &min, const Vec3f &max) {
    x = std::max(min.x, std::min(max.x, x));
    y = std::max(min.y, std::min(max.y, y));
    z = std::max(min.z, std::min(max.z, z));
    return {x, y, z};
}

Vec3f Vec3f::operator-()
{
    return {-(this->x), -(this->y), -(this->z)};
}