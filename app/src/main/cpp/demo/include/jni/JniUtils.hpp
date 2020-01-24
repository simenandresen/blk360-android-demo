#pragma once

#include "jni.h"
#include <string>
#include <vector>

using namespace std;

namespace jni_utils {


    string jstring_to_cpp_string(JNIEnv *env, jstring jstr);

    vector<string> java_string_array_to_vector(JNIEnv *env, jobjectArray jstring_array);

    vector<double> jsdouble_array_to_vector(JNIEnv *env, jdoubleArray jdouble_array);

    string get_java_object_member_string_var(JNIEnv *env, jobject object, string member_name);

    int get_java_object_member_int_var(JNIEnv *env, jobject object, string member_name);

    bool get_java_object_member_bool_var(JNIEnv *env, jobject object, string member_name);

    void throwJavaException(JNIEnv *env, const char *msg);

    void throwBlk360Exception(
            JNIEnv *env,
            const char *functionName,
            const char *file,
            int line,
            Blk360_Error e
    );

    jdoubleArray vector_to_jdoubleArray(JNIEnv *env, const vector<double> &vector);
}


#define ThrowBlk360Exception(env, error) jni_utils::throwBlk360Exception(env, __func__, __FILE__, __LINE__, error)


namespace jni_utils {

    string jstring_to_cpp_string(JNIEnv *env, jstring jstr) {
        const char *resultCStr = env->GetStringUTFChars(jstr, NULL);
        std::string cpp_str(resultCStr);
        env->ReleaseStringUTFChars(jstr, resultCStr);
        return cpp_str;
    }

    vector<string> java_string_array_to_vector(JNIEnv *env, jobjectArray jstring_array) {
        vector<string> vec;
        jsize string_count = env->GetArrayLength(jstring_array);
        for (int i = 0; i < string_count; i++) {
            jstring arr_element = (jstring) (env->GetObjectArrayElement(jstring_array, i));
            string path = jstring_to_cpp_string(env, arr_element);
            vec.push_back(path);
        }
        return vec;
    }

    vector<double> jsdouble_array_to_vector(JNIEnv *env, jdoubleArray jdouble_array) {
        vector<double> vec;
        jsize string_count = env->GetArrayLength(jdouble_array);
        double *data = env->GetDoubleArrayElements(jdouble_array, NULL);
        // Don't forget to release it
        for (int i = 0; i < string_count; i++) {
            vec.push_back(data[i]);
        }

        env->ReleaseDoubleArrayElements(jdouble_array, data, 0);
        return vec;
    }

    string get_java_object_member_string_var(JNIEnv *env, jobject object, string member_name) {
        jclass thisClass = env->GetObjectClass(object);
        jfieldID fidNumber = env->GetFieldID(thisClass, member_name.c_str(), "Ljava/lang/String;");
        if (fidNumber == NULL) {
            std::cout << "Cannot get member: " << member_name << " from jobject" << std::endl;
        }
        jstring jstr = (jstring) env->GetObjectField(object, fidNumber);
        return jstring_to_cpp_string(env, jstr);
    }

    bool get_java_object_member_bool_var(JNIEnv *env, jobject object, string member_name) {
        jclass thisClass = env->GetObjectClass(object);
        jfieldID fidNumber = env->GetFieldID(thisClass, member_name.c_str(), "Z");
        if (fidNumber == NULL) {
            std::cout << "Cannot get member: " << member_name << " from jobject" << std::endl;
        }
        jboolean jbool = env->GetBooleanField(object, fidNumber);
        return (bool) jbool;
    }

    int get_java_object_member_int_var(JNIEnv *env, jobject object, string member_name) {
        jclass thisClass = env->GetObjectClass(object);
        jfieldID fidNumber = env->GetFieldID(thisClass, member_name.c_str(), "I");
        if (fidNumber == NULL) {
            std::cout << "Cannot get member: " << member_name << " from jobject" << std::endl;
        }
        jint number = env->GetIntField(object, fidNumber);
        return (int) number;
    }

    void
    set_java_object_member_string(JNIEnv *env, jclass Class, jobject object, string member_name,
                                  const char *value) {
        jfieldID id = env->GetFieldID(Class, member_name.c_str(), "Ljava/lang/String;");
        env->SetObjectField(object, id, env->NewStringUTF(value));
    }

    void set_java_object_member_long(JNIEnv *env, jclass Class, jobject object, string member_name,
                                     long value) {
        jfieldID id = env->GetFieldID(Class, member_name.c_str(), "J");
        env->SetLongField(object, id, value);
    }

    void set_java_object_member_float(JNIEnv *env, jclass Class, jobject object, string member_name, float value) {
        jfieldID id = env->GetFieldID(Class, member_name.c_str(), "F");
        env->SetFloatField(object, id, value);
    }

    void set_java_object_member_int(JNIEnv *env, jclass Class, jobject object, string member_name,
                                    int value) {
        jfieldID id = env->GetFieldID(Class, member_name.c_str(), "I");
        env->SetIntField(object, id, value);
    }

    void set_java_object_member_bool(JNIEnv *env, jclass Class, jobject object, string member_name,
                                     bool value) {
        jfieldID id = env->GetFieldID(Class, member_name.c_str(), "Z");
        jboolean jvalue = jboolean(value);
        env->SetBooleanField(object, id, jvalue);
    }

    /**
     * http://stackoverflow.com/questions/8415032/catching-exceptions-thrown-from-native-code-running-on-android
     */
    void throwJavaException(JNIEnv *env, const char *msg) {
        jclass c = env->FindClass("java/lang/RuntimeException");

        if (NULL == c) {
            //B plan: null pointer ...
            c = env->FindClass("java/lang/NullPointerException");
        }

        env->ThrowNew(c, msg);
    }

    void throwBlk360Exception(
            JNIEnv *env,
            const char *functionName,
            const char *file,
            int line,
            Blk360_Error error
    ) {
        jclass exClass = env->FindClass(
                "com/demo/scanner/devices/blk360/nativeBindingObjects/Blk360Exception");

        jmethodID constructor =
                env->GetMethodID(exClass, "<init>",
                                 "(ILjava/lang/String;)V");

        jobject exception = env->NewObject(
                exClass,
                constructor,
                error.code,
                env->NewStringUTF(error.message)
        );


        // Find the __jni_setLocation method and call it with
        // the function name, file and line parameters
        jmethodID setLocation =
                env->GetMethodID(exClass, "__jni_setLocation",
                                 "(Ljava/lang/String;"
                                 "Ljava/lang/String;"
                                 "I)V");

        env->CallVoidMethod(exception, setLocation,
                            env->NewStringUTF(functionName),
                            env->NewStringUTF(file),
                            line);

        // Throw the exception. Since this is native code,
        // execution continues, and the execution will be abruptly
        // interrupted at the point in time when we return to the VM.
        // The calling code will perform the early return back to Java code.
        env->Throw((jthrowable) exception);

        // Clean up local reference
        env->DeleteLocalRef(exClass);
    }

    jdoubleArray vector_to_jdoubleArray(JNIEnv *env, const vector<double> &vector) {
        jdoubleArray output = env->NewDoubleArray(vector.size());
        env->SetDoubleArrayRegion(output, 0, vector.size(), &vector[0]);
        return output;
    }
}
