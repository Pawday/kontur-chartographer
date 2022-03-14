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

    deleteCanvasTest.Init(CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);

    CHECK(deleteCanvasTest.Exist());

    deleteCanvasTest.Delete();

    CHECK(false == Poco::File(canvasContextDir).exists());

}

TEST_CASE("canvas_get_chunk_test", "[canvas]")
{
    Poco::Path canvasContextDir = "canvas_get_chunk_test_dir";

    deleteDir(canvasContextDir);

    Charta::Canvas canvas(canvasContextDir);

    canvas.Init(CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);
    
    Charta::RawImage24 image4test(CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);

    uint8_t* imageRawData = image4test.GetRawData();

    for (int y = 0; y < CANVAS_CHUNK_SIZE; y++)
        for (int x = 0; x < CANVAS_CHUNK_SIZE; x++) 
        {
            if (x % 2 == 0)
            {
                imageRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 0] = 0xff;
                imageRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 1] = 0xff;
                imageRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 2] = 0xff;
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

    for (int y = 0; y < CANVAS_CHUNK_SIZE && testStatus; y++)
        for (int x = 0; x < CANVAS_CHUNK_SIZE && testStatus; x++)
        {
            if (x % 2 == 0)
            {
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 0] == 0xff;
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 1] == 0xff;
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 2] == 0xff;
            }
            else
            {
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 0] == 0x00;
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 1] == 0x00;
                testStatus &= chunkResRawData[(y * CANVAS_CHUNK_SIZE + x) * 3 + 2] == 0x00;
            }
        }

    CHECK(testStatus);
}

TEST_CASE("canvas_set_chunk_test", "[canvas]")
{
    Poco::Path canvasContextDir = "canvas_set_chunk_test_dir";

    deleteDir(canvasContextDir);

    Charta::Canvas canvas(canvasContextDir);

    canvas.Init(1000, 1000);
    
    for (int i = 0; i < 1000 / CANVAS_CHUNK_SIZE; i++) 
    {
        Charta::RawImage24 chunkImage(CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);
        uint8_t* chunkPixelRawData = chunkImage.GetRawData();
        
        chunkPixelRawData[3 * i + 0] = 0xff;
        chunkPixelRawData[3 * i + 1] = 0xff;
        chunkPixelRawData[3 * i + 2] = 0xff;
        canvas.SetChunkAt(i, i, chunkImage);
    }



    for (int i = 0; i < 1000 / CANVAS_CHUNK_SIZE; i++)
    {
        Charta::RawImage24 chunkImage = canvas.GetChunkAt(i, i);
        uint8_t* chunkImageRawPixelData = chunkImage.GetRawData();

        CHECK(chunkImageRawPixelData[3 * i + 0] == 0xff);
        CHECK(chunkImageRawPixelData[3 * i + 1] == 0xff);
        CHECK(chunkImageRawPixelData[3 * i + 2] == 0xff);

    }
}