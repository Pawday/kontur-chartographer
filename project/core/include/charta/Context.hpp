#pragma once

#include <Poco/File.h>

#include "charta/Canvas.hpp"

#define CHARTA_CONTEXT_INFO_FILE_NAME ".chartaContextInfo"

namespace Charta
{
    class Context
    {
    public:
        /**
         * Creates folder if it not exist or and initialise the Context
         * Also initialised the Context if folder exits and empty
         * or
         * Loads Charta::Context from folder if it initialised
         * @param workingDirectory
         */
        explicit Context(Poco::Path  workingDirectory);

        /**
         * Update Context from file System
         */
        void Load();

        /**
         * Save Context to file system
         */
        void Save();

        const std::vector<Charta::Canvas> &getCanvasList() const;

    private:
        void InitDirectory();
        void InitContextInfoFile();
        static bool IsDirectoryInitialised(const Poco::File& directory);

    private:
        Poco::Path _workingDirectory;
        Poco::Path _contextInfoFile;
        std::vector<Charta::Canvas> _canvasList;
    };
}