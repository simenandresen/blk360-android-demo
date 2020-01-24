#pragma once

namespace demo {

    struct ImageCoords {
        int i;
        int j;
    };

    struct ImageMetadata {

        ImageCoords origin;
        float pixelsPerMeter;
    };
}
