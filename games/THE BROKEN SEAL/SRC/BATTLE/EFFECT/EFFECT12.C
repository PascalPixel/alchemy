#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_RUNTIME.H"

void BattlePalette_UpdateBlend(void);
void BattleFx_BuildBuffer(s32, s32, s32, s32);

void BattleFx_InterpolateBuffers(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

void BattleEffect_InitializeBuffers(void)
{
    volatile s32 zero;
    struct BattleEffectBuffers *buffer;

    buffer = Runtime_AllocateBlock(0x20, sizeof(*buffer));
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000a81, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (u8 *)buffer->palette + 0x1c0, 0x84000070,
            (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(0x10000, (s32)buffer, (s32)buffer->target, 0);
    Scheduler_AddOrUpdateCallback((void (*)(void))BattlePalette_UpdateBlend, 0xc8f);
}

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    Scheduler_RemoveCallback((s32)&BattlePalette_UpdateBlend);
    Runtime_ReleaseHeapBlock(0x20);
}

void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, (s32)buffers, (s32)buffers->target, mode);
    }
}

void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, (s32)buffers, (s32)buffers->current, mode);
    }
}

void BattleFx_SetPrimaryBufferValue(u32 value)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->palette[0] = value;
    }
}

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->duration = mode;
        buffers->step = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->current,
                                        (s16 *)buffers->target,
                                        (s16 *)buffers->delta,
                                        mode);
    }
}

s32 BattleFx_ClampRgb555Channel(s32 value)
{
    if (value > 31)
        return 31;
    if (value < 0)
        value = 0;
    return value;
}

s32 BattleFx_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}
