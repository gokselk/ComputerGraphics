//
// Created by goksel on 2/8/22.
//

#ifndef HW3_OPENGL_MISC_H
#define HW3_OPENGL_MISC_H

#include "glm/glm/glm.hpp"

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct triangle {
    int vertex1;
    int vertex2;
    int vertex3;
};

#endif //HW3_OPENGL_MISC_H
