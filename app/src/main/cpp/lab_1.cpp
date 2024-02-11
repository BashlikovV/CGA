#pragma once

#include <jni.h>

#include <malloc.h>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "model/geometry/geometry.h"
#include "model/camera/camera.h"

using namespace std;

/* COMMON */

void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, int color);
void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, Vec3f vn0, Vec3f vn1, Vec3f vn2, int color);
short GetRValue(int rgb);
short GetGValue(int rgb);
short GetBValue(int rgb);
int RGB(short r, short g, short b);

char* getCPPString() {
    std::string hWString = "Hello, World!";
    char* string = (char*)malloc(sizeof(char) * hWString.size());

    strcpy(string, hWString.c_str());

    return string;
}

Vec3f calculateNormal(Face face) {
    return cross((Vec3f)(*face.points[2].vertex - *face.points[0].vertex), (Vec3f)(*face.points[1].vertex - *face.points[0].vertex)).normalize();
}

vector<Face> parseObjFile(string filePath) {
    vector<Face> faces;
    vector<Vec4f> vertexes;
    vector<Vec3f> normalies;
    ifstream in;
    in.open(filePath);
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            if (line.substr(0, 2) == "v ") {
                Vec4f vert;
                vert[3] = 1.0f;
                sscanf(line.c_str(), "v %f %f %f", &vert[0], &vert[1], &vert[2]);
                vertexes.push_back(vert);
            }
            if (line.substr(0, 3) == "vn ") {
                Vec3f normal;
                sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                normalies.push_back(normal.normalize());
            }
            if (line.substr(0, 2) == "f ") {
                Face face;
                istringstream v(line.substr(2));
                string token;
                int ind = 0;
                while (v >> token) {
                    Point p;
                    int vertexInd = -1;
                    int normalInd = -1;
                    sscanf(token.c_str(), "%i/%i/%i", &vertexInd, &p.textureInd, &normalInd);
                    p.vertex = &vertexes[vertexInd - 1];
                    if (normalInd > 0) p.normal = normalies[normalInd - 1];
                    face.points[ind] = p;
                    ind++;
                }
                faces.push_back(face);
            }
        }
        in.close();
        vertexes.clear();
    }

    std::map<Vec4f*, std::vector<Face*>> map;
    for (size_t i = 0; i < faces.size(); i++) {
        faces[i].normal = calculateNormal(faces[i]);
        for (size_t j = 0; j < 3; j++) {
            map[faces[i].points[j].vertex].push_back(&faces[i]);
        }
    }

    for (size_t i = 0; i < faces.size(); i++) {
        for (size_t j = 0; j < 3; j++) {
            if (faces[i].points[j].normal.x == -1.1111f) {
                std::vector<Face*> a = map[faces[i].points[j].vertex];
                Vec3f norm{};
                for (size_t k = 0; k < a.size(); k++) {
                    norm = norm + a[k]->normal;
                }
                norm = (norm / a.size()).normalize();
                faces[i].points[j].normal = norm;
            }
        }
    }

    return faces;
}

/* ANDROID */

extern "C"
JNIEXPORT jstring JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_getCPPString(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(getCPPString());
}

jobject getDrawContext(JNIEnv* env, jobject drawScope) {
    jclass drawScopeClass = (*env).FindClass("androidx/compose/ui/graphics/drawscope/DrawScope");
    const char* signature = "()Landroidx/compose/ui/graphics/drawscope/DrawContext;";
    jmethodID drawContextId = (*env).GetMethodID(
            drawScopeClass,"getDrawContext", signature
    );

    return (*env).CallObjectMethod(drawScope, drawContextId); //DrawContext
}

jobject getCanvas(JNIEnv* env, jobject drawContext) {
    jclass drawContextClass = (*env).FindClass("androidx/compose/ui/graphics/drawscope/DrawContext");
    const char* signature = "()Landroidx/compose/ui/graphics/Canvas;";
    jmethodID drawContextId = (*env).GetMethodID(
            drawContextClass,"getCanvas", signature
    );

    return (*env).CallObjectMethod(drawContext, drawContextId); //Canvas
}

jobject getNativeCanvas(JNIEnv* env, jobject canvas) {
    jclass drawContextClass = (*env).FindClass("androidx/compose/ui/graphics/AndroidCanvas");
    jfieldID internalCanvasId = (*env).GetFieldID(
            drawContextClass,"internalCanvas", "Landroid/graphics/Canvas;"
    );

    return (*env).GetObjectField(canvas, internalCanvasId); // NativeCanvas
}

jobject getDefaultPaint(JNIEnv* env) {
    jclass paintClass = (*env).FindClass("android/graphics/Paint");
    jmethodID constructor = (*env).GetMethodID(paintClass, "<init>", "()V");

    return (*env).NewObject(paintClass, constructor);
}

void drawLine(
    JNIEnv* env,
    jobject nativeCanvas,
    float startX,
    float startY,
    float stopX,
    float stopY,
    jobject paint
) {
    jclass nativeCanvasClass = (*env).FindClass("android/graphics/Canvas");
    jmethodID drawLineId = (*env).GetMethodID(
            nativeCanvasClass, "drawLine", "(FFFFLandroid/graphics/Paint;)V"
    );

    (*env).CallVoidMethod(nativeCanvas, drawLineId, startX, startY, stopX, stopY, paint);
}

clock_t deltaTime = 0;
unsigned int frames = 0;
double  frameRate = 60;
double  averageFrameTimeMilliseconds = 33.333;
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

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onPaint(
    JNIEnv *env,
    jobject thiz,
    jobject draw_scope,
    jstring path_to_file
) {
    light = light.normalize();
    jobject drawContext = getDrawContext(env, draw_scope);
    jobject canvas = getCanvas(env, drawContext);
    jobject nativeCanvas = getNativeCanvas(env, canvas);
    jobject paint = getDefaultPaint(env);

    std::string path = (*env).GetStringUTFChars(path_to_file, 0);
    std::vector<Face> faces = parseObjFile(path);
    clock_t beginFrame = clock();
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            int ind = i * width + j;
            buffer[ind] = 255;
            zbuffer[ind] = -100000;
        }
    }

    camera.updateViewMatrix();
    viewport(width, height);
    float zfar = 100.0f;
    float znear = 0.1f;
    projection(float(width) / height, 110, zfar, znear);

    Matrix finalMatrix = Viewport * Projection * ModelView;
    Vec4f drawFace[3];
    Vec3f normals[3];
    Face face;
    size_t size = faces.size();
    if (chained) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
            }

            for (int j = 0; j < 3; j++) {
                Vec3i a1 = drawFace[j];
                Vec3i a2 = drawFace[(j + 1) % 3];
                drawLine(env, nativeCanvas, a1[0], a1[1], a2[0], a2[1], paint);
            }
        }
    }
    if (lightMode == 1) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            }

            rasterize(drawFace[0], drawFace[1], drawFace[2], rgbWhite);
        }
    }
    if (lightMode == 2) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            }

            float colorMultiplier = (face.normal * light) + constLight;
            if (colorMultiplier > 1.0f) colorMultiplier = 1.0f;
            int color = RGB(GetRValue(rgbWhite) * colorMultiplier, GetGValue(rgbWhite) * colorMultiplier, GetBValue(rgbWhite) * colorMultiplier);
            rasterize(drawFace[0], drawFace[1], drawFace[2], color);
        }
    }
    if (lightMode == 3) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
                normals[j] = face.points[j].normal;
            }
            rasterize(drawFace[0], drawFace[1], drawFace[2], normals[0], normals[1], normals[2], rgbWhite);
        }
    }
    for (size_t i = 0; i < size; i++) {
        face = faces[i];

        if ((face.normal * camera.getFront()) + 1.0f < 0) {
            continue;
        }

        for (size_t j = 0; j < 3; j++) {
            drawFace[j] = finalMatrix * *face.points[j].vertex;
            drawFace[j] = drawFace[j] / drawFace[j].w;
            drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            normals[j] = face.points[j].normal;
        }

        // 3 lab
        //rasterize(drawFace[0], drawFace[1], drawFace[2], normals[0], normals[1], normals[2], rgbWhite);

        // 2 lab
        float colorMultiplier = (face.normal * light) + constLight;
        if (colorMultiplier > 1.0f) colorMultiplier = 1.0f;
        int color = RGB(GetRValue(rgbWhite) * colorMultiplier, GetGValue(rgbWhite) * colorMultiplier, GetBValue(rgbWhite) * colorMultiplier);
        rasterize(drawFace[0], drawFace[1], drawFace[2], color);

        // 1 lab
        for (int j = 0; j < 3; j++) {
            Vec3i a1 = drawFace[j];
            Vec3i a2 = drawFace[(j + 1) % 3];
            drawLine(env, nativeCanvas, a1[0], a1[1], a2[0], a2[1], paint);
        }
    }
}

int getNewColor(int color, float koef) {
    if (koef > 1.f) koef = 1.0f;
    if (koef < 0) koef = 0.0f;
    return RGB(GetRValue(color) * koef, GetGValue(color) * koef, GetBValue(color) * koef);
}

int addColors(int a1, int a2) {
    short R = GetRValue(a1);
    short G = GetGValue(a1);
    short B = GetBValue(a1);

    short R1 = GetRValue(a2);
    short G1 = GetGValue(a2);
    short B1 = GetBValue(a2);

    int(R) + R1 > 254 ? R = 254 : R = R + R1;
    int(G) + G1 > 254 ? G = 254 : G = G + G1;
    int(B) + B1 > 254 ? B = 254 : B = B + B1;

    return RGB(R, G, B);
}

void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, int color) {
    if (t0.y == t1.y && t0.y == t2.y) return;
    if (t0.y > t1.y) {std::swap(t0, t1);}
    if (t0.y > t2.y) {std::swap(t0, t2);}
    if (t1.y > t2.y) {std::swap(t1, t2);}
    int total_height = t2.y - t0.y;

    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;

        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec3i A = (Vec3f)t0 + Vec3f(t2 - t0) * alpha;
        Vec3i B = second_half ? (Vec3f)t1 + Vec3f(t2 - t1) * beta : (Vec3f)t0 + Vec3f(t1 - t0) * beta;

        if (A.x > B.x) {std::swap(A, B);}
        Vec3f C = B - A;

        for (int j = 0; j <= C.x; j++) {
            float phi = j == C.x ? 1.f : (float)j / C.x;
            Vec3i P = Vec3f(A) + C * phi;

            if (P.x < 0 || P.y < 0 || P.x >= width || P.y >= height) {
                continue;
            }
            int idx = P.x + P.y * width;
            if (zbuffer[idx] < P.z) {
                zbuffer[idx] = P.z;
                buffer[idx] = color;
            }
        }
    }
}

void rasterize(Vec3i t0, Vec3i t1, Vec3i t2, Vec3f vn0, Vec3f vn1, Vec3f vn2, int color) {
    if (t0.y == t1.y && t0.y == t2.y) return;
    if (t0.y > t1.y) {
        std::swap(t0, t1); std::swap(vn0, vn1);
    }
    if (t0.y > t2.y) {
        std::swap(t0, t2); std::swap(vn0, vn2);
    }
    if (t1.y > t2.y) {
        std::swap(t1, t2); std::swap(vn1, vn2);
    }
    int total_height = t2.y - t0.y;

    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;

        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec3i A = (Vec3f)t0 + Vec3f(t2 - t0) * alpha;
        Vec3i B = second_half ? (Vec3f)t1 + Vec3f(t2 - t1) * beta : (Vec3f)t0 + Vec3f(t1 - t0) * beta;

        Vec3f NormA = vn0 + (vn2 - vn0) * alpha;
        Vec3f NormB = second_half ? vn1 + (vn2 - vn1) * beta : vn0 + (vn1 - vn0) * beta;

        if (A.x > B.x) {
            std::swap(A, B);
            std::swap(NormA, NormB);
        }
        Vec3f C = B - A;
        Vec3f NormC = NormB - NormA;

        for (int j = 0; j <= C.x; j++) {
            float phi = j == C.x ? 1.f : (float)j / C.x;
            Vec3i P = Vec3f(A) + C * phi;
            Vec3f Norm = (NormA + NormC * phi).normalize();

            if (P.x < 0 || P.y < 0 || P.x >= width || P.y >= height) {
                continue;
            }


            float Id = 0.5f * (Norm * light);

            Vec3f R = (Norm * (Norm * light * 2.f) - light).normalize();
            float Is = (R * camera.getFront());

            int idx = P.x + P.y * width;
            if (zbuffer[idx] < P.z) {
                zbuffer[idx] = P.z;
                buffer[idx] = addColors(addColors(getNewColor(color, Is), getNewColor(color, Id)), getNewColor(color, constLight));
                //buffer[idx] = addColors(getNewColor(color, Id), getNewColor(color, constLight));
            }
        }
    }
}

short GetRValue(int rgb) {
    return (rgb >> 16) & 0xFF;
}

short GetGValue(int rgb) {
    return (rgb >> 8) & 0xFF;
}

short GetBValue(int rgb) {
    return rgb & 0xFF;
}

int RGB(short r, short g, short b) {
    return (r << 16) | (g << 8) | b;
}

extern "C"
JNIEXPORT void JNICALL
Java_by_bashlikovvv_lab_11_presentation_ui_MainActivity_onPaintWithTranslation(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jobject draw_scope,
                                                                               jstring path_to_file,
                                                                               jfloat x, jfloat y) {
    light = light.normalize();
    jobject drawContext = getDrawContext(env, draw_scope);
    jobject canvas = getCanvas(env, drawContext);
    jobject nativeCanvas = getNativeCanvas(env, canvas);
    jobject paint = getDefaultPaint(env);

    std::string path = (*env).GetStringUTFChars(path_to_file, 0);
    std::vector<Face> faces = parseObjFile(path);
    clock_t beginFrame = clock();
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            int ind = i * width + j;
            buffer[ind] = 255;
            zbuffer[ind] = -100000;
        }
    }

    camera.updateViewMatrix();
    viewport(width + x, height + y);
    float zfar = 100.0f;
    float znear = 0.1f;
    projection(float(width) / height, 110, zfar, znear);

    Matrix finalMatrix = Viewport * Projection * ModelView;
    Vec4f drawFace[3];
    Vec3f normals[3];
    Face face;
    size_t size = faces.size();
    if (chained) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
            }

            for (int j = 0; j < 3; j++) {
                Vec3i a1 = drawFace[j];
                Vec3i a2 = drawFace[(j + 1) % 3];
                drawLine(env, nativeCanvas, a1[0], a1[1], a2[0], a2[1], paint);
            }
        }
    }
    if (lightMode == 1) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            }

            rasterize(drawFace[0], drawFace[1], drawFace[2], rgbWhite);
        }
    }
    if (lightMode == 2) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            }

            float colorMultiplier = (face.normal * light) + constLight;
            if (colorMultiplier > 1.0f) colorMultiplier = 1.0f;
            int color = RGB(GetRValue(rgbWhite) * colorMultiplier, GetGValue(rgbWhite) * colorMultiplier, GetBValue(rgbWhite) * colorMultiplier);
            rasterize(drawFace[0], drawFace[1], drawFace[2], color);
        }
    }
    if (lightMode == 3) {
        for (size_t i = 0; i < size; i++) {
            face = faces[i];

            if ((face.normal * camera.getFront()) + 1.0f < 0) {
                continue;
            }

            for (size_t j = 0; j < 3; j++) {
                drawFace[j] = finalMatrix * *face.points[j].vertex;
                drawFace[j] = drawFace[j] / drawFace[j].w;
                drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
                normals[j] = face.points[j].normal;
            }
            rasterize(drawFace[0], drawFace[1], drawFace[2], normals[0], normals[1], normals[2], rgbWhite);
        }
    }
    for (size_t i = 0; i < size; i++) {
        face = faces[i];

        if ((face.normal * camera.getFront()) + 1.0f < 0) {
            continue;
        }

        for (size_t j = 0; j < 3; j++) {
            drawFace[j] = finalMatrix * *face.points[j].vertex;
            drawFace[j] = drawFace[j] / drawFace[j].w;
            drawFace[j][2] = (1.0f - drawFace[j][2]) * 10000.f;
            normals[j] = face.points[j].normal;
        }

        // 3 lab
        //rasterize(drawFace[0], drawFace[1], drawFace[2], normals[0], normals[1], normals[2], rgbWhite);

        // 2 lab
        float colorMultiplier = (face.normal * light) + constLight;
        if (colorMultiplier > 1.0f) colorMultiplier = 1.0f;
        int color = RGB(GetRValue(rgbWhite) * colorMultiplier, GetGValue(rgbWhite) * colorMultiplier, GetBValue(rgbWhite) * colorMultiplier);
        rasterize(drawFace[0], drawFace[1], drawFace[2], color);

        // 1 lab
        for (int j = 0; j < 3; j++) {
            Vec3i a1 = drawFace[j];
            Vec3i a2 = drawFace[(j + 1) % 3];
            drawLine(env, nativeCanvas, a1[0], a1[1], a2[0], a2[1], paint);
        }
    }
}