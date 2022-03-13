#pragma once

#include <Poco/File.h>
#include <Poco/Path.h>
#include <charta/RawImage24.hpp>

#define CANVAS_CONTEXT_INFO_FILE_NAME ".canvasInfo"

namespace Charta
{
    class Canvas
    {
    public:
        explicit Canvas(Poco::Path canvasContextDir);

        bool Exist();

        void Init(uint16_t width, uint16_t height);

        void Delete();

        void AppendImage(RawImage24 image, uint16_t xPos, uint16_t yPos);

        uint16_t GetWidth() const;

        uint16_t GetHeight() const;

    private:
        void LoadInfo();

        Poco::File _canvasContextInfoFile;
        Poco::Path _canvasContextDir;
        bool _exist;
        uint16_t _width;
        uint16_t _height;
    };
}