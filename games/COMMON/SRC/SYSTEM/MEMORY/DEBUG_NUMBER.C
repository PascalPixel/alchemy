#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"



void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((count - 1) > 7U) {
        count = 8;
    }
    Text_FormatHexToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - count));
}

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((count - 1) > 9U) {
        count = 0xA;
    }
    Text_FormatSignedDecimalToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - count));
}
