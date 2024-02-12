//
// Created by bashlikovvv on 2/12/24.
//

#ifndef LAB_1_DRAWER_H
#define LAB_1_DRAWER_H

#include <jni.h>

#include <malloc.h>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <time.h>

#include "model/geometry/geometry.h"
#include "model/camera/camera.h"

using namespace std;

class drawer {

private:
    clock_t deltaTime = 0;
    unsigned int frames = 0;
    double frameRate = 60;
    double averageFrameTimeMilliseconds = 33.333;
    bool rerender = true;
    const int rgbWhite = 0x00FFFFFF;
    const int rgbBlack = 0x00000000;
    const int height = 2297;
    const int width = 1080;
    unsigned int* buffer = new unsigned int[width * height];
    int* zbuffer = new int[width * height];
    float constLight = .2f;
    bool chained = true;
    int lightMode = 0;
    Camera camera{5.0f,  3.141557f / 2 ,  3.141557f / 2 , Vec3f(0.0f , 1.0f , 0.0f)};
    Vec3f light{ 1.0f, 0.0f, 1.0f};
    float zfar = 100.0f;
    float znear = 0.1f;
    jobject paint;
    vector<Face> faces;
    vector<Vec4f> vertexes;
    vector<Vec3f> normalies;
    void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, int color);
    void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, Vec3f vn0, Vec3f vn1, Vec3f vn2, int color);
    short GetRValue(int rgb);
    short GetGValue(int rgb);
    short GetBValue(int rgb);
    int RGB(short r, short g, short b);
    double clockToMilliseconds(clock_t ticks);
    void doLog(JNIEnv* env, string msg);
    jobject getDrawContext(JNIEnv* env, jobject drawScope);
    jobject getCanvas(JNIEnv* env, jobject drawContext);
    jobject getNativeCanvas(JNIEnv* env, jobject canvas);
    jobject getDefaultPaint(JNIEnv* env);
    void drawLine(JNIEnv* env,jobject nativeCanvas,float startX,float startY,float stopX,float stopY,jobject paint);
    int getNewColor(int color, float koef);
    int addColors(int a1, int a2);
    void invalidateView(JNIEnv* env, jobject view);
    Vec3f calculateNormal(Face face);

public:
    drawer(string pathToFile) {
        parseObjFile(pathToFile);
    }

    vector<Face> parseObjFile(string filePath);
    void painObject(JNIEnv *env,jobject thiz,jobject canvas);
    void translateObject(float x, float y);
    void scaleObject(float zoom);
    void rotateObject(float angle);

};


#endif //LAB_1_DRAWER_H
