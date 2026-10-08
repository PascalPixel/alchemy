#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "serial_runtime.h"


void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = (s32 *)gSerialTransfer.payload;
    const s32 *src = &LinkLobby_SlotValues[slot];
    dst[LinkLobby_SlotColumns[slot]] = *src;
}
