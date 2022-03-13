#include <utility>

#include <Poco/File.h>
#include <Poco/FileStream.h>

#include <Poco/JSON/Parser.h>

#include <charta/Canvas.hpp>



Charta::Canvas::Canvas(Poco::Path canvasContextDir) : _canvasContextDir(std::move(canvasContextDir))
{
    if (not Poco::File(this->_canvasContextDir).exists())
    {
        this->_exist = false;
        return;
    }

    if (not Poco::File(_canvasContextDir).isDirectory())
    {
        Poco::File(this->_canvasContextDir).remove();
        this->_exist = false;
        return;
    }

    this->_canvasContextInfoFile =
            Poco::Path(this->_canvasContextDir.toString() + Poco::Path::separator() + CANVAS_CONTEXT_INFO_FILE_NAME);

    if (this->_canvasContextInfoFile.exists())
    {
        this->_exist = true;
        this->LoadInfo();
    }
    else
        this->_exist = false;
}

void Charta::Canvas::LoadInfo() 
{
    size_t fileSize = this->_canvasContextInfoFile.getSize();
    char* buff = new char[fileSize + 1];
    
    Poco::FileInputStream is(this->_canvasContextInfoFile.path());
    
    is.read(buff, fileSize);

    buff[fileSize] = 0;

    std::string fileContent(buff);

    delete[] buff;

    Poco::JSON::Parser parser;

    const Poco::Dynamic::Var &var = parser.parse(fileContent);

    Poco::JSON::Object::Ptr fileInfoJsonObj = var.extract<Poco::JSON::Object::Ptr>();

    fileInfoJsonObj->get("CanvasWidth").convert(this->_width);
    fileInfoJsonObj->get("CanvasHeight").convert(this->_height);

}

bool Charta::Canvas::Exist()
{
    return this->_exist;
}

void Charta::Canvas::Init(uint16_t width, uint16_t height)
{
    if (this->_exist) return;
    
    this->_exist = true;

    if (not Poco::File(this->_canvasContextDir).exists())
    {
        Poco::File(this->_canvasContextDir).createDirectory();
        this->_canvasContextInfoFile =
            Poco::Path(this->_canvasContextDir.toString() + Poco::Path::separator() + CANVAS_CONTEXT_INFO_FILE_NAME);
    }

    this->_canvasContextInfoFile.createFile();

    Poco::JSON::Object canvasContextJsonObj;

    canvasContextJsonObj.set("CanvasWidth", width);
    canvasContextJsonObj.set("CanvasHeight", height);

    Poco::FileOutputStream os(this->_canvasContextInfoFile.path());

    canvasContextJsonObj.stringify(os, 1);

    os.close();

    this->_height = height;
    this->_width = width;
}

uint16_t Charta::Canvas::GetWidth() const
{
    return _width;
}

uint16_t Charta::Canvas::GetHeight() const
{
    return _height;
}
