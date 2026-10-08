#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "frame.h"



void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (gFrameCount >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Graphics_MergeShiftedTileRows((u32 *)0x06000220,
        (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows((u32 *)0x06000240,
        (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}
