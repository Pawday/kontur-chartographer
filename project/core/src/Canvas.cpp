#include <utility>

#include <Poco/File.h>
#include <Poco/FileStream.h>

#include <Poco/JSON/Parser.h>

#include <charta/Canvas.hpp>
#include <charta/Bmp24RGB.hpp>
#include <charta/ImageConverter.hpp>



Charta::Canvas::Canvas(Poco::Path canvasContextDir) : _canvasContextDir(std::move(canvasContextDir))
{
    if (!Poco::File(this->_canvasContextDir).exists())
    {
        this->_exist = false;
        return;
    }

    if (!Poco::File(_canvasContextDir).isDirectory())
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

    if (!Poco::File(this->_canvasContextDir).exists())
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

void Charta::Canvas::Delete()
{
    if (!this->_exist) return;
    Poco::File(this->_canvasContextDir).remove(true);
}


Charta::RawImage24 Charta::Canvas::GetChunkAt(uint16_t x, uint16_t y)
{
    Poco::File chunkFile((std::stringstream() <<
    this->_canvasContextDir.toString() << Poco::Path::separator() << "chunk_" << x << "_" << y << ".bmp").str());

    if (false == chunkFile.exists())
        return RawImage24(CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);


    Poco::FileInputStream chunkFileIS(chunkFile.path());

    uint8_t* readBuff = new uint8_t[chunkFile.getSize()];
    chunkFileIS.read((char*) readBuff, chunkFile.getSize());
    chunkFileIS.close();

    Bmp24RGB bmpChunk(readBuff);
    delete[] readBuff;

    return Charta::ImageConverter::BmpToRaw(bmpChunk).GetUncroppedSubImage(0, 0, CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);
}

void Charta::Canvas::SetChunkAt(uint16_t x, uint16_t y, const RawImage24& chunkImage)
{
    if (this->_width <= x * CANVAS_CHUNK_SIZE) return;
    if (this->_height <= y * CANVAS_CHUNK_SIZE) return;

    Poco::File chunkFile((std::stringstream() <<
    this->_canvasContextDir.toString() << Poco::Path::separator() << "chunk_" << x << "_" << y << ".bmp").str());

    if (false == chunkFile.exists())
        chunkFile.createFile();

    Poco::FileOutputStream chunkFileOS(chunkFile.path());

    RawImage24 sizedChunkImage = chunkImage.GetUncroppedSubImage(0, 0, CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);

    Bmp24RGB chunkImageBmpVersion = ImageConverter::RawToBmp(sizedChunkImage);

    char* writeBuffer = new char[chunkImageBmpVersion.GetFullSize()];

    chunkImageBmpVersion.WriteToBuffer((uint8_t*)writeBuffer);

    chunkFileOS.write(writeBuffer, chunkImageBmpVersion.GetFullSize());

    chunkFileOS.flush();
    chunkFileOS.close();

    delete[] writeBuffer;

}

void Charta::Canvas::AppendImage(uint16_t xPos, uint16_t yPos, Charta::RawImage24& image)
{
    uint16_t affectedChunkTopLeftCoordX = xPos / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkTopLeftCoordY = yPos / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkBottomRightCoordX = (xPos + image.GetWidth()) / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkBottomRightCoordY = (yPos + image.GetHeight()) / CANVAS_CHUNK_SIZE;

    uint16_t modifiedChunkAmountHorizontal = (affectedChunkBottomRightCoordX - affectedChunkTopLeftCoordX + 1);
    uint16_t modifiedChunkAmountVertical = (affectedChunkBottomRightCoordY - affectedChunkTopLeftCoordY + 1);

    RawImage24 affectedImageAligned = this->GetImage(affectedChunkTopLeftCoordX * CANVAS_CHUNK_SIZE,
                                               affectedChunkTopLeftCoordY * CANVAS_CHUNK_SIZE,
                                               modifiedChunkAmountHorizontal * CANVAS_CHUNK_SIZE,
                                               modifiedChunkAmountVertical * CANVAS_CHUNK_SIZE);

    affectedImageAligned.MergeImage(xPos - affectedChunkTopLeftCoordX * CANVAS_CHUNK_SIZE,
                                    yPos - affectedChunkTopLeftCoordY * CANVAS_CHUNK_SIZE,
                                    image);

    RawImage24 affectedImageSized = affectedImageAligned.GetUncroppedSubImage(0,0, this->_width, this->_height);

    for (uint16_t chunkYPos = affectedChunkTopLeftCoordY;
         chunkYPos <= affectedChunkTopLeftCoordY + affectedChunkBottomRightCoordY; chunkYPos++)
        for (uint16_t chunkXPos = affectedChunkTopLeftCoordX;
             chunkXPos <= affectedChunkTopLeftCoordX + affectedChunkBottomRightCoordX; chunkXPos++)
        {
            RawImage24 chunkImage = affectedImageSized.GetUncroppedSubImage(
                    (chunkXPos - affectedChunkTopLeftCoordX) * CANVAS_CHUNK_SIZE,
                    (chunkYPos - affectedChunkTopLeftCoordY) * CANVAS_CHUNK_SIZE,
                    CANVAS_CHUNK_SIZE, CANVAS_CHUNK_SIZE);

            this->SetChunkAt(chunkXPos, chunkYPos, chunkImage);
        }
}

Charta::RawImage24 Charta::Canvas::GetImage(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height)
{
    uint16_t affectedChunkTopLeftCoordX = xPos / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkTopLeftCoordY = yPos / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkBottomRightCoordX = (xPos + width) / CANVAS_CHUNK_SIZE;
    uint16_t affectedChunkBottomRightCoordY = (yPos + height) / CANVAS_CHUNK_SIZE;

    RawImage24 retChunkAligned((affectedChunkBottomRightCoordX - affectedChunkTopLeftCoordX + 1) * CANVAS_CHUNK_SIZE,
                               (affectedChunkBottomRightCoordY - affectedChunkTopLeftCoordY + 1) * CANVAS_CHUNK_SIZE);


    for (uint16_t chunkYPos = affectedChunkTopLeftCoordY;
         chunkYPos <= affectedChunkTopLeftCoordY + affectedChunkBottomRightCoordY; chunkYPos++)
        for (uint16_t chunkXPos = affectedChunkTopLeftCoordX;
             chunkXPos <= affectedChunkTopLeftCoordX + affectedChunkBottomRightCoordX; chunkXPos++) 
        {
            RawImage24 chunkImage = this->GetChunkAt(chunkXPos, chunkYPos);
            retChunkAligned.MergeImage((chunkXPos - affectedChunkTopLeftCoordX) * CANVAS_CHUNK_SIZE,
                                       (chunkYPos - affectedChunkTopLeftCoordY) * CANVAS_CHUNK_SIZE, chunkImage);
        }

    return retChunkAligned.GetUncroppedSubImage(xPos - affectedChunkTopLeftCoordX * CANVAS_CHUNK_SIZE,
                                                yPos - affectedChunkTopLeftCoordY * CANVAS_CHUNK_SIZE, width, height);
}


