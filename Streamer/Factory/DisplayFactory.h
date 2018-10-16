/*
 * The factory to gain the abstract pointer of video displaying module.
 * The UI class holds the abstract pointer.
 * The function returns a pointer of the object to implement the module.
 */

#ifndef _DISPLAY_FACTORY_H
#define _DISPLAY_FACTORY_H

#include "VideoDisplay.h"
#include <string>

namespace Display
{
    class DisplayFactory
    {
    public:
        DisplayFactory();
        ~DisplayFactory();

    public:
        VideoDisplay* CreateDisplayClass(const std::string& type, const int width, const int height, QWidget* widget = nullptr);
    };
}

#endif // _DISPLAY_FACTORY_H
