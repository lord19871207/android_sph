#include "GLRenderer.h"
#include <GLES3/gl3.h>
#include <stdlib.h>

void GLRenderer::renderFrame() {
    GLfloat val = rand() / (GLfloat) RAND_MAX;
    glClearColor(val, val, val, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
