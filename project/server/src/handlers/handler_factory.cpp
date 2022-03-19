#include <unordered_map>
#include <utility>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/Path.h>
#include <Poco/NumberParser.h>

#include "handlers/handler_factory.h"
#include "handlers/not_found_handler.h"
#include "handlers/bad_request_handler.h"
#include "handlers/canvas_create_handler.h"
#include "handlers/append_image_handler.h"
#include "handlers/get_image_handler.h"

#include "charta/Canvas.hpp"
#include "charta/Bmp24RGB.hpp"

using namespace Charta;
using namespace Poco::Net;

Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    const Poco::URI &uri = Poco::URI(request.getURI());
    Poco::Path path(uri.getPath());

    const std::string& rootDir = path.directory(0);

    if (rootDir != "chartas") return new NotFoundHandler();

    const std::string& reqMethod = request.getMethod();

    if (reqMethod == "POST")
    {
        const Poco::URI::QueryParameters& reqParams = uri.getQueryParameters();
        if (reqParams.empty()) return new BadRequestHandler("request parameters are empty");

        if (reqParams.size() >= 2)
        {
            std::string widthStrParam;
            std::string heightStrParam;
            std::string xStrParam;
            std::string yStrParam;

            int heightParam;
            int widthParam;
            int xParam;
            int yParam;

            for (size_t i = 0, size = reqParams.size(); i < size; i++)
            {
                if (reqParams[i].first == "width") { widthStrParam = reqParams[i].second; continue; };
                if (reqParams[i].first == "height") {heightStrParam = reqParams[i].second; continue;};
                if (reqParams[i].first == "x") {xStrParam = reqParams[i].second; continue;};
                if (reqParams[i].first == "y") {yStrParam = reqParams[i].second; continue;};
            }

            if (widthStrParam.empty()) return new BadRequestHandler("width is not provided");
            if (heightStrParam.empty()) return new BadRequestHandler("height is not provided");

            if(false == Poco::NumberParser::tryParse(widthStrParam, widthParam))
                return new BadRequestHandler("Not valid width parameter");
            if(false == Poco::NumberParser::tryParse(heightStrParam, heightParam))
                return new BadRequestHandler("Not valid height parameter");
            
            if (path.directory(1).empty())
            {
                //creating new canvas
                if (heightParam > 50000) return new BadRequestHandler("height is too large");
                if (widthParam > 20000) return new BadRequestHandler("width is too large");

                if (heightParam < 1) return new BadRequestHandler("height is less than one");
                if (widthParam < 1) return new BadRequestHandler("width is less than one");

                Poco::Path newCanvasContextPath(this->_workingDirectory.string());

                std::string newCanvasID = Poco::UUIDGenerator().createRandom().toString();

                newCanvasContextPath.append(newCanvasID);

                const std::string &string = newCanvasContextPath.toString();
                
                Charta::Canvas newCanvas(newCanvasContextPath);
                newCanvas.Init((uint16_t) widthParam,(uint16_t) heightParam);

                return new Charta::CanvasCreatedHandler(newCanvasID);
            } else
            {
                //appending image
                const std::string& canvasID = path.directory(1);

                Poco::File canvasContextDir(Poco::Path(this->_workingDirectory.string()).append(canvasID));

                if (false == canvasContextDir.exists()) return new NotFoundHandler();

                Canvas canvas(canvasContextDir.path());

                if (false == canvas.Exist()) return new NotFoundHandler();

                if(false == Poco::NumberParser::tryParse(xStrParam, xParam))
                    return new BadRequestHandler("Not valid x parameter");
                if(false == Poco::NumberParser::tryParse(yStrParam, yParam))
                    return new BadRequestHandler("Not valid y parameter");

                if (heightParam < 1) return new BadRequestHandler("height is less than one");
                if (widthParam < 1) return new BadRequestHandler("width is less than one");

                return new ImageAppendHandler(canvas, xParam, yParam, widthParam, heightParam);
            }

        } else
        {
            return new BadRequestHandler("Not enough parameters for POST request");
        }

    }
    if (reqMethod == "GET")
    {
        std::string canvasID = path.directory(1);
        if (canvasID.empty()) return new BadRequestHandler("Canvas id not specified");

        Poco::File canvasContextDir(Poco::Path(this->_workingDirectory.string()).append(canvasID));

        if (false == canvasContextDir.exists()) return new NotFoundHandler();

        Canvas canvas(canvasContextDir.path());

        if (false == canvas.Exist()) return new NotFoundHandler();
        
        
        const Poco::URI::QueryParameters& reqParams = uri.getQueryParameters();
        if (reqParams.empty()) return new BadRequestHandler("request parameters are empty");

        if (reqParams.size() >= 4)
        {
            std::string widthStrParam;
            std::string heightStrParam;
            std::string xStrParam;
            std::string yStrParam;

            int heightParam;
            int widthParam;
            int xParam;
            int yParam;

            for (size_t i = 0, size = reqParams.size(); i < size; i++)
            {
                if (reqParams[i].first == "width") { widthStrParam = reqParams[i].second; continue; };
                if (reqParams[i].first == "height") { heightStrParam = reqParams[i].second; continue; };
                if (reqParams[i].first == "x") { xStrParam = reqParams[i].second; continue; };
                if (reqParams[i].first == "y") { yStrParam = reqParams[i].second; continue; };
            }

            if (widthStrParam.empty()) return new BadRequestHandler("width is not provided");
            if (heightStrParam.empty()) return new BadRequestHandler("height is not provided");
            if (xStrParam.empty()) return new BadRequestHandler("x is not provided");
            if (yStrParam.empty()) return new BadRequestHandler("y is not provided");

            if(false == Poco::NumberParser::tryParse(widthStrParam, widthParam))
                return new BadRequestHandler("Not valid width parameter");
            if(false == Poco::NumberParser::tryParse(heightStrParam, heightParam))
                return new BadRequestHandler("Not valid height parameter");
            if(false == Poco::NumberParser::tryParse(xStrParam, xParam))
                return new BadRequestHandler("Not valid x parameter");
            if(false == Poco::NumberParser::tryParse(yStrParam, yParam))
                return new BadRequestHandler("Not valid y parameter");

            if (widthParam < 1)
                return new BadRequestHandler("width is less than one");
            if (heightParam < 1)
                return new BadRequestHandler("height is less than one");

            if (xParam > 20000)
                return new BadRequestHandler("x is greater than 20000");

            if (yParam > 50000)
                return new BadRequestHandler("y is greater than 50000");

            return new GetImageHandler(canvas, (uint16_t) widthParam, heightParam, xParam, yParam);
        }
        else
        {
            return new BadRequestHandler("Not enough parameters for GET request");
        }

    }
    if (reqMethod == "DELETE") {}

	return new NotFoundHandler();
}

HandlerFactory::HandlerFactory(std::filesystem::path workingDirectory) : _workingDirectory(std::move(workingDirectory)) {}