#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

// What every part of the system library's object shares: its includes. system.c includes it before
// the parts; a part includes only what it adds.

#include "game/game.h"
#include "graphics/graphics.h"
#include "sound/sound.h"
#include "macros.h"
#include "sdk/flash.h"
#include "graphics/resource.h"
#include "system/rom_header.h"
#include "sound/sound_driver_symbols.h"
#include "sdk/sound_driver.h"

// The screen block of the debug text's map, which the link test also draws on.
#define DEBUG_TEXT_SCREEN_BLOCK 4

// The system library's own inlines, which only its parts call (system_serial.c and
// system_matrix.c). A unit that includes only system.h does not define them, which keeps its
// constant-pool labels numbered as the ROM's build numbered them (see system.h).

// status, with LINK_STATUS_ERROR set when the link's last transfer failed.
static inline Sint32 sySerialAddErrorFlag(Sint32 status)
{
    if (SerialStateWork.transferError != 0)
        status |= LINK_STATUS_ERROR;
    return status;
}

// Resets the current matrix to the unit matrix.
static __inline__ void syUnitMatrixCurrentInline(void)
{
    ASM_MATRIX_UNIT_CURRENT(CurrentMatrix);
}

#endif
