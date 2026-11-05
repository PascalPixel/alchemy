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
    u16 count = work->count & 3;
    u16 buf[16];

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
