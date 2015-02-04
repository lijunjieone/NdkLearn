#include <string.h>
#include <jni.h>
#include <unistd.h>


JNIEXPORT jstring JNICALL Java_com_mx_jnilearn_MainActivity_getTextFromJni
  (JNIEnv * env, jobject thiz){
    char* arg = "hello jni";

    return (*env)->NewStringUTF(env,arg );

}
