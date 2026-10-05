#include "PROTO.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"


void BattleEffect_InitializeBuffers(void)
{
    volatile s32 zero;
    struct BattleEffectBuffers *buffer;

    buffer = Runtime_AllocateBlock(128, sizeof(*buffer));
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000a81, REG_DMA3);
    Dma_Set((void *)0x05000000, buffer, 0x84000070, REG_DMA3);
    Dma_Set((void *)0x05000200, (u8 *)buffer->palette + 0x1c0, 0x84000070,
            REG_DMA3);
    BattleFx_BuildBuffer(0x10000, buffer, buffer->target, 0);
    Scheduler_AddOrUpdateCallback((s32)BattlePalette_UpdateBlend, 0x48f);
}

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    Scheduler_RemoveCallback((s32)&BattlePalette_UpdateBlend);
    Runtime_ReleaseHeapBlock(128);
}

void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, buffers, buffers->target, mode);
    }
}

void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        BattleFx_BuildBuffer(value, buffers, buffers->current, mode);
    }
}

void BattleFx_SetPrimaryBufferValue(u32 value)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        buffers->palette[0] = value;
    }
}
