#include "Matrix.h"
#include <math.h>

Matrix4x4::Matrix4x4() {
    setIdentity();
}

virtual Matrix4x4::~Matrix4x4() {
}

void Matrix4x4::setIdentity() {
    for (int i = 0; i < 16; ++i) {
        m[i] = (i / 4 == i % 4) ? 1 : 0;
    }
}

void Matrix4x4::setPerspective(double fov, double aspect, double near, double far) {
    double D2R = M_PI / 180.0;
    double yScale = 1.0 / tan(D2R * fov / 2);
    double xScale = yScale / aspect;
    double nearmfar = near - far;

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