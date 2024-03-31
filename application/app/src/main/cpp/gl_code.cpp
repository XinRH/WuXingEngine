

#include "gles.h"
#include <function/ANDROIDgles.h>

extern "C" {
JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_setup(JNIEnv *env, jclass clazz, jint width, jint height) {
    WuXing::GLES::setupGraphics(width, height);
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_tick(JNIEnv *env, jclass clazz) {
    WuXing::GLES::renderFrame();
}
}

