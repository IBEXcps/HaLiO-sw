/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include "data.h"

class Connection
{
public:

    void addWifi(String ssid, String password);

    ESP8266WiFiClass* wifi();

    void run();

    void setData(dataStruct* d);

    static Connection& self();

    ~Connection();

private:
    Connection& operator = (Connection& other) = delete;
    Connection(const Connection& other) = delete;
    Connection();

    ESP8266WiFiMulti* WiFiMulti;
    dataStruct* data;
    bool connected;
};
