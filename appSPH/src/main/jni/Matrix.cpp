#include "Matrix.h"
#include <math.h>

Matrix4x4::Matrix4x4() {
    setIdentity();
}

Matrix4x4::~Matrix4x4() {
}

void Matrix4x4::setIdentity() {
    for (int i = 0; i < 16; ++i) {
        m[i] = (i / 4 == i % 4) ? 1 : 0;
    }
}

void Matrix4x4::setFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
                           GLfloat znear, GLfloat zfar) {
    GLfloat temp = 2.0f * znear;
    GLfloat temp2 = right - left;
    GLfloat temp3 = top - bottom;
    GLfloat temp4 = zfar - znear;

    m[0] = temp / temp2;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;

    m[4] = 0;
    m[5] = temp / temp3;
    m[6] = 0;
    m[7] = 0;

    m[8] = (right + left) / temp2;
    m[9] = (top + bottom) / temp3;
    m[10] = (-zfar - znear) / temp4;
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = (-temp * zfar) / temp4;
    m[15] = 0;
}


void Matrix4x4::setPerspective(GLfloat fov, GLfloat aspect, GLfloat znear, GLfloat zfar) {
    GLfloat ymax = (GLfloat) (znear * tan(fov * M_PI / 360.0));
    GLfloat xmax = ymax * aspect;
    setFrustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

void Matrix4x4::setLookAt(const Vector3f &eyePos, const Vector3f &center,
                          const Vector3f &upVector) {
    const Vector3f forward = (center - eyePos).normalize();
    const Vector3f side = forward.cross(upVector).normalize();
    const Vector3f up = side.cross(forward);

    m[0] = side.v[0];
    m[1] = up.v[0];
    m[2] = -forward.v[0];
    m[3] = 0;

    m[4] = side.v[1];
    m[5] = up.v[1];
    m[6] = -forward.v[1];
    m[7] = 0;

    m[8] = side.v[2];
    m[9] = up.v[2];
    m[10] = -forward.v[2];
    m[11] = 0;

    m[12] = -eyePos.v[0];
    m[13] = -eyePos.v[1];
    m[14] = -eyePos.v[2];
    m[15] = 1;
}
