#ifndef SPH_GLUTILS_H
#define SPH_GLUTILS_H

#include <android/log.h>

#define GL_CHECK(func) \
func; \
while (true) { \
    GLenum err = glGetError(); \
    if (err == GL_NO_ERROR) { \
        break; \
    } \
    __android_log_print(ANDROID_LOG_ERROR, "GLUtils", "Error %08x, at %s:%i - for %s", err, __FILE__, __LINE__, #func); \
}

#endif
