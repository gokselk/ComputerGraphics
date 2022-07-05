//
// Created by goksel on 11/29/21.
//

#include "Matrix3x3f.h"

Matrix3x3f::Matrix3x3f(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3) {
    m[0][0] = v1.x;
    m[0][1] = v1.y;
    m[0][2] = v1.z;
    m[1][0] = v2.x;
    m[1][1] = v2.y;
    m[1][2] = v2.z;
    m[2][0] = v3.x;
    m[2][1] = v3.y;
    m[2][2] = v3.z;
}

float Matrix3x3f::determinant() {
    return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
           m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
           m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
}

Matrix3x3f::Matrix3x3f() {
    m[0][0] = 0;
    m[0][1] = 0;
    m[0][2] = 0;
    m[1][0] = 0;
    m[1][1] = 0;
    m[1][2] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 0;
}

Matrix3x3f::Matrix3x3f(const Matrix3x3f &m) {
    this->m[0][0] = m.m[0][0];
    this->m[0][1] = m.m[0][1];
    this->m[0][2] = m.m[0][2];
    this->m[1][0] = m.m[1][0];
    this->m[1][1] = m.m[1][1];
    this->m[1][2] = m.m[1][2];
    this->m[2][0] = m.m[2][0];
    this->m[2][1] = m.m[2][1];
    this->m[2][2] = m.m[2][2];
}