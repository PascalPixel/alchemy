#include "types.h"
#include "item.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "render_input.h"
#include "edition.h"

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

struct ShopWork {
  u8 unk00[0x24];
  void *itemWindow;
  u8 unk28[0x4dd];
  u8 mode;
  u8 unk506;
  s8 partyAction;
};

void *Unit_Get(s32 unitId);
s32 Inventory_FindFar(s32 unitId, s32 item_id);
extern u8 MsgYouHave[], MsgShopNoneInStock[];

void RenderOutput_PrepareForRedrawFar(struct RenderInput *window);
void UiText_DrawQuantity(s32 value, s32 digits);
void UiText_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void Func_0810bf98(struct RenderInput *window, s32 x, s32 y, s32 unit);

/// Shows how many of an item the member already carries and draws the
/// member's inventory icons through Func_0810bf98 at (8, 8).
// The twin of ☀️'s Shop_DrawUnitItem (games/tbs/src/shop/shop.c), whose icon
// loop ⚓️ moved into that helper. It matches with the twin's quantityOffset
// local (differ 10 with the offset inside the read).
void Func_081095b0(struct RenderInput *window, s32 unitId, s32 itemId) {
  u8 *unitBytes;
  s32 slot;
  s32 quantityOffset;

  unitBytes = (u8 *)Unit_Get(unitId);
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + 216;
      UiText_DrawQuantity((*(u16 *)(unitBytes + quantityOffset) >> 11) + 1, 5);
      UiText_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
    } else {
      UiText_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
    }
    Func_0810bf98(window, 8, 8, unitId);
  }
}

s32 Item_AdjustCounterFar(s32 item_id, s32 amount);
void Func_081084f4(s32 messageId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Func_081096f8(s32 minimum, s32 maximum, s32 price);
s32 __udivsi3(s32, s32);
extern u8 MsgHowManyToBuy[];

#if (LANGUAGE == LANGUAGE_GERMAN)
#define QUANTITY_CURSOR_Y 120
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                    \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define QUANTITY_CURSOR_Y 104
#else
#define QUANTITY_CURSOR_Y 128
#endif

s32 Shop_SelectBuyNum(s32 unitId, s32 itemId) {
  struct ShopWork *shop;
  struct ShopUnit *unit;
  struct ItemDefinition *item;
  s32 quantity;
  s32 chance;
  s32 slot;
  s32 maximum;
  s32 result;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  item = Item_Get(itemId);
  result = 1;
  if (item->flags & 0x10) {
    Func_081084f4((s32)MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1)
      quantity = (unit->inventory[slot] >> 11) + 1;
    else
      quantity = 0;

    chance = 30;
    if ((u16)item->price != 0)
      chance = __udivsi3(gPartyState.coins, (u16)item->price);

    if (shop->partyAction == 2) {
      maximum = Item_AdjustCounterFar(itemId, 0);
      if (chance > maximum)
        maximum = Item_AdjustCounterFar(itemId, 0);
      else
        maximum = chance;
      chance = maximum;
    }

    chance += quantity;
    if (chance > 30)
      chance = 30;

    shop->mode = 12;
    Shop_PlaceCursor(0, QUANTITY_CURSOR_Y, 0x30);
    result = Func_081096f8(quantity, chance, (u16)item->price);
  }
  return result;
}
