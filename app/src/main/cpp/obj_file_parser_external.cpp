//
// Created by bashlikovvv on 2/10/24.
//
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
//#include "model/geometry/geometry.cpp"

using namespace std;

/* COMMON */
//
//Vec3f calculateNormal(Face face) {
//    return cross((Vec3f)(*face.points[2].vertex - *face.points[0].vertex), (Vec3f)(*face.points[1].vertex - *face.points[0].vertex)).normalize();
//}
//
//vector<Face> parseObjFile(string filePath) {
//    vector<Face> faces;
//    vector<Vec4f> vertexes;
//    vector<Vec3f> normalies;
//    ifstream in;
//    in.open(filePath);
//    if (in.is_open()) {
//        string line;
//        while (getline(in, line)) {
//            if (line.substr(0, 2) == "v ") {
//                Vec4f vert;
//                vert[3] = 1.0f;
//                sscanf(line.c_str(), "v %f %f %f", &vert[0], &vert[1], &vert[2]);
//                vertexes.push_back(vert);
//            }
//            if (line.substr(0, 3) == "vn ") {
//                Vec3f normal;
//                sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
//                normalies.push_back(normal.normalize());
//            }
//            if (line.substr(0, 2) == "f ") {
//                Face face;
//                istringstream v(line.substr(2));
//                string token;
//                int ind = 0;
//                while (v >> token) {
//                    Point p;
//                    int vertexInd = -1;
//                    int normalInd = -1;
//                    sscanf(token.c_str(), "%i/%i/%i", &vertexInd, &p.textureInd, &normalInd);
//                    p.vertex = &vertexes[vertexInd - 1];
//                    if (normalInd > 0) p.normal = normalies[normalInd - 1];
//                    face.points[ind] = p;
//                    ind++;
//                }
//                faces.push_back(face);
//            }
//        }
//        in.close();
//        vertexes.clear();
//    }
//
//    std::map<Vec4f*, std::vector<Face*>> map;
//    for (size_t i = 0; i < faces.size(); i++) {
//        faces[i].normal = calculateNormal(faces[i]);
//        for (size_t j = 0; j < 3; j++) {
//            map[faces[i].points[j].vertex].push_back(&faces[i]);
//        }
//    }
//
//    for (size_t i = 0; i < faces.size(); i++) {
//        for (size_t j = 0; j < 3; j++) {
//            if (faces[i].points[j].normal.x == -1.1111f) {
//                std::vector<Face*> a = map[faces[i].points[j].vertex];
//                Vec3f norm{};
//                for (size_t k = 0; k < a.size(); k++) {
//                    norm = norm + a[k]->normal;
//                }
//                norm = (norm / a.size()).normalize();
//                faces[i].points[j].normal = norm;
//            }
//        }
//    }
//
//    return faces;
//}
//
/* ANDROID */

extern "C"
JNIEXPORT jstring JNICALL
Java_by_bashlikovvv_lab_11_data_ObjFileParser_getLibraryName(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("object files parser");
}

jclass getPointClass(JNIEnv *env) {
    return (*env).FindClass("by/bashlikovvv/lab_1/model/Point");
}

jclass getFaceClass(JNIEnv *env) {
    return (*env).FindClass("by/bashlikovvv/lab_1/model/Face");
}

jclass getVec3fClass(JNIEnv *env) {
    return (*env).FindClass("by/bashlikovvv/lab_1/model/Vec3f");
}

jclass getVec4fClass(JNIEnv *env) {
    return (*env).FindClass("by/bashlikovvv/lab_1/model/Vec4f");
}

jobject getFaceClassObject(JNIEnv *env, jobjectArray *points, jobject *vec3f) {
    jobject result;
    jvalue args[2]; //Array of Point-s, Vec3f

    jclass clazz = getFaceClass(env);
    const char* signature = "([Lby/bashlikovvv/lab_1/model/Point;Lby/bashlikovvv/lab_1/model/Vec3f;)V";
    jmethodID constructor = (*env).GetMethodID(clazz, "<init>", signature);

    args[0].l = *points;
    args[1].l = *vec3f;
    result = (*env).NewObjectA(clazz, constructor, args);

    return result;
}

jobject getVec3fClassObject(JNIEnv *env, double i, double j, double k) {
    jobject result;
    jvalue args[3];

    jclass clazz = getVec3fClass(env);
    const char* signature = "(DDD)V";
    jmethodID constructor = (*env).GetMethodID(clazz, "<init>", signature);

    args[0].d = i;
    args[1].d = j;
    args[2].d = k;
    result = (*env).NewObjectA(clazz, constructor, args);

    return result;
}

jobject getVec4fClassObject(JNIEnv *env, double v, double x, double y, double z) {
    jobject result;
    jvalue args[4];

    jclass clazz = getVec4fClass(env);
    const char* signature = "(DDDD)V";
    jmethodID constructor = (*env).GetMethodID(clazz, "<init>", signature);

    args[0].d = v;
    args[1].d = x;
    args[2].d = y;
    args[3].d = z;
    result = (*env).NewObjectA(clazz, constructor, args);

    return result;
}

jobject getPointClassObject(JNIEnv *env, jobject vec3f, jobject vec4f) {
    jobject result;
    jvalue args[2];

    jclass clazz = getPointClass(env);
    const char* signature = "(Lby/bashlikovvv/lab_1/model/Vec4f;Lby/bashlikovvv/lab_1/model/Vec3f;)V";
    jmethodID constructor = (*env).GetMethodID(clazz, "<init>", signature);

    args[0].l = vec4f;
    args[1].l = vec3f;
    result = (*env).NewObjectA(clazz, constructor, args);

    return result;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_by_bashlikovvv_lab_11_data_ObjFileParser_getStructuresFromFile(
    JNIEnv *env,
    jobject thiz,
    jstring filename
) {
    string path = env->GetStringUTFChars(filename, 0);

//    vector<Face> faces = parseObjFile(path);
    vector<Face> faces;
    int size = faces.size();
    jobjectArray result = env->NewObjectArray(size, getFaceClass(env), nullptr);
    for (int i = 0; i < size; ++i) {
        int pointsSize = sizeof(faces[i].points);
        jobjectArray points = env->NewObjectArray(pointsSize, getPointClass(env), nullptr);

        for (int j = 0; j < 3; ++j) {
            Point point = faces[i].points[j];
            Vec4f vec4f = *point.vertex;
            Vec3f vec3F = point.normal;
            jobject vec4fObject = getVec4fClassObject(env, vec4f.w, vec4f.x, vec4f.y, vec4f.z);
            jobject vec3fObject = getVec3fClassObject(env, vec3F.x, vec3F.y, vec3F.z);
            jobject pointClassObject = getPointClassObject(env, vec3fObject, vec4fObject);
            (*env).SetObjectArrayElement(points, j, pointClassObject);
        }

        Vec3f normal = faces[i].normal;
        jobject vec3f = getVec3fClassObject(env, normal.x, normal.y, normal.z);
        jobject faceClassObject = getFaceClassObject(env, &points, &vec3f);
        (*env).SetObjectArrayElement(result, i, faceClassObject);
    }

    return result;
}