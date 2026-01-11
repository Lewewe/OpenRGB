/*---------------------------------------------------------*\
| SkyloongControllerDetect.cpp                              |
|                                                           |
|   Detector for Skyloong Keyboards                         |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|   yzc26623 (GK87 Pro implement)               11 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "SkyloongGK87ProController.h"
#include "RGBController_SkyloongGK87Pro.h"

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define SKYLOONG_KEYBOARD_VID          0x1EA7
#define SKYLOONG_GK87_PRO_PID          0x0907
#define SKYLOONG_GK87_PRO_I            1

/******************************************************************************************\
*                                                                                          *
*   DetectSkyloongGK87Pro                                                                 *
*                                                                                          *
*       Tests the USB address to see if a Skyloong GK87 Pro controller exists there.      *
*                                                                                          *
\******************************************************************************************/
void DetectSkyloongGK87Pro(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SkyloongGK87ProController* controller          = new SkyloongGK87ProController(dev, info->path, name);
        RGBController_SkyloongGK87Pro* rgb_controller  = new RGBController_SkyloongGK87Pro(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("Skyloong GK87 Pro", DetectSkyloongGK87Pro, SKYLOONG_KEYBOARD_VID, SKYLOONG_GK87_PRO_PID, SKYLOONG_GK87_PRO_I);
