#include "JniWrapper.h"
#include "GLRenderer.h"
#include "SPHSimulation.h"
#include <android/log.h>

SPHSimulation *sphSimulation = nullptr;
GLRenderer *glRenderer = nullptr;

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceCreated(
        JNIEnv *env, jobject obj) {
    sphSimulation = new SPHSimulation(100);
    glRenderer = new GLRenderer();
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceChanged(
        JNIEnv *env, jobject obj, jint width, jint height) {
    if (sphSimulation) {
        double aspect = (double) height / width;
        sphSimulation->initialize(-1, 1, aspect, -aspect, 0, -1);
    }
    if (glRenderer) {
        glRenderer->surfaceChanged(width, height);
    }
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onRenderFrame(
        JNIEnv *env, jobject obj) {
    if (sphSimulation) {
    }
    if (glRenderer && sphSimulation) {
        glRenderer->renderFrame(sphSimulation);
    }
}

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceReleased(
        JNIEnv *env, jobject obj) {
    if (sphSimulation) {
        delete sphSimulation;
        sphSimulation = nullptr;
    }
    if (glRenderer) {
        delete glRenderer;
        glRenderer = nullptr;
    }
}
