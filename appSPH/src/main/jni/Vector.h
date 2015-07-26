#ifndef SPH_VECTOR_H
#define SPH_VECTOR_H

#include <GLES3/gl3.h>

class Vector3 {

public:
    Vector3();

    Vector3(GLfloat x, GLfloat y, GLfloat z);

    Vector3(const Vector3 &vec);

    virtual ~Vector3();

    Vector3 operator+(const Vector3 &vec);

    void operator+=(const Vector3 &vec);

    Vector3 operator-(const Vector3 &vec);

    void operator-=(const Vector3 &vec);

    Vector3 operator*(const Vector3 &vec);

    void operator*=(const Vector3 &vec);

    Vector3 operator/(const Vector3 &vec);

    void operator/=(const Vector3 &vec);

    GLfloat length();

    Vector3 normalize();

    GLfloat dot(const Vector3 &vec);

    Vector3 cross(const Vector3 &vec);

    GLfloat v[3];
};

#endif
