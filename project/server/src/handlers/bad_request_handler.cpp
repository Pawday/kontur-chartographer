#include "bad_request_handler.h"

#include <utility>
#include "Poco/Net/HTTPServerResponse.h"

using namespace Charta;
using namespace Poco::Net;

void Charta::BadRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    if (false == this->_message.empty())
        response.send() << this->_message;
    else
        response.send();
}
BadRequestHandler::BadRequestHandler(std::string message) : _message(std::move(message)) {}
