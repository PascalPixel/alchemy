#include "TYPES.H"

s32 LuckyDice_Run();
s32 Audio_PlayCue(s32);

/* The interrupt master-enable word (IME) before and after the blanked frame. */
#define REG_IME (*(volatile u16 *)0x04000000)

s32 Runtime_BlankDisplayAndRun(void)
{
    REG_IME = 0x40;
    Audio_PlayCue(9);
    LuckyDice_Run();
    return 0;
}
