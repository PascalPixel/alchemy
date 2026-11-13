/// @file
/// The quantity bar, the price digits and their sprites, and the hand cursor.
// Part of shop.c.

#include "../shop_internal.h"
#include "module/shop/shop.h"
#include "module/tla.h"

void QuantityBar_Fill(s32 count, s32 selector, u8 *base) {
  u32 shifted = selector << 4;
  u16 *offset;

  selector = shifted + 1;

  if (count > 0) {
    offset = gShop_SelectorOffsets;
    do {
      struct TileQuantityBarSegmentView *record =
          (struct TileQuantityBarSegmentView *)(base + *offset++);
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

struct WindowSprite *Shop_CreatePriceSprite(s32 value, s32 window, s32 x,
                                            s32 y) {
  u8 *buf;
  s32 slot;
  struct WindowSprite *sprite;

  buf = HeapSlot_AllocEwram(56, 0x400);
  sprite = 0;
  Dma_Set(gShop_PriceTiles, buf, DMA_COPY_WORDS(64), REG_DMA3);
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
  slot = VramCache_FindFreeEntry();
  if (slot != 96) {
    VramCache_Load(slot, 0x100, buf);
    sprite = WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X32,
                                    (struct RenderInput *)window, x, y);
  }
  HeapSlot_Release(56);
  return sprite;
}

void Shop_SaveAndTintFieldPalette(s32 layer) {
  u8 *map = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->block32;
  u8 *saved = work + 0xe00;

  Dma_Set(saved, map + 0x222, DMA_COPY_WORDS(336), REG_DMA3);
  Dma_Set(saved, work + 0x380, DMA_COPY_WORDS(672), REG_DMA3);
  BattleEffect_ApplyColorToTargetBufferFar(layer, 1);
  BattleEffect_StartBufferInterpolationFar(16);
}

void Shop_RestoreFieldPalette(void) {
  BattleEffect_ApplyColorToTargetBufferFar(
      (s32)Ram_HeapSlots->eventWork + 0x222, 1);
  BattleEffect_StartBufferInterpolationFar(16);
}

// MenuCursor_Advance moves the shop cursor one step of its glide toward the
// target, setting the sprite's position and its OAM x and y, and stops the
// glide on its last step; The Broken Seal's MenuCursor_Advance
// (games/tbs/src/shop/shop.c). The x field is written through a u32 mask, as
// MenuCursor_MoveTowardTarget does, so the
// 0x1ff mask is split as the native's.

void MenuCursor_Advance(struct ShopCursor *cursor) {
  struct WindowSprite *sprite;
  s32 kind;
  s32 step;
  s32 x;
  s32 y;
  u32 mask;

  if (cursor == NULL)
    return;
  kind = cursor->kind;
  if (kind == 0)
    return;
  sprite = cursor->anchor;
  step = ++cursor->active;
  x = cursor->x + (cursor->targetX - (s16)cursor->x) * step / kind;
  sprite->x = x;
  mask = 0x1ff;
  mask &= x;
  sprite->sprite.oam.x = mask;
  y = cursor->y + (cursor->targetY - (s16)cursor->y) * step / kind;
  sprite->y = y;
  sprite->sprite.oam.y = y;
  if (step == kind) {
    cursor->kind = 0;
    cursor->active = 0;
  }
}

/// Moves the cursor's sprite a quarter of the way to its target on each
/// axis, at least one pixel.
void MenuCursor_MoveTowardTarget(struct ShopCursor *cursor) {
  struct ShopCursorSprite3 *sprite;
  s32 delta;
  s32 step;

  sprite = (struct ShopCursorSprite3 *)cursor->anchor;
  if (sprite != NULL) {
    delta = sprite->x - cursor->targetX;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->x = sprite->x - step;
      else
        sprite->x = sprite->x - 1;
      sprite->oam.x = sprite->x;
    } else if (delta < 0) {
      if (step != 0)
        sprite->x = sprite->x + step;
      else
        sprite->x = sprite->x + 1;
      sprite->oam.x = sprite->x;
    }

    delta = sprite->y - cursor->targetY;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->y = sprite->y - step;
      else
        sprite->y = sprite->y - 1;
      sprite->oam.y = sprite->y;
    } else if (delta < 0) {
      if (step != 0)
        sprite->y = sprite->y + step;
      else
        sprite->y = sprite->y + 1;
      sprite->oam.y = sprite->y;
    }
  }
}

void MenuCursor_Set(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind) {
  struct WindowSprite *anchor = cursor->anchor;

  cursor->x = (u16)anchor->x;
  cursor->y = (u16)anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->kind = kind;
  cursor->active = 0;
}

/// Puts the cursor at x, y at once: both its position and its target, and its
/// sprite with them.
void MenuCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y) {
  {
    struct ShopCursorSprite3 *sprite =
        (struct ShopCursorSprite3 *)cursor->anchor;

    cursor->targetX = x;
    cursor->kind = 1;
    cursor->active = 0;
    cursor->x = x;
    sprite->oam.x = sprite->x = x;
  }
  {
    struct ShopCursorSprite3 *sprite;

    cursor->targetY = y;
    cursor->y = y;
    sprite = (struct ShopCursorSprite3 *)cursor->anchor;
    sprite->oam.y = sprite->y = y;
  }
}

void MenuCursor_Place(void *window, s32 x, s32 y) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;

  if (window != NULL) {
    x += (s16)((struct RenderInput *)window)->x * 8 + 8;
    y += (s16)((struct RenderInput *)window)->y * 8 + 8;
  }
  MenuCursor_Set(&shop->cursor, x, y, (s8)shop->mode);
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

void VramBlock_LoadResource(s32 entryId, s32 size, s32 resourceId) {
  void *source;
  void *buffer;

  if (entryId > 95)
    return;
  source = Resource_GetTableEntry(resourceId);
  buffer = Heap_Alloc(size);
  Resource_DecodeByteLzInRam(source, buffer);
  VramCache_Load(entryId, size, buffer);
  Heap_Release(buffer);
}

#undef Resource_DecodeByteLzInRam
