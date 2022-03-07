#pragma once

#include <cstdint>
#include <stdexcept>

#include <Poco/UUID.h>
#include <Poco/Path.h>

#define CHARTA_CANVAS_INFO_FILE_NAME ".canvasInfo"

namespace Charta
{
    class Canvas
    {
    public:
        Canvas(uint16_t width, uint16_t height);
        Canvas(Poco::Path& canvasDirectory);
        ~Canvas();

        const Poco::UUID &getUuid() const;

    private:
        uint16_t width;
        uint16_t height;
        Poco::UUID _uuid;
    };
}