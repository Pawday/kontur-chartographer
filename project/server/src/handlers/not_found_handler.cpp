#include "handlers/not_found_handler.h"
#include "Poco/Net/HTTPServerResponse.h"

using namespace Charta;
using namespace Poco::Net;

void NotFoundHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.send();
}
