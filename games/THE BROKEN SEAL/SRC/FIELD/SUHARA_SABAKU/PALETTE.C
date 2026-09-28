/* The Suhara desert: the encounter palette's pulse. */
#include "SABAKU.H"

void EncounterPalette_Pulse(void)
{
    u16 phase = gFrameCount & 63;
    s32 level;

    if (phase > 31)
        phase = 64 - phase;
    level = (phase >> 1) + 7;
    level |= (level << 10) | (level << 5);
    EncounterPalette = ((u32)level << 16) >> 16;
}
