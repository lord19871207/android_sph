#ifndef SPH_GLRENDERER_H
#define SPH_GLRENDERER_H

#include <GLES3/gl3.h>
#include "GLProgram.h"
#include "Matrix.h"

class GLRenderer {

public:
    GLRenderer();

    virtual ~GLRenderer();

    void surfaceChanged(GLint width, GLint height);

    void renderFrame();

private:
    GLint width;
    GLint height;

    GLProgram *glProgramCube;

    Matrix4x4 projectionMatrix;
    Matrix4x4 viewMatrix;

};

#endif
