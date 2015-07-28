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

const GLchar *PARTICLE_VS = "#version 300 es\n"
        GL_STRINGIFY(
                layout(location = 0) in vec4 position;
                uniform mat4 viewMatrix;
                uniform mat4 projMatrix;
                uniform vec4 particlePosition;
                uniform float particleScale;
                out vec2 outPosition;
                void main() {
                        vec4 pos = position * particleScale + particlePosition;
                        gl_Position = projMatrix * viewMatrix * vec4(pos.xyz, 1.0);
                        outPosition = position.xy;
                }
        );

const GLchar *PARTICLE_FS = "#version 300 es\n"
        GL_STRINGIFY(
                in vec2 outPosition;
                out vec4 fragColor;
                void main() {
                        float len = length(outPosition);
                        if (len > 1.0) {
                                discard;
                        } else {
                                fragColor.rgb = vec3(0.5, 0.7, 1.0) * (1.2 - len * 0.5);
                                fragColor.a = 1.0;
                        }
                }
        );

#endif
