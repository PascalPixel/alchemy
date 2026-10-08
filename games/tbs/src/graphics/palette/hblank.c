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
#include "dma.h"
#include "scene.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "io_reg.h"

#define gBattleFxWork (*(struct BattleEffectWork * *)&gBattleFxWork)


void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 i;

    for (i = 0; i != 63; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue += blue_delta;
        green += green_delta;
        red += red_delta;
        if (blue > 31)
            blue = 31;
        if (green > 31)
            green = 31;
        if (red > 31)
            red = 31;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}

void BattleFx_ArmPaletteHBlankDma(void)
{
    struct BattleEffectWork *work = gBattleFxWork;
    volatile u16 *channel = REG_DMA0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work->sheet + 0x1f80, (void *)0x05000000, 0xa2600001, (volatile u32 *)channel);
}
