#include <memory>

#include "charta/Bmp24RGB.hpp"


#define WRITE_UINT32(addr, value) \
*(addr + 0) = ((value & 0x000000ff) >> 0);\
*(addr + 1) = ((value & 0x0000ff00) >> 8);\
*(addr + 2) = ((value & 0x00ff0000) >> 16);\
*(addr + 3) = ((value & 0xff000000) >> 24)\

#define READ_UINT32(addr) (\
(*((uint8_t*) (addr + 0)) << 0)   |\
(*((uint8_t*) (addr + 1)) << 8)   |\
(*((uint8_t*) (addr + 2)) << 16)  |\
(*((uint8_t*) (addr + 3)) << 24)) \

Charta::Bmp24RGB::Bmp24RGB(uint32_t width, uint32_t height) : width(width), height(height)
{
    uint32_t pixelRawDataSize =  width * height * 4;

    this->_rawPixelData = new uint8_t[pixelRawDataSize];
    this->_headerData = new uint8_t[54];

    this->_headerData[0] = 'B';
    this->_headerData[1] = 'M';

    this->_fullSize = pixelRawDataSize + 54;

    WRITE_UINT32(this->_headerData + 2, this->_fullSize);

    WRITE_UINT32(this->_headerData + 6, 0);

    WRITE_UINT32(this->_headerData + 0xA, 54);

    WRITE_UINT32(this->_headerData + 0x0E, 40);

    WRITE_UINT32(this->_headerData + 0x12, width);
    WRITE_UINT32(this->_headerData + 0x16, height);

    this->_headerData[0x1A] =  1;
    this->_headerData[0x1B] =  0;
    this->_headerData[0x1C] = 24; // bits per pixel
    this->_headerData[0x1D] = 0;

    WRITE_UINT32(this->_headerData + 0x1E, 0);

    WRITE_UINT32(this->_headerData + 0x22, pixelRawDataSize);

    WRITE_UINT32(this->_headerData + 0x26, 2835); //MAGIC vertical pixel density
    WRITE_UINT32(this->_headerData + 0x2A, 2835); //MAGIC horizontal pixel density

    WRITE_UINT32(this->_headerData + 0x2E, 0);
    WRITE_UINT32(this->_headerData + 0x32, 0);

    for (uint32_t i = 0; i < pixelRawDataSize; i++)
        this->_rawPixelData[i] = 0;

}

Charta::Bmp24RGB::~Bmp24RGB()
{
    delete[] this->_headerData;
    delete[] this->_rawPixelData;
}

Charta::Bmp24RGB::Bmp24RGB(const uint8_t* rawBmpData)
{
    this->_headerData = new uint8_t[54];

    for (uint8_t i = 0; i < 54; i++)
        this->_headerData[i] = rawBmpData[i];

    this->width = READ_UINT32(_headerData + 0x12);
    this->height = READ_UINT32(_headerData + 0x16);

    uint32_t pixelRawDataSize = this->width * this->height * 4;

    this->_rawPixelData = new uint8_t[pixelRawDataSize];

    this->_fullSize = pixelRawDataSize + 54;

    for (uint32_t i = 0; i < pixelRawDataSize; i++)
        this->_rawPixelData[i] = rawBmpData[54 + i];

}

Charta::Bmp24RGB::Bmp24RGB(Charta::Bmp24RGB &&other)
{
    this->_rawPixelData = other._rawPixelData;
    this->_headerData = other._headerData;

    other._rawPixelData = nullptr;
    other._headerData = nullptr;
}

uint32_t Charta::Bmp24RGB::GetFullSize() const
{
    return _fullSize;
}

void Charta::Bmp24RGB::WriteToBuffer(uint8_t* buffer, size_t bufferSize, size_t offset)
{
    for (size_t i = 0; i < 54 && (i + offset) < bufferSize; i++)
        buffer[offset + i] = this->_headerData[i];

    for (size_t i = 0; i < this->height * this->width * 4 && (i + offset) < bufferSize - 54; i++)
        buffer[offset + 54 + i] = this->_rawPixelData[i];
}

uint8_t *Charta::Bmp24RGB::GetRawPixelData() const
{
    return _rawPixelData;
}

uint32_t Charta::Bmp24RGB::GetWidth() const
{
    return width;
}

uint32_t Charta::Bmp24RGB::GetHeight() const
{
    return height;
}
