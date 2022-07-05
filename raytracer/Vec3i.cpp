//
// Created by goksel on 11/29/21.
//

#include "Vec3i.h"

Vec3i::Vec3i() : parser::Vec3i{} {

}

Vec3i::Vec3i(int x, int y, int z) : parser::Vec3i{x, y, z} {

}

Vec3i::Vec3i(const parser::Vec3i &v) : parser::Vec3i{v} {

}

Vec3i::Vec3i(const parser::Vec3f &v) : parser::Vec3i{static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)} {

}

Vec3i Vec3i::operator+(const Vec3i &v) const {
    return {x + v.x, y + v.y, z + v.z};
}

Vec3i Vec3i::operator-(const Vec3i &v) const {
    return {x - v.x, y - v.y, z - v.z};
}

Vec3i Vec3i::operator*(const Vec3i &v) const {
    return {x * v.x, y * v.y, z * v.z};
}

Vec3i Vec3i::operator*(const int &v) const {
    return {x * v, y * v, z * v};
}

Vec3i Vec3i::operator/(const Vec3i &v) const {
    return {x / v.x, y / v.y, z / v.z};
}

Vec3i Vec3i::operator/(const int &v) const {
    return {x / v, y / v, z / v};
}

Vec3i &Vec3i::operator+=(const Vec3i &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3i &Vec3i::operator-=(const Vec3i &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vec3i &Vec3i::operator*=(const Vec3i &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

Vec3i &Vec3i::operator*=(const int &v) {
    x *= v;
    y *= v;
    z *= v;
    return *this;
}

Vec3i &Vec3i::operator/=(const Vec3i &v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

Vec3i &Vec3i::operator/=(const int &v) {
    x /= v;
    y /= v;
    z /= v;
    return *this;
}

bool Vec3i::operator==(const Vec3i &v) const {
    return x == v.x && y == v.y && z == v.z;
}

bool Vec3i::operator!=(const Vec3i &v) const {
    return x != v.x || y != v.y || z != v.z;
}

int &Vec3i::operator[](const int &i) {
    return *(&x + i);
}

const int &Vec3i::operator[](const int &i) const {
    return *(&x + i);
}

float Vec3i::length() const {
    return std::sqrt(lengthSquared());
}

float Vec3i::lengthSquared() const {
    return static_cast<float>(x * x + y * y + z * z);
}

Vec3i Vec3i::normalize() const {
    return *this / length();
}

Vec3i Vec3i::cross(const Vec3i &v) const {
    return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

int Vec3i::dot(const Vec3i &v) const {
    return x * v.x + y * v.y + z * v.z;
}

float Vec3i::angle(const Vec3i &v) const {
    return 0.0f; // Unimplemented
}

float Vec3i::distance(const Vec3i &v) const {
    return std::sqrt(distanceSquared(v));
}

float Vec3i::distanceSquared(const Vec3i &v) const {
    return static_cast<float>((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}