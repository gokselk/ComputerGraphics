#include "Rotation.h"
#include "Vec3.h"
#include "Helpers.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Rotation::Rotation() {}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z) {
    this->rotationId = rotationId;
    this->angle = angle;
    this->ux = x;
    this->uy = y;
    this->uz = z;
}

std::ostream &operator<<(std::ostream &os, const Rotation &r) {
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux
       << ", " << r.uy << ", " << r.uz << "]";

    return os;
}

Matrix4 Rotation::getMatrix() const {
    double alpha = angle * M_PI / 180;
    double c = cos(alpha);
    double s = sin(alpha);
    double rotationMatrix[4][4] = {{c + ux * ux * (1 - c),      ux * uy * (1 - c) - uz * s,
                                                                                            ux * uz * (1 - c) +
uy * s,                                                                                                            0},
                                   {uy * ux * (1 - c) + uz * s, c + uy * uy * (1 - c),      uy * uz * (1 - c) -
                                                                                            ux * s,                0},
                                   {uz * ux * (1 - c) - uy * s, uz * uy * (1 - c) + ux * s, c + uz * uz * (1 - c), 0},
                                   {0,                          0,                          0,                     1}};
    return {rotationMatrix};
}
