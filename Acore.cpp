#include "Acore.h"
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <Math.h>

JNIEXPORT jobject Java_jxl_LocalLocateCore_server_Acore_getParameters(JNIEnv *env, jobject obj){
	jclass jclass_Parameters = env->FindClass("jxl/LocalLocateCore/server/Parameters");
	jmethodID construct = env->GetMethodID(jclass_Parameters,"<init>","()V");
	jobject parameters = env->NewObject(jclass_Parameters, construct);

	jfieldID jfID_default_min_match_apno = env->GetFieldID(jclass_Parameters, "default_min_match_apno", "I");
	jfieldID jfID_default_min_rssi_dis = env->GetFieldID(jclass_Parameters, "default_min_rssi_dis", "D");
	jfieldID jfID_default_xcor_error_value = env->GetFieldID(jclass_Parameters, "default_xcor_error_value", "D");
	jfieldID jfID_default_ycor_error_value = env->GetFieldID(jclass_Parameters, "default_ycor_error_value", "D");
	jfieldID jfID_default_nbr_p_value = env->GetFieldID(jclass_Parameters, "default_nbr_p_value", "I");
	jfieldID jfID_default_max_nbr_no = env->GetFieldID(jclass_Parameters, "default_max_nbr_no", "I");
	jfieldID jfID_bias1 = env->GetFieldID(jclass_Parameters, "bias1", "I");
	jfieldID jfID_max_ap_feature_no = env->GetFieldID(jclass_Parameters, "max_ap_feature_no", "I");
	jfieldID jfID_bias2 = env->GetFieldID(jclass_Parameters, "bias2", "I");
	jfieldID jfID_default_nbr_no = env->GetFieldID(jclass_Parameters, "default_nbr_no", "I");
	jfieldID jfID_xxx = env->GetFieldID(jclass_Parameters, "xxx", "Ljava/lang/String;");
	env->DeleteLocalRef(jclass_Parameters);

	env->SetIntField(parameters, jfID_default_min_match_apno, DEFAULT_MIN_MATCH_APNO);
	env->SetDoubleField(parameters, jfID_default_min_rssi_dis, DEFAULT_MIN_RSSI_DIS);
	env->SetDoubleField(parameters, jfID_default_xcor_error_value, DEFAULT_XCOR_ERROR_VALUE);
	env->SetDoubleField(parameters, jfID_default_ycor_error_value, DEFAULT_YCOR_ERROR_VALUE);
	env->SetIntField(parameters, jfID_default_nbr_p_value, DEFAULT_NBR_P_VALUE);
	env->SetIntField(parameters, jfID_default_max_nbr_no, DEFAULT_MAX_NBR_NO);
	env->SetIntField(parameters, jfID_bias1, BIAS1);
	env->SetIntField(parameters, jfID_max_ap_feature_no, MAX_AP_FEATURE_NO);
	env->SetIntField(parameters, jfID_bias2, BIAS2);
	env->SetIntField(parameters, jfID_default_nbr_no, DEFAULT_NBR_NO);

	char* reFloorchars = "qZu%Mn@pV&";
	jstring reFloor = env->NewStringUTF(reFloorchars);
	env->SetObjectField(parameters, jfID_xxx, reFloor);

	return parameters;
}

JNIEXPORT jobject Java_jxl_LocalLocateCore_server_Acore_mesc(JNIEnv *env, jobject obj, jobject fingerWifiList, jobject userWifiList) {
	int matchApNo = 0;
	double msecValue = 0;

	double sum_trainRssi = 0;
	double sum_testRssi = 0;

	double ave_trainRssi = 0;
	double ave_testRssi = 0;

	int trainArray[MAX_AP_FEATURE_NO];
	int testArray[MAX_AP_FEATURE_NO];
	double trainFeature[MAX_AP_FEATURE_NO * 2];
	double testFeature[MAX_AP_FEATURE_NO * 2];
	//-------------------------------------------------------------------------------------------
	// 获得class类型
	jclass cls_fwList = env->GetObjectClass(fingerWifiList);
	jclass cls_uwList = env->GetObjectClass(userWifiList);

	// 获得List长度
	jmethodID mID_size_fwList = env->GetMethodID(cls_fwList, "size", "()I");
	jmethodID mID_size_uwList = env->GetMethodID(cls_uwList, "size", "()I");
	jint len_fwList = env->CallIntMethod(fingerWifiList, mID_size_fwList);
	jint len_uwList = env->CallIntMethod(userWifiList, mID_size_uwList);

	// 获得get方法的Id
	jmethodID mID_get_fwList = env->GetMethodID(cls_fwList, "get",
			"(I)Ljava/lang/Object;");
	jmethodID mID_get_uwList = env->GetMethodID(cls_uwList, "get",
			"(I)Ljava/lang/Object;");

	env->DeleteLocalRef(cls_fwList);
	env->DeleteLocalRef(cls_uwList);

	// 取出List里第i个
	//-------------------------------------------------------------------------------------------
	/***** 记录一行指纹的AP匹配个数和欧氏距离 *****/
	for (int j = 0; j < len_fwList; j++) {
		jobject obj_fwList_j = env->CallObjectMethod(fingerWifiList,mID_get_fwList, j);
		jclass cls_fwList = env->GetObjectClass(obj_fwList_j);

		jfieldID jfID_fwMac_j = env->GetFieldID(cls_fwList, "Mac", "Ljava/lang/String;");
		jfieldID jfID_fwRssi_j = env->GetFieldID(cls_fwList, "Rssi", "I");

		jstring fwMac_j = (jstring) env->GetObjectField(obj_fwList_j, jfID_fwMac_j);
		jint fwRssi_j = env->GetIntField(obj_fwList_j, jfID_fwRssi_j);
		env->DeleteLocalRef(cls_fwList);
		env->DeleteLocalRef(obj_fwList_j);
		jboolean b1 = true;
		const char* fwMacChars_j = env->GetStringUTFChars(fwMac_j, &b1);

		for (int f = 0; f < len_uwList; f++) {
			jobject obj_uwList_f = env->CallObjectMethod(userWifiList, mID_get_uwList, f);
			jclass cls_uwList = env->GetObjectClass(obj_uwList_f);

			jfieldID jfID_uwMac_f = env->GetFieldID(cls_uwList, "Mac", "Ljava/lang/String;");
			jfieldID jfID_uwRssi_f = env->GetFieldID(cls_uwList, "Rssi", "I");

			jstring uwMac_f = (jstring) env->GetObjectField(obj_uwList_f, jfID_uwMac_f);
			jint uwRssi_f = env->GetIntField(obj_uwList_f, jfID_uwRssi_f);
			env->DeleteLocalRef(cls_uwList);
			env->DeleteLocalRef(obj_uwList_f);
			jboolean b2 = true;
			const char* uwMacChars_j = env->GetStringUTFChars(uwMac_f, &b2);

			if (strcmp(uwMacChars_j, fwMacChars_j) == 0)
			{
				sum_trainRssi = sum_trainRssi + fwRssi_j;
				trainArray[matchApNo] = fwRssi_j;
				sum_testRssi = sum_testRssi + uwRssi_f;
				testArray[matchApNo] = uwRssi_f;
				matchApNo++;
			}
			env->DeleteLocalRef(uwMac_f);
			env->ReleaseStringUTFChars(uwMac_f, uwMacChars_j);
		}
		env->DeleteLocalRef(fwMac_j);
		env->ReleaseStringUTFChars(fwMac_j, fwMacChars_j);
	}

	if (matchApNo > 0) {
		ave_trainRssi = sum_trainRssi / matchApNo;
		ave_testRssi = sum_testRssi / matchApNo;
		for (int kk = 0; kk < matchApNo; kk++) {

			trainFeature[kk] = (double) trainArray[kk] - ave_trainRssi;
			trainFeature[matchApNo + kk] = (log(
					(double) trainArray[kk] / ave_trainRssi) + 2);

			testFeature[kk] = (double) testArray[kk] - ave_testRssi;
			testFeature[matchApNo + kk] = (log(
					(double) testArray[kk] / ave_testRssi) + 2);
		}

		double dis = 0;
		for (int i = 0; i < matchApNo; i++) {
			dis = dis
					+ (trainFeature[i] - testFeature[i])
							* (trainFeature[i] - testFeature[i]);
		}
		msecValue = dis / matchApNo;
	}

	jclass jclass_Mesc = env->FindClass("jxl/LocalLocateCore/server/Mesc");
	jmethodID mthod_init = env->GetMethodID(jclass_Mesc,"<init>","()V");
	jobject jobject_Mesc = env->NewObject(jclass_Mesc, mthod_init);

	jfieldID jfID_MatchApNo = env->GetFieldID(jclass_Mesc, "MatchApNo", "I");
	jfieldID jfID_Mesc = env->GetFieldID(jclass_Mesc, "Mesc", "D");

	env->SetIntField(jobject_Mesc, jfID_MatchApNo, matchApNo);
	env->SetDoubleField(jobject_Mesc, jfID_Mesc, msecValue);
	return jobject_Mesc;
}

JNIEXPORT jboolean Java_jxl_LocalLocateCore_server_Acore_jxlCondition1(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo){
	if(float((float)matchApNo / (float)fingerApNo) >= (0.4) && float((float)matchApNo / (float)userApNo) >= (0.4)) {
		return true;
	} else {
		return false;
	}
}

JNIEXPORT jboolean Java_jxl_LocalLocateCore_server_Acore_jxlCondition11(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo){
	if ((float) matchApNo / fingerApNo * (float) matchApNo / userApNo > 0.25) {
		return true;
	} else {
		return false;
	}
}

JNIEXPORT jboolean Java_jxl_LocalLocateCore_server_Acore_jxlCondition2(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo){
	if ((float) matchApNo / fingerApNo >= 0.2 && (float) matchApNo / userApNo >= 0.2 && (float) matchApNo / fingerApNo < 0.4 || (float) matchApNo / userApNo < 0.4) {
		return true;
	} else {
		return false;
	}
}

JNIEXPORT jboolean Java_jxl_LocalLocateCore_server_Acore_jxlCondition3(JNIEnv *env, jobject obj, jint matchApNo, jint fingerApNo, jint userApNo){
	if ((float) matchApNo / fingerApNo < 0.2 && (float) matchApNo / userApNo > 0.1 || (float) matchApNo / fingerApNo > 0.1 && (float) matchApNo / userApNo < 0.2) {
		return true;
	} else {
		return false;
	}
}

JNIEXPORT jdouble Java_jxl_LocalLocateCore_server_Acore_jxlAveDis11(JNIEnv *env, jobject obj, jint fingerApNo, jint userApNo, jdouble msec, jint matchApNo){
		double rssiDis = (double) fingerApNo / matchApNo * userApNo / matchApNo
				* msec;
		jdouble aveRssiDis = rssiDis / matchApNo + BIAS1;
		return aveRssiDis;
}

JNIEXPORT jdouble Java_jxl_LocalLocateCore_server_Acore_jxlAveDis12(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo){
		double rssiDis = msec + BIAS2;
		jdouble aveRssiDis = rssiDis / matchApNo + BIAS1;
		return aveRssiDis;
}

JNIEXPORT jdouble Java_jxl_LocalLocateCore_server_Acore_jxlAveDis2(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo){
		double rssiDis = msec + BIAS2;
		jdouble aveRssiDis = sqrt(rssiDis / matchApNo) + 2 * BIAS1;
		return aveRssiDis;
}

JNIEXPORT jdouble Java_jxl_LocalLocateCore_server_Acore_jxlAveDis3(JNIEnv *env, jobject obj, jdouble msec, jint matchApNo){
		double rssiDis = msec + BIAS2;
		jdouble aveRssiDis = sqrt(rssiDis / matchApNo) + 3 * BIAS1;
		return aveRssiDis;
}

JNIEXPORT void Java_jxl_LocalLocateCore_server_Acore_updateNeighborP(JNIEnv *env, jobject obj, jdouble curRssiDis, jobject curFingerprint, jobjectArray nbrPointsArray){
	// 将该条记录放入备选队列
	for (int w = 0; w < DEFAULT_MAX_NBR_NO; w++) {
		//----- w ------------------------------------------------------------------------
		jobject jobject_w = (jobject) env->GetObjectArrayElement(nbrPointsArray, w);
		jclass jclass_w = (jclass) env->GetObjectClass(jobject_w);
		jfieldID jfID_Id_w = env->GetFieldID(jclass_w, "Id", "I");
		jfieldID jfID_Floor_w = env->GetFieldID(jclass_w, "Floor", "Ljava/lang/String;");
		jfieldID jfID_Xcor_w = env->GetFieldID(jclass_w, "Xcor", "D");
		jfieldID jfID_Ycor_w = env->GetFieldID(jclass_w, "Ycor", "D");
		jfieldID jfID_Distance_w = env->GetFieldID(jclass_w, "Distance", "D");
		env->DeleteLocalRef(jclass_w);

		jdouble distance_w = env->GetDoubleField(jobject_w, jfID_Distance_w);

		//----- cur ------------------------------------------------------------------------
		jclass jclass_cur = (jclass) env->GetObjectClass(curFingerprint);
		jfieldID jfID_Id_cur = env->GetFieldID(jclass_cur, "Id", "I");
		jfieldID jfID_Floor_cur = env->GetFieldID(jclass_cur, "Floor", "Ljava/lang/String;");
		jfieldID jfID_Xcor_cur = env->GetFieldID(jclass_cur, "Xcor", "D");
		jfieldID jfID_Ycor_cur = env->GetFieldID(jclass_cur, "Ycor", "D");
		env->DeleteLocalRef(jclass_cur);

		jint id_cur = env->GetIntField(curFingerprint, jfID_Id_cur);
		jstring floor_cur = (jstring) env->GetObjectField(curFingerprint, jfID_Floor_cur);
		jdouble xcor_cur = env->GetDoubleField(curFingerprint, jfID_Xcor_cur);
		jdouble ycor_cur = env->GetDoubleField(curFingerprint, jfID_Ycor_cur);
		jdouble distance_cur = curRssiDis;

		if (curRssiDis < distance_w) {
			for (int q = DEFAULT_MAX_NBR_NO - 1; q > w; q--) {
				//----- q-1 ------------------------------------------------------------------------
				jobject jobject_q_1 = (jobject) env->GetObjectArrayElement(nbrPointsArray, q-1);
				jclass jclass_q_1 = (jclass) env->GetObjectClass(jobject_q_1);
				jfieldID jfID_Id_q_1 = env->GetFieldID(jclass_q_1, "Id", "I");
				jfieldID jfID_Floor_q_1 = env->GetFieldID(jclass_q_1, "Floor", "Ljava/lang/String;");
				jfieldID jfID_Xcor_q_1 = env->GetFieldID(jclass_q_1, "Xcor", "D");
				jfieldID jfID_Ycor_q_1 = env->GetFieldID(jclass_q_1, "Ycor", "D");
				jfieldID jfID_Distance_q_1 = env->GetFieldID(jclass_q_1, "Distance", "D");

				jint id_q_1 = env->GetIntField(jobject_q_1, jfID_Id_q_1);
				jstring floor_q_1 = (jstring) env->GetObjectField(jobject_q_1, jfID_Floor_q_1);
				jdouble xcor_q_1 = env->GetDoubleField(jobject_q_1, jfID_Xcor_q_1);
				jdouble ycor_q_1 = env->GetDoubleField(jobject_q_1, jfID_Ycor_q_1);
				jdouble distance_q_1 = env->GetDoubleField(jobject_q_1, jfID_Distance_q_1);
				env->DeleteLocalRef(jobject_q_1);
				env->DeleteLocalRef(jclass_q_1);

				//----- q ------------------------------------------------------------------------
				jobject jobject_q = (jobject) env->GetObjectArrayElement(nbrPointsArray, q);
				jclass jclass_q = (jclass) env->GetObjectClass(jobject_q);
				jfieldID jfID_Id_q = env->GetFieldID(jclass_q, "Id", "I");
				jfieldID jfID_Floor_q = env->GetFieldID(jclass_q, "Floor", "Ljava/lang/String;");
				jfieldID jfID_Xcor_q = env->GetFieldID(jclass_q, "Xcor", "D");
				jfieldID jfID_Ycor_q = env->GetFieldID(jclass_q, "Ycor", "D");
				jfieldID jfID_Distance_q = env->GetFieldID(jclass_q, "Distance", "D");
				env->DeleteLocalRef(jclass_q);

				//----- q <- q-1 ----------------------------------------------------
				env->SetIntField(jobject_q, jfID_Id_q, id_q_1);
				env->SetObjectField(jobject_q, jfID_Floor_q, floor_q_1);
				env->SetDoubleField(jobject_q, jfID_Xcor_q, xcor_q_1);
				env->SetDoubleField(jobject_q, jfID_Ycor_q, ycor_q_1);
				env->SetDoubleField(jobject_q, jfID_Distance_q, distance_q_1);
				env->DeleteLocalRef(floor_q_1);
				env->DeleteLocalRef(jobject_q);
			}
			//-----w <- cor --------------------------------------------
			env->SetIntField(jobject_w, jfID_Id_w, id_cur);
			env->SetObjectField(jobject_w, jfID_Floor_w, floor_cur);
			env->SetDoubleField(jobject_w, jfID_Xcor_w, xcor_cur);
			env->SetDoubleField(jobject_w, jfID_Ycor_w, ycor_cur);
			env->SetDoubleField(jobject_w, jfID_Distance_w, distance_cur);
			break;
		} else {
			continue;
		}
	}
}

JNIEXPORT jobject Java_jxl_LocalLocateCore_server_Acore_Interloc(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray, jint neighborNo){
		if (neighborNo > DEFAULT_NBR_NO) {
			neighborNo = DEFAULT_NBR_NO;
		}

		double w[neighborNo];
		double distanceSum = 0; // 权重计算的分母

		// ///////////////////距离权重计算方法
		double xCor = 0;
		double yCor = 0;
		for (int i = 0; i < neighborNo; i++) {
			jobject jobject_i = (jobject) env->GetObjectArrayElement(nbrPointsArray, i);
			jclass jclass_i = (jclass) env->GetObjectClass(jobject_i);
			jfieldID jfID_Distance_i = env->GetFieldID(jclass_i, "Distance", "D");
			jdouble distance_i = env->GetDoubleField(jobject_i, jfID_Distance_i);
			env->DeleteLocalRef(jobject_i);
			env->DeleteLocalRef(jclass_i);

			distanceSum = distanceSum + distance_i;
		}

		for (int j = 0; j < neighborNo; j++) {
			jobject jobject_j = (jobject) env->GetObjectArrayElement(nbrPointsArray, j);
			jclass jclass_j = (jclass) env->GetObjectClass(jobject_j);
			jfieldID jfID_Distance_j = env->GetFieldID(jclass_j, "Distance", "D");
			jdouble distance_j = env->GetDoubleField(jobject_j, jfID_Distance_j);
			env->DeleteLocalRef(jobject_j);
			env->DeleteLocalRef(jclass_j);

			w[j] = (distanceSum - distance_j) / ((neighborNo - 1) * distanceSum);
		}

		for (int q = 0; q < neighborNo; q++) {
			jobject jobject_q = (jobject) env->GetObjectArrayElement(nbrPointsArray, q);
			jclass jclass_q = (jclass) env->GetObjectClass(jobject_q);
			jfieldID jfID_Xcor_q = env->GetFieldID(jclass_q, "Xcor", "D");
			jfieldID jfID_Ycor_q = env->GetFieldID(jclass_q, "Ycor", "D");
			jdouble xcor_q = env->GetDoubleField(jobject_q, jfID_Xcor_q);
			jdouble ycor_q = env->GetDoubleField(jobject_q, jfID_Ycor_q);
			env->DeleteLocalRef(jobject_q);
			env->DeleteLocalRef(jclass_q);

			xCor = xCor + w[q] * xcor_q;
			yCor = yCor + w[q] * ycor_q;
		}

		jclass jclass_PointDouble = env->FindClass("jxl/LocalLocateCore/server/PointDouble");
		jmethodID mthod_init = env->GetMethodID(jclass_PointDouble,"<init>","()V");
		jobject jobject_pointDouble = env->NewObject(jclass_PointDouble, mthod_init);

		jfieldID jfID_Xcor_q = env->GetFieldID(jclass_PointDouble, "Xcor", "D");
		jfieldID jfID_Ycor_q = env->GetFieldID(jclass_PointDouble, "Ycor", "D");

		env->SetDoubleField(jobject_pointDouble, jfID_Xcor_q, xCor);
		env->SetDoubleField(jobject_pointDouble, jfID_Ycor_q, yCor);
		return jobject_pointDouble;
}

JNIEXPORT jint Java_jxl_LocalLocateCore_server_Acore_resetNbrPointsWeight(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray){
	jint num_neghborPoints = 0;
	for (int i = 0; i < DEFAULT_MAX_NBR_NO; i++) {
		jobject jobject_i = (jobject) env->GetObjectArrayElement(nbrPointsArray, i);
		jclass jclass_i = (jclass) env->GetObjectClass(jobject_i);

		jfieldID jfID_Id_i = env->GetFieldID(jclass_i, "Id", "I");
		jfieldID jfID_Distance_i = env->GetFieldID(jclass_i, "Distance", "D");
		jint id_i = env->GetIntField(jobject_i, jfID_Id_i);
		jdouble distance_i = env->GetDoubleField(jobject_i, jfID_Distance_i);
		if (id_i != 0) {
			env->SetDoubleField(jobject_i, jfID_Distance_i, distance_i - BIAS1);
			num_neghborPoints++;
		}
	}
	return num_neghborPoints;
}

JNIEXPORT jstring Java_jxl_LocalLocateCore_server_Acore_decideFloor(JNIEnv *env, jobject obj, jobjectArray nbrPointsArray){
	char* reFloorchars = "";
	jstring reFloor = env->NewStringUTF(reFloorchars);

	int len = (env)->GetArrayLength(nbrPointsArray);
	if(len < 3) {
		jobject jobject0 = (jobject) env->GetObjectArrayElement(nbrPointsArray, 0);
		jclass jclass0 = (jclass) env->GetObjectClass(jobject0);
		//String Floor
		jfieldID jfID_Floor0 = env->GetFieldID(jclass0, "Floor", "Ljava/lang/String;");
		jstring floor0jstring = (jstring) env->GetObjectField(jobject0, jfID_Floor0);
		reFloor = floor0jstring;
	} else{
		jboolean b = true;
		jobject jobject0 = (jobject) env->GetObjectArrayElement(nbrPointsArray, 0);
		jclass jclass0 = (jclass) env->GetObjectClass(jobject0);
		jfieldID jfID_Floor0 = env->GetFieldID(jclass0, "Floor", "Ljava/lang/String;");
		jstring floor0jstring = (jstring) env->GetObjectField(jobject0, jfID_Floor0);
		const char* floor0chars = env->GetStringUTFChars(floor0jstring, &b);

		jobject jobject1 = (jobject) env->GetObjectArrayElement(nbrPointsArray, 1);
		jclass jclass1 = (jclass) env->GetObjectClass(jobject1);
		jfieldID jfID_Floor1 = env->GetFieldID(jclass1, "Floor", "Ljava/lang/String;");
		jstring floor1jstring = (jstring) env->GetObjectField(jobject1, jfID_Floor1);
		const char* floor1chars = env->GetStringUTFChars(floor1jstring, &b);

		jobject jobject2 = (jobject) env->GetObjectArrayElement(nbrPointsArray, 2);
		jclass jclass2 = (jclass) env->GetObjectClass(jobject2);
		jfieldID jfID_Floor2 = env->GetFieldID(jclass0, "Floor", "Ljava/lang/String;");
		jstring floor2jstring = (jstring) env->GetObjectField(jobject2, jfID_Floor2);
		const char* floor2chars = env->GetStringUTFChars(floor2jstring, &b);

		if (strcmp(floor1chars, floor2chars) == 0) {
			reFloor = floor1jstring;
		} else {
			reFloor = floor0jstring;
		}
	}
	return reFloor;
}

JNIEXPORT jdouble Java_jxl_LocalLocateCore_server_Acore_rssiDistance(JNIEnv *env, jobject obj, jdouble rssi1_X, jdouble rssi1_Y, jdouble rssi2_X, jdouble rssi2_Y){
		jdouble error = sqrt(pow(rssi1_X - rssi2_X, 2) + pow(rssi1_Y - rssi2_Y, 2));
		return error;
}
