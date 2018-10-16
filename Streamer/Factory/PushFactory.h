/*
 * The factory to gain the abstract pointer of streamer pushing module.
 * The UI class holds the abstract pointer.
 * The function returns a pointer of the object to implement the module.
 */

#ifndef _PUSH_FACTORY_H
#define _PUSH_FACTORY_H

#include "PushFlow.h"
#include <string>

namespace Push
{
    class PushFactory
    {
    public:
        PushFactory();
        ~PushFactory();

    public:
        PushFlow* CreatePushClass(const std::string& type, const std::string& url);
    };
}

#endif // _PUSH_FACTORY_H
