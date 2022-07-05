//
// Created by goksel on 11/29/21.
//

#include "Vec4f.h"

Vec4f::Vec4f() : parser::Vec4f{} {}

Vec4f::Vec4f(float x, float y, float z, float w) : parser::Vec4f{x, y, z, w} {

}

Vec4f::Vec4f(const parser::Vec4f &v) : parser::Vec4f{v} {

}

Vec4f& Vec4f::operator=(const Vec4f &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}