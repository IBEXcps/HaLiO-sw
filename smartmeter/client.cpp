/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "debug.h"
#include "client.h"

WebClient::WebClient()
{
}

void WebClient::printJson(JsonObject& json, String string)
{
    char payload[512] = {0};
    json.prettyPrintTo(payload, sizeof(payload));

    debug("%s", string.c_str());
    Serial.println(payload);
}

void WebClient::uploadData()
{
    String node = String("http://") +host+ ":" + port + "/nodes/" + data->nodeId + "/";

    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["node"] = node;
    root["value"] = data->averageCurrent;

    HTTPClient http;
    http.setAuthorization(user.c_str(), password.c_str());

    http.begin(host, port, "/data/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    char payload[256] = {0};
    root.printTo(payload, sizeof(payload));
    int httpCode = http.POST(payload);
    debug("httpcode: %d", httpCode);

    printJson(root, "Sending :");

    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED)
        {
            String string = http.getString();
            JsonObject& root = jsonBuffer.parseObject(string);
            if (!root.success())
            {
                debug("parseObject() failed");
                return;
            }
            printJson(root, "Received :");
        }
    }
    else
    {
        debug("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

bool WebClient::doThings()
{
    bool isTrue = false;
    String nodeid = data->nodeId;

    HTTPClient http;
    http.setAuthorization(user.c_str(), password.c_str());
    http.begin(host,port, "/nodes/"+nodeid+"/"); //HTTP
    int httpCode = http.GET();

    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED)
        {
            String payload = http.getString();
            StaticJsonBuffer<512> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(payload);

            if (!root.success())
            {
                debug("parseObject() failed");
                data->relayStatus = false;
                return false;
            }
            data->nodeName = root["nome"].asString();
            printJson(root, "Received :");
            isTrue = (String("true") == root["relayState"]);
        }
    }
    else
    {
        debug("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    data->relayStatus = isTrue;
    return isTrue;
}

void WebClient::setData(dataStruct* d)
{
    data = d;
}

WebClient& WebClient::self()
{
    static WebClient self;
    return self;
}

WebClient::~WebClient()
{
}
