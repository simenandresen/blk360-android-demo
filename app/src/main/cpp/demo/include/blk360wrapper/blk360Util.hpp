#pragma once

#include <string>
#include "BLK360.h"


namespace demo {

    void checkAndThrowError(const std::string &topic = "");

    void raiseOnConnectionLostEvent(
            Blk360_SubscriptionHandle sessionSubscription,
            Blk360_Event baseEvent
    );

    void writeImageInfoToJson(const Blk360_ImageProperties &imageProperties,
                              const std::string &imagePath);

    void writeCameraCalibrationToJson(const Blk360_CameraCalibration &cameraCalibration,
                                      const std::string &jsonPath);

    void writeMetadataToJson(Blk360_MeasurementHandle measurement, const std::string &jsonPath);
}
