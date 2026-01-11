/*---------------------------------------------------------*\
| RGBController_SkyloongGK87Pro.h                           |
|                                                           |
|   RGBController for Skyloong GK87 Pro                     |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|   yzc26623 (GK87 Pro implement)               11 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkyloongGK87ProController.h"

#define BRIGHTNESS_MIN      0
#define BRIGHTNESS_MAX      127

class RGBController_SkyloongGK87Pro : public RGBController
{
public:
    RGBController_SkyloongGK87Pro(SkyloongGK87ProController* controller_ptr);
    ~RGBController_SkyloongGK87Pro();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SkyloongGK87ProController*   controller;
};
