//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include "parser.h"

class Mesh : public parser::Mesh {
public:
    explicit Mesh(const parser::Mesh &mesh);
};

#endif //RAYTRACER_MESH_H
