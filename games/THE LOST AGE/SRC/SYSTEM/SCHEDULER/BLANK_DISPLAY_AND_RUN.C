#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 LuckyDice_Run();

/* The interrupt master-enable word (IME) before and after the blanked frame. */
#define REG_IME (*(volatile u16 *)0x04000000)

s32 Runtime_BlankDisplayAndRun(void)
{
    REG_IME = 0x40;
    Audio_PlayCue(2);
    LuckyDice_Run();
    return 0;
}
