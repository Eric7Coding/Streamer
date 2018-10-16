/*
 * The factory to gain the abstract pointer of video encoding module.
 * The UI class holds the abstract pointer.
 * The function returns a pointer of the object to implement the module.
 */

#ifndef _ENCODE_FACTORY_H
#define _ENCODE_FACTORY_H

#include "VideoEncode.h"
#include <string>

namespace Encode
{
    class EncodeFactory
    {
    public:
        EncodeFactory();
        ~EncodeFactory();

    public:
        VideoEncode* CreateEncodeClass(const std::string& type, const int width, const int height);
    };
}

#endif // _ENCODE_FACTORY_H
