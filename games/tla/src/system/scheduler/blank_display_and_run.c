#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "io_reg.h"


s32 Runtime_BlankDisplayAndRun(void)
{
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
    Audio_PlayCue(2);
    LuckyDice_Run();
    return 0;
}
