#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>

class Color {
public:
    double r, g, b;

    Color();

    Color(double r, double g, double b);

    Color(const Color &other);

    Color &operator=(const Color &other);

    Color operator+(const Color &other) const;

    Color operator-(const Color &other) const;

    Color operator*(const Color &other) const;

    Color operator*(double scalar) const;

    Color operator/(double scalar) const;

    Color &operator+=(const Color &other);

    Color &operator-=(const Color &other);

    Color &operator*=(const Color &other);

    Color &operator*=(double scalar);

    Color &operator/=(double scalar);

    bool operator==(const Color &other) const;

    bool operator!=(const Color &other) const;

    Color round() const;

    friend std::ostream &operator<<(std::ostream &os, const Color &c);
};

#endif