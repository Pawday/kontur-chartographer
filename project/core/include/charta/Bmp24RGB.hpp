#pragma once

#include <cinttypes>
#include <charta/RawImage24.hpp>

namespace Charta
{
    /**
     * Raw BGR Image with 4 byte alignment and BMP header
     */
    class Bmp24RGB
    {
    public:

        /**
         * create Bmp24RGB image and allocate memory for it
         */
        Bmp24RGB(uint32_t width, uint32_t height);

        /**
         * Move constructor
         */
        Bmp24RGB(Bmp24RGB&& other);

        /**
         * create Bmp24RGB image from bmp file and allocate memory for it
         * (memory would be copied)
         */
        Bmp24RGB(const uint8_t* rawBmpData);

        void WriteToBuffer(uint8_t* buffer, size_t bufferSize ,size_t offset);

        ~Bmp24RGB();

        uint32_t GetFullSize() const;

        uint8_t* GetRawPixelData() const;

        uint32_t GetWidth() const;

        uint32_t GetHeight() const;

    private:
        uint32_t width;
        uint32_t height;
        uint32_t _fullSize;

        uint8_t* _headerData;
        uint8_t* _rawPixelData;
    };
}