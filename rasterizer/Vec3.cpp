#include "Vec3.h"
#include "Helpers.h"
#include <iomanip>

Vec3::Vec3() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}

Vec3::Vec3(double x, double y, double z, int colorId) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

Vec3::Vec3(const Vec3 &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

[[maybe_unused]] double Vec3::getElementAt(int index) const {
    switch (index) {
        case 0:
            return this->x;

        case 1:
            return this->y;

        default:
            return this->z;
    }
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v) {

    os << std::fixed << std::setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}

double Vec3::dot(const Vec3 &other) const {
    return dotProductVec3(*this, other);
}

Vec3 Vec3::cross(const Vec3 &other) const {
    return crossProductVec3(*this, other);
}

[[maybe_unused]] double Vec3::magnitude() const {
    return magnitudeOfVec3(*this);
}

[[maybe_unused]] bool Vec3::isEqual(const Vec3 &other) const {
    return areEqualVec3(*this, other);
}

Vec3 Vec3::operator+(const Vec3 &other) const {
    return addVec3(*this, other);
}

Vec3 Vec3::operator-(const Vec3 &other) const {
    return subtractVec3(*this, other);
}

Vec3 Vec3::operator*(const double &other) const {
    return multiplyVec3WithScalar(*this, other);
}

Vec3 Vec3::operator/(const double &other) const {
    return multiplyVec3WithScalar(*this, 1.0 / other);
}

Vec3 Vec3::operator-() const {
    return multiplyVec3WithScalar(*this, -1.0);
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
    *this = addVec3(*this, other);
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other) {
    *this = subtractVec3(*this, other);
    return *this;
}

Vec3 &Vec3::operator*=(const double &other) {
    *this = multiplyVec3WithScalar(*this, other);
    return *this;
}

Vec3 &Vec3::operator/=(const double &other) {
    *this = multiplyVec3WithScalar(*this, 1.0 / other);
    return *this;
}

bool Vec3::operator==(const Vec3 &other) const {
    return areEqualVec3(*this, other);
}

bool Vec3::operator!=(const Vec3 &other) const {
    return !areEqualVec3(*this, other);
}

Vec3 &Vec3::operator=(const Vec3 &other) = default;

