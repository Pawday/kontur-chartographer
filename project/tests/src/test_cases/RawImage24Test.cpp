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
}