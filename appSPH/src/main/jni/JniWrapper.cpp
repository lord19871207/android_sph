#include "JniWrapper.h"
#include "GLRenderer.h"
#include <android/log.h>

GLRenderer *glRenderer = nullptr;

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceCreated(
        JNIEnv *env, jobject obj) {
    glRenderer = new GLRenderer();
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceChanged(
        JNIEnv *env, jobject obj, jint width, jint height) {
    if (glRenderer) {
        glRenderer->surfaceChanged(width, height);
    }
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onRenderFrame(
        JNIEnv *env, jobject obj) {
    if (glRenderer) {
        glRenderer->renderFrame();
    }
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceReleased(
        JNIEnv *env, jobject obj) {
    if (glRenderer) {
        delete glRenderer;
        glRenderer = nullptr;
    }
}
