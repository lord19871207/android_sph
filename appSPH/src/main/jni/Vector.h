#ifndef SPH_VECTOR_H
#define SPH_VECTOR_H

#include <GLES3/gl3.h>
#include <math.h>

template<class T>
class Vector3 {

public:
    Vector3();

    Vector3(T x, T y, T z);

    Vector3(const Vector3<T> &vec);

    virtual ~Vector3();

    Vector3<T> &operator=(const Vector3<T> &vec);

    Vector3<T> operator+() const;

    Vector3<T> operator+(const Vector3<T> &vec) const;

    void operator+=(const Vector3<T> &vec);

    Vector3<T> operator-() const;

    Vector3<T> operator-(const Vector3<T> &vec) const;

    void operator-=(const Vector3<T> &vec);

    T length() const;

    Vector3<T> normalize() const;

    T dot(const Vector3<T> &vec) const;

    Vector3<T> cross(const Vector3<T> &vec) const;

    T v[3];
};

typedef Vector3 <double> Vector3d;
typedef Vector3 <GLfloat> Vector3f;

template<class T>
Vector3<T>::Vector3() {
    v[0] = v[1] = v[2] = 0;
}

template<class T>
Vector3<T>::Vector3(T x, T y, T z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

template<class T>
Vector3<T>::Vector3(const Vector3<T> &vec) {
    v[0] = vec.v[0];
    v[1] = vec.v[1];
    v[2] = vec.v[2];
}

template<class T>
Vector3<T>::~Vector3() {
}

template<class T>
Vector3<T> &Vector3<T>::operator=(const Vector3<T> &vec) {
    v[0] = vec.v[0];
    v[1] = vec.v[1];
    v[2] = vec.v[2];
    return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator+() const {
    return Vector3<T>(v[0], v[1], v[2]);
}

template<class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &vec) const {
    return Vector3<T>(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2]);
}

template<class T>
void Vector3<T>::operator+=(const Vector3<T> &vec) {
    v[0] += vec.v[0];
    v[1] += vec.v[1];
    v[2] += vec.v[2];
}

template<class T>
Vector3<T> Vector3<T>::operator-() const {
    return Vector3<T>(-v[0], -v[1], -v[2]);
}

template<class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &vec) const {
    return Vector3<T>(v[0] - vec.v[0], v[1] - vec.v[1], v[2] - vec.v[2]);
}

template<class T>
void Vector3<T>::operator-=(const Vector3<T> &vec) {
    v[0] -= vec.v[0];
    v[1] -= vec.v[1];
    v[2] -= vec.v[2];
}

template<class T>
T Vector3<T>::length() const {
    return (T) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

template<class T>
Vector3<T> Vector3<T>::normalize() const {
    T len = length();
    return Vector3<T>(v[0] / len, v[1] / len, v[2] / len);
}

template<class T>
T Vector3<T>::dot(const Vector3<T> &vec) const {
    return v[0] * vec.v[0] + v[1] * vec.v[1] + v[2] * vec.v[2];
}

template<class T>
Vector3<T> Vector3<T>::cross(const Vector3<T> &vec) const {
    return Vector3<T>(
            v[1] * vec.v[2] - v[2] * vec.v[1],
            v[2] * vec.v[0] - v[0] * vec.v[2],
            v[0] * vec.v[1] - v[1] * vec.v[0]
    );
}

#endif
