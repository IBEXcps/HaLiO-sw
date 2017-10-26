/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <ESP8266mDNS.h>
#include <Thread.h>
#include <ThreadController.h>

#include "debug.h"
#include "display.h"
#include "client.h"
#include "connection.h"
#include "data.h"
#include "ota.h"

#define RED_LED D6
#define YELLOW_LED D7
#define GREEN_LED D4
#define RELAY D2
#define CURRENT_SENSOR A0
#define BUTTON D5

void ledTest()
{
    static uint id = 0;
    switch(id++)
    {
       case(0):
          digitalWrite(RED_LED, !digitalRead(RED_LED));
          break;
       case(1):
          digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
          break;
       case(2):
          digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
          break;
       default:
          break;
    }
    id = id%3;
}

float readCurrent()
{
    static float current = 0;

    Data::self().data->actualCurrent = (analogRead(CURRENT_SENSOR)-450.5)/450.5*30.0;
    current = current*0.95 + 0.05*Data::self().data->actualCurrent;
    Data::self().data->averageCurrent = current;
    return current;
}

void changeButtonState()
{
    Data::self().data->buttonState = !Data::self().data->buttonState;
}

void runWebClient()
{
    // Declare variables
    static bool state = true;
    static bool change = false;

    if (change)
    {
        WebClient::self().uploadData();
    }
    else
    {
        state = WebClient::self().doThings();
        digitalWrite(RELAY, !state);
        debug("State %d", state);
    }
    change = !change;
}

void setupHW()
{
    //Serial
    Serial.begin(115200);
    debug("Starting Hardware.");

    //Led pins
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RELAY, OUTPUT);
    pinMode(BUTTON, INPUT);
    //Cathod comun
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RELAY, OUTPUT);
}

void screenChange()
{
    static uint id = 0;
    static uint slt = 0;
    static const uint delay = Display::self().delayBetweenFrames;
    static const uint fps = (uint)Display::self().targetFps;
    static const uint numOfScreens = 3;

    id++;
    slt = id%(fps*delay*numOfScreens);

    if(slt<=(fps*delay*1))
    {
        Display::self().displayLogo();
    }
    else if(slt<=(fps*delay*2))
    {
        Display::self().displayData();
    }
    else
    {
        Display::self().displayDataMin();
    }
}

Thread ledThread = Thread();
Thread displayThread = Thread();
Thread connectionThread = Thread();
Thread webClientThread = Thread();
Thread printDataThread = Thread();
ThreadController groupOfThreads = ThreadController();

void setup()
{
    setupHW();
    Data::self();
    Display::self();
    Display::self().setData(Data::self().data);
    Connection::self();
    Connection::self().setData(Data::self().data);
    Connection::self().addWifi("SSID", "pass");
    Connection::self().run();
    WebClient::self();
    WebClient::self().setData(Data::self().data);
    Ota::self();
    Ota::self().setData(Data::self().data);

    ledThread.onRun(ledTest);
    ledThread.setInterval(500);

    displayThread.onRun([](){screenChange();});
    displayThread.setInterval(1000/Display::self().targetFps);

    attachInterrupt(digitalPinToInterrupt(BUTTON), changeButtonState, CHANGE);

    connectionThread.onRun([](){Connection::self().run();});
    connectionThread.setInterval(1000);

    webClientThread.onRun(runWebClient);
    webClientThread.setInterval(5000);

    printDataThread.onRun([]()
    {
        const static unsigned long startTime = micros();
        readCurrent();
        Data::self().data->upTime = (micros() - startTime)/1e3;
        Data::self().print();
        debug("Free memory : %d", ESP.getFreeHeap());
    });
    printDataThread.setInterval(1000);

    groupOfThreads.add(&ledThread);
    groupOfThreads.add(&displayThread);
    groupOfThreads.add(&connectionThread);
    groupOfThreads.add(&webClientThread);
    groupOfThreads.add(&printDataThread);
}

void loop()
{
    groupOfThreads.run();
    Ota::self().handle();
    yield();
}
