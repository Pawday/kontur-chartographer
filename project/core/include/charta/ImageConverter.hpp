#pragma once


namespace Charta
{
    class ImageConverter
    {
        static void RawToBmp
        (
            uint16_t height,
            uint16_t width,
            const uint8_t* inputRawImageDataBuffer,
            uint8_t* bmpPixelDataBuffer
        );
    };
}