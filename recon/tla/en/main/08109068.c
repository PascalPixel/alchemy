// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 136 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8). No
// single empty asm or one-pass block reaches 0.
// First draft of Func_08109068: draws a seven-item stock page of the shop,
// the selection highlight, page arrows and price sprites; the twin of ☀️'s
// Shop_DrawStock (games/tbs/src/shop/shop.c), with the page start written
// selected / 7 * 7 (the native divides and multiplies) and the zero locals
// of the twin. Near miss, differ 6 of 136, instruction order only: the
// native spills selected (str r1, [sp, #8]) before it builds the work
// address 0x030000dc and the stockCount offset; here the spill comes after
// their first moves.
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
