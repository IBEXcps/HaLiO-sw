/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include "data.h"
#include "debug.h"

class Ota
{
public:
    void setData(dataStruct* d);
    void handle();

    enum States {
        START,
        DOWNLOADING,
        FINISHED,
        ERROR,
    };

    static Ota& self();
    ~Ota();

private:
    Ota& operator = (Ota& other) = delete;
    Ota(const Ota& other) = delete;
    Ota();

    dataStruct* data;
    static uint port;
};
