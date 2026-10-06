#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FRAME.H"
#include "IO_REG.H"

void Makyuri_CyclePalette(void)
{
    volatile u16 cur;
    u32 i;
    s32 v;
    s32 bits;
    u16 *dst;

    cur = 0;
    if (gFrameCount % 5 != 0)
        return;
    gPaletteCycleRed = (gPaletteCycleRed + 4) & 31;
    for (i = 0; i < 6; i++) {
        cur = BG_PLTT_COLOR(6, 14 - i) & 31;
        v = cur;
        if (i <= 2)
            v -= v * 4 / 10;
        dst = (u16 *)&BG_PLTT_COLOR(6, 15 - i);
        bits = (gPaletteCycleBlue << 10) | (gPaletteCycleGreen << 5);
        *dst = v | bits;
    }
    BG_PLTT_COLOR(6, 9) = gPaletteCycleRed | bits;
}

void Makyuri_ClearPalette(void)
{
    u32 i;

    for (i = 0; i < 7; i++)
        BG_PLTT_COLOR(6, 15 - i) = 0;
}
