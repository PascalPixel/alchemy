#include "types.h"
#include "mem.h"
#include "display_transition.h"
#include "dma.h"
#include "system.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "ram_buffer.h"

/// The Broken Seal's ScreenTransition_FillTilemapAndSolidTile: blanks the
/// screen block at 0x06002000 and, unless color is -1, fills the first tile
/// with that colour.
void DisplayTransition_FillTilemapAndSolidTile(s32 color) {
  struct DisplayTransitionState *work = Ram_HeapSlots->displayTransition;
  Dma_Fill32(0xf000f000, (void *)0x06002000, 0x500);
  if (color != -1) {
    u32 pattern = 0;
    s32 cnt;
    u32 *tile;
    for (cnt = 7; cnt >= 0; --cnt)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (cnt = 7; cnt >= 0; --cnt)
      *tile++ = pattern;
    Dma_Set(work->solidTile, (void *)0x06000000, 0x84000008,
            (volatile u32 *)0x040000d4);
  }
}
