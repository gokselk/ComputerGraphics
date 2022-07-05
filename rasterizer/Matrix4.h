#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <iostream>
#include "Vec4.h"

class Matrix4 {
public:
    double val[4][4];

    Matrix4();

    Matrix4(double val[4][4]);

    Matrix4(const Matrix4 &other);

    Matrix4 operator+(const Matrix4 &other);

    Matrix4 operator-(const Matrix4 &other);

    Matrix4 operator*(const Matrix4 &other);

    Matrix4 operator*(const double &other);

    Matrix4 operator/(const double &other);

    Matrix4 &operator=(const Matrix4 &other);

    Matrix4 &operator+=(const Matrix4 &other);

    Matrix4 &operator-=(const Matrix4 &other);

    Matrix4 &operator*=(const Matrix4 &other);

    Matrix4 &operator*=(const double &other);

    Matrix4 &operator/=(const double &other);

    Vec4 operator*(const Vec4 &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m);
};

#endif