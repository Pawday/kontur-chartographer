#include <Poco/Net/HTTPServerResponse.h>

#include <utility>

#include "canvas_create_handler.h"

using namespace Poco::Net;

void Charta::CanvasCreatedHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    response.setStatusAndReason(HTTPResponse::HTTP_CREATED);
    response.send() << this->_canvasStringId;
}

Charta::CanvasCreatedHandler::CanvasCreatedHandler(std::string canvasStringId) :
_canvasStringId(std::move(canvasStringId)) {}
