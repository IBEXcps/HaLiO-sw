/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <Arduino.h>
#include "ota.h"
#include "display.h"

uint Ota::port = 5222;

Ota::Ota()
{
    debug("Starting OTA.");

    ArduinoOTA.setPort(port);
    ArduinoOTA.onStart([ = ]() {
        Display::self().ota(START, data->otaProgress);
    });
    ArduinoOTA.onEnd([ = ]() {
        Display::self().ota(FINISHED, data->otaProgress);
    });
    ArduinoOTA.onProgress([ = ](unsigned int progress, unsigned int total) {
        data->otaProgress = progress / (total / 100);
        Display::self().ota(DOWNLOADING, data->otaProgress);
    });
    ArduinoOTA.onError([ = ](ota_error_t error) {
        Display::self().ota(ERROR, data->otaProgress, error);
    });
    ArduinoOTA.begin();
}

void Ota::setData(dataStruct* d)
{
    data = d;
}

void Ota::handle()
{
    ArduinoOTA.handle();
}

Ota& Ota::self()
{
    static Ota self;
    return self;
}

Ota::~Ota()
{
}
