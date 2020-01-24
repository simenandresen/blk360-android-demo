#pragma once

namespace demo {
namespace log {

    enum log_level {
        LOG_NONE = 0,
        LOG_ERROR = 1,
        LOG_WARNING = 2,
        LOG_INFO = 3,
        LOG_DEBUG = 4,
        LOG_VERBOSE = 5,
    };

    static const char* LOG_TAG = "demo-native";

    void print(log_level level, const char *format, ...);

} // namespace log
} // namespace demo
