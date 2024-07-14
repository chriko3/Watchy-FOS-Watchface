#ifndef WATCHY_FOS_H
#define WATCHY_FOs_H

#include <Watchy.h>
#include "Px437_IBM_BIOS5pt7b.h"

class WatchyFOS : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        virtual void handleButtonPress();
};

#endif