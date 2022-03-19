#pragma once
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "chartographer_application.h"

namespace Charta
{
	class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

	private:
        std::filesystem::path _workingDirectory;
    public:
        HandlerFactory(std::filesystem::path workingDirectory);
    };
}
