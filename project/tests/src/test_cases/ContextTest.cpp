#include "catch2/catch.hpp"

#include <charta/Context.hpp>

void CheckContextInfoFileInitialised(const Poco::Path& contextDirectory)
{
    //TODO: check json content

    Poco::File contextInfoFile(contextDirectory.getFileName() + Poco::Path::separator() + CHARTA_CONTEXT_INFO_FILE_NAME);
    CHECK(contextInfoFile.exists());
    CHECK(contextInfoFile.isFile());
    CHECK(contextInfoFile.getSize() != 0);
}

TEST_CASE("context_create_and_init_directory", "[context]")
{
    Poco::Path nonExitingDirectory("test_context_create_and_init_directory_temp");

    Poco::File nonExitingDirectoryFile(nonExitingDirectory);

    if (nonExitingDirectoryFile.exists())
        nonExitingDirectoryFile.remove(true);

    CHECK(not nonExitingDirectoryFile.exists());
    Charta::Context context(nonExitingDirectory);
    CHECK(nonExitingDirectoryFile.exists());

    CheckContextInfoFileInitialised(nonExitingDirectory);

    nonExitingDirectoryFile.remove(true);
}

TEST_CASE("context_create_in_existed_empty_directory", "[context]")
{
    Poco::Path exitingDirectory("test_context_create_in_empty_directory_temp");

    Poco::File exitingDirectoryFile(exitingDirectory);

    if (exitingDirectoryFile.exists())
        exitingDirectoryFile.remove();

    exitingDirectoryFile.createDirectory();

    CHECK(exitingDirectoryFile.exists());
    Charta::Context context(exitingDirectory);

    CheckContextInfoFileInitialised(exitingDirectory);

    exitingDirectoryFile.remove(true);
}

TEST_CASE("context_create_in_existed_file", "[context]")
{
    Poco::Path exitingFilePath("test_context_create_in_existed_file.temp");

    Poco::File exitingFile(exitingFilePath);

    if (exitingFile.exists())
    {
        if (exitingFile.isDirectory())
            exitingFile.remove(true);
    }

    exitingFile.createFile();

    CHECK(exitingFile.exists());
    CHECK(exitingFile.isFile());
    CHECK_THROWS(Charta::Context(exitingFilePath));

    exitingFile.remove();
}