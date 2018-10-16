/*
 * The factory to gain the abstract pointer of network quality computing module.
 * The UI class holds the abstract pointer.
 * The function returns a pointer of the object to implement the module.
 */

#ifndef _NETWORK_FACTORY_H
#define _NETWORK_FACTORY_H

#include "NetworkQuality.h"
#include <string>

namespace Network
{
    class NetworkFactory
    {
    public:
        NetworkFactory();
        ~NetworkFactory();

    public:
        NetworkQuality* CreateNetworkClass(const std::string& type, const std::string& ip, const unsigned short& port);
    };
}

#endif // NETWORK_FACTORY_H
