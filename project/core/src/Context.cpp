#pragma clang diagnostic push
#include <stdexcept>
#include <algorithm>

#include <charta/Context.hpp>
#include <utility>

#include <Poco/URI.h>
#include <Poco/FileStream.h>
#include <Poco/Path.h>
#include <Poco/JSON/Parser.h>



Charta::Context::Context(Poco::Path workingDirectory) : _workingDirectory(std::move(workingDirectory))
{
    Poco::File workingDirectoryFile = Poco::File(_workingDirectory);
    if (not workingDirectoryFile.exists())
    {
        workingDirectoryFile.createDirectory();
        this->InitDirectory();
        return;
    }

    if (not workingDirectoryFile.isDirectory()) throw std::invalid_argument("workingDirectory arg must be a Directory");

    if (not Context::IsDirectoryInitialised(workingDirectoryFile))
    {
        this->InitDirectory();
        return;
    }


    this->InitContextInfoFile();
    this->_canvasList = std::vector<Canvas>();
    this->Load();
}

bool Charta::Context::IsDirectoryInitialised(const Poco::File& directory)
{
    std::vector<Poco::File> fileList;
    directory.list(fileList);

    return std::any_of(fileList.begin(), fileList.end(), [](auto& item)
    {
        return item.isFile() && Poco::Path(item.path()).getFileName() == CHARTA_CONTEXT_INFO_FILE_NAME;
    });
}

void Charta::Context::InitDirectory()
{
    this->InitContextInfoFile();
}

void Charta::Context::Load()
{
    Poco::FileInputStream infoFileStream(this->_contextInfoFile.absolute().toString());

    Poco::JSON::Parser parser;

    const Poco::JSON::Object::Ptr jsonInfoContent = parser.parse(infoFileStream).extract<Poco::JSON::Object::Ptr>();

    const Poco::JSON::Array::Ptr& canvasJsonList = jsonInfoContent->getArray("CanvasList");
    
    this->_canvasList.clear();

    for (size_t i = 0, length = canvasJsonList->size(); i < length; i++) 
    {
        const Poco::SharedPtr<Poco::JSON::Object>& canvasObject = canvasJsonList->getObject(i);

        const std::string &canvasFolderString = canvasObject->getValue<std::string>("CanvasFolder");

        Poco::Path canvasFolderPath(this->_workingDirectory);
        canvasFolderPath.append(canvasFolderString);

        _canvasList.emplace_back(canvasFolderPath);
    }

    infoFileStream.close();
}

void Charta::Context::InitContextInfoFile()
{
    Poco::File contextInfoFile = Poco::Path(_workingDirectory.absolute().toString() + Poco::Path::separator() + CHARTA_CONTEXT_INFO_FILE_NAME);
    if (not contextInfoFile.exists())
    {
        Poco::JSON::Object contextInfoJsonObj;

        Poco::JSON::Array CanvasListJsonArray;
        contextInfoJsonObj.set("CanvasList", CanvasListJsonArray);
        contextInfoFile.createFile();
        Poco::FileOutputStream contextInfoFileOS(contextInfoFile.path());
        contextInfoJsonObj.stringify(contextInfoFileOS);
        contextInfoFileOS.close();
    }

    this->_contextInfoFile = Poco::Path(contextInfoFile.path());
}

const std::vector<Charta::Canvas> &Charta::Context::getCanvasList() const
{
    return _canvasList;
};