#ifndef BLK360_H
#define BLK360_H

#ifdef _WIN32
    #define BLK360_SHARED __declspec(dllexport)
#else
    #define BLK360_SHARED __attribute__ ((visibility ("default")))
#endif

#include <stdint.h>
#include <stddef.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#define BLK360_LIBRARY_VERSION 0x03020200

#if __cplusplus
extern "C" {
#endif

/****************** Type definitions ************************/

typedef uint64_t Blk360_Handle;
typedef uint64_t Blk360_seconds_t;
typedef uint64_t Blk360_milliseconds_t;
typedef uint64_t Blk360_microseconds_t;
typedef uint64_t Blk360_scanId_t;
typedef uint32_t Blk360_index_t;
typedef uint32_t Blk360_version_t;
typedef float Blk360_radians_t;
typedef uint32_t Blk360_percent_t;

/****************** Handles ************************/

BLK360_SHARED bool Blk360_Handle_IsNull(Blk360_Handle handle);
BLK360_SHARED Blk360_Handle Blk360_Handle_Null();

typedef struct { Blk360_Handle handle; } Blk360_SubscriptionHandle;
typedef struct { Blk360_Handle handle; } Blk360_EventQueueHandle;
typedef struct { Blk360_Handle handle; } Blk360_SessionHandle;
typedef struct { Blk360_Handle handle; } Blk360_ProcessingWorkflowHandle;
typedef struct { Blk360_Handle handle; } Blk360_DataManipulationWorkflowHandle;
typedef struct { Blk360_Handle handle; } Blk360_ImageHandle;
typedef struct { Blk360_Handle handle; } Blk360_IrImageHandle;
typedef struct { Blk360_Handle handle; } Blk360_PointCloudHandle;
typedef struct { Blk360_Handle handle; } Blk360_ProcessedImageHandle;
typedef struct { Blk360_Handle handle; } Blk360_SystemWorkflowHandle;
typedef struct { Blk360_Handle handle; } Blk360_MeasurementHandle;
typedef struct { Blk360_Handle handle; } Blk360_MeasurementWorkflowHandle;
typedef struct { Blk360_Handle handle; } Blk360_SceneParametersHandle;
typedef struct { Blk360_Handle handle; } Blk360_MeasurementEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_ImageEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_IrImageEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_WifiNameEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_AvailableWifiNetworkEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_PointCloudChunkEnumeratorHandle;
typedef struct { Blk360_Handle handle; } Blk360_PointCloudChunkHandle;
typedef struct { Blk360_Handle handle; } Blk360_StitchedPanoramaHandle;

/****************** Data objects ************************/
typedef struct
{
    uint32_t abi;
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} Blk360_Version;

typedef struct {
    Blk360_milliseconds_t callTimeout;
    Blk360_milliseconds_t sessionTimeout;
    char clientName[127 + 1];
    bool forceConnection;
} Blk360_SessionParameters;

typedef enum {
    Blk360_ConnectionStatus_OK = 0,
    Blk360_ConnectionStatus_Lost = 1,
    Blk360_ConnectionStatus_ForceDisconnected = 2,
    Blk360_ConnectionStatus_Disconnected = 3
} Blk360_ConnectionStatus;

typedef struct
{
    char name[127 + 1];
    char serialNumber[31 + 1];
    char articleNumber[31 + 1];
    char hwVersion[31 + 1];
    char fwVersion[31 + 1];
} Blk360_DeviceInformation;

typedef enum {
    Blk360_DeviceStatus_Invalid = 0,
    Blk360_DeviceStatus_Idle = 1,
    Blk360_DeviceStatus_Busy = 2,
    Blk360_DeviceStatus_Error = 3,
    Blk360_DeviceStatus_FatalError = 4,
    Blk360_DeviceStatus_ShuttingDown = 5
} Blk360_DeviceStatus;

typedef enum {
    Blk360_CurrentOperation_None = 0,
    Blk360_CurrentOperation_Standalone = 1,
    Blk360_CurrentOperation_PanoramaShoot = 2,
    Blk360_CurrentOperation_PointCloudScan = 3,
    Blk360_CurrentOperation_FormattingStorage = 4
} Blk360_CurrentOperation;

typedef enum {
    Blk360_DetailedError_None = 0,
    Blk360_DetailedError_Low_Battery = 1,
    Blk360_DetailedError_Internal = 2,
    Blk360_DetailedError_Storage_Exceeded = 3,
    Blk360_DetailedError_Device_Moved = 4,
    Blk360_DetailedError_Device_Blocked = 5,
    Blk360_DetailedError_Storage_Corrupted = 6,
    Blk360_DetailedError_SWUpdate_Failed = 7,
    Blk360_DetailedError_Communication = 8,
    Blk360_DetailedError_Function = 9,
    Blk360_DetailedError_Other = 10,
    Blk360_DetailedError_Fatal_Calibration = 11,
    Blk360_DetailedError_Fatal_HW = 12,
    Blk360_DetailedError_Fatal_Other = 13,
    Blk360_DetailedError_Temp_Low = 14,
    Blk360_DetailedError_Temp_High = 15,
    Blk360_DetailedError_Temp_Critical = 16,
} Blk360_DetailedError;

typedef struct
{
    Blk360_DetailedError error;
    char description[255 + 1];
} Blk360_StatusDetails;

typedef struct {
    Blk360_scanId_t currentOperationScanId;
    Blk360_DeviceStatus status;
    Blk360_CurrentOperation currentOperation;
    Blk360_StatusDetails statusDetails;
} Blk360_DeviceState;

typedef enum
{
    Blk360_PanoramaMode_HDR = 0,
    Blk360_PanoramaMode_LDR = 1
} Blk360_PanoramaMode;

typedef enum
{
    Blk360_ExposureCorrection_None = 0,
    Blk360_ExposureCorrection_MinusFive = 1,
    Blk360_ExposureCorrection_MinusFour = 2,
    Blk360_ExposureCorrection_MinusThree = 3,
    Blk360_ExposureCorrection_MinusTwo = 4,
    Blk360_ExposureCorrection_MinusOne = 5,
    Blk360_ExposureCorrection_PlusOne = 6,
    Blk360_ExposureCorrection_PlusTwo = 7,
    Blk360_ExposureCorrection_PlusThree = 8,
    Blk360_ExposureCorrection_PlusFour = 9,
    Blk360_ExposureCorrection_PlusFive = 10,
} Blk360_ExposureCorrection;

typedef enum
{
    Blk360_PointCloudDensity_Medium = 1,
    Blk360_PointCloudDensity_Low = 2,
    Blk360_PointCloudDensity_High = 3,
} Blk360_PointCloudDensity;

typedef struct
{
    uint32_t numberOfExposures;
    float exposureFactor;
} Blk360_HDRParameters;

typedef enum
{
    FlashMode_Auto = 0,
    FlashMode_On = 1,
    FlashMode_Off = 2
} Blk360_FlashMode;

typedef struct
{
    Blk360_FlashMode flashMode;
} Blk360_LDRParameters;

typedef struct
{
    Blk360_microseconds_t exposure;
    Blk360_PanoramaMode panoramaMode;
    Blk360_ExposureCorrection exposureCorrection;
    Blk360_HDRParameters hdrParameters;
    Blk360_LDRParameters ldrParameters;
    bool enableAutoExposureRun;
} Blk360_PanoramaParameters;

typedef struct
{
    float clippingValue; // the value can be in range [0.0; 5.0f]
} Blk360_ProcessingWorkflowConfiguration;

typedef struct
{
    Blk360_PointCloudDensity density;
} Blk360_PointCloudParameters;

typedef enum
{
    Blk360_IrGainMode_Invalid = 0,
    Blk360_IrGainMode_High = 1,
    Blk360_IrGainMode_Low = 2,
    Blk360_IrGainMode_Auto = 3,
} Blk360_IrGainMode;

typedef struct
{
    Blk360_IrGainMode mode;
    float sceneEmissivity;
} Blk360_IrParameters;

typedef enum
{
    Blk360_StandbyMode_Enabled = 1,
    Blk360_StandbyMode_Disabled = 2,
    Blk360_StandbyMode_Default = 3
} Blk360_StandbyMode;

typedef struct
{
    Blk360_seconds_t timeout;
    Blk360_StandbyMode mode;
} Blk360_StandbyConfig;

typedef struct
{
    Blk360_StandbyConfig standby;
    Blk360_IrParameters irImgSettings;
    bool enablePositionTracking;
} Blk360_DeviceConfig;

typedef struct
{
    Blk360_scanId_t scanId;
    Blk360_PanoramaParameters panoramaParameters;
    Blk360_PointCloudParameters pointCloudParameters;
    Blk360_IrParameters irParameters;
    bool enablePanorama;
    bool enablePointCloud;
    bool enableIr;
} Blk360_MeasurementParameters;

typedef enum
{
    Blk360_MeasurementDataStatus_Complete = 0,
    Blk360_MeasurementDataStatus_Partial = 1,
    Blk360_MeasurementDataStatus_InProgress = 2,
    Blk360_MeasurementDataStatus_WriteProtected = 3,
    Blk360_MeasurementDataStatus_Corrupted = 4
} Blk360_MeasurementDataStatus;

typedef struct
{
    Blk360_seconds_t timestamp;
    uint32_t scanlineCount;
    uint32_t scanlineSize;
    Blk360_radians_t hAngleBegin;
    Blk360_radians_t hAngleEnd;
    Blk360_radians_t vAngleBegin;
    Blk360_radians_t vAngleEnd;
    Blk360_MeasurementDataStatus status;
} Blk360_PointCloudProperties;

typedef struct
{
    float red;
    float green;
    float blue;
} Blk360_WhitebalanceGain;

typedef struct
{
    uint32_t width;
    uint32_t height;
} Blk360_ImageSize;

typedef struct
{
    Blk360_ImageSize panoramaSize;
} Blk360_StitchedPanoramaProperties;

typedef enum
{
    Blk360_Dimension_Width,
    Blk360_Dimension_Height
} Blk360_Dimension;

typedef struct
{
    Blk360_Dimension dimension;
    uint32_t value;
} Blk360_StitchedPanoramaSizeConfiguration;

typedef struct
{
    Blk360_seconds_t timestamp;
    Blk360_WhitebalanceGain whitebalanceGain;
    uint32_t width;
    uint32_t height;
    uint32_t imageCount;
    Blk360_MeasurementDataStatus status;
} Blk360_PanoramaProperties;

typedef struct
{
    // Positive values are counterclockwise.
    Blk360_radians_t tiltX;
    Blk360_radians_t tiltY;
} Blk360_InertialData;

typedef struct
{
    float min;
    float max;
} Blk360_IrTemperature;

typedef struct
{
    Blk360_seconds_t timestamp;
    Blk360_IrTemperature temperature;
    uint32_t width;
    uint32_t height;
    uint32_t imageCount;
    Blk360_MeasurementDataStatus status;
} Blk360_IrProperties;

typedef struct {
    Blk360_scanId_t scanId;
    Blk360_PanoramaParameters panoramaParameters;
    Blk360_PanoramaProperties panoramaProperties;
    Blk360_IrProperties irProperties;
    Blk360_PointCloudProperties pointCloudProperties;
    Blk360_PointCloudParameters pointCloudParameters;
    bool hasPanorama;
    bool hasIr;
    bool hasPointCloud;
    bool hasInertial;
} Blk360_Measurement;

typedef struct {
    Blk360_scanId_t scanId;
    bool deletePanorama;
    bool deleteIr;
    bool deletePointCloud;
    bool deleteInertial;
} Blk360_MeasurementDelete;

typedef struct
{
    Blk360_seconds_t selfTimer;
    Blk360_MeasurementParameters measurementParameters;
} Blk360_ButtonPressConfig;

typedef struct {
    uint64_t capacity;
    uint64_t used;
} Blk360_DeviceStorage;

typedef struct {
    float red;
    float green;
    float blue;
} Blk360_AutoWhiteBalanceGain;

typedef struct {
    Blk360_microseconds_t exposure;
    Blk360_AutoWhiteBalanceGain wbGain;
    bool withFlash;
} Blk360_SceneParameters;

typedef struct {
    float Ar;
    float Ag;
    float Ab;
    float Br;
    float Bg;
    float Bb;
    float Cr;
    float Cg;
    float Cb;
    float Dr;
    float Dg;
    float Db;
    float Er;
    float Eg;
    float Eb;
    float N;
} Blk360_LinearizationPolynomial;

typedef struct {
    float k1;
    float k2;
    float p1;
    float p2;
    float k3;
    float kd1;
    float kd2;
    float kd3;
    float p3;
    float p4;
} Blk360_LensGeometry;

typedef struct
{
    // Column major arrangement
    double elements[4 * 4];
} Blk360_Matrix4;

typedef struct {
    // 4x4 camera transformation matrix(16 values, rotation + translation).
    // Column - major; X points to the right, Y up and Z backward; translation is
    // in meters. Implicit value(if not calibrated) should be an identity
    // transformation.
    Blk360_Matrix4 transformation;

    // 4x4 camera projection matrix(16 values).Column - major; focal length is
    // relative to image plane of size 2x2 units, principal point offset is
    // relative to center with Y pointing up(flip sign if going from OpenCV).
    // Implicit value(if not calibrated) should be a meaningful projection.
    Blk360_Matrix4 projection;

    // 4x4 color correction matrix(16 values).Column - major, top - left
    // 3x3 part is color correction, rightmost column black level shift.
    //  C0 C3 C6 Sr
    //  C1 C4 C7 Sg
    //  C2 C5 C8 Sb
    //  0  0  0  1
    // If not calibrated, the array is empty.In that case an identity
    // correction will be applied.
    // Same values for all three cameras.
    Blk360_Matrix4 color;

    // Logarithmic polynomial for linearization of each channel (16 values):
    // Ar Ag Ab Br Bg Bb Cr Cg Cb Dr Dg Db Er Eg Eb N
    // If not calibrated, the array is empty. In that case an identity
    // correction will be applied.
    // Same values for all three cameras.
    Blk360_LinearizationPolynomial linearization;

    // Vignetting correction grid (6 + W*H*3 values). First six values define
    // the grid (number of points in X and Y, distance between points, offset
    // of the grid center relative to image center, with distance and offset
    // values relative to image size). Then follow W*H values for each channel,
    // ordered bottom up, left to right. Applied to RAW float values by
    // multiplying given pixel with interpolated value for corresponding
    // channel.
    //
    //  W H Dx Dy Cx Cy Tr1 Tg1 Tb1 Tr2 Tg2 Tb2 ...
    //
    // As things stand, W and H are assumed to not exceed 64 and 48.
    // These are ints, cast to floats.
    // If not calibrated, the array is empty. In that case an identity
    // correction will be applied.
    float vignetting[6 + (64 * 48 * 3)];

    // Camera lens geometry calibration (10 values). Y pointing up, in order:
    // k1 k2 p1 p2 k3 kd1 kd2 kd3 p3 p4
    // If not calibrated, the array is empty. In that case an identity
    // correction will be applied.
    // In essence, this is a barrel distortion, which here of course
    // amounts to an un - distortion.For more info, please search for
    // decomposeProjectionMatrix in the OpenCV documentation.At some
    // point after this step, the image has to be cropped, e.g.by 64
    // pixels on all four sides.
    Blk360_LensGeometry geometry;

    // Factors with which to multiply the R, G and B gains found by the
    // auto - white - balance algorithm.
    // Default is 1.0 for all three.
    // Same values for all three cameras.
    Blk360_AutoWhiteBalanceGain wbFactors;
}  Blk360_CameraCalibration;

typedef struct {
    // Camera transformation matrix. The matrix is calculated with OpenGL
    // coordinate system in mind (image on the XY plane, camera pointing down
    // the negative Z axis).
    Blk360_Matrix4 transformation;

    // Camera projection matrix. The matrix is calculated with OpenGL
    // coordinate system in mind, with viewed volume being projected into a
    // [ -1.0; 1.0 ] range in all directions.
    Blk360_Matrix4 projection;

    // Actual image width (pixels) taking downscale into account.
    // The value is guaranteed to be the same for all images in
    // the same session processed with the same format
    // and downscale parameters. If downscale is set to `0`,
    // this value is equal to width.
    uint32_t width;

    // Actual image height (pixels) taking downscale
    // into account. The value is guaranteed to be the same for all images in
    // the same session processed with the same format
    // and downscale parameters. If downscale is set to `0`, this value is equal to height.
    uint32_t height;

    // Exposure time of the color image. This is the actually used
    // exposure time, which will differ from exposure
    // in case of auto-exposure, and in case of HDR. Since this is the actually
    // used exposure time, this has, in case of auto-exposure, also taken
    // exposure_correction into account. When using HDR, use this to identify the dark image,
    // the normal image and the light image, and as input in the HDR merging algorithm.
    Blk360_microseconds_t exposure;
} Blk360_ImageProperties;

typedef struct
{
    Blk360_ImageProperties imageProperties;
    Blk360_IrTemperature temperature;
} Blk360_IrImageProperties;

typedef struct
{
    //A string that is at most 31 chars long, not counting the terminating zero.
    char ssid[31 + 1];
} Blk360_WifiNetworkName;

typedef struct
{
    // Either a string of between 8 and 63 printable US
    // ASCII chars, not counting the terminating zero,
    // or a string of exactly 64 chars, where each char
    // matches / [0 - 9][A - F][a - f] / (written out HEX). Strings
    // with invalid length or content are rejected.
    // If empty password is given, it's supposed that no
    // password should be set.
    char wpa2Password[63 + 1];
} Blk360_WifiNetworkPassword;

typedef struct
{
    char dns[16][64];
    char ipV6[46];
    char ipV4[16];
    char netmaskV4[16];
    char gatewayV4[16];
} Blk360_WifiNetworkSettings;

typedef struct
{
    Blk360_WifiNetworkName name;
    Blk360_WifiNetworkPassword password;
    Blk360_WifiNetworkSettings settings;
} Blk360_WifiNetwork;

typedef struct
{
    Blk360_seconds_t leaseTime;
    int32_t signal;
    Blk360_WifiNetworkName name;
    Blk360_WifiNetworkSettings settings;
} Blk360_WifiNetworkConfiguration;

typedef struct
{
    Blk360_WifiNetworkName name;
    int32_t signal;
} Blk360_AvailableWifiNetwork;

/****************** ErrorHandling ************************/
typedef enum
{
    Blk360_Error_Ok = 0,
    Blk360_Error_InvalidInstanceHandle = 1,      // The given API Handle does not represent a valid object
    Blk360_Error_InvalidOperation = 2,           // The requested operation is not appropriate/allowed at the time when it was called
    Blk360_Error_InvalidArgument = 3,            // Input argument for a function/method is not acceptable
    Blk360_Error_ConnectionError = 4,            // RPC connection has broken
    Blk360_Error_SessionError = 5,               // Problem with the RPC session: the requested session was already released by the server
    Blk360_Error_InternalError = 6,              // Generic internal error
    Blk360_Error_ApiNotInitialized = 7,          // The API is not initialized
    Blk360_Error_DeviceError = 8,                // Error on device side; might include communication errors
    Blk360_Error_IncompatibleVersion = 9,        // Header API and compiled library versions are incompatible
} Blk360_ErrorCode;

typedef struct
{
    Blk360_ErrorCode code;
    char message[251 + 1];
} Blk360_Error;

BLK360_SHARED Blk360_Error Blk360_Api_GetLastError();
BLK360_SHARED void Blk360_Api_New(Blk360_version_t version);
BLK360_SHARED void Blk360_Api_Release();

BLK360_SHARED Blk360_Version Blk360_Api_GetLibraryVersion();
BLK360_SHARED Blk360_Version Blk360_Api_DecodeVersion(Blk360_version_t version);

/****************** Events ************************/
typedef enum
{
    Blk360_OperationStatus_Error = -1,
    Blk360_OperationStatus_Success = 0,
    Blk360_OperationStatus_Started = 1,
} Blk360_OperationStatus;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    char payload[248];
} Blk360_Event;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_scanId_t scanId;
    uint64_t scanningDone; // acquired point cloud data size
    uint64_t scanningTotal; // full point cloud data size; if scan is during acquisition value is estimated
    uint32_t imagesDone; // finished non-IR images count
    uint32_t imagesTotal; // total count of non-IR images
    uint32_t irImagesDone; // finished IR images count
    uint32_t irImagesTotal; // total count of IR images
} Blk360_ProgressEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_scanId_t scanId;
    Blk360_index_t index;
    Blk360_percent_t progress;
} Blk360_ImageDownloadProgressEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_OperationStatus status;
} Blk360_OperationStatusEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_ErrorCode errorCode;
    char message[244];
} Blk360_ErrorEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_scanId_t scanId;

    Blk360_index_t chunkIndex;

    Blk360_PointCloudChunkHandle handle;
} Blk360_PointCloudChunkAvailableEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_scanId_t scanId;

    uint64_t estimatedTotalSize;
    Blk360_percent_t progress;
} Blk360_PointCloudProcessProgressEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_ConnectionStatus status;
} Blk360_ConnectionStateChangedEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    char message[248];
} Blk360_DeviceDiscoveryErrorEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    char name[127 + 1];
    char serialNumber[31 + 1];
    char IPv4[16];
    char IPv6[46];
    uint16_t port;
    bool isAPICompatible;
} Blk360_DiscoveredDeviceEvent;

typedef struct
{
    Blk360_SubscriptionHandle sender;
    Blk360_percent_t progress;
} Blk360_PanoramaStitchingProgressEvent;

BLK360_SHARED Blk360_EventQueueHandle Blk360_EventQueue_New(size_t capacity);
BLK360_SHARED void Blk360_EventQueue_Release(Blk360_EventQueueHandle instance);
BLK360_SHARED Blk360_Event Blk360_EventQueue_Pop(Blk360_EventQueueHandle instance);
BLK360_SHARED void Blk360_EventQueue_Clear(Blk360_EventQueueHandle instance);
BLK360_SHARED bool Blk360_EventQueue_IsEmpty(Blk360_EventQueueHandle instance);
BLK360_SHARED bool Blk360_EventQueue_HasOverflown(Blk360_EventQueueHandle instance);
BLK360_SHARED bool Blk360_EventQueue_Wait(Blk360_EventQueueHandle instance, Blk360_milliseconds_t timeout);

/****************** Enumerators ************************/
BLK360_SHARED void Blk360_MeasurementEnumerator_Release(Blk360_MeasurementEnumeratorHandle instance);
BLK360_SHARED bool Blk360_MeasurementEnumerator_MoveNext(Blk360_MeasurementEnumeratorHandle instance);
BLK360_SHARED Blk360_MeasurementHandle Blk360_MeasurementEnumerator_GetCurrent(Blk360_MeasurementEnumeratorHandle instance);

BLK360_SHARED bool Blk360_ImageEnumerator_MoveNext(Blk360_ImageEnumeratorHandle instance);
BLK360_SHARED Blk360_ImageHandle Blk360_ImageEnumerator_GetCurrent(Blk360_ImageEnumeratorHandle instance);

BLK360_SHARED bool Blk360_IrImageEnumerator_MoveNext(Blk360_IrImageEnumeratorHandle instance);
BLK360_SHARED Blk360_IrImageHandle Blk360_IrImageEnumerator_GetCurrent(Blk360_IrImageEnumeratorHandle instance);

BLK360_SHARED void Blk360_WifiNameEnumerator_Release(Blk360_WifiNameEnumeratorHandle instance);
BLK360_SHARED bool Blk360_WifiNameEnumerator_MoveNext(Blk360_WifiNameEnumeratorHandle instance);
BLK360_SHARED Blk360_WifiNetworkName Blk360_WifiNameEnumerator_GetCurrent(Blk360_WifiNameEnumeratorHandle instance);

BLK360_SHARED void Blk360_AvailableWifiNetworkEnumerator_Release(Blk360_AvailableWifiNetworkEnumeratorHandle instance);
BLK360_SHARED bool Blk360_AvailableWifiNetworkEnumerator_MoveNext(Blk360_AvailableWifiNetworkEnumeratorHandle instance);
BLK360_SHARED Blk360_AvailableWifiNetwork Blk360_AvailableWifiNetworkEnumerator_GetCurrent(Blk360_AvailableWifiNetworkEnumeratorHandle instance);

BLK360_SHARED bool Blk360_PointCloudChunkEnumerator_MoveNext(Blk360_PointCloudChunkEnumeratorHandle instance);
BLK360_SHARED Blk360_PointCloudChunkHandle Blk360_PointCloudChunkEnumerator_GetCurrent(Blk360_PointCloudChunkEnumeratorHandle instance);

/****************** Accessors ************************/
BLK360_SHARED Blk360_Measurement Blk360_Measurement_GetInfo(Blk360_MeasurementHandle instance);
BLK360_SHARED Blk360_InertialData Blk360_Measurement_GetInertial(Blk360_MeasurementHandle measurement);
BLK360_SHARED bool Blk360_Measurement_IsPanoramaDownloaded(Blk360_MeasurementHandle measurement);
BLK360_SHARED bool Blk360_Measurement_IsIrPanoramaDownloaded(Blk360_MeasurementHandle measurement);
BLK360_SHARED Blk360_ImageEnumeratorHandle Blk360_Measurement_GetPanorama(Blk360_MeasurementHandle measurement);
BLK360_SHARED Blk360_IrImageEnumeratorHandle Blk360_Measurement_GetIrPanorama(Blk360_MeasurementHandle measurement);
BLK360_SHARED Blk360_PointCloudHandle Blk360_Measurement_GetPointCloud(Blk360_MeasurementHandle measurement);
BLK360_SHARED Blk360_ImageHandle Blk360_Measurement_GetImageAtIndex(Blk360_MeasurementHandle measurement, Blk360_index_t index);
BLK360_SHARED void Blk360_Measurement_Release(Blk360_MeasurementHandle measurement);

BLK360_SHARED bool Blk360_Image_IsDownloaded(Blk360_ImageHandle image);
BLK360_SHARED bool Blk360_Image_IsProcessed(Blk360_ImageHandle image);
BLK360_SHARED Blk360_ImageProperties Blk360_Image_GetProperties(Blk360_ImageHandle image);
BLK360_SHARED uint64_t Blk360_Image_GetDataSizeInBytes(Blk360_ImageHandle image);
BLK360_SHARED const char* Blk360_Image_GetData(Blk360_ImageHandle image);

BLK360_SHARED bool Blk360_IrImage_IsDownloaded(Blk360_IrImageHandle irImage);
BLK360_SHARED bool Blk360_IrImage_IsProcessed(Blk360_IrImageHandle irImage);
BLK360_SHARED Blk360_IrImageProperties Blk360_IrImage_GetProperties(Blk360_IrImageHandle irImage);
BLK360_SHARED uint64_t Blk360_IrImage_GetDataSizeInBytes(Blk360_IrImageHandle irImage);
BLK360_SHARED const char* Blk360_IrImage_GetData(Blk360_IrImageHandle irImage);

BLK360_SHARED bool Blk360_PointCloud_IsDownloaded(Blk360_PointCloudHandle handle);
BLK360_SHARED bool Blk360_PointCloud_IsProcessed(Blk360_PointCloudHandle handle);
BLK360_SHARED Blk360_PointCloudChunkEnumeratorHandle Blk360_PointCloud_GetChunks(Blk360_PointCloudHandle handle);

BLK360_SHARED const char* Blk360_PointCloudChunk_GetData(Blk360_PointCloudChunkHandle handle);
BLK360_SHARED uint64_t Blk360_PointCloudChunk_GetDataSizeInBytes(Blk360_PointCloudChunkHandle handle);

BLK360_SHARED Blk360_SceneParameters Blk360_SceneParameters_GetData(Blk360_SceneParametersHandle instance);
BLK360_SHARED bool Blk360_SceneParameters_IsDetectionFinished(Blk360_SceneParametersHandle instance);
BLK360_SHARED void Blk360_SceneParameters_Release(Blk360_SceneParametersHandle instance);

/****************** DeviceDiscovery ************************/
BLK360_SHARED Blk360_SubscriptionHandle Blk360_DeviceDiscovery_OnError(Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_DeviceDiscovery_Start(Blk360_EventQueueHandle queue);
BLK360_SHARED void Blk360_DeviceDiscovery_Stop();

/****************** Session ************************/
BLK360_SHARED Blk360_SessionParameters Blk360_SessionParameters_New();

BLK360_SHARED Blk360_SessionHandle Blk360_Session_New_Default(const char* serverAddress);
BLK360_SHARED Blk360_SessionHandle Blk360_Session_New(const char* serverAddress, Blk360_SessionParameters sessionParameters);
BLK360_SHARED void Blk360_Session_Release(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_Session_Reconnect(Blk360_SessionHandle instance);
BLK360_SHARED Blk360_SessionParameters Blk360_Session_GetParameters(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_Session_SetParameters(Blk360_SessionHandle instance, Blk360_SessionParameters sessionParameters);

BLK360_SHARED bool Blk360_Session_IsConnected(Blk360_SessionHandle instance);
BLK360_SHARED bool Blk360_Session_IsConnectionLost(Blk360_SessionHandle instance);
BLK360_SHARED Blk360_ConnectionStatus Blk360_Session_GetConnectionStatus(Blk360_SessionHandle instance);

BLK360_SHARED Blk360_SubscriptionHandle Blk360_Session_OnConnectionStateChanged(Blk360_SessionHandle instance, Blk360_EventQueueHandle queue);
/****************** SystemWorkflow ************************/
BLK360_SHARED Blk360_SystemWorkflowHandle Blk360_SystemWorkflow_Create(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_Release(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED Blk360_DeviceInformation Blk360_SystemWorkflow_GetDeviceInformation(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED Blk360_DeviceState Blk360_SystemWorkflow_GetDeviceState(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_ClearErrorState(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED Blk360_seconds_t Blk360_SystemWorkflow_GetDeviceTime(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_SetDeviceTime(Blk360_SystemWorkflowHandle instance, Blk360_seconds_t seconds);

BLK360_SHARED Blk360_ButtonPressConfig Blk360_ButtonPressConfig_GetDefault();
BLK360_SHARED Blk360_ButtonPressConfig Blk360_SystemWorkflow_GetButtonPressConfig(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_SetButtonPressConfig(Blk360_SystemWorkflowHandle instance, Blk360_ButtonPressConfig config);
BLK360_SHARED void Blk360_SystemWorkflow_ResetButtonPressConfig(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED Blk360_DeviceConfig Blk360_SystemWorkflow_GetDeviceConfig(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_SetDeviceConfig(Blk360_SystemWorkflowHandle instance, Blk360_DeviceConfig settings);
BLK360_SHARED void Blk360_SystemWorkflow_ResetDeviceConfig(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED Blk360_percent_t Blk360_SystemWorkflow_GetDeviceBatteryLevel(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED Blk360_DeviceStorage Blk360_SystemWorkflow_GetStorageUsage(Blk360_SystemWorkflowHandle instance);

BLK360_SHARED bool Blk360_SystemWorkflow_Wifi_IsClientModeOn(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED void Blk360_SystemWorkflow_Wifi_SetClientMode(Blk360_SystemWorkflowHandle instance, bool enabled);
BLK360_SHARED void Blk360_SystemWorkflow_Wifi_AddNetwork(Blk360_SystemWorkflowHandle instance, Blk360_WifiNetworkName networkName, Blk360_WifiNetworkPassword networkPassword);
BLK360_SHARED void Blk360_SystemWorkflow_Wifi_RemoveNetwork(Blk360_SystemWorkflowHandle instance, Blk360_WifiNetworkName networkName);
BLK360_SHARED void Blk360_SystemWorkflow_Wifi_SetNetworkConfiguration(Blk360_SystemWorkflowHandle instance, Blk360_WifiNetwork network);
BLK360_SHARED void Blk360_SystemWorkflow_Wifi_Connect(Blk360_SystemWorkflowHandle instance, Blk360_WifiNetworkName networkName);
BLK360_SHARED Blk360_WifiNetworkConfiguration Blk360_SystemWorkflow_Wifi_GetConnectedNetwork(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED Blk360_WifiNameEnumeratorHandle Blk360_SystemWorkflow_Wifi_GetConfiguredNetworks(Blk360_SystemWorkflowHandle instance);
BLK360_SHARED Blk360_AvailableWifiNetworkEnumeratorHandle Blk360_SystemWorkflow_Wifi_GetAvailableNetworks(Blk360_SystemWorkflowHandle instance);

/****************** MeasurementWorkflow ************************/
BLK360_SHARED Blk360_MeasurementWorkflowHandle Blk360_MeasurementWorkflow_Create(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_MeasurementWorkflow_Release(Blk360_MeasurementWorkflowHandle instance);

BLK360_SHARED Blk360_MeasurementParameters Blk360_MeasurementParameters_New();
BLK360_SHARED Blk360_scanId_t Blk360_MeasurementWorkflow_Start(Blk360_MeasurementWorkflowHandle instance, Blk360_MeasurementParameters parameters);
BLK360_SHARED void Blk360_MeasurementWorkflow_Cancel(Blk360_MeasurementWorkflowHandle instance);
BLK360_SHARED Blk360_scanId_t Blk360_MeasurementWorkflow_GetActiveJob(Blk360_MeasurementWorkflowHandle instance);
BLK360_SHARED Blk360_SceneParametersHandle Blk360_MeasurementWorkflow_RunSceneParametersDetection(Blk360_MeasurementWorkflowHandle instance, Blk360_FlashMode flashMode);

BLK360_SHARED Blk360_SubscriptionHandle Blk360_MeasurementWorkflow_OnProgress(Blk360_MeasurementWorkflowHandle instance, Blk360_EventQueueHandle eventQueue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_MeasurementWorkflow_OnError(Blk360_MeasurementWorkflowHandle instance, Blk360_EventQueueHandle eventQueue);

/****************** DataManipulationWorkflow ************************/
BLK360_SHARED Blk360_DataManipulationWorkflowHandle Blk360_DataManipulationWorkflow_Create(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_DataManipulationWorkflow_Release(Blk360_DataManipulationWorkflowHandle instance);
BLK360_SHARED void Blk360_DataManipulationWorkflow_Cancel(Blk360_DataManipulationWorkflowHandle instance);

BLK360_SHARED void Blk360_DataManipulationWorkflow_DownloadPointCloud(Blk360_DataManipulationWorkflowHandle instance, Blk360_PointCloudHandle pointCloud);
BLK360_SHARED void Blk360_DataManipulationWorkflow_DownloadImage(Blk360_DataManipulationWorkflowHandle instance, Blk360_ImageHandle image);
BLK360_SHARED void Blk360_DataManipulationWorkflow_DownloadIrImage(Blk360_DataManipulationWorkflowHandle instance, Blk360_IrImageHandle image);
BLK360_SHARED void Blk360_DataManipulationWorkflow_DownloadPanorama(Blk360_DataManipulationWorkflowHandle instance, Blk360_MeasurementHandle measurement);
BLK360_SHARED void Blk360_DataManipulationWorkflow_DownloadIrPanorama(Blk360_DataManipulationWorkflowHandle instance, Blk360_MeasurementHandle measurement);

BLK360_SHARED Blk360_MeasurementHandle Blk360_DataManipulationWorkflow_GetMeasurementByScanId(Blk360_DataManipulationWorkflowHandle instance, Blk360_scanId_t scanId);
BLK360_SHARED Blk360_MeasurementEnumeratorHandle Blk360_DataManipulationWorkflow_GetMeasurements(Blk360_DataManipulationWorkflowHandle instance);

BLK360_SHARED Blk360_CameraCalibration Blk360_DataManipulationWorkflow_GetCameraCalibration(Blk360_ImageHandle image);

BLK360_SHARED Blk360_MeasurementDelete Blk360_MeasurementDelete_GetWholeMeasurement(Blk360_scanId_t scanId);
BLK360_SHARED Blk360_MeasurementDelete Blk360_MeasurementDelete_GetDefault(Blk360_scanId_t scanId);
BLK360_SHARED void Blk360_DataManipulationWorkflow_DeleteMeasurement(Blk360_DataManipulationWorkflowHandle instance, Blk360_MeasurementDelete measurementDelete);
BLK360_SHARED void Blk360_DataManipulationWorkflow_FormatStorage(Blk360_DataManipulationWorkflowHandle instance);

BLK360_SHARED Blk360_SubscriptionHandle Blk360_DataManipulationWorkflow_OnProgress(Blk360_DataManipulationWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_DataManipulationWorkflow_OnImageDownloadProgress(Blk360_DataManipulationWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_DataManipulationWorkflow_OnError(Blk360_DataManipulationWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_DataManipulationWorkflow_OnFormattingStateChanged(Blk360_DataManipulationWorkflowHandle instance, Blk360_EventQueueHandle queue);

/****************** StitchedPanorama ************************/
BLK360_SHARED Blk360_StitchedPanoramaHandle Blk360_StitchedPanorama_New(Blk360_StitchedPanoramaSizeConfiguration size);
BLK360_SHARED uint64_t Blk360_StitchedPanorama_GetDataSizeInBytes(Blk360_StitchedPanoramaHandle instance);
BLK360_SHARED const char* Blk360_StitchedPanorama_GetData(Blk360_StitchedPanoramaHandle instance);
BLK360_SHARED Blk360_StitchedPanoramaProperties Blk360_StitchedPanorama_Properties(Blk360_StitchedPanoramaHandle instance);
BLK360_SHARED Blk360_ImageSize Blk360_StitchedPanorama_CalculateSize(Blk360_StitchedPanoramaSizeConfiguration config);
BLK360_SHARED bool Blk360_StitchedPanorama_IsReady(Blk360_StitchedPanoramaHandle instance);
BLK360_SHARED void Blk360_StitchedPanorama_Release(Blk360_StitchedPanoramaHandle instance);

/****************** ProcessingWorkflow ************************/
BLK360_SHARED Blk360_ProcessingWorkflowHandle Blk360_ProcessingWorkflow_Create(Blk360_SessionHandle instance);
BLK360_SHARED void Blk360_ProcessingWorkflow_Release(Blk360_ProcessingWorkflowHandle instance);
BLK360_SHARED void Blk360_ProcessingWorkflow_Cancel(Blk360_ProcessingWorkflowHandle instance);
BLK360_SHARED void Blk360_ProcessingWorkflow_Configure(Blk360_ProcessingWorkflowHandle instance, Blk360_ProcessingWorkflowConfiguration config);
BLK360_SHARED Blk360_ProcessingWorkflowConfiguration Blk360_ProcessingWorkflow_GetConfiguration(Blk360_ProcessingWorkflowHandle instance);
BLK360_SHARED Blk360_ProcessingWorkflowConfiguration Blk360_ProcessingWorkflowConfiguration_GetDefault();
BLK360_SHARED void Blk360_ProcessingWorkflow_ProcessImage(Blk360_ProcessingWorkflowHandle instance, Blk360_ImageHandle image);
BLK360_SHARED void Blk360_ProcessingWorkflow_ProcessIrImage(Blk360_ProcessingWorkflowHandle instance, Blk360_IrImageHandle image);
BLK360_SHARED void Blk360_ProcessingWorkflow_ProcessIrPanorama(Blk360_ProcessingWorkflowHandle instance, Blk360_MeasurementHandle measurement);
BLK360_SHARED Blk360_ImageHandle Blk360_ProcessingWorkflow_ProcessHDRImage(Blk360_ProcessingWorkflowHandle instance, Blk360_MeasurementHandle measurement, Blk360_index_t hdrImageIndex);
BLK360_SHARED void Blk360_ProcessingWorkflow_ProcessPointCloud(Blk360_ProcessingWorkflowHandle instance, Blk360_PointCloudHandle pointCloud);

BLK360_SHARED void Blk360_ProcessingWorkflow_StitchedPanorama_AddImage(Blk360_ProcessingWorkflowHandle instance, Blk360_StitchedPanoramaHandle panorama, Blk360_ImageHandle image);
BLK360_SHARED void Blk360_ProcessingWorkflow_StitchedPanorama_AddPanorama(Blk360_ProcessingWorkflowHandle instance, Blk360_StitchedPanoramaHandle panorama, Blk360_ImageEnumeratorHandle images);

BLK360_SHARED Blk360_SubscriptionHandle Blk360_ProcessingWorkflow_OnStitchingProgress(Blk360_ProcessingWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_ProcessingWorkflow_OnStitchingError(Blk360_ProcessingWorkflowHandle instance, Blk360_EventQueueHandle queue);

BLK360_SHARED Blk360_SubscriptionHandle Blk360_ProcessingWorkflow_OnPointCloudChunkAvailable(Blk360_ProcessingWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_ProcessingWorkflow_OnPointCloudProcessProgress(Blk360_ProcessingWorkflowHandle instance, Blk360_EventQueueHandle queue);
BLK360_SHARED Blk360_SubscriptionHandle Blk360_ProcessingWorkflow_OnPointCloudProcessError(Blk360_ProcessingWorkflowHandle instance, Blk360_EventQueueHandle queue);

#ifdef __cplusplus // extern "C" {
}
#endif

#endif //BLK360_H
