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
    GL_CHECK(glClearColor(0, 0, 0, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(glUseProgram(glProgramCube->id));
    GL_CHECK(GLint projM = glGetUniformLocation(glProgramCube->id, "projMatrix"));
    GL_CHECK(GLint viewM = glGetUniformLocation(glProgramCube->id, "viewMatrix"));

    GL_CHECK(glUniformMatrix4fv(projM, 1, GL_FALSE, projectionMatrix.m));
    GL_CHECK(glUniformMatrix4fv(viewM, 1, GL_FALSE, viewMatrix.m));

    GLfloat aspect = (GLfloat) height / width;

    GLfloat VERTICES[] = {
            -1, -aspect, 0, -1, -aspect, -1, -1, aspect, 0,  // left
            -1, -aspect, -1, -1, aspect, -1, -1, aspect, 0,  // left
            1, aspect, 0, 1, aspect, -1, 1, -aspect, 0,      // right
            1, aspect, -1, 1, -aspect, -1, 1, -aspect, 0,    // right
            -1, aspect, 0, -1, aspect, -1, 1, aspect, 0,     // top
            -1, aspect, -1, 1, aspect, -1, 1, aspect, 0,     // top
            1, -aspect, 0, 1, -aspect, -1, -1, -aspect, 0,   // bottom
            1, -aspect, -1, -1, -aspect, -1, -1, -aspect, 0, // bottom
            -1, aspect, -1, -1, -aspect, -1, 1, aspect, -1,  // floor
            -1, -aspect, -1, 1, -aspect, -1, 1, aspect, -1,  // floor
    };
    GLfloat COLORS[] = {
            .3, .6, .9, 1, .3, .6, .9, 1, .3, .6, .9, 1,  // left
            .3, .6, .9, 1, .3, .6, .9, 1, .3, .6, .9, 1,  // left
            .3, .6, .9, 1, .3, .6, .9, 1, .3, .6, .9, 1,  // right
            .3, .6, .9, 1, .3, .6, .9, 1, .3, .6, .9, 1,  // right
            .25, .55, .85, 1, .25, .55, .85, 1, .25, .55, .85, 1,  // top
            .25, .55, .85, 1, .25, .55, .85, 1, .25, .55, .85, 1,  // top
            .25, .55, .85, 1, .25, .55, .85, 1, .25, .55, .85, 1,  // bottom
            .25, .55, .85, 1, .25, .55, .85, 1, .25, .55, .85, 1,  // bottom
            .2, .5, .8, 1, .2, .5, .8, 1, .2, .5, .8, 1,  // floor
            .2, .5, .8, 1, .2, .5, .8, 1, .2, .5, .8, 1,  // floor
    };

    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VERTICES));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, COLORS));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 30));

    GL_CHECK(glDisableVertexAttribArray(0));
    GL_CHECK(glDisableVertexAttribArray(1));
}
