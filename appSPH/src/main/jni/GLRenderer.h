#ifndef SPH_GLRENDERER_H
#define SPH_GLRENDERER_H

#include <GLES3/gl3.h>
#include "GLProgram.h"
#include "SPHSimulation.h"
#include "Matrix.h"

class GLRenderer {

public:
    GLRenderer();

    virtual ~GLRenderer();

    void surfaceChanged(GLint width, GLint height);

    void renderFrame(const SPHSimulation *simulation);

private:
    void renderBackground();

    void renderParticles(const SPHSimulation *simulation);

    GLint width;
    GLint height;

    GLProgram *glProgramCube;
    GLProgram *glProgramParticle;

    Matrix4x4 projectionMatrix;
    Matrix4x4 viewMatrix;

};

#endif
