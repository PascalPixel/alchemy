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
  u32 step;
  s32 duration;
  u16 *bank;

  if (GameFlag_Test(0x152) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  step = ++work->step << 24;
  /* FAKEMATCH(one-pass): the duration's reload in a one-pass block, through
   * a local, which keeps the step's first shift ahead of the load; as
   * (s32)step >> 24 < work->duration, 2 of 175 instructions differ in
   * tla-en (an empty asm in its place also matches, as in
   * PaletteFade_Update). */
  do {
    duration = work->duration;
  } while (0);
  if ((s32)step >> 24 < duration) {
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
  bank = work->packed[work->page];
  IO_WRITE_QUEUE_PUSH(({
                        s32 address;
                        /* FAKEMATCH(one-pass): the BG palette's address in a
                         * one-pass block, as PaletteFade_Update writes it;
                         * as IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, ...), 4 of
                         * 175 instructions differ in tla-en. */
                        do {
                          address = (s32)BG_PLTT;
                        } while (0);
                        address;
                      }),
                      (u32)bank, DMA_COPY_WORDS(112));
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + 224), DMA_COPY_WORDS(112));
}
