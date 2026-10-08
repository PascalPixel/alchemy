#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "project.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_state.h"
#include "btlwork.h"

extern u16 gBgScroll[];

void WaitFrames(s32 frames);

void BattleEffect_SetupBlendedDisplay(void)
{
    void **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
    u8 *buffer = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
    struct BtlWork *battle = slots[0];
    volatile u32 zero;

    REG_DISPCNT = 1;
    gBgScroll[3] = 32;
    BattleBackground_LoadFar(1, battle->background, 24);
    zero = 0;
    Dma_Set((const void *)&zero, buffer, 0x85001000, REG_DMA3);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x06004000, 0x85001000, REG_DMA3);
    WaitFrames(1);
    REG_BLDALPHA = 0x100e;
    REG_BLDCNT = 0x3f46;
    REG_DISPCNT = 0x7741;
    gProjection.center_y = 120;
}
