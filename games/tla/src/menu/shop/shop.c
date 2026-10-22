#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "render_input.h"
#include "mem.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "edition.h"

struct Record_080b06c0 {
  u8 filler0[4];
  u8 values[21];
};

void Shop_FillSelector(s32 count, s32 selector, u8 *base) {
  u32 shifted = selector << 4;
  u16 *offset;

  selector = shifted + 1;

  if (count > 0) {
    offset = gShop_SelectorOffsets;
    do {
      struct Record_080b06c0 *record =
          (struct Record_080b06c0 *)(base + *offset++);
      record->values[0] = selector;
      record->values[4] = selector;
      record->values[8] = selector;
      record->values[12] = selector;
      record->values[16] = selector;
      record->values[20] = selector;
      count--;
    } while (count != 0);
  }
}

void Shop_CopyGlyphs(s32 digit, u8 *tiles, u32 place) {
  u8 *src = gShop_GlyphBytes + ((u32)digit << 5);
  u8 *dst = tiles + gShop_GlyphRowOffsets[place] + 2;
  s32 count = 3;

  do {
    if (*src != 0) {
      dst[0] = *src++;
      if (*src != 0) {
        dst[1] = *src++;
        if (*src != 0) {
          dst[30] = *src++;
          if (*src != 0)
            dst[31] = *src++;
        }
      }
    }
    dst += 4;
    count--;
  } while (count >= 0);
}

extern const u8 gShop_PriceTiles[];

struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x,
                                            s32 y) {
  u8 *buf;
  s32 slot;
  struct RenderOutput *sprite;

  buf = Runtime_AllocateBlock(56, 0x400);
  sprite = 0;
  Dma_Set(gShop_PriceTiles, buf, 0x84000040, REG_DMA3);
  Shop_CopyGlyphs(value % 10, buf, 0);
  value = value / 10;
  if (value != 0) {
    Shop_CopyGlyphs(value % 10, buf, 1);
    value = value / 10;
    if (value != 0) {
      Shop_CopyGlyphs(value % 10, buf, 2);
      value = value / 10;
      if (value != 0) {
        s32 last;

        Shop_CopyGlyphs(value % 10, buf, 3);
        last = value / 10;
        if (last != 0)
          Shop_CopyGlyphs(last % 10, buf, 4);
      }
    }
  }
  slot = Resource_FindFreeEntry();
  if (slot != 96) {
    VramCache_Load(slot, 0x100, buf);
    sprite = RenderOutput_CreateFar(slot, 0x80008000,
                                    (struct RenderInput *)window, x, y);
  }
  Runtime_ReleaseHeapBlock(56);
  return sprite;
}

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#else
#endif

void Shop_RestoreSceneTiles(s32 layer) {
  u8 *map = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->block32;
  u8 *saved = work + 0xe00;

  Dma_Set(saved, map + 0x222, 0x84000150, REG_DMA3);
  Dma_Set(saved, work + 0x380, 0x840002a0, REG_DMA3);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  Func_080c8368(layer, 1);
  BattleEffect_ApplyColorToSourceBufferFar(16);
#else
  BattleEffect_ApplyColorToTargetBufferFar(layer, 1);
  BattleEffect_StartBufferInterpolationFar(16);
#endif
}

void Shop_InitializeEffect(void) {
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  Func_080c8368((s32)Ram_HeapSlots->eventWork + 0x222, 1);
  BattleEffect_ApplyColorToSourceBufferFar(16);
#else
  BattleEffect_ApplyColorToTargetBufferFar((s32)Ram_HeapSlots->eventWork + 0x222,
                                       1);
  BattleEffect_StartBufferInterpolationFar(16);
#endif
}
