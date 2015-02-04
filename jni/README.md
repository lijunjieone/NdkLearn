#Android ndk-gdb调试笔记
---
#目的
可以调试ndk代码
#步骤
- 写一个比较简单的工程/tmp/a/下，工程的包名为com.mx.jnitest,类名MainActivity，在工程中使用jni返回一个字符串
- 定义一个jni的代码实现
  > private static native String getTextFromJni()
-  加载这个jni库
  > System.loadLibrary("hello-jni")
加载的两种方式loadLibrary和load，区别在于load需要制定绝对路径
- 使用javap命令生成头文件
  > javap -classpath /tmp/a com.mx.jnitest.MainActivity 
-  实现getTextFromJni()
  ```java
JNIEXPORT jstring JNICALL Java_com_mx_jnilearn_MainActivity_getTextFromJni
  (JNIEnv * env, jobject thiz){
    char* arg = "hello jni";

    char* arg2 = "hello jni2";
    return (*env)->NewStringUTF(env,arg );

}
```
- 编译so,NDK_DEBUG=1很重要
>ndk-build NDK_DEBUG=1
- debug方式运行工程。
- 在工程根目录下/tmp/a/ 运行ndk-gdb
- 进入ndk-gdb 命令行
- i sharedlibrary 可以看到那些so被加载，如果我们的so没有加载可以用下面命令
>file obj/local/armeabi/libsearch-algorithm.so
- l 命令可以显示源码,不能显示源码肯定不能调试
- info breakpoints 显示断点列表，目前可能为空
- b hello-jni.c:8 在hello-jni.c文件的第8行设置一个断点
- c 继续运行
- display arg 查看某个变量



#Android ndk-stack 定位jni错误
---

#目的
当一个发生在jni的崩溃引起程序闪退是如何定位问题
#步骤
- 加入错误代码

``` java
static jstring func1(JNIEnv * env) {
    char* arg5 = "hello jni5";

	env=0;
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
```

>因为env=0，所以调用env的对象时会发生闪退

- 运行程序
- adb logcat | grep DEBUG 监控闪退日志
>********** Crash dump: **********
Build fingerprint: 'google/hammerhead/hammerhead:5.0/LRX21O/1570415:user/release-keys'
pid: 15424, tid: 15424, name: com.mx.jnilearn  >>> com.mx.jnilearn <<<
signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
Stack frame #00 pc 00000c0c  /data/app/com.mx.jnilearn-1/lib/arm/libhello-jni.so
Stack frame #01 pc 00000c84  /data/app/com.mx.jnilearn-1/lib/arm/libhello-jni.so (Java_com_mx_jnilearn_MainActivity_getTextFromJni+80)
Stack frame #02 pc 000dc0a1  /data/dalvik-cache/arm/data@app@com.mx.jnilearn-1@base.apk@classes.dex

- 闪退后使用
- 在工程根目录下使用 adb logcat | ndk-stack -sym obj/local/armeabi-v7a/
>********** Crash dump: **********
Build fingerprint: 'google/hammerhead/hammerhead:5.0/LRX21O/1570415:user/release-keys'
pid: 15424, tid: 15424, name: com.mx.jnilearn  >>> com.mx.jnilearn <<<
signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
Stack frame #00 pc 00000c0c  /data/app/com.mx.jnilearn-1/lib/arm/libhello-jni.so: Routine func1 at /Users/lijunjie/mac/dev/build/android/JniLearn/jni/hello-jni.c:9
Stack frame #01 pc 00000c84  /data/app/com.mx.jnilearn-1/lib/arm/libhello-jni.so (Java_com_mx_jnilearn_MainActivity_getTextFromJni+80): Routine Java_com_mx_jnilearn_MainActivity_getTextFromJni at /Users/lijunjie/mac/dev/build/android/JniLearn/jni/hello-jni.c:25
Stack frame #02 pc 000dc0a1  /data/dalvik-cache/arm/data@app@com.mx.jnilearn-1@base.apk@classes.dex

可以看到在hello-jni.c的第9行发生了错误。

