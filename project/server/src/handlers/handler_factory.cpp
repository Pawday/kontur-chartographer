#include "handlers/handler_factory.h"
#include "handlers/not_found_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"

using namespace charta;
using namespace Poco::Net;

Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	return new NotFoundHandler();
}
