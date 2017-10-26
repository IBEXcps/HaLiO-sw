/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once
#define NUM(X) #X
#include <Arduino.h>

struct dataStruct
{
    // Actual = Last measure
    // Average = average since last data send
    float actualCurrent = 0.0f;
    float averageCurrent = 0.0f;
    float actualConsumption = 0.0f;
    float averageConsumption = 0.0f;
    float actualRSSI = 0.0f;
    float averageRSSI = 0.0f;
    String nodeId = NUM(1); // Birthday XYZ6098
    String nodeName = "Kitchen"; // User can give it a name line WillDaCozinha
    String nodeStatus = "Idle";
    String networkName = ""; // Wifi name
    String hostName = ""; // Hostname
    String networkIP = "0.0.0.0";
    unsigned int upTime = 0;
    unsigned int relayTime = 0; // Time since last status change
    bool relayStatus = false; // on off
    bool buttonState = false; //on off
    unsigned int otaProgress = 0; // ota progress
};

class Data
{
public:
    void print();

    static Data& self();

    ~Data();

    dataStruct* data;

private:
    Data& operator = (Data& other) = delete;
    Data(const Data& other) = delete;
    Data();
};
