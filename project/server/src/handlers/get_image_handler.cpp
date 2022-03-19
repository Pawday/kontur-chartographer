#include "get_image_handler.h"

#include <utility>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/MediaType.h>

#include "charta/Bmp24RGB.hpp"
#include "charta/ImageConverter.hpp"

using namespace Poco::Net;

Charta::GetImageHandler::GetImageHandler
(
    Charta::Canvas canvas,
    uint16_t width,
    uint16_t height,
    int xPos,
    int yPos
) :
_canvas(std::move(canvas)),
_width(width),
_height(height),
_xPos(xPos),
_yPos(yPos) {}

static void SendNoIntersectionError(HTTPServerResponse &response)
{
    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
    response.send() << "no intersection";
}

void Charta::GetImageHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{

    if (this->_xPos < 0 || this->_yPos < 0)
    {
        if (this->_width + this->_xPos < 0 || this->_height + this->_yPos < 0)
        {
            SendNoIntersectionError(response);
            return;
        }
        uint16_t canvasHeight = this->_canvas.GetHeight();
        uint16_t canvasWidth = this->_canvas.GetWidth();

        if (this->_xPos > canvasWidth || this->_yPos > canvasHeight)
        {
            SendNoIntersectionError(response);
            return;
        }

        RawImage24 retImage(this->_width, this->_height);

        if (this->_xPos < 0 && this->_yPos >= 0)
            retImage.MergeImage(
                    -this->_xPos,
                    0,
                    this->_canvas.GetImage(0, this->_yPos, this->_width + this->_xPos, this->_height));

        if (this->_xPos >= 0 && this->_yPos < 0)
        {
            retImage.MergeImage(
                    0,
                    -this->_yPos,
                    this->_canvas.GetImage(this->_xPos, 0, this->_width, this->_height + this->_yPos));
        }

        if (this->_xPos < 0 && this->_yPos < 0)
        {
            retImage.MergeImage(
                    -this->_xPos,
                    -this->_yPos,
                    this->_canvas.GetImage(0, 0, this->_width + this->_xPos, this->_height + this->_yPos));
        }

        Charta::Bmp24RGB bmpRes = Charta::ImageConverter::RawToBmp(retImage);
        uint32_t bmpSize = bmpRes.GetFullSize();
        uint8_t* buffer = new uint8_t[bmpSize];
        bmpRes.WriteToBuffer(buffer);
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("image/bmp");
        response.send().write((char*) buffer, bmpSize);
        delete[] buffer;
        return;
    }


    RawImage24 retRawImage = this->_canvas.GetImage(this->_xPos, this->_yPos, this->_width, this->_height);

    Charta::Bmp24RGB bmpRes = Charta::ImageConverter::RawToBmp(retRawImage);
    uint32_t bmpSize = bmpRes.GetFullSize();
    uint8_t* buffer = new uint8_t[bmpSize];
    bmpRes.WriteToBuffer(buffer);
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("image/bmp");
    response.send().write((char*) buffer, bmpSize);
    delete[] buffer;
}
