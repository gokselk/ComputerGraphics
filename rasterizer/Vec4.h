#ifndef __VEC4_H__
#define __VEC4_H__

#include <iostream>

class Vec4 {
public:
    double x, y, z, t;
    int colorId;


    Vec4();

    Vec4(double x, double y, double z, double t, int colorId);

    Vec4(const Vec4 &other);

    [[nodiscard]] double getElementAt(int index) const;

    void perspectiveDivide();

    friend std::ostream &operator<<(std::ostream &os, const Vec4 &v);
};

#endif