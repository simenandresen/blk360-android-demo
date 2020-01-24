//
// Created by simen on 14.06.19.
//

#include <jni.h>
#include <iostream>
#include <string>
#include "blk360wrapper/Blk360Wrapper.hpp"
#include "debuglog.hpp"
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <sstream>



using namespace demo::log;


extern "C" {
JNIEXPORT jstring JNICALL
Java_com_demo_blk360demo_CreateScanAsync_createScan(JNIEnv *env, jobject thiz) {
    try {
        createScan();
    } catch (const Blk360_Error &error) {
        std::stringstream ss;
        ss << "Error with message: " << error.message << " and code: " << error.code << std::endl;
        return (env)->NewStringUTF(ss.str().c_str());
    } catch (const Blk360_ErrorEvent &event) {
        std::stringstream ss;
        ss << "Error with message: " << event.message << " and code: " << event.errorCode << std::endl;
        return (env)->NewStringUTF(ss.str().c_str());
    } catch (const std::exception &e) {
        std::string error = e.what();
        return (env)->NewStringUTF(e.what());
    }
    return (env)->NewStringUTF("Scanning complete");
}
}

