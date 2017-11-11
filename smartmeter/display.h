/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <vector>

#include "SSD1306Brzo.h"
#include "SH1106Brzo.h"
#include "OLEDDisplayUi.h"
#include "images.h"
#include "data.h"
#include "ota.h"

class Display
{
public:

    void displayData();
    void displayDataMin();
    void displayLogo();
    void run();
    void setDisplayFps(const float fps);
    void setDisplayDelay(const uint delay);
    void ota(Ota::States state, float progress, ota_error_t error = OTA_BEGIN_ERROR);

    void setData(dataStruct* d);

    static Display& self();

    ~Display();

    SSD1306Brzo* display;
    OLEDDisplayUi* ui;

    uint targetFps;
    uint delayBetweenFrames;

private:
    void sine(unsigned x = 64, unsigned y = 16);
    Display& operator = (Display& other) = delete;
    Display(const Display& other) = delete;
    Display();

    dataStruct* data;
    float fps;
    long lastMillis = 0;
    static std::vector<String> strings;
    std::vector<void (*)()> screens;
};
