#include <catch2/catch.hpp>

#include <Poco/File.h>
#include <Poco/FileStream.h>

#include "charta/Bmp24RGB.hpp"

TEST_CASE("bmp_image_create_test", "[bmp]")
{
    Charta::Bmp24RGB bmp(100,100);

    bmp.GetFullSize();
}