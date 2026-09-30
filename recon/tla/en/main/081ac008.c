/*
 * Draft: Runtime_BlankDisplayAndRun does not yet match; 3 halfwords differ from ☀️'s C, first at +0xa (movs r0, #2).
 * Links as recon/tla/raw/081ac000.s.
 */
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
