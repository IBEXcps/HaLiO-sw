/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 * Copyright 2017 Willian Galvani <willian@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "debug.h"
#include "connection.h"

Connection::Connection() :
    WiFiMulti(new ESP8266WiFiMulti),
    connected(false)
{
}

void Connection::addWifi(String ssid, String password)
{
    WiFiMulti->addAP(ssid.c_str(), password.c_str());
}

ESP8266WiFiClass* Connection::wifi()
{
    return &WiFi;
}

void Connection::run()
{
    if (!connected)
    {
        debug("Starting Wifi. (Running..)");
        connected = true;
    }

    if ((WiFiMulti->run() == WL_CONNECTED))
    {
        debug("Connected wifi %s", wifi()->SSID().c_str());
        debug("macAddress %s",wifi()->macAddress().c_str());
        debug("Hostname %s",wifi()->hostname().c_str());
        debug("IP %s", wifi()->localIP().toString().c_str());
        debug("BSSID %s",wifi()->BSSIDstr().c_str());
        debug("RSSI %d",wifi()->RSSI());
        data->hostName = wifi()->hostname();
        data->networkName = wifi()->SSID();
        data->actualRSSI = wifi()->RSSI();
        data->networkIP = wifi()->localIP().toString();
        data->nodeStatus = "Connected !";
        return;
    }
    data->nodeStatus = "Not connected";
    debug("NOT connected !");
}

void Connection::setData(dataStruct* d)
{
    data = d;
}

Connection& Connection::self()
{
    static Connection self;
    return self;
}

Connection::~Connection()
{
}
