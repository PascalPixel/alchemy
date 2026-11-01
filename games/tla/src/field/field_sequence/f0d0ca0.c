#include "types.h"
#include "palette_buffers.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"

s32 GameFlag_Test(s32 flag);

void BattlePalette_UpdateBlend(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *add = work->delta;
  s32 i;

  if (GameFlag_Test(0x152) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  if (++work->step < work->duration) {
    u16 *sum = work->current;
    for (i = 0; i < 0x540; i++) {
      *sum++ += *add++;
    }
  } else {
    /* FAKEMATCH(temporary): called through a local pointer to its fixed
     * entry, which loads the entry ahead of the size's shift; as a plain
     * Iwram_CopyWords call, 2 of 175 instructions differ in tla-en. */
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    copy(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  {
    u16 *packed = work->packed[1 ^ work->page];
    s32 blue = 0x7c00;
    u16 *current;

    i = 0x1c0;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                  (((s16)current[2] >> 10) & 0x1f);
      current += 3;
    }
  }

  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed, DMA_COPY_WORDS(112));
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + 224),
                        DMA_COPY_WORDS(112));
  }
}
