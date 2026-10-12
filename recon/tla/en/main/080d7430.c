#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"

void Func_080d67e8(void);

void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    struct FieldBlendWork *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s16 *target;

    work = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
    buffers = Ram_HeapSlots->block_32;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, REG_DMA3);
    BattleFx_BuildBuffer(from, (u16 *)buffers, (u16 *)work, 1);
    target = work->to;
    BattleFx_BuildBuffer(to, (u16 *)buffers, (u16 *)target, 1);
    BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
    work->timer = 120;
    work->enabled = 0;
    Scheduler_AddOrUpdateCallback(Func_080d67e8, 0x480);
}
