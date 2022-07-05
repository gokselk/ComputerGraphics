#include "Color.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Color::Color() {
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(double r, double g, double b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(const Color &other) {
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}

std::ostream &operator<<(std::ostream &os, const Color &c) {
    os << std::fixed << std::setprecision(0) << "rgb(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

Color &Color::operator=(const Color &other) = default;

Color Color::operator+(const Color &other) const {
    return {this->r + other.r, this->g + other.g, this->b + other.b};
}

Color Color::operator-(const Color &other) const {
    return {this->r - other.r, this->g - other.g, this->b - other.b};
}

Color Color::operator*(const Color &other) const {
    return {this->r * other.r, this->g * other.g, this->b * other.b};
}

Color Color::operator*(double scalar) const {
    return {this->r * scalar, this->g * scalar, this->b * scalar};
}

Color Color::operator/(double scalar) const {
    return {this->r / scalar, this->g / scalar, this->b / scalar};
}

Color &Color::operator+=(const Color &other) {
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    return *this;
}

Color &Color::operator-=(const Color &other) {
    this->r -= other.r;
    this->g -= other.g;
    this->b -= other.b;
    return *this;
}

Color &Color::operator*=(const Color &other) {
    this->r *= other.r;
    this->g *= other.g;
    this->b *= other.b;
    return *this;
}

Color &Color::operator*=(double scalar) {
    this->r *= scalar;
    this->g *= scalar;
    this->b *= scalar;
    return *this;
}

Color &Color::operator/=(double scalar) {
    this->r /= scalar;
    this->g /= scalar;
    this->b /= scalar;
    return *this;
}

bool Color::operator==(const Color &other) const {
    return this->r == other.r && this->g == other.g && this->b == other.b;
}

bool Color::operator!=(const Color &other) const {
    return !(*this == other);
}

Color Color::round() const {
    return {std::round(this->r), std::round(this->g), std::round(this->b)};
}
