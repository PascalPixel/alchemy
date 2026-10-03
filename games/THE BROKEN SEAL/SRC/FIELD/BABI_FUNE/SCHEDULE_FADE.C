#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "CALL.H"
#include "VRAM_BLOCK.H"

extern s16 BabiFune_FadeSlot;
extern s16 BabiFune_FadeStep;

struct Half {
    u16 v;
};

/* A sprite as the slot entries take it: its attributes packed in the
   second word and its tile in the third. */
struct FadeSprite {
    u32 unused;
    u32 attributes;
    u32 tile;
};

extern struct FadeSprite BabiFune_FadeSprites[24];


/* Each frame of the Babi Fune fade: count the step down and push three
   columns of eight wide sprites, the first sliding in from the left edge
   and the other two from the right as the step shrinks. */
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

/* Babi Fune: fill a 256-byte scratch block with colour 1, load it into a
 * newly claimed VRAM slot, set the fade step to 48 and schedule the fade. */
void BabiFune_ScheduleFade(void)
{
    u8 *buf;
    volatile u32 fill;

    buf = (u8 *)Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 0x100);
    BabiFune_FadeSlot = Resource_FindFreeEntry();
    fill = 0x11111111;
    Dma_Set((const void *)&fill, buf, 0x85000040, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(BabiFune_FadeSlot, 0x100, (s32)buf);
    /* The halfword constant comes from the literal pool (HImode move). */
    BabiFune_FadeStep = 0x30;
    Scheduler_AddOrUpdateCallback((s32)(BabiFune_StepFade), 0xc80);
}
