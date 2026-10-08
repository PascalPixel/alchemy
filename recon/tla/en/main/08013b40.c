/* ported from games/tbs/src/system/io.c */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "frame.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"


void Input_HandleKeyInterrupt(void)
{
    if (gSleepActive == 0) {
        REG_KEYCNT = 0xc3ff;
        gResetRequested = 1;
    }
}
