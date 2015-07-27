#ifndef SPH_GLSHADERS_H
#define SPH_GLSHADERS_H

#include <GLES3/gl3.h>

#define GL_STRINGIFY(str) #str

const GLchar *CUBE_VS = "#version 300 es\n"
        GL_STRINGIFY(
                layout(location = 0) in vec4 position;
                uniform mat4 viewMatrix;
                uniform mat4 projMatrix;
                void main() {
                    gl_Position = projMatrix * viewMatrix * vec4(position.xyz, 1.0);
                }
        );

const GLchar *CUBE_FS = "#version 300 es\n"
        GL_STRINGIFY(
                out vec4 fragColor;
                void main() {
                    fragColor = vec4(1.0);
                }
        );

#endif
