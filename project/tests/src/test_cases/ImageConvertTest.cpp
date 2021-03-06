#include <catch2/catch.hpp>

#include <charta/ImageConverter.hpp>

TEST_CASE("image_convert_bmp_to_raw_test", "[image_convert]")
{
    using namespace Charta;
    //example 3 x 2 bmp file in binary
    uint8_t bmpFileDump[78] =
    {
            0x42, 0x4D, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00,
            0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00,
            0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xBA, 0x00, 0xAB,
            0xBA, 0x00, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00,
            0xFF, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    Charta::Bmp24RGB bmp(bmpFileDump);

    CHECK(bmp.GetWidth() == 3);
    CHECK(bmp.GetHeight() == 2);

    RawImage24 image = ImageConverter::BmpToRaw(bmp);

    uint8_t* imagePixelData = image.GetRawData();

    //check all 6 pixels 0_0

    CHECK(imagePixelData[0] == 0xff);
    CHECK(imagePixelData[1] == 0x00);
    CHECK(imagePixelData[2] == 0x00);

    CHECK(imagePixelData[3] == 0x00);
    CHECK(imagePixelData[4] == 0xff);
    CHECK(imagePixelData[5] == 0x00);

    CHECK(imagePixelData[6] == 0x00);
    CHECK(imagePixelData[7] == 0x00);
    CHECK(imagePixelData[8] == 0xff);

    CHECK(imagePixelData[9] == 0xff);
    CHECK(imagePixelData[10] == 0xff);
    CHECK(imagePixelData[11] == 0xff);

    CHECK(imagePixelData[12] == 0xab);
    CHECK(imagePixelData[13] == 0x00);
    CHECK(imagePixelData[14] == 0xba);

    CHECK(imagePixelData[15] == 0xab);
    CHECK(imagePixelData[16] == 0x00);
    CHECK(imagePixelData[17] == 0xba);

}

TEST_CASE("image_convert_raw_to_bmp_test", "[image_convert]")
{
    //RAW -> BMP -> RAW

    using namespace Charta;
    
    RawImage24 rawImageInput(3, 3);

    uint8_t* inputImageRawPixelData = rawImageInput.GetRawData();

    for (int i = 0; i < 9; i++)
    {
        inputImageRawPixelData[i * 3 + 0] = i * 3 + 0;
        inputImageRawPixelData[i * 3 + 1] = i * 3 + 1;
        inputImageRawPixelData[i * 3 + 2] = i * 3 + 2;
    }

    Bmp24RGB bmpOutput = ImageConverter::RawToBmp(rawImageInput);

    CHECK(bmpOutput.GetHeight() == 3);
    CHECK(bmpOutput.GetWidth() == 3);

    uint8_t* bmpImageInputRawBuffer = new uint8_t[bmpOutput.GetFullSize()];
    bmpOutput.WriteToBuffer(bmpImageInputRawBuffer, bmpOutput.GetFullSize(), 0);
    
    Bmp24RGB bmpImageInput(bmpImageInputRawBuffer);
    delete[] bmpImageInputRawBuffer;

    RawImage24 bmpToRawResult = ImageConverter::BmpToRaw(bmpImageInput);

    CHECK(bmpToRawResult.GetWidth() == 3);
    CHECK(bmpToRawResult.GetHeight() == 3);

    uint8_t* rawImageOutputPixelData = bmpToRawResult.GetRawData();

    for (int i = 0; i < 9; i++)
    {
        CHECK(rawImageOutputPixelData[i * 3 + 0] == i * 3 + 0);
        CHECK(rawImageOutputPixelData[i * 3 + 1] == i * 3 + 1);
        CHECK(rawImageOutputPixelData[i * 3 + 2] == i * 3 + 2);
    }

}