#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>

class Vec3 {
public:
    double x, y, z;
    int colorId;

    Vec3();

    Vec3(double x, double y, double z, int colorId);

    Vec3(const Vec3 &other);

    [[maybe_unused]] [[nodiscard]] double getElementAt(int index) const;

    [[nodiscard]] double dot(const Vec3 &rhs) const;

    [[nodiscard]] Vec3 cross(const Vec3 &other) const;

    [[maybe_unused]] [[nodiscard]] double magnitude() const;

    [[maybe_unused]] [[nodiscard]] bool isEqual(const Vec3 &other) const;

    Vec3 operator+(const Vec3 &other) const;

    Vec3 operator-(const Vec3 &other) const;

    Vec3 operator*(const double &other) const;

    Vec3 operator/(const double &other) const;

    Vec3 operator-() const;

    Vec3 &operator=(const Vec3 &other);

    Vec3 &operator+=(const Vec3 &other);

    Vec3 &operator-=(const Vec3 &other);

    Vec3 &operator*=(const double &other);

    Vec3 &operator/=(const double &other);

    bool operator==(const Vec3 &other) const;

    bool operator!=(const Vec3 &other) const;


    friend std::ostream &operator<<(std::ostream &os, const Vec3 &v);
};

#endif