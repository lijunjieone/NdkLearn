#include <string.h>
#include <jni.h>
#include <unistd.h>

static jstring func1(JNIEnv * env) {
    char* arg5 = "hello jni5";

//env=0;
	return (*env)->NewStringUTF(env,arg5);
}

JNIEXPORT jstring JNICALL Java_com_mx_jnilearn_MainActivity_getTextFromJni
  (JNIEnv * env, jobject thiz){
    char* arg = "hello jni";

    char* arg2 = "hello jni2";

    char* arg3 = "hello jni3";

    char* arg4 = "hello jni4";



    return func1(env);

}
