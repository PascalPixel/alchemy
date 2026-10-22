// Draft, differ 15 of 118 (2026-10-19; was 33): draws the selected item's panel
// in the sell and drop menus: its name, then why it cannot be dropped or
// removed, or (selling) its price times the quantity held. Close to ☀️'s
// Shop_DrawUseItem (games/tbs/src/shop/shop.c) in what it draws. Left: other:
// the native loads the inventory halfword twice (once for the 0x1ff mask, again
// for the count), where CSE here reads it once; with it the high registers
// differ (the menu work in r11 natively, r9 here; slot*2 in r9, r10 here; the
// unit in r10, r11 here) and the mask's movs sits a slot earlier. Tried: the
// slot as a 9/2/5 bitfield struct (41, and a 4-byte slot). Now: buy.c's barrier
// before the 0x1ff mask and a memory barrier after it, which makes the count
// reload the halfword as native (33 to 15; 17 without the first). Left: the
// unit's copy to r10 comes before the first load (the second load then goes
// through it), the parameter copy and the work's move to r11 against sub sp. A
// volatile second read instead: 31.
#include "types.h"
#include "ram_buffer.h"

struct ShopRuntime {
  u8 unk000[0x50b];
  s8 dropMode;
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

// Unnamed in the catalog yet: 4673, 4674 and 4666 take placeholders here.
extern u8 Msg04673[], Msg04674[], Msg04666[];
// Message 607, item 0's name: item names follow it in item order.
extern u8 Msg00607[];

struct ShopUnit *Unit_Get(s32 unitId);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, void *window, s32 x,
                                  s32 y);
s32 Func_080ad268(s32 unitId, s32 slot);
s32 Func_080ad270(s32 unitId, s32 slot);
s32 Shop_GetSellPrice(s32 item);

void Shop_DrawSellItem(void *window, s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  u32 raw = unit->inventory[slot];
  s32 item;
  s32 quantity;
  s32 flags;
  s32 price;
  s32 message;

  asm volatile("");
  item = 0x1ff;
  item &= raw;
  asm volatile("" : : : "memory");
  quantity = (unit->inventory[slot] >> 11) + 1;
  if (window == 0)
    return;
  RenderOutput_RedrawSavedRectFar(window);
  UiText_DrawCharacterAtOffsetFar(item + (s32)Msg00607, window, 0, 0);
  if (shop->dropMode == 0)
    flags = Func_080ad268(unitId, slot);
  else
    flags = Func_080ad270(unitId, slot);
  if (flags & 2) {
    UiText_DrawCharacterAtOffsetFar((s32)Msg04673, window, 0, 8);
  } else if (flags & 1) {
    UiText_DrawCharacterAtOffsetFar((s32)Msg04674, window, 0, 8);
  } else if (shop->dropMode == 0) {
    price = Shop_GetSellPrice(unit->inventory[slot]) * quantity;
    message = (s32)Msg04666;
    UiText_DrawCharacterAtOffsetFar(message, window, 8, 8);
    UiText_DrawNumberInWindowFar(price, 5, window, 40, 8);
    UiText_DrawCharacterAtOffsetFar(message - 5, window, 80, 8);
  }
}
