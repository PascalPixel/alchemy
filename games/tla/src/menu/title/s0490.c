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

void Fade_ToBlack(s32 frames);

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH: forwarding through the inline keeps the buffer address a
   * constant of each call, as ☀️'s engine call layer does (called directly, the
   * address is kept in r5). */
  Resource_DecodeType01(source, destination);
}

// The title logo, as ☀️'s Title_LoadBackground: its palette, its tiles
// decoded and copied to VRAM, a 30 by 20 tilemap of consecutive tiles with
// two blank cells closing each row, and the four scroll pairs cleared.
void Func_02000490(void) {
  u8 *res;
  s32 id;
  u16 *map;
  /* FAKEMATCH: the column counter is pinned to r4: the native keeps the row
   * counter in r0 and the column counter in r4, where this C (like ☀️'s native)
   * gives the column counter r0 (differ 20 unpinned; for loops with the pin,
   * 17). */
  register u32 x asm("r4");
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blank;

  id = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  {
    /* FAKEMATCH: the control value is pinned to r3 as a halfword, and a
     * one-pass block keeps it ahead of the register's address: the native loads
     * 0x681 before it builds BG2CNT's address (plainly, one reordered row; an
     * empty asm moves more). */
    register u16 control asm("r3");

    control = 0x681;
    do {
    } while (0);
    REG_BG2CNT = control;
  }
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)0x05000000, 0x84000070, REG_DMA3);
  res += 0x1c0;
  Decode_(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
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
  Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
  ((struct TitleWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
}
