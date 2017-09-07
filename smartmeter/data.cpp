/**
 * HaLiO software
 * Copyright 2017 Patrick José Pereira <patrick@ibexcps.com>
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "data.h"
#include "debug.h"

Data::Data() :
    data(new dataStruct)
{
    debug("Starting Data.");
    print();
}

void Data::print()
{
    String string;
    string.concat("\n\rActual current : ");
    string.concat(data->actualCurrent);
    string.concat("\n\rAverage current : ");
    string.concat(data->averageCurrent);
    string.concat("\n\rActual consumption : ");
    string.concat(data->actualConsumption);
    string.concat("\n\rAverage consumption : ");
    string.concat(data->averageConsumption);
    string.concat("\n\rActual RSSI : ");
    string.concat(data->actualRSSI);
    string.concat("\n\rAverage RSSI : ");
    string.concat(data->averageRSSI);
    string.concat("\n\rnode Id : ");
    string.concat(data->nodeId);
    string.concat("\n\rNode name : ");
    string.concat(data->nodeName);
    string.concat("\n\rNetwork name : ");
    string.concat(data->networkName);
    string.concat("\n\rNetwork IP : ");
    string.concat(data->networkIP);
    string.concat("\n\rupTime : ");
    string.concat(data->upTime);
    string.concat("\n\rrelayTime : ");
    string.concat(data->relayTime);
    string.concat("\n\rrelayStatus : ");
    string.concat(data->relayStatus);
    debug("%s", string.c_str());
}

Data& Data::self()
{
    static Data self;
    return self;
}

Data::~Data()
{
}
