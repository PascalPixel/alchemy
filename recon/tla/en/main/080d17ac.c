/* Matches (differ 0): ☀️'s BattleFx_StartBufferInterpolation. It goes after
   BattleFx_PackTargetBuffer (080d1760, a draft at 12) in
   BATTLE/EFFECT/EFFECT12.C, and waits for it. */
#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"

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
