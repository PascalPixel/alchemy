#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "MEM.H"
#include "CALL.H"
#include "VRAM_BLOCK.H"
#include "IO_REG.H"


struct Half {
    u16 v;
};

struct FadeSprite {
    u32 unused;
    u32 attributes;
    u32 tile;
};

extern struct FadeSprite BabiFune_FadeSprites[24];

void BabiFune_StepFade(void)
{
    u32 *entry;
    u32 tile;
    s32 step;
    s32 x;
    u32 i;

    tile = gVramBlockCache[BabiFune_FadeSlot].offset >> 5;
    entry = (u32 *)BabiFune_FadeSprites;
    if (BabiFune_FadeStep != 0)
        BabiFune_FadeStep--;
    for (i = 0; i < 8; i++) {
        s32 left;

        step = BabiFune_FadeStep;
        left = -step / 2;
        *entry++ = 0;
        *entry++ = (left & 0xff) | (i << 21) | 0x80004000;
        *entry++ = tile;
    }
    x = (step / 2 + 136) & 0xff;
    for (i = 0; i < 8; i++) {
        *entry++ = 0;
        *entry++ = (i << 21) | x | 0x80004000;
        *entry++ = tile;
    }
    i = 0;
    x = (BabiFune_FadeStep / 2 + 152) & 0xff;
    for (; i < 8; i++) {
        *entry++ = 0;
        *entry++ = (i << 21) | x | 0x80004000;
        *entry++ = tile;
    }
    for (i = 0; i < 24; i++)
        Runtime_PushSlotEntry(&BabiFune_FadeSprites[i], 255);
}

void BabiFune_ScheduleFade(void)
{
    u8 *buf;
    volatile u32 fill;

    buf = (u8 *)Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 0x100);
    BabiFune_FadeSlot = Resource_FindFreeEntry();
    fill = 0x11111111;
    Dma_Set((const void *)&fill, buf, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x40, REG_DMA3);
    Engine_VramLoad(BabiFune_FadeSlot, 0x100, (s32)buf);

    BabiFune_FadeStep = 0x30;
    Scheduler_AddOrUpdateCallback((s32)(BabiFune_StepFade), 0xc80);
}
