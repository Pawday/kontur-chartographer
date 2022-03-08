#include <memory>

#include "charta/Bmp24RGB.hpp"


#define WRITE_UINT32(addr,value) \
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

    this->_fileSize = pixelRawDataSize + 54;

    WRITE_UINT32(this->_headerData + 2, this->_fileSize);

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

Charta::Bmp24RGB::Bmp24RGB(uint8_t* rawBmpData)
{
    this->_headerData = new uint8_t[54];

    for (uint8_t i = 0; i < 54; i++)
        this->_headerData[i] = rawBmpData[i];

    this->width = READ_UINT32(_headerData + 0x12);
    this->height = READ_UINT32(_headerData + 0x16);

    uint32_t pixelRawDataSize = this->width * this->height * 4;

    this->_rawPixelData = new uint8_t[pixelRawDataSize];

    this->_fileSize = pixelRawDataSize + 54;

    for (uint32_t i = 0; i < pixelRawDataSize; i++)
        this->_rawPixelData[i] = rawBmpData[54 + i];

}


Charta::Bmp24RGB::Bmp24RGB(Charta::RawImage24& image) : Charta::Bmp24RGB(image.getWidth(), image.getHeight())
{
    uint8_t *inputRawImageData = image.getRawData();

    for (uint32_t y = 0; y < this->height; y++)
    {
        size_t bmpPadding = (4 - ((this->width * 3) % 4)) * (this->height - 1 - y);
        if ((this->width * 3) % 4 == 0) bmpPadding = 0;

        for (uint32_t x = 0; x < this->width; x++)
        {
            //MAP RAW 24 bit pixel data to 24 bit BMP image with padding every raw

            size_t rawImagePixelPos = (y * this->width + x);
            size_t bmpImagePixelPos = ((this->height - 1 - y) * this->width + x);


            uint8_t InputRed = inputRawImageData[rawImagePixelPos * 3 + 0];
            uint8_t InputGreen = inputRawImageData[rawImagePixelPos * 3 + 1];
            uint8_t InputBlue = inputRawImageData[rawImagePixelPos * 3 + 2];

            this->_rawPixelData[bmpImagePixelPos * 3 + 0 + bmpPadding] = InputBlue;
            this->_rawPixelData[bmpImagePixelPos * 3 + 1 + bmpPadding] = InputGreen;
            this->_rawPixelData[bmpImagePixelPos * 3 + 2 + bmpPadding] = InputRed;
        }
    }
}

uint32_t Charta::Bmp24RGB::GetFullSize() const
{
    return _fileSize;
}

void Charta::Bmp24RGB::WriteToBuffer(uint8_t* buffer, size_t bufferSize, size_t offset)
{
    for (size_t i = 0; i < 54 && (i + offset) < bufferSize; i++)
        buffer[offset + i] = this->_headerData[i];

    for (size_t i = 0; i < this->height * this->width * 4 && (i + offset) < bufferSize - 54; i++)
        buffer[offset + 54 + i] = this->_rawPixelData[i];
}
