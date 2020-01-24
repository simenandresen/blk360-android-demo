#include "debuglog.hpp"

#ifdef __ANDROID__
#include <android/log.h>

void demo::log::print(log_level level, const char *format, ...) {


    android_LogPriority priority;

    switch (level){
        case LOG_NONE: priority = ANDROID_LOG_SILENT; break;
        case LOG_ERROR: priority = ANDROID_LOG_ERROR; break;
        case LOG_WARNING: priority = ANDROID_LOG_WARN; break;
        case LOG_INFO: priority = ANDROID_LOG_INFO; break;
        case LOG_DEBUG: priority = ANDROID_LOG_DEBUG; break;
        case LOG_VERBOSE: priority = ANDROID_LOG_VERBOSE; break;
        default: priority = ANDROID_LOG_DEFAULT;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);

    __android_log_vprint(priority, LOG_TAG, format, arg_ptr);

    va_end(arg_ptr);
}

#else

#include <cstdio>

void demo::log::print(log_level level, const char *format, ...) {

    // Todo: Print level
    va_list arg_ptr;
    va_start(arg_ptr, format);
    printf(format, arg_ptr);
    va_end(arg_ptr);
}

#endif
