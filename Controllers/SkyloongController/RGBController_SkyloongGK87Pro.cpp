/*---------------------------------------------------------*\
| RGBController_SkyloongGK87Pro.cpp                         |
|                                                           |
|   RGBController for Skyloong GK87 Pro                     |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|   yzc26623 (GK87 Pro implement)               11 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_SkyloongGK87Pro.h"
#include "KeyboardLayoutManager.h"
#define NA  0xFFFFFFFF


using namespace std::chrono_literals;

/*---------------------------------------------------------------------*\
|  Skyloong GK87 Pro Keyboard KLM Layout                               |
\*---------------------------------------------------------------------*/
layout_values keyboard_offset_values =
{
    {
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK  */
        0,           3,    4,    5,    6,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC   INS  HOME  PGUP  */
        22,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,    38,   39,   40,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \    DEL   END  PGDN  */
        44,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,    60,   61,   62,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #    ENTR                   */
        66,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,    80,
    /* LSFT    Z     X     C     V     B     N     M     ,     .     /     #    RSFT              ARWU       */
        88,   89,   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,   102,              105,
    /* LCTL LWIN  LALT         SPC   SPC   SPC        RALT  RMNU  RCTL  RFNC                ARWL  ARWD  ARWR */
        110, 111,  112,              116,              120,  121,  124,  125,                126,  127,  128,
	},
    {
        /* Add more regional layout fixes here */
    }
};

/**------------------------------------------------------------------*\
    @name Skyloong GK87 Pro
    @category Keyboard
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :o:
    @detectors SkyloongControllerDetect
    @comment
\*-------------------------------------------------------------------*/

RGBController_SkyloongGK87Pro::RGBController_SkyloongGK87Pro(SkyloongGK87ProController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Skyloong";
    description                 = "Skyloong GK87 Pro Keyboard";
    location                    = controller->GetDeviceLocation();
    type                        = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = BRIGHTNESS_MIN;
    Direct.brightness_max       = BRIGHTNESS_MAX;
    Direct.brightness           = BRIGHTNESS_MAX;

    modes.push_back(Direct);

    SetupZones();
}

RGBController_SkyloongGK87Pro::~RGBController_SkyloongGK87Pro()
{
    delete controller;
}

void RGBController_SkyloongGK87Pro::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create the keyboard zone using Keyboard Layout Manager    |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, KEYBOARD_SIZE_TKL, keyboard_offset_values);

    matrix_map_type * new_map   = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = new_kb.GetRowCount();
    new_zone.matrix_map->width  = new_kb.GetColumnCount();

    new_zone.matrix_map->map    = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count         = new_kb.GetKeyCount();
    new_zone.leds_min           = new_zone.leds_count;
    new_zone.leds_max           = new_zone.leds_count;

    /*---------------------------------------------------------*\
    | Matrix map still uses declared zone rows and columns      |
    |   as the packet structure depends on the matrix map       |
    \*---------------------------------------------------------*/
    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name            = new_kb.GetKeyNameAt(led_idx);
        new_led.value           = new_kb.GetKeyValueAt(led_idx);

        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_SkyloongGK87Pro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SkyloongGK87Pro::DeviceUpdateLEDs()
{
    controller->SendColorPacket(colors, &leds, modes[active_mode].brightness);
}

void RGBController_SkyloongGK87Pro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK87Pro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK87Pro::DeviceUpdateMode()
{
}
