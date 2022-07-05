//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_MATRIX3X3F_H
#define RAYTRACER_MATRIX3X3F_H

#include "Vec3f.h"

class Matrix3x3f {
private:
    float m[3][3]{};
public:
    Matrix3x3f(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3);
    Matrix3x3f(const Matrix3x3f &m);
    Matrix3x3f();
    float determinant();
};

#endif //RAYTRACER_MATRIX3X3F_H
