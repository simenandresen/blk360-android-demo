#include "blk360wrapper/Blk360Wrapper.hpp"
#include <thread>
#include <filesystem>
#include <iostream>
#include <exception>
#include <unistd.h>
#include <cmath>
#include <jni/JniUtils.hpp>
#include "blk360wrapper/blk360Util.hpp"
#include "util/Bmp.hpp"
#include "debuglog.hpp"


using namespace demo::log;

namespace {
    Blk360_SessionHandle session{Blk360_Handle_Null()};

    Blk360_DataManipulationWorkflowHandle dataManipulationWorkflow{Blk360_Handle_Null()};

    Blk360_ProcessingWorkflowHandle processingWorkflow{Blk360_Handle_Null()};

    Blk360_MeasurementWorkflowHandle measurementWorkflow{Blk360_Handle_Null()};

    Blk360_SystemWorkflowHandle systemWorkflow{Blk360_Handle_Null()};

    Blk360_MeasurementHandle measurement{Blk360_Handle_Null()};

    Blk360_EventQueueHandle deviceProgressEventQueue{Blk360_Handle_Null()};
    Blk360_EventQueueHandle downloadProgressEventQueue{Blk360_Handle_Null()};
    Blk360_EventQueueHandle downloadPcEventQueue{Blk360_Handle_Null()};
}

namespace sample {
    void checkError();

    void cleanup();

    void print(const Blk360_ImageProperties &);

    void printAndRaiseError(const Blk360_Event &);
}

void createScan() {
    print(LOG_INFO, "Start scanning");
    Blk360_Api_New(BLK360_LIBRARY_VERSION);
    sample::checkError();

    session = Blk360_Session_New_Default("192.168.10.90:8081");
    sample::checkError();

    measurementWorkflow = Blk360_MeasurementWorkflow_Create(session);
    sample::checkError();

    dataManipulationWorkflow = Blk360_DataManipulationWorkflow_Create(session);
    sample::checkError();

    processingWorkflow = Blk360_ProcessingWorkflow_Create(session);
    sample::checkError();

    systemWorkflow = Blk360_SystemWorkflow_Create(session);
    sample::checkError();

    // perform measurement

    Blk360_MeasurementParameters parameters = Blk360_MeasurementParameters_New();

    parameters.enablePanorama = true;
    parameters.enablePointCloud = false;
    parameters.enableIr = false;
    parameters.panoramaParameters.panoramaMode = Blk360_PanoramaMode_LDR;

    deviceProgressEventQueue = Blk360_EventQueue_New(500);
    sample::checkError();

    const Blk360_SubscriptionHandle measurementEvents = Blk360_MeasurementWorkflow_OnProgress(
            measurementWorkflow, deviceProgressEventQueue);
    sample::checkError();

    const Blk360_SubscriptionHandle measurementErrorEvents = Blk360_MeasurementWorkflow_OnError(
            measurementWorkflow, deviceProgressEventQueue);
    sample::checkError();

    const Blk360_scanId_t scanId = Blk360_MeasurementWorkflow_Start(measurementWorkflow,
                                                                    parameters);
    sample::checkError();

    print(LOG_INFO, "Assigned scan id: %i", scanId);

    uint32_t imagesTotal = 0;

    while (Blk360_EventQueue_Wait(deviceProgressEventQueue, 30000)) {
        const Blk360_Event baseEvent = Blk360_EventQueue_Pop(deviceProgressEventQueue);

        if (baseEvent.sender.handle == measurementEvents.handle) {
            const auto &progressEvent = reinterpret_cast<const Blk360_ProgressEvent &>(baseEvent);
            if (progressEvent.imagesDone > 0) {
                print(LOG_INFO, "Got first image!");
                imagesTotal = progressEvent.imagesTotal;
                break;
            }
        } else if (baseEvent.sender.handle == measurementErrorEvents.handle) {
            sample::printAndRaiseError(baseEvent);
            sample::cleanup();
        } else {
            print(LOG_ERROR, "Unexpected event");
            Blk360_MeasurementWorkflow_Cancel(measurementWorkflow);
            sample::cleanup();
            throw std::runtime_error("Something went wrong");
        }
    }

    measurement = Blk360_DataManipulationWorkflow_GetMeasurementByScanId(dataManipulationWorkflow,
                                                                         scanId);
    sample::checkError();

    downloadProgressEventQueue = Blk360_EventQueue_New(1000);
    sample::checkError();

    const Blk360_SubscriptionHandle imageDownloadProgressEvents = Blk360_DataManipulationWorkflow_OnImageDownloadProgress(
            dataManipulationWorkflow, downloadProgressEventQueue);
    sample::checkError();

    const Blk360_SubscriptionHandle downloadErrorEvents = Blk360_DataManipulationWorkflow_OnError(
            dataManipulationWorkflow, downloadProgressEventQueue);
    sample::checkError();

    Blk360_DataManipulationWorkflow_DownloadPanorama(dataManipulationWorkflow, measurement);
    sample::checkError();

    uint32_t finishedImages = 0;

    while (Blk360_EventQueue_Wait(downloadProgressEventQueue, 20000)) {
        const Blk360_Event baseEvent = Blk360_EventQueue_Pop(downloadProgressEventQueue);

        if (baseEvent.sender.handle == imageDownloadProgressEvents.handle) {
            const auto &event = reinterpret_cast<const Blk360_ImageDownloadProgressEvent &>(baseEvent);
            std::cout << "Download progress (image " << event.index + 1 << "): " << event.progress
                      << "%" << std::endl;
            if (event.progress == 100) {
                std::cout << "Processing image " << event.index + 1 << std::endl;

                auto imageHandle = Blk360_Measurement_GetImageAtIndex(measurement, event.index);

                Blk360_ProcessingWorkflow_ProcessImage(processingWorkflow, imageHandle);
                auto info = Blk360_Image_GetProperties(imageHandle);
                sample::checkError();
                sample::print(info);

                const char *dataPtr = Blk360_Image_GetData(imageHandle);
                sample::checkError();
                const uint64_t dataSize = Blk360_Image_GetDataSizeInBytes(imageHandle);
                sample::checkError();

                std::cout << "Processed image " << event.index + 1 << std::endl;
                if (++finishedImages == imagesTotal) {
                    break;
                }
            }
        } else if (baseEvent.sender.handle == downloadErrorEvents.handle) {
            sample::printAndRaiseError(baseEvent);
        } else {
            std::cerr << "Unexpected event." << std::endl;
            sample::cleanup();
            throw std::runtime_error("Unexpected event");
        }
    }


    downloadPcEventQueue = Blk360_EventQueue_New(1000);
    const auto pointCloudChunkAvailable = Blk360_ProcessingWorkflow_OnPointCloudChunkAvailable(
            processingWorkflow, downloadPcEventQueue);
    sample::checkError();

    const auto pointCloudProcessProgress = Blk360_ProcessingWorkflow_OnPointCloudProcessProgress(
            processingWorkflow, downloadPcEventQueue);
    sample::checkError();

    const auto pointCloudError = Blk360_ProcessingWorkflow_OnPointCloudProcessError(
            processingWorkflow, downloadPcEventQueue);
    sample::checkError();

    const auto dataManipulationOnError = Blk360_DataManipulationWorkflow_OnError(
            dataManipulationWorkflow, downloadPcEventQueue);
    sample::checkError();




    sample::cleanup();
}

namespace sample {
    void checkError() {
        Blk360_Error error = Blk360_Api_GetLastError();

        if (error.code != Blk360_Error_Ok) {
            print(LOG_ERROR, "Error - code: %u message: %s", error.code, error.message);
            throw error;
        }
    }

    void cleanup() {
        Blk360_EventQueue_Release(downloadPcEventQueue);
        Blk360_EventQueue_Release(downloadProgressEventQueue);
        Blk360_EventQueue_Release(deviceProgressEventQueue);
        Blk360_Measurement_Release(measurement);
        Blk360_ProcessingWorkflow_Release(processingWorkflow);
        Blk360_DataManipulationWorkflow_Release(dataManipulationWorkflow);
        Blk360_MeasurementWorkflow_Release(measurementWorkflow);
        Blk360_SystemWorkflow_Release(systemWorkflow);
        Blk360_Session_Release(session);
        Blk360_Api_Release();
    }

    void print(const Blk360_ImageProperties &properties) {
        print(LOG_INFO, "Exposure %u", properties.exposure);
        print(LOG_INFO, "Width %u", properties.width);
        print(LOG_INFO, "Height %u", properties.height);
    }

    void printAndRaiseError(const Blk360_Event &baseEvent) {
        const auto &error = reinterpret_cast<const Blk360_ErrorEvent &>(baseEvent);
        print(LOG_ERROR, "Error code: %u", error.errorCode);
        print(LOG_ERROR, "Error message %s", error.message);
        throw error;
    }
}


