#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Triangle.h"
#include "Matrix4.h"
#include <iostream>

class Mesh {

public:
    int meshId;
    int type; // 0 for wireframe, 1 for solid
    int numberOfTransformations;
    std::vector<int> transformationIds;
    std::vector<char> transformationTypes;
    int numberOfTriangles;
    std::vector<Triangle> triangles;

    Mesh();

    Mesh(int meshId, int type, int numberOfTransformations, std::vector<int> transformationIds,
         std::vector<char> transformationTypes, int numberOfTriangles, std::vector<Triangle> triangles);

    friend std::ostream &operator<<(std::ostream &os, const Mesh &m);
};

#endif