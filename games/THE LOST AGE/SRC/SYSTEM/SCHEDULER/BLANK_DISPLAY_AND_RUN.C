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
#include "FIELD_EVENT.H"
#include "IO_REG.H"


s32 Runtime_BlankDisplayAndRun(void)
{
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
    Audio_PlayCue(2);
    LuckyDice_Run();
    return 0;
}
