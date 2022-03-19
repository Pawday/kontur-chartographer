#include <Poco/Net/HTTPServerResponse.h>

#include <utility>

#include "canvas_delete_handler.h"

using namespace Poco::Net;


Charta::CanvasDeleteHandler::CanvasDeleteHandler(Charta::Canvas canvas) : _canvas(std::move(canvas)) {}

void Charta::CanvasDeleteHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    this->_canvas.Delete();
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
    response.send();
}
