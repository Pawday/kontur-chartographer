#include <iostream>
#include <catch2/catch.hpp>

#include <Poco/File.h>

#include "charta/RawImage24.hpp"

TEST_CASE("raw_image_create_test", "[image]")
{
    uint16_t width = 10;
    uint16_t height = 10;
    Charta::RawImage24 image(width, height);

    uint8_t* imageRawData = image.GetRawData();

    //image should be black
    for (int i = 0; i < (width * height); i++)
    {
        CHECK(imageRawData[i * 3 + 0] == 0);
        CHECK(imageRawData[i * 3 + 1] == 0);
        CHECK(imageRawData[i * 3 + 2] == 0);
    }
}

TEST_CASE("raw_image_combine_test", "[image]")
{
    Charta::RawImage24 image1(10, 10);
    
    Charta::RawImage24 image2(5, 5);

    uint8_t* image1RawPixelData = image1.GetRawData();

    for (int i = 0; i < 100; ++i)
    {
        image1RawPixelData[i * 3 + 0] = 0xff;
        image1RawPixelData[i * 3 + 1] = 0xff;
        image1RawPixelData[i * 3 + 2] = 0xff;
    }

    image1.MergeImage(0, 0, image2);

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

TEST_CASE("raw_image_get_sub_image_uncropped_test", "[image]")
{
    using namespace Charta;

    const uint16_t w = 10;
    const uint16_t h = 10;

    RawImage24 sourceImage(w, h);

    uint8_t* imageRawData = sourceImage.GetRawData();
    
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            imageRawData[(y * w + x) * 3 + 0] = 0xff;
            imageRawData[(y * w + x) * 3 + 1] = 0xff;
            imageRawData[(y * w + x) * 3 + 2] = 0xff;
        }
    }

    RawImage24 subImage = sourceImage.GetUncroppedSubImage(5, 5, 10, 10);

    uint8_t* subImageRawData = subImage.GetRawData();

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (y < 5 && x < 5)
            {
                if (subImageRawData[(y * 10 + x) * 3 + 0] != 0xff) FAIL();
                if (subImageRawData[(y * 10 + x) * 3 + 1] != 0xff) FAIL();
                if (subImageRawData[(y * 10 + x) * 3 + 1] != 0xff) FAIL();
            }
            else
            {
                if (subImageRawData[(y * 10 + x) * 3 + 0] != 0x00) FAIL();
                if (subImageRawData[(y * 10 + x) * 3 + 1] != 0x00) FAIL();
                if (subImageRawData[(y * 10 + x) * 3 + 1] != 0x00) FAIL();
            }
        }
    }


    
}