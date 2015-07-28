#ifndef SPH_MATRIX_H
#define SPH_MATRIX_H

#include "Vector3.h"
#include <GLES3/gl3.h>

class Matrix4x4 {

public:
    Matrix4x4();

    virtual ~Matrix4x4();

    void setIdentity();

    void setFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
                    GLfloat znear, GLfloat zfar);

    void setPerspective(GLfloat fov, GLfloat aspect, GLfloat znear, GLfloat zfar);

    void setLookAt(const Vector3f &eyePos, const Vector3f &lookAt, const Vector3f &upVector);

    GLfloat m[16];

};

#endif
