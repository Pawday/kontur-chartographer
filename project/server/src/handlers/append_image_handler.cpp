#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <utility>

#include "charta/Bmp24RGB.hpp"
#include "charta/ImageConverter.hpp"

#include "append_image_handler.h"

using namespace Poco::Net;

void Charta::ImageAppendHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    std::istream &istream = request.stream();

    size_t contentLength = request.getContentLength();

    uint8_t* buff = new uint8_t[contentLength];
    istream.read((char*) buff, contentLength);

    Charta::Bmp24RGB inputBmp(buff);
    delete[] buff;


    RawImage24 rawInputImage = Charta::ImageConverter::BmpToRaw(inputBmp);

    this->_canvas.AppendImage(this->_xPos, this->_yPos, rawInputImage);

    response.setStatus(HTTPResponse::HTTP_OK);
    response.send();
}

Charta::ImageAppendHandler::ImageAppendHandler
(
    Charta::Canvas canvas,
    uint16_t xPos, uint16_t yPos,
    uint16_t width,
    uint16_t height
):
    _canvas(std::move(canvas)),
    _xPos(xPos),
    _yPos(yPos),
    _width(width),
    _height(height){}
                                                                                  