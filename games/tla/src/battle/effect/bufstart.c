#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"

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
