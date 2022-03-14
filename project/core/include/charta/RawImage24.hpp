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
        RawImage24(RawImage24&& other);

        ~RawImage24();

        void MergeImage(uint16_t xPos, uint16_t yPos, const RawImage24& image);

        RawImage24 GetUncroppedSubImage(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height) const;

        uint16_t GetWidth() const;

        uint16_t GetHeight() const;

        uint8_t *GetRawData() const;

    private:
        uint16_t _width;
        uint16_t _height;

        uint8_t* _rawData;
    };
}