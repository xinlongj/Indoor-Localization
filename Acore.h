#include <stdio.h>
#include <string.h>
#include <jni.h>

#define DEFAULT_MIN_MATCH_APNO		0
#define DEFAULT_MIN_RSSI_DIS		10000000
#define DEFAULT_XCOR_ERROR_VALUE	-100
#define DEFAULT_YCOR_ERROR_VALUE	-100
#define DEFAULT_NBR_P_VALUE			9999999
#define DEFAULT_MAX_NBR_NO			5
#define BIAS1						100000
#define MAX_AP_FEATURE_NO			230
#define BIAS2						1000
#define DEFAULT_NBR_NO				4

extern "C" {
JNIEXPORT jobject	Java_jxl_LocalLocateCore_server_Acore_getParameters(JNIEnv *env, jobject obj);
JNIEXPORT jobject	Java_jxl_LocalLocateCore_server_Acore_mesc(JNIEnv *env, jobject obj, jobject fingerWifiList, jobject userWifiList);
JNIEXPORT jboolean	Java_jxl_LocalLocateCore_server_Acore_jxlCondition1(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo);
JNIEXPORT jboolean	Java_jxl_LocalLocateCore_server_Acore_jxlCondition11(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo);
JNIEXPORT jboolean	Java_jxl_LocalLocateCore_server_Acore_jxlCondition2(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo);
JNIEXPORT jboolean	Java_jxl_LocalLocateCore_server_Acore_jxlCondition3(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo);
JNIEXPORT jdouble	Java_jxl_LocalLocateCore_server_Acore_jxlAveDis11(JNIEnv *env, jobject obj, jint fingerApNo, jint userApNo, jdouble msec, jint matchApNo);
JNIEXPORT jdouble	Java_jxl_LocalLocateCore_server_Acore_jxlAveDis12(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo);
JNIEXPORT jdouble	Java_jxl_LocalLocateCore_server_Acore_jxlAveDis2(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo);
JNIEXPORT jdouble	Java_jxl_LocalLocateCore_server_Acore_jxlAveDis3(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo);
JNIEXPORT void 		Java_jxl_LocalLocateCore_server_Acore_updateNeighborP(JNIEnv *env, jobject obj, jdouble curRssiDis, jobject curFingerprint, jobjectArray nbrPointsArray);
JNIEXPORT jobject	Java_jxl_LocalLocateCore_server_Acore_Interloc(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray, jint neighborNo);
JNIEXPORT jint 		Java_jxl_LocalLocateCore_server_Acore_resetNbrPointsWeight(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray);
JNIEXPORT jstring	Java_jxl_LocalLocateCore_server_Acore_decideFloor(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray);
JNIEXPORT jdouble	Java_jxl_LocalLocateCore_server_Acore_rssiDistance(JNIEnv *env, jobject obj, jdouble rssi1_X, jdouble rssi1_Y, jdouble rssi2_X, jdouble rssi2_Y);
}
