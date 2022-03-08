#pragma once

#include <cinttypes>

namespace Charta
{
    /**
     * Raw RGB Image without alignment
     */
    class RawImage24
    {
    public:
        RawImage24(uint16_t width, uint16_t height);
        ~RawImage24();

        void AppendImage(uint16_t xPos, uint16_t yPos, RawImage24& image);

        uint16_t getWidth() const;

        uint16_t getHeight() const;

        uint8_t *getRawData() const;

    private:
        uint16_t _width;
        uint16_t _height;

        uint8_t* _rawData;
    };
}