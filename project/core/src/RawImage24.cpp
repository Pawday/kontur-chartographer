#include <memory>
#include "charta/RawImage24.hpp"

Charta::RawImage24::RawImage24(uint16_t width, uint16_t height) : _width(width), _height(height)
{
    this->_rawData = new uint8_t[width * height * 3];
    memset(this->_rawData, 0, width * height * 3);
}

Charta::RawImage24::~RawImage24()
{
    delete[] this->_rawData;
}

void Charta::RawImage24::AppendImage(uint16_t xPos, uint16_t yPos, RawImage24& image)
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

uint16_t Charta::RawImage24::getWidth() const
{
    return _width;
}

uint16_t Charta::RawImage24::getHeight() const
{
    return _height;
}

uint8_t *Charta::RawImage24::getRawData() const
{
    return _rawData;
}
