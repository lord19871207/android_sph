#ifndef SPH_JNIWRAPPER_H
#define SPH_JNIWRAPPER_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceCreated(
        JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceChanged(
        JNIEnv *env, jobject obj, jint width, jint height);

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onRenderFrame(
        JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_io_harism_app_sph_JniWrapper_onSurfaceReleased(
        JNIEnv *env, jobject obj);

#ifdef __cplusplus
}
#endif

#endif
