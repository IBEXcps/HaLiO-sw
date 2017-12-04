/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#define DEBUG

#ifdef DEBUG
#include "debugclass.h"

#define debug(fmt, args ...)  do { \
        sprintf(Debug::self().debugBuffer, "[%s:%d]: " fmt "\n\r", __PRETTY_FUNCTION__, __LINE__, ## args); \
        Debug::self().print(); \
    } while(0)
#else
#define debug(fmt, args ...)
#endif
