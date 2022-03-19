#pragma once
#include "Poco/Net/HTTPRequestHandler.h"

namespace Charta
{
    class BadRequestHandler : public Poco::Net::HTTPRequestHandler
    {
    public:
        explicit BadRequestHandler(std::string message);
        BadRequestHandler() = default;
        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    private:
        std::string _message;
    };
}
