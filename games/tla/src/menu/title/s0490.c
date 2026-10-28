#include "types.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "resource.h"

struct TitleWork {
  u8 unk00[20];
  u16 mode;
};

extern char ResourceId_LogoOnBlack;

void Fade_ToBlack(s32 vblanks);

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH: forwarding through the inline keeps the buffer address a
   * constant of each call, as ☀️'s system call layer does (called directly, the
   * address is kept in r5). */
  Resource_DecodeType01(source, destination);
}

/// The title logo: its palette, its tiles decoded and copied to VRAM, a 30 by
/// 20 tilemap of consecutive tiles with two blank cells closing each row, and
/// the four scroll pairs cleared.
// As ☀️'s Title_LoadBackground.
void Func_02000490(void) {
  u8 *res;
  s32 id;
  u16 *map;
  /* FAKEMATCH: the column counter pinned to r4 and the row counter left in
   * r0, as native; unpinned, 20 instructions differ in tla-en. */
  register u32 x asm("r4");
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blank;

  id = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  {
    /* FAKEMATCH: the control value pinned to r3 in a one-pass block loads
     * 0x681 before BG2CNT's address, as native; plainly, one row is reordered
     * in tla-en. */
    register u16 control asm("r3");

    control = 0x681;
    do {
    } while (0);
    REG_BG2CNT = control;
  }
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  res += 0x1c0;
  Decode_(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, DMA_COPY_WORDS(9600),
          REG_DMA3);
  /* FAKEMATCH: a one-pass block ends the scheduling block, so the tilemap
   * address loads before the first tile number is built (one reordered row
   * without it). */
  do {
  } while (0);
  map = (u16 *)0x06003000;
  tile = 0x1a0;
  // Written with goto: as for loops, the pinned column counter keeps its exit
  // test ahead of the loop (differ 17).
  y = 0;
row:
  x = 0;
column:
  *map++ = tile++;
  x++;
  if (x <= 29)
    goto column;
  *map++ = blank;
  *map++ = blank;
  y++;
  if (y <= 19)
    goto row;
  scroll = gBgScroll;
  y = 0;
scroll_pair:
  scroll->y = 0;
  scroll->x = 0;
  scroll++;
  y++;
  if (y <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct TitleWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
}
