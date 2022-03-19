#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace Charta
{
    class CanvasCreatedHandler : public Poco::Net::HTTPRequestHandler
    {

    public:
        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        CanvasCreatedHandler(std::string canvasStringId);

    private:
        std::string _canvasStringId;
    };


}