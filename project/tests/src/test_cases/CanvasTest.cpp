#include <catch2/catch.hpp>

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>

#include "charta/Canvas.hpp"


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

    CHECK(!Poco::File(canvasContextDirNotExistedDir).exists());
    Charta::Canvas canvas(canvasContextDirNotExistedDir);

    CHECK(!canvas.Exist());
    canvas.Init(100, 100);
    CHECK(Poco::File(canvasContextDirNotExistedDir).exists());
    CHECK(canvas.Exist());

    //_______________________________________________________________________

    Poco::Path canvasContextDirExisted("canvas_create_test_existed_dir");

    deleteAnyThenCreateDir(canvasContextDirExisted);

    CHECK(Poco::File(canvasContextDirExisted).exists());
    Charta::Canvas canvas2(canvasContextDirExisted);

    CHECK(Poco::File(canvasContextDirExisted).exists());
    CHECK(!canvas2.Exist());
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