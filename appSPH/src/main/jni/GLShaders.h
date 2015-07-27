#ifndef SPH_GLSHADERS_H
#define SPH_GLSHADERS_H

#include <GLES3/gl3.h>

#define GL_STRINGIFY(str) #str

const GLchar *CUBE_VS = "#version 300 es\n"
        GL_STRINGIFY(
                layout(location = 0) in vec4 position;
                layout(location = 1) in vec4 color;
                uniform mat4 viewMatrix;
                uniform mat4 projMatrix;
                out vec4 outColor;
                void main() {
                        outColor = color;
                        gl_Position = projMatrix * viewMatrix * position;
                }
        );

const GLchar *CUBE_FS = "#version 300 es\n"
        GL_STRINGIFY(
                in vec4 outColor;
                out vec4 fragColor;
                void main() {
                        fragColor = outColor;
                }
        );

#endif
