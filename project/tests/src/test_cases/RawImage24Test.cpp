#include <catch2/catch.hpp>

#include <Poco/File.h>
#include <Poco/FileStream.h>

#include <charta/RawImage24.hpp>
#include <charta/Bmp24RGB.hpp>

TEST_CASE("raw_image_create_test", "[image]")
{
    uint16_t width = 100;
    uint16_t height = 100;
    Charta::RawImage24 image(width, height);

    uint8_t* imageRawData = image.getRawData();

    //image should be black
    for (int i = 0; i < (width * height); i++)
    {
        CHECK(imageRawData[i * 3 + 0] == 0);
        CHECK(imageRawData[i * 3 + 1] == 0);
        CHECK(imageRawData[i * 3 + 2] == 0);
    }
}

TEST_CASE("image_combine_test", "[image]")
{
    Charta::RawImage24 image1(10, 10);
    
    Charta::RawImage24 image2(5, 5);

    uint8_t* image1RawPixelData = image1.getRawData();

    for (int i = 0; i < 100; ++i)
    {
        image1RawPixelData[i * 3 + 0] = 0xff;
        image1RawPixelData[i * 3 + 1] = 0xff;
        image1RawPixelData[i * 3 + 2] = 0xff;
    }

    image1.AppendImage(0,0,image2);

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            size_t pixelPosition = (y * 10 + x) * 3;
            if (x < 5 && y < 5)
            {
                CHECK(image1RawPixelData[pixelPosition + 0] == 0);
                CHECK(image1RawPixelData[pixelPosition + 1] == 0);
                CHECK(image1RawPixelData[pixelPosition + 2] == 0);
            }
            else
            {
                CHECK(image1RawPixelData[pixelPosition + 0] == 0xff);
                CHECK(image1RawPixelData[pixelPosition + 1] == 0xff);
                CHECK(image1RawPixelData[pixelPosition + 2] == 0xff);
            }
        }
    }

}