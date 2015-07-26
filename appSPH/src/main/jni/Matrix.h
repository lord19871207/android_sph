#ifndef SPH_MATRIX_H
#define SPH_MATRIX_H

#include "Vector.h"
#include <GLES3/gl3.h>

class Matrix4x4 {

public:
    Matrix4x4();

    virtual ~Matrix4x4();

    void setIdentity();

    void setPerspective(double fov, double aspect, double near, double far);

    void setLookAt(const Vector3<GLfloat> &eyePos, const Vector3<GLfloat> &lookAt,
                   const Vector3<GLfloat> &upVector);

    GLfloat m[16];

};

#endif
