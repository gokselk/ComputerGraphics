#include "Matrix4.h"
#include "Helpers.h"
#include <iostream>
#include <iomanip>

Matrix4::Matrix4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] = 0;
        }
    }
}

Matrix4::Matrix4(double val[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] = val[i][j];
        }
    }
}

Matrix4::Matrix4(const Matrix4 &other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] = other.val[i][j];
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Matrix4 &m) {

    os << std::fixed << std::setprecision(6) << "|" << m.val[0][0] << "|" << m.val[0][1] << "|" << m.val[0][2] << "|"
       << m.val[0][3] << "|" << std::endl << "|" << m.val[1][0] << "|" << m.val[1][1] << "|" << m.val[1][2] << "|"
       << m.val[1][3] << "|" << std::endl << "|" << m.val[2][0] << "|" << m.val[2][1] << "|" << m.val[2][2] << "|"
       << m.val[2][3] << "|" << std::endl << "|" << m.val[3][0] << "|" << m.val[3][1] << "|" << m.val[3][2] << "|"
       << m.val[3][3] << "|";

    return os;
}

Matrix4 Matrix4::operator+(const Matrix4 &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = this->val[i][j] + other.val[i][j];
        }
    }
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4 &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = this->val[i][j] - other.val[i][j];
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4 &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.val[i][j] += this->val[i][k] * other.val[k][j];
            }
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const double &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = this->val[i][j] * other;
        }
    }
    return result;
}

Matrix4 Matrix4::operator/(const double &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = this->val[i][j] / other;
        }
    }
    return result;
}

Matrix4 &Matrix4::operator+=(const Matrix4 &other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] += other.val[i][j];
        }
    }
    return *this;
}

Matrix4 &Matrix4::operator-=(const Matrix4 &other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] -= other.val[i][j];
        }
    }
    return *this;
}

Matrix4 &Matrix4::operator*=(const Matrix4 &other) {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.val[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.val[i][j] += this->val[i][k] * other.val[k][j];
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->val[i][j] = result.val[i][j];
        }
    }
    return *this;
}

Matrix4 &Matrix4::operator*=(const double &other) {
    for (auto &i: this->val) {
        for (double &j: i) {
            j *= other;
        }
    }
    return *this;
}

Matrix4 &Matrix4::operator/=(const double &other) {
    for (auto &i: this->val) {
        for (double &j: i) {
            j /= other;
        }
    }
    return *this;
}

Vec4 Matrix4::operator*(const Vec4 &other) const {
    return multiplyMatrixWithVec4(*this, other);
}

Matrix4 &Matrix4::operator=(const Matrix4 &other) = default;
