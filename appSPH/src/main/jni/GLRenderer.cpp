#include "GLRenderer.h"
#include "GLShaders.h"
#include "GLUtils.h"
#include <stdlib.h>

GLRenderer::GLRenderer() {
    GL_CHECK(glProgramCube = new GLProgram(CUBE_VS, CUBE_FS));
    GL_CHECK(glProgramParticle = new GLProgram(PARTICLE_VS, PARTICLE_FS));
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

void GLRenderer::renderFrame(const SPHSimulation *simulation) {
    renderBackground();
    renderParticles(simulation);
}

void GLRenderer::renderBackground() {
    GL_CHECK(glClearColor(0, 0, 0, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(GLint projM = glGetUniformLocation(glProgramCube->id, "projMatrix"));
    GL_CHECK(GLint viewM = glGetUniformLocation(glProgramCube->id, "viewMatrix"));

    GL_CHECK(glUseProgram(glProgramCube->id));
    GL_CHECK(glUniformMatrix4fv(projM, 1, GL_FALSE, projectionMatrix.m));
    GL_CHECK(glUniformMatrix4fv(viewM, 1, GL_FALSE, viewMatrix.m));

    GLfloat aspect = (GLfloat) height / width;
    GLfloat zf = -.1f;

    GLfloat VERTICES[] = {
            -1, -aspect, 0, -1, -aspect, zf, -1, aspect, 0,  // left
            -1, -aspect, zf, -1, aspect, zf, -1, aspect, 0,  // left
            1, aspect, 0, 1, aspect, zf, 1, -aspect, 0,      // right
            1, aspect, zf, 1, -aspect, zf, 1, -aspect, 0,    // right
            -1, aspect, 0, -1, aspect, zf, 1, aspect, 0,     // top
            -1, aspect, zf, 1, aspect, zf, 1, aspect, 0,     // top
            1, -aspect, 0, 1, -aspect, zf, -1, -aspect, 0,   // bottom
            1, -aspect, zf, -1, -aspect, zf, -1, -aspect, 0, // bottom
            -1, aspect, zf, -1, -aspect, zf, 1, aspect, zf,  // floor
            -1, -aspect, zf, 1, -aspect, zf, 1, aspect, zf,  // floor
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

void GLRenderer::renderParticles(const SPHSimulation *simulation) {
    GL_CHECK(GLint projM = glGetUniformLocation(glProgramParticle->id, "projMatrix"));
    GL_CHECK(GLint viewM = glGetUniformLocation(glProgramParticle->id, "viewMatrix"));
    GL_CHECK(GLint particlePos = glGetUniformLocation(glProgramParticle->id, "particlePosition"));
    GL_CHECK(GLint particleScale = glGetUniformLocation(glProgramParticle->id, "particleScale"));

    GL_CHECK(glUseProgram(glProgramParticle->id));
    GL_CHECK(glUniformMatrix4fv(projM, 1, GL_FALSE, projectionMatrix.m));
    GL_CHECK(glUniformMatrix4fv(viewM, 1, GL_FALSE, viewMatrix.m));
    GL_CHECK(glUniform1f(particleScale, 0.04));

    GLfloat VERTICES[] = {-1, 1, -1, -1, 1, 1, 1, -1};

    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, VERTICES));
    GL_CHECK(glEnableVertexAttribArray(0));

    std::vector<SPHParticle *>::const_iterator iter = simulation->particles.begin();
    while (iter != simulation->particles.end()) {
        SPHParticle *particle = *iter++;
        GL_CHECK(glUniform4f(particlePos,
                             (GLfloat) particle->position.v[0],
                             (GLfloat) particle->position.v[1],
                             -.05f,
                             0));
        GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }

    GL_CHECK(glDisableVertexAttribArray(0));
}
