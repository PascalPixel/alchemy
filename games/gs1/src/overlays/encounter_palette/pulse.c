#include "types.h"

#define FrameCounter (*(u32 *)0x03001e40)
#define EncounterPalette (*(volatile u16 *)0x0500019e)

void EncounterPalette_Pulse(void)
{
    u16 phase = FrameCounter & 63;
    s32 level;

    if (phase > 31)
        phase = 64 - phase;
    level = (phase >> 1) + 7;
    level |= (level << 10) | (level << 5);
    EncounterPalette = ((u32)level << 16) >> 16;
}
