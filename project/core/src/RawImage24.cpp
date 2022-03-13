#include <memory>
#include "charta/RawImage24.hpp"

Charta::RawImage24::RawImage24(uint16_t width, uint16_t height) : _width(width), _height(height)
{
    this->_rawData = new uint8_t[width * height * 3];
    memset(this->_rawData, 0, width * height * 3);
}

Charta::RawImage24::RawImage24(Charta::RawImage24&& other)
{
    this->_rawData = other._rawData;
    other._rawData = nullptr;
}

Charta::RawImage24::~RawImage24()
{
    delete[] this->_rawData;
}

void Charta::RawImage24::MergeImage(uint16_t xPos, uint16_t yPos, const RawImage24& image)
{
    for (uint16_t y = 0; y < image._height; y++)
    {
        if (y + yPos >= this->_height) break;

        for (uint16_t x = 0; x < image._width; x++)
        {
            if (x + xPos >= this->_width) break;

            uint8_t inputImagePixelRComponent = image._rawData[(y * image._width + x) * 3 + 0];
            uint8_t inputImagePixelGComponent = image._rawData[(y * image._width + x) * 3 + 1];
            uint8_t inputImagePixelBComponent = image._rawData[(y * image._width + x) * 3 + 2];

            this->_rawData[((y + yPos) * this->_width + x + xPos) * 3 + 0] = inputImagePixelRComponent;
            this->_rawData[((y + yPos) * this->_width + x + xPos) * 3 + 1] = inputImagePixelGComponent;
            this->_rawData[((y + yPos) * this->_width + x + xPos) * 3 + 2] = inputImagePixelBComponent;
        }
    }
}

Charta::RawImage24 Charta::RawImage24::GetUncroppedSubImage(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height)
{
    RawImage24 ret(width, height);

    uint8_t* retImageRawPixelData = ret.GetRawData();
    
    for (int y = 0; y < height; y++)
    {
        if (this->_height - 1 < (y + yPos)) break;

        for (int x = 0; x < width; x++)
        {
            if (this->_width - 1 < (x + xPos)) break;
            retImageRawPixelData[(y * width + x) * 3 + 0] = this->_rawData[((y + yPos) * width + x + xPos) * 3 + 0];
            retImageRawPixelData[(y * width + x) * 3 + 1] = this->_rawData[((y + yPos) * width + x + xPos) * 3 + 1];
            retImageRawPixelData[(y * width + x) * 3 + 2] = this->_rawData[((y + yPos) * width + x + xPos) * 3 + 2];
        }
    }

    return ret;
}

uint16_t Charta::RawImage24::GetWidth() const
{
    return _width;
}

uint16_t Charta::RawImage24::GetHeight() const
{
    return _height;
}

uint8_t *Charta::RawImage24::GetRawData() const
{
    return _rawData;
}