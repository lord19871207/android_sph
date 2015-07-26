#include "JniWrapper.h"
#include "GLRenderer.h"
#include <android/log.h>

GLRenderer *glRenderer;

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceCreated(
        JNIEnv *env, jobject obj) {
    __android_log_print(ANDROID_LOG_ERROR, "libSPH", "onSurfaceCreated");
    glRenderer = new GLRenderer();
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceChanged(
        JNIEnv *env, jobject obj, jint width, jint height) {
    __android_log_print(ANDROID_LOG_ERROR, "libSPH", "onSurfaceChanged %d %d", width, height);
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onRenderFrame(
        JNIEnv *env, jobject obj) {
    glRenderer->renderFrame();
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceReleased(
        JNIEnv *env, jobject obj) {
    __android_log_print(ANDROID_LOG_ERROR, "libSPH", "onSurfaceDestroyed");
    if (glRenderer) {
        delete glRenderer;
        glRenderer = nullptr;
    }
}
