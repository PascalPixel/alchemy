#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "heap_release.h"
#include "dma.h"
#include "ram_buffer.h"

struct DmaFxPage {
  u32 lines[0x1e0];
};

struct DmaFxPages {
  struct DmaFxPage pages[2];
  u8 page;
};

/// Writes the current page's first entry each frame; HBlank DMA writes the
/// rest.
void Func_080d5ff8(void) {
  struct DmaFxPages *work = Ram_HeapSlots->unk84[1];
  u32 *line = work->pages[work->page].lines;
  volatile u32 *dest = (volatile u32 *)&REG_BG1HOFS;
  u8 *event = Ram_HeapSlots->eventWork;
  volatile u32 *target;

  if (event != NULL) {
    if (*(s8 *)(event + 0xca4) != 0)
      return;
    if (*(s16 *)(event + 0x164) != 0)
      return;
  }
  {
    volatile u16 *dma = REG_DMA0;
    u16 control;

    /* FAKEMATCH: the one-pass block reads the control word before its mask is
     * built (one reordered pair, differ 2 without it), as in Func_080d64b8. */
    do {
      control = dma[5];
    } while (0);
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    target = dest;
    /* FAKEMATCH: the barrier copies the transfer destination before the last
     * control read, as the native does; plain, the copy follows the first line
     * word (one reordered pair, differ 2). */
    asm volatile("" : "+l"(target));
    (void)dma[5];
  }
  *dest = *line++;
  *dest = *line++;
  *dest = *line++;
  Dma_Set(
      line, (void *)target,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)
              << 16 |
          3,
      REG_DMA0);
}
