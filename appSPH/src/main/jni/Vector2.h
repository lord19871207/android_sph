#ifndef SPH_VECTOR2_H
#define SPH_VECTOR2_H

#include <GLES3/gl3.h>
#include <math.h>

template<class T>
class Vector2 {

public:
    Vector2();

    Vector2(T x, T y);

    Vector2(const Vector2<T> &vec);

    virtual ~Vector2();

    Vector2<T> &operator=(const Vector2<T> &vec);

    Vector2<T> operator+(const Vector2<T> &vec) const;

    void operator+=(const Vector2<T> &vec);

    Vector2<T> operator-() const;

    Vector2<T> operator-(const Vector2<T> &vec) const;

    void operator-=(const Vector2<T> &vec);

    Vector2<T> operator*(T value) const;

    void operator*=(T value);

    T length() const;

    Vector2<T> normalize() const;

    T dot(const Vector2<T> &vec) const;

    T v[2];
};

typedef Vector2<double> Vector2d;
typedef Vector2<GLfloat> Vector2f;

template<class T>
Vector2<T>::Vector2() {
    v[0] = v[1] = 0;
}

template<class T>
Vector2<T>::Vector2(T x, T y) {
    v[0] = x;
    v[1] = y;
}

template<class T>
Vector2<T>::Vector2(const Vector2<T> &vec) {
    v[0] = vec.v[0];
    v[1] = vec.v[1];
}

template<class T>
Vector2<T>::~Vector2() {
}

template<class T>
Vector2<T> &Vector2<T>::operator=(const Vector2<T> &vec) {
    v[0] = vec.v[0];
    v[1] = vec.v[1];
    return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T> &vec) const {
    return Vector2<T>(v[0] + vec.v[0], v[1] + vec.v[1]);
}

template<class T>
void Vector2<T>::operator+=(const Vector2<T> &vec) {
    v[0] += vec.v[0];
    v[1] += vec.v[1];
}

template<class T>
Vector2<T> Vector2<T>::operator-() const {
    return Vector2<T>(-v[0], -v[1]);
}

template<class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &vec) const {
    return Vector2<T>(v[0] - vec.v[0], v[1] - vec.v[1]);
}

template<class T>
void Vector2<T>::operator-=(const Vector2<T> &vec) {
    v[0] -= vec.v[0];
    v[1] -= vec.v[1];
}

template<class T>
Vector2<T> Vector2<T>::operator*(T value) const {
    return Vector2<T>(v[0] * value, v[1] * value);
}

template<class T>
void Vector2<T>::operator*=(T value) {
    v[0] *= value;
    v[1] *= value;
}

template<class T>
T Vector2<T>::length() const {
    return (T) sqrt(v[0] * v[0] + v[1] * v[1]);
}

template<class T>
Vector2<T> Vector2<T>::normalize() const {
    T len = length();
    return Vector2<T>(v[0] / len, v[1] / len);
}

template<class T>
T Vector2<T>::dot(const Vector2<T> &vec) const {
    return v[0] * vec.v[0] + v[1] * vec.v[1];
}

#endif
