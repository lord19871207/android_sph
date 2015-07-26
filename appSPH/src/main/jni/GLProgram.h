#ifndef SPH_GLPROGRAM_H
#define SPH_GLPROGRAM_H

#include <GLES3/gl3.h>

class GLProgram {

public:
    GLProgram(const GLchar *vsSource, const GLchar *fsSource);

    virtual ~GLProgram();

private:
    GLuint compileShader(const GLchar *source, GLenum type);

public:
    GLuint id;
};


#endif
