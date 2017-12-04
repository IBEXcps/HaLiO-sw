/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <vector>
#include <Arduino.h>

class Debug
{
public:
    static Debug& self();
    ~Debug();

    void print();

    char debugBuffer[512];
    std::vector<String> debugMsgs;

private:
    Debug& operator = (Debug& other) = delete;
    Debug(const Debug& other) = delete;
    Debug();
};
