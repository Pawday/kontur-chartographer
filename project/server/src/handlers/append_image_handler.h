#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include "charta/Canvas.hpp"

namespace Charta
{
    class ImageAppendHandler : public Poco::Net::HTTPRequestHandler
    {
    public:
        ImageAppendHandler(Canvas canvas, int32_t xPos, int32_t yPos, uint16_t width, uint16_t height);

    public:
        Charta::Canvas _canvas;
        int32_t _xPos;
        int32_t _yPos;
        uint16_t _width;
        uint16_t _height;

        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    };
}