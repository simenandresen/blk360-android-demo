#pragma once
#include <scan_tools/image_metadata.h>

namespace demo {

    struct Scan;

    ImageMetadata makeTopdownImage(const Scan &scan, float *image_buffer, size_t image_size);
}
