/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "data.h"

class WebClient
{
public:

    void printJson(JsonObject& json, String string);

    void uploadData();

    bool doThings();

    void setData(dataStruct* d);

    static WebClient& self();

    ~WebClient();

private:
    WebClient& operator = (WebClient& other) = delete;
    WebClient(const WebClient& other) = delete;
    WebClient();

    String host = "server.com";
    int port = 5228;

    String user = "admin";
    String password = "admin";
    dataStruct* data;
};
