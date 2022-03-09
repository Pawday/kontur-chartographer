#include "charta/ImageConverter.hpp"

Charta::Bmp24RGB Charta::ImageConverter::RawToBmp(RawImage24& inputRawImage)
{
    uint16_t height = inputRawImage.GetHeight();
    uint16_t width = inputRawImage.GetWidth();

    uint8_t* inputRawImageDataBuffer = inputRawImage.GetRawData();
    
    Bmp24RGB ret(width, height);

    uint8_t* outputBMPPixelDataBuffer = ret.GetRawPixelData();
    
    for (uint32_t y = 0; y < height; y++)
    {
        size_t bmpPadding = (4 - ((width * 3) % 4)) * (height - 1 - y);
        if ((width * 3) % 4 == 0) bmpPadding = 0;

        for (uint32_t x = 0; x < width; x++)
        {
            //MAP RAW 24 bit pixel data to 24 bit BMP image with padding every raw

            size_t rawImagePixelPos = (y * width + x);
            size_t bmpImagePixelPos = ((height - 1 - y) * width + x);

            uint8_t inputRed = inputRawImageDataBuffer[rawImagePixelPos * 3 + 0];
            uint8_t inputGreen = inputRawImageDataBuffer[rawImagePixelPos * 3 + 1];
            uint8_t inputBlue = inputRawImageDataBuffer[rawImagePixelPos * 3 + 2];

            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 0 + bmpPadding] = inputBlue;
            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 1 + bmpPadding] = inputGreen;
            outputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 2 + bmpPadding] = inputRed;
        }
    }
    
    //Bmp24RGB destructor will not be called here
    return ret;
}

Charta::RawImage24 Charta::ImageConverter::BmpToRaw(Bmp24RGB& inputBMPImage)
{
    uint32_t height = inputBMPImage.GetHeight();
    uint32_t width = inputBMPImage.GetWidth();

    uint8_t* inputBMPPixelDataBuffer = inputBMPImage.GetRawPixelData();
    
    RawImage24 ret(width, height);

    uint8_t* rawImageDataBuffer = ret.GetRawData();
    
    for (uint32_t y = 0; y < height; y++)
    {
        size_t bmpPadding = (4 - ((width * 3) % 4)) * (height - 1 - y);
        if ((width * 3) % 4 == 0) bmpPadding = 0;

        for (uint32_t x = 0; x < width; x++)
        {
            size_t rawImagePixelPos = (y * width + x);
            size_t bmpImagePixelPos = ((height - 1 - y) * width + x);

            uint8_t inputBlue = inputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 0 + bmpPadding];
            uint8_t inputGreen = inputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 1 + bmpPadding];
            uint8_t inputRed = inputBMPPixelDataBuffer[bmpImagePixelPos * 3 + 2 + bmpPadding];

            rawImageDataBuffer[rawImagePixelPos * 3 + 0] = inputRed;
            rawImageDataBuffer[rawImagePixelPos * 3 + 1] = inputGreen;
            rawImageDataBuffer[rawImagePixelPos * 3 + 2] = inputBlue;

        }
    }

    //RawImage24 destructor will not be called here
    return ret;
}

