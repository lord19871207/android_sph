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

void Matrix4x4::setPerspective(double fov, double aspect, double near, double far) {
    const double D2R = M_PI / 180.0;
    const double yScale = 1.0 / tan(D2R * fov / 2);
    const double xScale = yScale / aspect;
    const double nearmfar = near - far;

    m[0] = (GLfloat) xScale;
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;

    m[4] = 0;
    m[5] = (GLfloat) yScale;
    m[6] = 0;
    m[7] = 0;

    m[8] = 0;
    m[9] = 0;
    m[10] = (GLfloat) ((far + near) / nearmfar);
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = (GLfloat) (2 * far * near / nearmfar);
    m[15] = 0;
}

void Matrix4x4::setLookAt(const Vector3<GLfloat> &eyePos, const Vector3<GLfloat> &lookAt,
                          const Vector3<GLfloat> &upVector) {
    const Vector3<GLfloat> zaxis = (lookAt - eyePos).normalize();
    const Vector3<GLfloat> xaxis = upVector.cross(zaxis).normalize();
    const Vector3<GLfloat> yaxis = zaxis.cross(xaxis);
    const Vector3<GLfloat> negEye = -eyePos;

    m[0] = xaxis.v[0];
    m[1] = xaxis.v[1];
    m[2] = xaxis.v[2];
    m[3] = xaxis.dot(negEye);

    m[4] = yaxis.v[0];
    m[5] = yaxis.v[1];
    m[6] = yaxis.v[2];
    m[7] = yaxis.dot(negEye);

    m[8] = zaxis.v[0];
    m[9] = zaxis.v[1];
    m[10] = zaxis.v[2];
    m[11] = zaxis.dot(negEye);

    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}
