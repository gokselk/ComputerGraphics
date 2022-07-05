//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "parser.h"

class Triangle : public parser::Triangle {
public:
    explicit Triangle(parser::Triangle t);
};

#endif //RAYTRACER_TRIANGLE_H
