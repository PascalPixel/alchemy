/* Draft, not exact: 17 differing halfwords, 184-byte candidate for the
   184-byte owner (2026-09-23). Residual: the scratch register for the fill
   zero and the 0x1500 and 0x1f80 offsets is r1 where the reference uses r3,
   which reorders the argument loads around them; s32-typed arguments and an
   unprototyped interpolation call do not change it
   (Scheduler_AddOrUpdateCallback returns s32, which fixed the callback's
   argument order). */

#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 BattleFx_BuildBuffer(s32 source, void *reference, void *destination, s32 mode);
void BattleFx_InterpolateBuffers(s16 *from, s16 *to, s16 *step, s32 frames);
void Func_080949a8(void);

/* Builds the buffers for two effect sources and the per-frame step between
   them for a twelve-frame blend, then schedules the blend. */
void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    u8 *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s32 value;

    work = Runtime_AllocateBlock(30, 0x1f88);
    buffers = Data_03001ed0;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(from, buffers, work, 1);
    BattleFx_BuildBuffer(to, buffers, work + 0xa80, 1);
    BattleFx_InterpolateBuffers((s16 *)(work + 0xa80), (s16 *)work, (s16 *)(work + 0x1500), 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->buffer_e00, 1);
    /* FAKEMATCH: the halfword constants pass through an int so GCC builds them
       with mov instead of loading them from the pool. */
    value = 120;
    *(u16 *)(work + 0x1f80) = value;
    value = 0;
    *(u16 *)(work + 0x1f82) = value;
    Scheduler_AddOrUpdateCallback(Func_080949a8, 0xc80);
}
