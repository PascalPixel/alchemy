#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "io_reg.h"
#include "types.h"


s32 Runtime_BlankDisplayAndRunScene(void)
{
    REG_DISPCNT = 0x40;
    Func_081b83c4();
    return 0;
}
