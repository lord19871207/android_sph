#include "Vector.h"
#include <math.h>

Vector3::Vector3() {
    v[0] = v[1] = v[2] = 0;
}

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Vector3::Vector3(const Vector3 &vec) {
    v[0] = vec.v[0];
    v[1] = vec.v[1];
    v[2] = vec.v[2];
}

virtual Vector3::~Vector3() {
}

Vector3 Vector3::operator+(const Vector3 &vec) {
    return Vector3(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2]);
}

void Vector3::operator+=(const Vector3 &vec) {
    v[0] += vec.v[0];
    v[1] += vec.v[1];
    v[2] += vec.v[2];
}

Vector3 Vector3::operator-(const Vector3 &vec) {
    return Vector3(v[0] - vec.v[0], v[1] - vec.v[1], v[2] - vec.v[2]);
}

void Vector3::operator-=(const Vector3 &vec) {
    v[0] -= vec.v[0];
    v[1] -= vec.v[1];
    v[2] -= vec.v[2];
}

Vector3 Vector3::operator*(const Vector3 &vec) {
    return Vector3(v[0] * vec.v[0], v[1] * vec.v[1], v[2] * vec.v[2]);
}

void Vector3::operator*=(const Vector3 &vec) {
    v[0] *= vec.v[0];
    v[1] *= vec.v[1];
    v[2] *= vec.v[2];
}

Vector3 Vector3::operator/(const Vector3 &vec) {
    return Vector3(v[0] / vec.v[0], v[1] / vec.v[1], v[2] / vec.v[2]);
}

void Vector3::operator/=(const Vector3 &vec) {
    v[0] /= vec.v[0];
    v[1] /= vec.v[1];
    v[2] /= vec.v[2];
}

GLfloat Vector3::length() {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

Vector3 Vector3::normalize() {
    GLfloat len = length();
    return Vector3(v[0] / len, v[1] / len, v[2] / len);
}

GLfloat Vector3::dot(const Vector3 &vec) {
    return v[0] * vec.v[0] + v[1] * vec.v[1] + v[2] * vec.v[2];
}

Vector3 Vector3::cross(const Vector3 &vec) {
    return Vector3(
            v[1] * vec.v[2] - v[2] * vec.v[1],
            v[2] * vec.v[0] - v[0] * vec.v[2],
            v[0] * vec.v[1] - v[1] * vec.v[0]
    );
}


