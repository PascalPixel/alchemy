/* Draft, not exact (2026-09-24): 14 differing halfwords, 180 of 180 bytes.
   Taking the frame-count address before its value (as in the exact
   BattleFx_StartBufferBlend) fixes the scratch registers up to the last
   store. Residual: the reference adds the 0x1f82 offset into r5 and builds
   the 1 with movs after it; here the 1 is pooled and the offset folds into
   the store address. The do-while is a FAKEMATCH search artefact. */

#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 BattleFx_BuildBuffer(s32 source, void *reference, void *destination, s32 mode);
void BattleFx_InterpolateBuffers(s16 *from, s16 *to, s16 *step, s32 frames);
void Func_080949a8(void);
extern u8 Value_00001f82;

/* Builds two 0xa80-byte buffers and the per-frame step between them for a
   twelve-frame blend, then schedules the blend. */
void BattleFx_StartTwelveFrameBlend(void)
{
    u8 *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s32 value;
    s32 one;
    u8 *target;
    u16 *frames;

    work = Runtime_AllocateBlock(30, 0x1f88);
    buffers = Data_03001ed0;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(0x10003, buffers, work, 1);
    BattleFx_BuildBuffer(0x10005, buffers, work + 0xa80, 1);
    BattleFx_InterpolateBuffers((s16 *)(work + 0xa80), (s16 *)work, (s16 *)(work + 0x1500), 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->buffer_e00, 1);
    /* FAKEMATCH: the halfword constants pass through an int so GCC builds them
       with mov instead of loading them from the pool. */
    frames = (u16 *)(work + 0x1f80);
    value = 600;
    *frames = value;
    frames = (u16 *)(work + (s32)&Value_00001f82);
    do { *frames = 1; } while (0);
    Scheduler_AddOrUpdateCallback(Func_080949a8, 0xc80);
}
