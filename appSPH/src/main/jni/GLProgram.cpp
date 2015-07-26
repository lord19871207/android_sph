#include "GLProgram.h"
#include <android/log.h>

GLProgram::GLProgram(const GLchar *vsSource, const GLchar *fsSource) {
    id = glCreateProgram();
    if (!id) {
        return;
    }

    GLuint vsComp = compileShader(vsSource, GL_VERTEX_SHADER);
    GLuint fsComp = compileShader(fsSource, GL_FRAGMENT_SHADER);

    if (vsComp && fsComp) {
        GLint linkStatus = GL_FALSE;
        glAttachShader(id, vsComp);
        glAttachShader(id, fsComp);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLchar infoLog[2048];
            glGetProgramInfoLog(id, 2048, nullptr, infoLog);
            __android_log_print(ANDROID_LOG_ERROR, "GLProgram", "%s", infoLog);
            glDeleteProgram(id);
            id = 0;
        }
    }

    glDeleteShader(vsComp);
    glDeleteShader(fsComp);
}

GLProgram::~GLProgram() {
    glDeleteProgram(id);
    id = 0;
}

GLuint GLProgram::compileShader(const GLchar *source, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader) {
        return 0;
    }

    GLint compileStatus = GL_FALSE;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLchar infoLog[2048];
        glGetShaderInfoLog(shader, 2048, nullptr, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "GLProgram", "%s", infoLog);
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}
