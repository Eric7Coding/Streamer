/*
 * The factory to gain the abstract pointer of screen shot module.
 * The UI class holds the abstract pointer.
 * The function returns a pointer of the object to implement the module.
 */

#ifndef _INTERCEPT_FACTORY_H
#define _INTERCEPT_FACTORY_H

#include "InterceptScreen.h"
#include <string>

namespace Catpure
{
    class InterceptFactory
    {
    public:
        InterceptFactory();
        ~InterceptFactory();

    public:
        InterceptScreen* CreateInterceptClass(const std::string& type, const int width, const int height);
    };
}

#endif // _INTERCEPT_FACTORY_H
