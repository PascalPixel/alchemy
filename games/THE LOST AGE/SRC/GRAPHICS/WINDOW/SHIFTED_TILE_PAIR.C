#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FRAME.H"



void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (gFrameCount >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase++;
    Graphics_MergeShiftedTileRows((u32 *)0x06000220,
        (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows((u32 *)0x06000240,
        (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}
