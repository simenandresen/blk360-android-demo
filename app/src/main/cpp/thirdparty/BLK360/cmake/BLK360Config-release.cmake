#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BLK360" for configuration "Release"
set_property(TARGET BLK360 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(BLK360 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/BLK360/lib/${CMAKE_ANDROID_ARCH_ABI}/libBLK360.so"
  IMPORTED_SONAME_RELEASE "libBLK360.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS BLK360 )
list(APPEND _IMPORT_CHECK_FILES_FOR_BLK360 "${_IMPORT_PREFIX}/BLK360/lib/${CMAKE_ANDROID_ARCH_ABI}/libBLK360.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
