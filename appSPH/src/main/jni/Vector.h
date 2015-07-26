#ifndef SPH_VECTOR_H
#define SPH_VECTOR_H

#include <GLES3/gl3.h>

template<class T>

class Vector3 {

public:
    Vector3<T>();

    Vector3<T>(T x, T y, T z);

    Vector3<T>(const Vector3<T> &vec);

    virtual ~Vector3<T>();

    Vector3<T> operator+() const;

    Vector3<T> operator+(const Vector3<T> &vec) const;

    void operator+=(const Vector3<T> &vec);

    Vector3<T> operator-() const;

    Vector3<T> operator-(const Vector3<T> &vec) const;

    void operator-=(const Vector3<T> &vec);

    Vector3<T> operator*(const Vector3<T> &vec) const;

    void operator*=(const Vector3<T> &vec);

    Vector3<T> operator/(const Vector3<T> &vec) const;

    void operator/=(const Vector3<T> &vec);

    T length() const;

    Vector3<T> normalize();

    T dot(const Vector3<T> &vec) const;

    Vector3<T> cross(const Vector3<T> &vec) const;

    T v[3];
};

#endif
