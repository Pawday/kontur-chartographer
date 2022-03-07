#include <cstdint>

#include <Poco/FileStream.h>
#include <Poco/JSON/Parser.h>

#include "charta/Canvas.hpp"



Charta::Canvas::Canvas(uint16_t width, uint16_t height) : width(width), height(height)
{
    if (width > 20000) throw std::out_of_range("Canvas width cannot be greater than 20000 px");
    if (height > 50000) throw std::out_of_range("Canvas height cannot be greater than 50000 px");

    _uuid = Poco::UUID();
}



Charta::Canvas::~Canvas()
{
    
}

Charta::Canvas::Canvas(Poco::Path& canvasDirectory)
{
    Poco::FileInputStream canvasInfoFileStream(canvasDirectory.toString() + Poco::Path::separator() + CHARTA_CANVAS_INFO_FILE_NAME);
    
    Poco::JSON::Parser parser;

    Poco::JSON::Object::Ptr canvasInfoJsonObj = parser.parse(canvasInfoFileStream).extract<Poco::JSON::Object::Ptr>();

    this->width = canvasInfoJsonObj->getValue<uint16_t>("CanvasWidth");
    this->height = canvasInfoJsonObj->getValue<uint16_t>("CanvasHeight");
    this->_uuid = canvasInfoJsonObj->getValue<Poco::UUID>("CanvasUUID");

    canvasInfoFileStream.close();
}

const Poco::UUID &Charta::Canvas::getUuid() const
{
    return _uuid;
}
