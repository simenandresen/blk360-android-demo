#pragma once

#include "BLK360.h"
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <cstdint>

namespace bmp {
    void
    save(const Blk360_ImageHandle &, const char *dataPtr, uint64_t dataSize, std::string imagePath);
}

namespace {
    static constexpr uint32_t BYTES_PER_PIXEL = 3; // RGB8
    static constexpr uint32_t FILE_HEADER_SIZE = 14;
    static constexpr uint32_t DIB_HEADER_SIZE = 40;
    static constexpr uint32_t IMAGE_START_POINT = FILE_HEADER_SIZE + DIB_HEADER_SIZE;
    // Not taking padding into account, since image dimensions are multiplicities of 8.

    std::array<char, FILE_HEADER_SIZE> getFileHeader(uint32_t width, uint32_t height) {
        std::array<char, FILE_HEADER_SIZE> header =
                {
                        0, 0,     // signature
                        0, 0, 0, 0, // image file size in bytes
                        0, 0, 0, 0, // reserved
                        0, 0, 0, 0, // start of pixel array
                };

        const uint32_t file_size =
                FILE_HEADER_SIZE + DIB_HEADER_SIZE + (BYTES_PER_PIXEL * width * height);

        header[0] = 'B';
        header[1] = 'M';

        header[2] = reinterpret_cast<const char *>(&file_size)[0];
        header[3] = reinterpret_cast<const char *>(&file_size)[1];
        header[4] = reinterpret_cast<const char *>(&file_size)[2];
        header[5] = reinterpret_cast<const char *>(&file_size)[3];

        header[10] = reinterpret_cast<const char *>(&IMAGE_START_POINT)[0];
        header[11] = reinterpret_cast<const char *>(&IMAGE_START_POINT)[1];
        header[12] = reinterpret_cast<const char *>(&IMAGE_START_POINT)[2];
        header[13] = reinterpret_cast<const char *>(&IMAGE_START_POINT)[3];

        return header;
    }

    std::array<char, DIB_HEADER_SIZE> getDIBHeader(uint32_t width, uint32_t height) {
        std::array<char, DIB_HEADER_SIZE> header =
                {
                        0, 0, 0, 0, // header size
                        0, 0, 0, 0, // image width
                        0, 0, 0, 0, // image height
                        1, 0,     // number of color planes
                        0, 0,     // bits per pixel
                        0, 0, 0, 0, // compression
                        0, 0, 0, 0, // image size
                        0, 0, 0, 0, // horizontal resolution
                        0, 0, 0, 0, // vertical resolution
                        0, 0, 0, 0, // colors in color table
                        0, 0, 0, 0, // important color count
                };

        header[0] = reinterpret_cast<const char *>(&DIB_HEADER_SIZE)[0];
        header[1] = reinterpret_cast<const char *>(&DIB_HEADER_SIZE)[1];
        header[2] = reinterpret_cast<const char *>(&DIB_HEADER_SIZE)[2];
        header[3] = reinterpret_cast<const char *>(&DIB_HEADER_SIZE)[3];

        header[4] = reinterpret_cast<const char *>(&width)[0];
        header[5] = reinterpret_cast<const char *>(&width)[1];
        header[6] = reinterpret_cast<const char *>(&width)[2];
        header[7] = reinterpret_cast<const char *>(&width)[3];

        header[8] = reinterpret_cast<const char *>(&height)[0];
        header[9] = reinterpret_cast<const char *>(&height)[1];
        header[10] = reinterpret_cast<const char *>(&height)[2];
        header[11] = reinterpret_cast<const char *>(&height)[3];

        uint16_t bitsPerPixel = BYTES_PER_PIXEL * 8;
        header[14] = reinterpret_cast<const char *>(&bitsPerPixel)[0];
        header[15] = reinterpret_cast<const char *>(&bitsPerPixel)[1];

        return header;
    }
}

namespace bmp {
    void save(
            const Blk360_ImageHandle &image,
            const char *dataPtr,
            uint64_t dataSize,
            uint32_t width,
            uint32_t height,
            std::string imagePath) {
        std::ofstream file;
        file.open(imagePath, std::fstream::out | std::fstream::binary);

        auto fileHeader = getFileHeader(width, height);
        auto DIBHeader = getDIBHeader(width, height);

        file.write(&fileHeader[0], FILE_HEADER_SIZE);
        file.write(&DIBHeader[0], DIB_HEADER_SIZE);

        //file.write(dataPtr, dataSize);
        std::vector<char> buffer(dataSize);

        // dataPtr seems to be in RGB, however, the bitmap buffer seems to be in bgr
        for (auto index = 0; index < dataSize; index += 3) {
            buffer[index] = dataPtr[index + 2];
            buffer[index + 1] = dataPtr[index + 1];
            buffer[index + 2] = dataPtr[index];
        }
        file.write(&buffer[0], dataSize);

        file.close();
    }
}

