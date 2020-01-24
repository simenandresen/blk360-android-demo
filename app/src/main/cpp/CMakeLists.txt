cmake_minimum_required(VERSION 3.12)

project(blk360wrapper)

find_package(BLK360 CONFIG REQUIRED)

include_directories(
        demo/include
)

add_library(blk360wrapper-lib SHARED
        demo/src/native-binder.cpp
        demo/src/Blk360Wrapper.cpp
        demo/src/debuglog.cpp
        )

target_link_libraries(
        blk360wrapper-lib
        PRIVATE
        BLK360
        EGL
        m
        z
        android
        log
        GLESv3
)
