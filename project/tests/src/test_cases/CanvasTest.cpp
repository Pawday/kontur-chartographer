#include <catch2/catch.hpp>

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>

#include "charta/Canvas.hpp"
#include "charta/ImageConverter.hpp"


void deleteAnyThenCreateDir(const Poco::Path& path)
{
    Poco::File pathAsFile(path);
    if (pathAsFile.exists())
        pathAsFile.remove(true);
    pathAsFile.createDirectory();
}

void deleteDir(const Poco::Path& path)
{
    Poco::File pathAsFile(path);
    if (pathAsFile.exists()) pathAsFile.remove(true);
}

TEST_CASE("canvas_create_test", "[canvas]")
{
    Poco::Path canvasContextDirNotExistedDir("canvas_create_test_not_existed_dir");

    deleteDir(canvasContextDirNotExistedDir);

    CHECK(false == Poco::File(canvasContextDirNotExistedDir).exists());
    Charta::Canvas canvas(canvasContextDirNotExistedDir);

    CHECK(false == canvas.Exist());
    canvas.Init(100, 100);
    CHECK(Poco::File(canvasContextDirNotExistedDir).exists());
    CHECK(canvas.Exist());

    //_______________________________________________________________________

    Poco::Path canvasContextDirExisted("canvas_create_test_existed_dir");

    deleteAnyThenCreateDir(canvasContextDirExisted);

    CHECK(Poco::File(canvasContextDirExisted).exists());
    Charta::Canvas canvas2(canvasContextDirExisted);

    CHECK(Poco::File(canvasContextDirExisted).exists());
    CHECK(false == canvas2.Exist());
    canvas2.Init(100, 100);
    CHECK(canvas2.Exist());

}

TEST_CASE("canvas_load_test", "[canvas]")
{
    Poco::Path canvasContextDir("canvas_load_test_dir");

    deleteAnyThenCreateDir(canvasContextDir);

    {
        Charta::Canvas canvasInitiator(canvasContextDir);
        canvasInitiator.Init(102, 342);
    }

    Charta::Canvas loadedCanvas(canvasContextDir);

    CHECK(loadedCanvas.Exist());

    CHECK(loadedCanvas.GetWidth() == 102);
    CHECK(loadedCanvas.GetHeight() == 342);

}

TEST_CASE("canvas_delete_test", "[canvas]")
{
    Poco::Path canvasContextDir("canvas_delete_test_dir");

    deleteDir(canvasContextDir);

    Charta::Canvas deleteCanvasTest(canvasContextDir);

    CHECK(false == deleteCanvasTest.Exist());

    deleteCanvasTest.Init(100, 100);

    CHECK(deleteCanvasTest.Exist());

    deleteCanvasTest.Delete();

    CHECK(false == Poco::File(canvasContextDir).exists());

}

TEST_CASE("canvas_get_chunk_test", "[canvas]")
{
    Poco::Path canvasContextDir = "canvas_get_chunk_test";

    deleteDir(canvasContextDir);

    Charta::Canvas canvas(canvasContextDir);

    canvas.Init(100, 100);
    
    Charta::RawImage24 image4test(100, 100);

    uint8_t* imageRawData = image4test.GetRawData();

    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++) 
        {
            if (x % 2 == 0)
            {
                imageRawData[(y * 100 + x) * 3 + 0] = 0xff;
                imageRawData[(y * 100 + x) * 3 + 1] = 0xff;
                imageRawData[(y * 100 + x) * 3 + 2] = 0xff;
            }
        };

    Charta::Bmp24RGB bmpTestImage = Charta::ImageConverter::RawToBmp(image4test);

    Poco::File chunkFileToCreateForTest(canvasContextDir.toString() + Poco::Path::separator() + "chunk_1_1.bmp");
    chunkFileToCreateForTest.createFile();

    Poco::FileOutputStream os(chunkFileToCreateForTest.path());

    char* bmpImageBuffer = new char[bmpTestImage.GetFullSize()];

    bmpTestImage.WriteToBuffer((uint8_t*) bmpImageBuffer, bmpTestImage.GetFullSize(), 0);
    os.write((char*) bmpImageBuffer, bmpTestImage.GetFullSize());
    os.flush();
    os.close();
    
    delete[] bmpImageBuffer;

    Charta::RawImage24 chunkImageRes = canvas.GetChunkAt(1, 1);

    uint8_t* chunkResRawData = chunkImageRes.GetRawData();

    bool testStatus = true;

    for (int y = 0; y < 100 && testStatus; y++)
        for (int x = 0; x < 100 && testStatus; x++)
        {
            if (x % 2 == 0)
            {
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 0] == 0xff;
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 1] == 0xff;
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 2] == 0xff;
            }
            else
            {
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 0] == 0x00;
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 1] == 0x00;
                testStatus &= chunkResRawData[(y * 100 + x) * 3 + 2] == 0x00;
            }
        }

    CHECK(testStatus);
}