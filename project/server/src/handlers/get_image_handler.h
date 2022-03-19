#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include "charta/Canvas.hpp"

namespace Charta
{
    class GetImageHandler: public Poco::Net::HTTPRequestHandler
    {
    public:
        GetImageHandler(Canvas canvas, uint16_t width, uint16_t height, int xPos, int yPos);

    private:
        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        Canvas _canvas;
        uint16_t _width;
        uint16_t _height;
        int _xPos;
        int _yPos;
    };
}