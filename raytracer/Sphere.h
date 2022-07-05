//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "parser.h"

class Sphere : public parser::Sphere {
public:
    explicit Sphere(const parser::Sphere &sphere);
};

#endif //RAYTRACER_SPHERE_H
