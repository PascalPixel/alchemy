// Near miss, differ 2 of 136 (tla-en, 2026-10-31): instruction order only.
// The native spills selected (str r1, [sp, #8]) right after sub sp; here
// movs r3, #192, the first of the three moves that build the heap slots'
// address 0x030000dc, comes before it. In sched2 that move has priority 46
// (three steps ahead of the heap slot load, priority 43) and the spill 45
// (the same load costs 2 after a store under -mthumb-slow-loads), so the
// spill's priority always trails the move's by one and no reshaping moves
// the pair; permute 4 minutes found nothing better. No single empty asm or
// one-pass block reaches 0 (2026-10-26).
// First draft of Func_08109068: draws a seven-item stock page of the shop,
// the selection highlight, page arrows and price sprites; the twin of ☀️'s
// Shop_DrawStock (games/tbs/src/shop/shop/buy.c), with the page start written
// selected / 7 * 7 (the native divides and multiplies) and the zero locals
// of the twin.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopRuntime {
  u8 unk000[0x270];
  s16 stockItemIds[0x13f];
  u16 previousPageIcon;
  u16 nextPageIcon;
  u8 unk4f2[0x10];
  s16 stockCount;
};

struct ItemDefinition {
  u16 price;
};

void WindowSprite_PrepareForRedrawFar(struct RenderInput *window);
struct ItemDefinition *Item_Get(s32 item);
struct WindowSprite *WindowSprite_CreateLoadedFar(s32 item, s32 mode,
                                                  struct RenderInput *window,
                                                  s32 x, s32 y);
struct WindowSprite *Shop_CreatePriceSprite(s32 value,
                                            struct RenderInput *window, s32 x,
                                            s32 y);

void Func_08109068(struct RenderInput *window, s32 selected) {
  struct ShopRuntime *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  s32 pageSlot;
  s32 priceX;
  s32 zero;
  s32 highlightFrame;
  s16 itemId;
  struct ItemDefinition *definition;
  struct WindowSprite *icon;

  shop = Ram_HeapSlots->menuRuntime;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected / 7 * 7;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    216, -16);
      zero = 0;
      icon->kind = zero;
      icon->active = 17;
      icon->animStep = zero;
    }
    if (stockIndex + 7 < itemCount) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, 0x40000000, window, 216,
                                    24);
      zero = 0;
      icon->kind = zero;
      icon->active = 15;
      icon->animStep = zero;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (; (u32)pageSlot < 7 && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        definition = Item_Get(itemId);
        icon =
            WindowSprite_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->active = 9;
          highlightFrame = 10;
          icon->animStep = highlightFrame;
          icon->sentinel = 253;
        }
        icon = Shop_CreatePriceSprite(definition->price, window, priceX, 0);
        icon->sentinel = 251;
      }
    }
  }
}
