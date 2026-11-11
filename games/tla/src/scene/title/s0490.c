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
  /* FAKEMATCH(wrapper): the decode forwarded through this inline keeps the
   * buffer address a constant of each call, as The Broken Seal's system call
   * layer does; as a direct call, which keeps the address in r5, 17
   * of 94 instructions differ in tla-en. */
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
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blank;

  id = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  {
    /* FAKEMATCH(pin): the control value pinned to r3, so 0x681 loads before
     * BG2CNT's address; as a plain store, one row is reordered in tla-en. */
    register u16 control asm("r3");

    control = 0x681;

    REG_BG2CNT = control;
  }
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  res += 0x1c0;
  Decode_(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800,
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

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
