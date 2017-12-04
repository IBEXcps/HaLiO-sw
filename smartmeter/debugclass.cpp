/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "debugclass.h"

Debug::Debug()
{
}

void Debug::print()
{
    Serial.print(debugBuffer);
    debugMsgs.push_back(debugBuffer);
    if (debugMsgs.size() > 50) debugMsgs.pop_back();
}

Debug& Debug::self()
{
    static Debug self;
    return self;
}

Debug::~Debug()
{
}
