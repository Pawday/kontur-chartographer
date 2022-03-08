#include <charta/RawImage24.hpp>
#include <charta/Bmp24RGB.hpp>

#include "charta/ImageConverter.hpp"

void Charta::ImageConverter::RawToBmp(uint16_t height, uint16_t width, const uint8_t* inputRawImageDataBuffer, uint8_t* outputBMPPixelDataBuffer)
{
    for (uint32_t y = 0; y < height; y++)
    {
        size_t bmpPadding = (4 - ((width * 3) % 4)) * (height - 1 - y);
        if ((width * 3) % 4 == 0) bmpPadding = 0;

        for (uint32_t x = 0; x < width; x++)
        {
            //MAP RAW 24 bit pixel data to 24 bit BMP image with padding every raw

            size_t rawImagePixelPos = (y * width + x);
            size_t bmpImagePixelPos = ((height - 1 - y) * width + x);

            uint8_t InputRed = inputRawImageDataBuffer[rawImagePixelPos * 3 + 0];
            uint8_t InputGreen = inputRawImageDataBuffer[rawImagePixelPos * 3 + 1];
            uint8_t InputBlue = inputRawImageDataBuffer[rawImagePixelPos * 3 + 2];

            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 0 + bmpPadding] = InputBlue;
            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 1 + bmpPadding] = InputGreen;
            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 2 + bmpPadding] = InputRed;
        }
    }
}