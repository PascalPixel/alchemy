#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"

s32 LuckyDice_Run(void);

/* Turns every layer off and runs the lucky dice game. */
s32 Runtime_BlankDisplayAndRun(void)
{
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
    Audio_PlayCue(2);
    LuckyDice_Run();
    return 0;
}
