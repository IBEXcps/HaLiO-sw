/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <vector>
#include "math.h"
#include "display.h"
#include "debug.h"

std::vector<String> Display::strings = {
    "------------ OTA ------------",
    "Starting...",
    "Downloading...",
    "DONE !",
    "System will restart.",
    "------------ OTA ------------",
};

Display::Display() :
    // Initialize the OLED display using brzo_i2c
    // D3 -> SDA
    // D5 -> SCL
    display(new SSD1306Brzo(0x3c, D3, D5)),
    delayBetweenFrames(5),
    targetFps(20)
{
    debug("Starting Display.");
    // Initialising the UI will init the display too.
    display->init();

    display->flipScreenVertically();
    display->setFont(ArialMT_Plain_10);
}

void Display::setDisplayFps(const float fps)
{
    targetFps = fps;
}

void Display::setDisplayDelay(const uint delay)
{
    delayBetweenFrames = delay;
}

void Display::displayData()
{
    display->clear();

    String rssi = String(data->actualRSSI) + " dB";

    String wunit = " Wh";
    if (((int)data->averageConsumption)%1000)
        wunit = " kWh";
    String wh = String(data->averageConsumption/1000) + wunit;
    fps = 0.97*fps + 0.03*1e3/(millis() - lastMillis);
    lastMillis = millis();

    std::vector<String> strings;
    strings.push_back(String(fps) + " FPS");
    strings.push_back(String(data->actualCurrent) + " A");
    strings.push_back(String(data->actualConsumption) + " Wh'");
    strings.push_back(wh);
    strings.push_back(String(data->upTime/1000.0) + " s");
    strings.push_back(data->relayStatus ? "ON" : "OFF");

    uint pos = 0;
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    for (auto string: strings)
        display->drawString(0, 10*pos++, string);
    strings.clear();

    strings.push_back(data->nodeId);
    strings.push_back(data->nodeName);
    strings.push_back(data->hostName);
    strings.push_back(data->networkName);
    strings.push_back(data->networkIP);
    strings.push_back(rssi);

    pos = 0;
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    for (auto string: strings)
        display->drawString(127, 10*pos++, string);

    // write the buffer to the display
    display->display();    
}

void Display::displayDataMin()
{
    display->clear();

    display->setFont(ArialMT_Plain_10);
    String wunit = " Wh";
    if (((int)data->averageConsumption)%1000)
        wunit = " kWh";
    String wh = String(data->averageConsumption/1000) + wunit;
    fps = 0.97*fps + 0.03*1e3/(millis() - lastMillis);
    lastMillis = millis();

    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);

    display->setFont(ArialMT_Plain_16);
    display->drawString(64, 10, data->nodeName);
    display->setFont(ArialMT_Plain_10);
    display->drawString(64, 24, data->relayStatus ? "ON" : "OFF");
    display->setFont(ArialMT_Plain_24);
    display->drawString(64, 40, wh);

    display->setFont(ArialMT_Plain_10);
    // write the buffer to the display
    display->display();    
}

void Display::displayLogo()
{
    display->clear();
    display->drawXbm(0,0, HaLiO_Logo_inv_width, HaLiO_Logo_inv_height, HaLiO_Logo_inv_bits);
    display->display();
}

void Display::setData(dataStruct* d)
{
    data = d;
}

void Display::sine(unsigned x, unsigned y)
{
    static long sinX = 0;
    for (int i=0; i<128-x; i++)
    {
        float t = ((float)i* 2.0 * PI + sinX++)/(128-x);
        display->setPixel(x+i, y + int(y*sin(t)));
    }
    display->drawHorizontalLine(x, y, 128-x);
}

void Display::ota(Ota::States state, float progress, ota_error_t error)
{
    uint pos = 0;
    Display::self().display->clear();
    Display::self().display->setTextAlignment(TEXT_ALIGN_CENTER);
    switch (state) {
        case Ota::FINISHED:
            pos = 4;
            Display::self().display->drawString(64, 10*pos, strings[pos]); pos--;
            Display::self().display->drawString(64, 10*pos, strings[pos]);
        case Ota::DOWNLOADING:
            pos = 2;
            Display::self().display->drawString(64, 10*pos, strings[pos]);
        case Ota::START:
            pos = 1;
            if(progress)
                Display::self().display->drawLine(0, 0, progress*127/100, 0);
            Display::self().display->drawString(64, 10*pos, strings[pos]); pos--;
            Display::self().display->drawString(64, 10*pos, strings[pos]);
            pos = 5;
            Display::self().display->drawString(64, 10*pos, strings[pos]);
            Display::self().display->display();
            debug("OTA: Progress: %.1f", progress);
            Display::self().display->display();
            break;
        case Ota::ERROR:
        {
            pos = 4;
            String e = "ERROR ";
            e += String(error);
            if (error == OTA_AUTH_ERROR) {
                e += " Auth Failed.";
            } else if (error == OTA_BEGIN_ERROR) {
                e += " OTA: Begin Failed.";
            } else if (error == OTA_CONNECT_ERROR) {
                e += " OTA: Connect Failed.";
            } else if (error == OTA_RECEIVE_ERROR) {
                e += " OTA: Receive Failed.";
            } else if (error == OTA_END_ERROR) {
                e += " OTA: End Failed.";
            };
            Display::self().display->drawString(64, 10*pos, e);
            Display::self().display->display();
            debug("%s", e.c_str());
            delay(10000);
            break;
        }
        default:
            break;
    }
}

Display& Display::self()
{
    static Display self;
    return self;
}

Display::~Display()
{
}
