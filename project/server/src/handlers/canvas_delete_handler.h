#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include "charta/Canvas.hpp"

namespace Charta
{
    class CanvasDeleteHandler : public Poco::Net::HTTPRequestHandler
    {
    public:
        CanvasDeleteHandler(Canvas canvas);

        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        Charta::Canvas _canvas;
    };
}