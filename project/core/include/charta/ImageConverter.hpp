#pragma once

#include "charta/RawImage24.hpp"
#include "charta/Bmp24RGB.hpp"

namespace Charta
{
    class ImageConverter
    {
    public:
        static Bmp24RGB RawToBmp(RawImage24& inputRawImage);
        static RawImage24 BmpToRaw(Bmp24RGB& inputBMPImage);
    };
}