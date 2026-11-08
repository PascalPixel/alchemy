#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write_queue.h"
#include "palette_buffers.h"
#include "ram_buffer.h"

/// Steps the palette fade on heap slot 0x80 by one frame, as The Broken Seal's
/// PaletteFade_Update does: adds each channel's step until the last frame,
/// which copies the target instead, packs the channels into the hidden page
/// of RGB555 colours and queues that page for the BG and OBJ palettes.
void PaletteFade_Update(void) {
  struct PaletteFade *work = Ram_HeapSlots->block32;
  u16 *delta = work->delta;
  u16 *current;
  u16 *packed;
  s32 i;
  u16 *bank;
  s32 blue;

  if (work->duration == 0)
    return;
  if ((s8)++work->step < work->duration) {
    current = work->current;
    for (i = 0; i <= 0x5ff; i++)
      *current++ += *delta++;
  } else {
    Dma_Set(work->target, work->current, DMA_COPY_WORDS(768), REG_DMA3);
    work->duration = 0;
  }
  packed = work->packed[work->page ^ 1];
  blue = 0x7c00;
  i = 512;
  current = work->current;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                (((s16)current[2] >> 10) & 0x1f);
    current += 3;
  }
  work->page ^= 1;
  bank = work->packed[work->page];
  IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)bank, DMA_COPY_WORDS(128));
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + 256), DMA_COPY_WORDS(128));
}
