#include "GLRenderer.h"
#include "GLShaders.h"
#include "GLUtils.h"
#include <stdlib.h>

GLRenderer::GLRenderer() {
    GL_CHECK(glProgramCube = new GLProgram(CUBE_VS, CUBE_FS));
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glEnable(GL_DEPTH_TEST));
}

GLRenderer::~GLRenderer() {
    if (glProgramCube) {
        delete glProgramCube;
        glProgramCube = nullptr;
    }
}

void GLRenderer::surfaceChanged(GLint width, GLint height) {
    this->width = width;
    this->height = height;
    GLfloat aspect = (GLfloat) height / width;
    projectionMatrix.setFrustum(-1, 1, -aspect, aspect, 1, 10);
    viewMatrix.setLookAt(Vector3f(0, 0, 1), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
}

void GLRenderer::renderFrame() {
    GLfloat val = rand() / (GLfloat) RAND_MAX;
    GL_CHECK(glClearColor(val, val, val, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(glUseProgram(glProgramCube->id));
    GL_CHECK(GLint projM = glGetUniformLocation(glProgramCube->id, "projMatrix"));
    GL_CHECK(GLint viewM = glGetUniformLocation(glProgramCube->id, "viewMatrix"));

    GL_CHECK(glUniformMatrix4fv(projM, 1, GL_FALSE, projectionMatrix.m));
    GL_CHECK(glUniformMatrix4fv(viewM, 1, GL_FALSE, viewMatrix.m));

    GLfloat aspect = (GLfloat) height / width;

    GLfloat VERTICES[] = {-1, -aspect, 0, -1, -aspect, -1, -1, aspect, 0, -1, aspect, -1};
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VERTICES));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    GL_CHECK(glDisableVertexAttribArray(0));
}
