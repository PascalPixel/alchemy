#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames);

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        buffers->duration = mode;
        buffers->step = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->current,
                                    (s16 *)buffers->target,
                                    (s16 *)buffers->delta,
                                    mode);
    }
}

void BattleFx_BuildBuffer(s32 mode, struct BattleEffectBuffers *source, u16 *destination, s32 half);

void BattleFx_LoadBufferPalette(const u16 *colours)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        /* FAKEMATCH: the entry held in a local loads its address after the size is started, as the native does; called directly the address loads first (differ 2). */
        s32 (*copy)(void *destination, const void *source, s32 size) = Iwram_CopyWords;

        copy(buffers->palette, colours, 0x1c0);
        BattleFx_BuildBuffer(0x10000, buffers, buffers->target, 1);
    }
}
