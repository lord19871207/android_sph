#include "GLProgram.h"
#include "GLUtils.h"
#include <android/log.h>

GLProgram::GLProgram(const GLchar *vsSource, const GLchar *fsSource) {
    GL_CHECK( id = glCreateProgram() );
    if (!id) {
        return;
    }

    GL_CHECK( GLuint vsComp = compileShader(vsSource, GL_VERTEX_SHADER) );
    GL_CHECK( GLuint fsComp = compileShader(fsSource, GL_FRAGMENT_SHADER) );

    if (vsComp && fsComp) {
        GLint linkStatus = GL_FALSE;
        GL_CHECK( glAttachShader(id, vsComp) );
        GL_CHECK( glAttachShader(id, fsComp) );
        GL_CHECK( glLinkProgram(id) );
        GL_CHECK( glGetProgramiv(id, GL_LINK_STATUS, &linkStatus) );
        if (linkStatus != GL_TRUE) {
            GLchar infoLog[2048];
            GL_CHECK( glGetProgramInfoLog(id, 2048, nullptr, infoLog) );
            __android_log_print(ANDROID_LOG_ERROR, "GLProgram", "%s", infoLog);
            GL_CHECK( glDeleteProgram(id) );
            id = 0;
        }
    }

    GL_CHECK( glDeleteShader(vsComp) );
    GL_CHECK( glDeleteShader(fsComp) );
}

GLProgram::~GLProgram() {
    GL_CHECK( glDeleteProgram(id) );
    id = 0;
}

GLuint GLProgram::compileShader(const GLchar *source, GLenum type) {
    GL_CHECK( GLuint shader = glCreateShader(type) );
    if (!shader) {
        return 0;
    }

    GLint compileStatus = GL_FALSE;
    GL_CHECK( glShaderSource(shader, 1, &source, nullptr) );
    GL_CHECK( glCompileShader(shader) );
    GL_CHECK( glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus) );
    if (compileStatus != GL_TRUE) {
        GLchar infoLog[2048];
        GL_CHECK( glGetShaderInfoLog(shader, 2048, nullptr, infoLog) );
        __android_log_print(ANDROID_LOG_ERROR, "GLProgram", "%s", infoLog);
        GL_CHECK( glDeleteShader(shader) );
        shader = 0;
    }

    return shader;
}
