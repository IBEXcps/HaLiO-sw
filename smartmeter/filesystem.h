/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

// http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html
#include "FS.h"

class FileSystem
{
public:
    void debugInfo();
    void format();
    void start();

    // https://github.com/esp8266/Arduino/blob/da4cab5b276d3acb9d64d38adae77da1f597b3b4/cores/esp8266/FS.h
    static fs::FS FS;

    static FileSystem& self();
    ~FileSystem();

private:
    FileSystem& operator = (FileSystem& other) = delete;
    FileSystem(const FileSystem& other) = delete;
    FileSystem();

    void error(String message);

    static const char* _lockFileName;
    FSInfo _fileSystemInfo;
    bool _fileSystemOk;
};
