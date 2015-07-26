#ifndef SPH_MATRIX_H
#define SPH_MATRIX_H

#include <GLES3/gl3.h>

class Matrix4x4 {

public:
    Matrix4x4();

    virtual ~Matrix4x4();

    void setIdentity();

    void setPerspective(double fov, double aspect, double near, double far);

    GLfloat m[16];

};

#endif
