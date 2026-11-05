// Draft, differ 6 of 145 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 6 of 145 (tla-en); older figures below predate it.
// Draft, differ 107 of 145, skeleton gap 27 (tla-en, 2026-11-01)
// Overlay 674's palette queue step, the twin of ☀️'s PaletteCycle_Step and of
// the main image's PaletteQueue_StepCycles behind three event-work checks.
// Reading position and count as s16 into u16 locals gives the native's ldrsh,
// the lsl/lsr pairs and the rotation's whole tail; masking in a second statement
// gives its ldrh, movs #3 order. Left: the count stays unshifted where the
// native keeps (count & 3) << 16 in fp and shifts it back each pass, and this
// compile hoists sp and len - start out of the two inner loops where the native
// redoes them in the body, so the allocation of the inner block differs.
// Tried without gain: all 120 declaration orders of the inner block, while and
// do forms of both loops, s32 and s16 locals with casts, bit-field and split
// masks, an inline wrapper, *(buf + j), and s16 timer, buffer and source types;
// goto loops are worse. alchemy permute (6 minutes, on the older form) found
// nothing closer.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

struct PaletteCycle {
  u16 *source;
  s16 position;
  u16 timer;
  s16 delay;
  s16 count;
  u16 colors[16];
};

struct PaletteCycleWork {
  struct PaletteCycle entries[4];
  u16 count;
};

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->unk70[0])
#define EVENT_S16(offset) (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + (offset)))

void Func_020034fc(void) {
  if (EVENT_S16(0x166) == 0 && EVENT_S16(0x15a) == 0 && EVENT_S16(0x15e) == 0) {
    struct PaletteCycleWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count;
    u16 buf[16];

    count &= 3;
    for (i = 0; i < count; i++) {
      struct PaletteCycle *cycle = &work->entries[i];
      if (cycle->timer == 0) {
        u16 start = cycle->position;
        u16 len = cycle->count;
        void *dest = cycle->source;
        u16 *src = cycle->colors;
        u8 j;
        for (j = len - start; j < len; j++)
          buf[j] = *src++;
        for (j = 0; j < len - start; j++)
          buf[j] = *src++;
        Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
        start++;
        if (start >= len)
          start = 0;
        cycle->position = start;
        cycle->timer = cycle->delay;
      } else {
        cycle->timer--;
      }
    }
  }
}
