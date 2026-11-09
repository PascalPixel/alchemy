#include "types.h"
#include "palette_buffers.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                 s32 frames);

void BattleEffect_StartBufferInterpolation(s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->duration = mode;
    buffers->step = 0;
    BattleEffect_InterpolateBuffers((s16 *)buffers->current, (s16 *)buffers->target,
                                (s16 *)buffers->delta, mode);
  }
}

void BattleEffect_BuildBuffer(s32 mode, struct BattleEffectBuffers *source,
                          u16 *destination, s32 half);

void BattleEffect_LoadBufferPalette(const u16 *colours) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    // The routine's entry in a local loads after the size; as a direct call,
    // 2 of 23 instructions differ in tla-en.
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    copy(buffers->palette, colours, 0x1c0);
    BattleEffect_BuildBuffer(0x10000, buffers, buffers->target, 1);
  }
}
