#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "io_reg.h"


s32 BattlePres_RunMosaicFadeOut(void)
{
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    s32 i;
    volatile u16 *reg;

    bg0cnt = REG_BG0CNT;
    REG_BG0CNT = bg0cnt | 0x40;
    reg = &REG_BG0CNT;
    bg1cnt = *++reg;
    *reg = bg1cnt | 0x40;
    bg2cnt = *++reg;
    *reg = bg2cnt | 0x40;
    bg3cnt = *++reg;
    *reg = bg3cnt | 0x40;
    reg += 0x21;
    *reg = 0x3eee;

    Blend_SetDarkenTarget16(16);
    for (i = 0; i <= 15; i++) {
        Random16();
        Random16();
        Random16();
        Random16();
        REG_MOSAIC = (i << 8) | i;
        WaitFrames(1);
    }

    REG_DISPCNT = 1;
    WaitFrames(4);
    reg = &REG_BG0CNT;
    *reg++ = bg0cnt;
    *reg++ = bg1cnt;
    *reg++ = bg2cnt;
    *reg = bg3cnt;
    return 0;
}
