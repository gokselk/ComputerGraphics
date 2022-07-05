//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_VEC4F_H
#define RAYTRACER_VEC4F_H

#include "parser.h"

class Vec4f : public parser::Vec4f {
public:
    Vec4f();
    Vec4f(float x, float y, float z, float w);

public:
    explicit Vec4f(const parser::Vec4f &v);

    Vec4f& operator=(const Vec4f &v);
};

#endif //RAYTRACER_VEC4F_H
