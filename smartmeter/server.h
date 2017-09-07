/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#pragma once

#include <ESP8266WebServer.h>

class WebServer
{
public:

    void handleRoot();

    void handleInline();

    void handleNotFound();

    void handleClient();

    static WebServer& self();

    ~WebServer();

private:
    WebServer& operator = (WebServer& other) = delete;
    WebServer(const WebServer& other) = delete;
    WebServer();

    ESP8266WebServer* server;
};
