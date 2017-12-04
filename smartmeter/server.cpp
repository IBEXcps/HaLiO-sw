/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <algorithm>

#include "debug.h"
#include "server.h"

WebServer::WebServer() :
    // Create webServer
    server(new ESP8266WebServer(80))
{
    server->on("/", [this](){handleRoot();});
    server->on("/debug", [this](){handleDebug();});
    server->onNotFound([this](){handleNotFound();});
    server->begin();
}

void WebServer::handleRoot()
{
    server->send(200, "text/plain", "hello from esp8266!");
}

void WebServer::handleDebug()
{
    String debugOutput;
    std::reverse(Debug::self().debugMsgs.begin(), Debug::self().debugMsgs.end());
    while (Debug::self().debugMsgs.size()) {
        debugOutput += Debug::self().debugMsgs.back();
        debugOutput.replace("\n", "");
        Debug::self().debugMsgs.pop_back();
    }

    server->send(200, "text/plain", debugOutput.c_str());
}

void WebServer::handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server->uri();
    message += "\nMethod: ";
    message += (server->method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server->args();
    message += "\n";
    for (uint8_t i=0; i<server->args(); i++)
    {
        message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
    }
    server->send(404, "text/plain", message);
}

void WebServer::handleClient()
{
    server->handleClient();
}

WebServer& WebServer::self()
{
    static WebServer self;
    return self;
}

WebServer::~WebServer()
{
}
