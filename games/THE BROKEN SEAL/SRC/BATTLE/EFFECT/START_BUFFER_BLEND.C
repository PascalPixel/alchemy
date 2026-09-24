#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"

extern u8 Value_00001f82;

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 BattleFx_BuildBuffer(s32 source, void *reference, void *destination, s32 mode);
void BattleFx_InterpolateBuffers(s16 *from, s16 *to, s16 *step, s32 frames);
void Func_080949a8(void);

/* Builds the buffers for two effect sources and the per-frame step between
   them for a twelve-frame blend, then schedules the blend. The work block
   holds the from buffer, the to buffer at +0xa80, the step at +0x1500 and
   the frame count and position at +0x1f80. */
void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    u8 *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s32 value;
    u8 *target;
    u16 *frames;

    work = Runtime_AllocateBlock(30, 0x1f88);
    buffers = Data_03001ed0;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(from, buffers, work, 1);
    target = work + 0xa80;
    BattleFx_BuildBuffer(to, buffers, target, 1);
    BattleFx_InterpolateBuffers((s16 *)target, (s16 *)work, (s16 *)(work + 0x1500), 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->buffer_e00, 1);
    frames = (u16 *)(work + 0x1f80);
    /* FAKEMATCH: the halfword constants pass through an int so GCC builds
       them with mov instead of loading them from the pool. */
    value = 120;
    *frames = value;
    value = 0;
    *(u16 *)(work + (s32)&Value_00001f82) = value;
    Scheduler_AddOrUpdateCallback(Func_080949a8, 0xc80);
}
