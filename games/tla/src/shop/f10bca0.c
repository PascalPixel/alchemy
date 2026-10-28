#include "types.h"
#include "ram_buffer.h"

struct ShopRuntime {
  u8 unk000[0x50b];
  s8 dropMode; ///< 0 while selling, else dropping
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

extern u8 MsgItemName[];
extern u8 MsgCannotDrop[];
extern u8 MsgCannotRemove[];
extern u8 MsgPriceHeading[];

struct ShopUnit *Unit_Get(s32 unitId);
void WindowSprite_RedrawSavedRectFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, void *window, s32 x,
                                  s32 y);
s32 Func_080ad268(s32 unitId, s32 slot);
s32 Func_080ad270(s32 unitId, s32 slot);
s32 Shop_GetSellPrice(s32 item);

/// Draws the selected item's panel in the sell and drop lists: its name, then
/// why it cannot be dropped or removed, or, while selling, what the whole
/// stack would sell for. The Broken Seal's Shop_DrawUseItem, with this game's
/// check chosen by the list's mode. Called by Func_0810b9c0 each time the
/// cursor moves.
void Func_0810bca0(void *window, s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  s32 item = unit->inventory[slot] & 0x1ff;
  u32 raw = unit->inventory[slot];
  s32 quantity = (raw >> 11) + 1;
  s32 flags;
  s32 price;
  s32 message;

  if (window == 0)
    return;
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, 0, 0);
  if (shop->dropMode == 0)
    flags = Func_080ad268(unitId, slot);
  else
    flags = Func_080ad270(unitId, slot);
  if (flags & 2) {
    Text_DrawCharacterAtOffsetFar((s32)MsgCannotDrop, window, 0, 8);
  } else if (flags & 1) {
    Text_DrawCharacterAtOffsetFar((s32)MsgCannotRemove, window, 0, 8);
  } else if (shop->dropMode == 0) {
    price = Shop_GetSellPrice(unit->inventory[slot]) * quantity;
    message = (s32)MsgPriceHeading;
    Text_DrawCharacterAtOffsetFar(message, window, 8, 8);
    Text_DrawNumberInWindowFar(price, 5, window, 40, 8);
    // Five messages before the price heading: MsgCoins.
    Text_DrawCharacterAtOffsetFar(message - 5, window, 80, 8);
  }
}
