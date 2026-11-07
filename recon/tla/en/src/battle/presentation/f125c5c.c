#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "battle_display.h"
#include "io_write.h"

extern const u8 gData_0812cc74[];

/// Copies this frame's BG2CNT word and starts the HBlank DMA of the BG2 lines
/// and the affine block, while the background view is in mode 2.
void BattleStaging_UpdateHBlankScroll(void) {
  struct BattleAffineHdma *records;
  u16 *record;

  if (((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)->mode ==
      2) {
    records = Ram_HeapSlots->unk28[0];
    record = records->lines[records->page];
    Io_Write16(record[0], &REG_BG2CNT);
    Dma_Set(record + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&records->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
  }
}

void Func_08125c5c(void) {
  u32 index = *(s32 *)Ram_HeapSlots->unk84[9] - 1;

  if (index <= 31)
    Dma_Set(gData_0812cc74 + (index >> 2) * 32, (void *)0x06005000,
            DMA_COPY_WORDS(8), REG_DMA3);
}

void BattleTransition_AdvanceTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  u32 frame;
  u32 next;

  timer = (u32 *)Ram_HeapSlots->unk84[9];
  frame = *timer;
  camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
  scroll = 0x34 - frame;
  if (scroll > 0x20)
    scroll = 0x20;
  if (scroll < 0)
    scroll = 0;
  gBgScroll[0].y = scroll;
  if (frame <= 0x50U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 0x50U) {
    BattleCamera_SetView(0, 0, 0, 0xb4 - next);
    return;
  }
  BattleCamera_SetView(0, 0, 0, 0x64);
}

/// Draws the transition's two rows of tiles, closing in from row 13 as the
/// timer runs; the lower row takes the vertically flipped tiles.
void BattleTransition_DrawRows(void) {
  u32 i;
  s32 frame;
  s32 aligned;
  s32 tile;
  u32 row;
  u16 *tiles;

  frame = *(s32 *)Ram_HeapSlots->unk84[9];
  if (frame >= 0 && frame <= 79) {
    tile = (7 & frame) + 0xf081;
    if (frame >= 0)
      aligned = frame;
    else
      aligned = frame + 7;
    {
      /* FAKEMATCH: both values pinned and held by the barrier load the
       * tilemap's address after the 13 and before the row is subtracted; as
       * plain C, 2 instructions differ in tla-en. */
      register s32 start asm("r2") = 13;
      /* FAKEMATCH: the second pin of the barrier above. */
      register u32 base asm("r5") = 0x06006000;

      /* FAKEMATCH: the barrier of the two pins above. */
      __asm__("" : "+l"(start), "+l"(base));
      row = start - (aligned >> 3);
      i = 0;
      tiles = (u16 *)((row << 6) + base);
    }
    do {
      i++;
      *tiles = tile;
      tiles++;
    } while (i != 32);

    tile |= 0x800;
    aligned = frame;
    if (frame < 0)
      aligned += 7;
    row = (aligned >> 3) + 13;
    if (row <= 20) {
      i = 0;
      tiles = (u16 *)((row << 6) + 0x06006000);
      do {
        i++;
        *tiles = tile;
        tiles++;
      } while (i != 32);
    }
  }
}
