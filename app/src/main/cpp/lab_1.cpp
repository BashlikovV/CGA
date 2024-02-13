#pragma once

#include <jni.h>

using namespace std;

/* COMMON */

#include <string>
#include "drawer.h"

drawer drawerObject("/data/user/0/by.bashlikovvv.lab_1/files/teapot.obj");

char* getCPPString() {
    std::string hWString = "Hello, World!";
    char* string = (char*)malloc(sizeof(char) * hWString.size());

    strcpy(string, hWString.c_str());

    return string;
}

/* ANDROID */

extern "C"
JNIEXPORT jstring JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_getCPPString(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(getCPPString());
}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onPaint(
    JNIEnv *env,
    jobject thiz,
    jobject draw_scope,
    jstring path_to_file
) {

}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onTranslate(
    JNIEnv *env,
    jobject thiz,
    jfloat x,
    jfloat y
) {
    drawerObject.translateObject(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onRotate(
    JNIEnv *env,
    jobject thiz,
    jfloat angle
) {
    drawerObject.rotateObject(angle);
}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onScale(
    JNIEnv *env,
    jobject thiz,
    jfloat zoom
) {
    drawerObject.scaleObject(zoom);
}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_view_CanvasView_prepareEnvironment(
        JNIEnv *env,
        jobject thiz,
        jstring path_to_file
) {
    drawerObject.parseObjFile(env->GetStringUTFChars(path_to_file, 0));
    drawerObject.setDrawLineMethodId(env);
}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_view_CanvasView_onPaintCanvas(
        JNIEnv *env,
        jobject thiz,
        jobject canvas
) {
    drawerObject.nativeCanvas = canvas;
    drawerObject.painObject(env, thiz);
}