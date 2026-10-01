#include "DMA.H"
#include "CALL.H"

void BabiFune_StepFade();
extern s16 BabiFune_FadeSlot;
extern u16 BabiFune_FadeStep;
s32 Runtime_BumpAllocateAlternatePool();
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached();

struct Half {
    u16 v;
};

/* Babi Fune: fill a 256-byte scratch block with colour 1, load it into a
 * newly claimed VRAM slot, set the fade step to 48 and schedule the fade. */
void BabiFune_ScheduleFade(void)
{
    u8 *buf;
    volatile u32 fill;

    buf = (u8 *)Value1(Runtime_BumpAllocateAlternatePool, 0x100);
    BabiFune_FadeSlot = Resource_FindFreeEntry();
    fill = 0x11111111;
    Dma_Set((const void *)&fill, buf, 0x85000040, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(BabiFune_FadeSlot, 0x100, (s32)buf);
    /* The halfword constant comes from the literal pool (HImode move). */
    BabiFune_FadeStep = 0x30;
    Scheduler_AddOrUpdateCallback((s32)(BabiFune_StepFade), 0xc80);
}
