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

    rawInputImage = rawInputImage.GetUncroppedSubImage(0, 0, this->_width, this->_height);

    if (this->_yPos < 0 && this->_xPos < 0)
    {
        if (-this->_xPos > this->_width || -this->_yPos > this->_height)
        {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            response.send();
            return;
        }
        this->_width += this->_xPos;
        this->_height += this->_yPos;
        rawInputImage = rawInputImage.GetUncroppedSubImage(-this->_xPos, -this->_yPos, this->_width, this->_height);
        this->_xPos = 0;
        this->_yPos = 0;
    }

    if (this->_xPos < 0 && this->_yPos >= 0)
    {
        if (-this->_xPos > this->_width)
        {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            response.send();
            return;
        }
        this->_width -= (-this->_xPos);
        rawInputImage = rawInputImage.GetUncroppedSubImage(-this->_xPos, this->_yPos, this->_width, this->_height);
        this->_xPos = 0;
    }

    if (this->_yPos < 0 && this->_xPos >= 0)
    {
        if (-this->_yPos > this->_height)
        {
            response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
            response.send();
            return;
        }
        this->_height -= -(this->_yPos);
        rawInputImage = rawInputImage.GetUncroppedSubImage(this->_xPos, -this->_yPos, this->_width, this->_height);
        this->_yPos = 0;
    }

    this->_canvas.AppendImage(this->_xPos, this->_yPos, rawInputImage);

    response.setStatusAndReason(HTTPResponse::HTTP_OK);
    response.send();
}

Charta::ImageAppendHandler::ImageAppendHandler
(
    Charta::Canvas canvas,
    int32_t xPos, int32_t yPos,
    uint16_t width,
    uint16_t height
):
    _canvas(std::move(canvas)),
    _xPos(xPos),
    _yPos(yPos),
    _width(width),
    _height(height){}
                                                                                  