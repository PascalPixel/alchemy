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
void Title_LoadBackground(void) {
  u8 *encodedGraphics;
  s32 resourceId;
  u16 *tilemap;
  u32 screenX;
  u32 screenY;
  u16 tileIndex;
  struct BgScroll *bgScroll;
  s32 blankTile;

  resourceId = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  REG_BG2CNT = 0x681;
  gBgScroll[2].y = 0;
  blankTile = 0x1ff;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 0x1c0;
  Decode_(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800,
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  // Fill the visible 30 columns and pad each 32-entry tilemap row.
  tilemap = (u16 *)0x06003000;
  tileIndex = 0x1a0;
  // Written with goto: as for loops, the pinned column counter keeps its exit
  // test ahead of the loop (differ 17).
  screenY = 0;
row:
  screenX = 0;
column:
  *tilemap++ = tileIndex++;
  screenX++;
  if (screenX <= 29)
    goto column;
  *tilemap++ = blankTile;
  *tilemap++ = blankTile;
  screenY++;
  if (screenY <= 19)
    goto row;
  // Reset all four background scroll pairs.
  bgScroll = gBgScroll;
  screenY = 0;
scroll_pair:
  bgScroll->y = 0;
  bgScroll->x = 0;
  bgScroll++;
  screenY++;
  if (screenY <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct TitleWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
}
