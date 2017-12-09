/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <vector>

#include <Arduino.h>

#include "debug.h"
#include "filesystem.h"

fs::FS FileSystem::FS = SPIFFS;
const char* FileSystem::_lockFileName = "/.fileSystem";

FileSystem::FileSystem():
    _fileSystemOk(false)
{
    FS.info(_fileSystemInfo);
}

void FileSystem::start()
{
    debug("Starting file system...");
    if (!FS.begin()) {
        _fileSystemOk = false;
        error("Failed to start file system !");
        return;
    }
    debug("Done !");

    debug("Checking for file system lock...");
    if (!FS.exists(_lockFileName)) {
        debug("Creating new file system..");
        debug("It'll take some time !");
        if (!FS.format()) {
            _fileSystemOk = false;
            error("Failed to format file system !");
            return;
        }

        debug("Creating file system lock...");
        File lockFile = FS.open(_lockFileName, "w");
        if (!lockFile) {
            _fileSystemOk = false;
            error("Failed creating file system lock !");
            return;
        }
        lockFile.write(0x00, 1);
        lockFile.close();
    }
    _fileSystemOk = true;
    debug("Done !");

    debug("File system setup finished normally.");
}

void FileSystem::debugInfo()
{
    std::vector<String> strings;
    strings.push_back("MEMORY:");
    strings.push_back("\t totalBytes: " + String(_fileSystemInfo.totalBytes));
    strings.push_back("\t usedBytes: " + String(_fileSystemInfo.usedBytes));
    strings.push_back("\t blockSize: " + String(_fileSystemInfo.blockSize));
    strings.push_back("\t pageSize: " + String(_fileSystemInfo.pageSize));
    strings.push_back("\t maxOpenFiles: " + String(_fileSystemInfo.maxOpenFiles));
    strings.push_back("\t maxPathLength: " + String(_fileSystemInfo.maxPathLength));

    for (const auto string : strings) {
        debug("%s", string.c_str());
    }
}

void FileSystem::error(String message)
{
    if(!message.length()) {
        debug("%s", message.c_str());
    }
    debug("Failed ! The sytem will not be able to work properly !");
}

void FileSystem::format()
{
    debug("Deleting file system lock...");
    if (!FS.remove(_lockFileName)) {
        debug("Failed deleting file system lock !");
        return;
    }
    start();
}

FileSystem& FileSystem::self()
{
    static FileSystem self;
    return self;
}

FileSystem::~FileSystem()
{
}
