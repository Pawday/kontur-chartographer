#include "chartographer_application.h"
#include "handlers/handler_factory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/OptionSet.h"

using namespace Charta;
using namespace Poco::Net;
using namespace Poco::Util;

void ChartographerApplication::defineOptions(OptionSet& options)
{
	ServerApplication::defineOptions(options);
	this->setUnixOptions(true);

	options.addOption(
		Option{ "folder", "f", "working folder" }
			.required(true)
			.repeatable(false)
			.argument("FOLDER")
			.callback(OptionCallback<ChartographerApplication>{ this, &ChartographerApplication::handle_working_folder })
	);
}

void ChartographerApplication::initialize(Application& app)
{
	ServerApplication::initialize(app);
    create_directories(this->working_folder_);
}

int ChartographerApplication::main(const std::vector<std::string>& args)
{
	ServerSocket socket{ 8080 };
	HTTPServer server{ new Charta::HandlerFactory(this->working_folder_), socket, new HTTPServerParams{} };
	server.start();
	waitForTerminationRequest();
	server.stop();

	return ExitCode::EXIT_OK;
}

void ChartographerApplication::handle_working_folder(const std::string& name, const std::string& value)
{
    this->working_folder_ = value;
}

