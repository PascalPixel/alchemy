#include "types.h"
#include "item.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "edition.h"

extern u8 MsgNoItems[];
void RenderOutput_PrepareForRedrawFar(void *window);
void Func_0810bf98(void *window, s32 x, s32 y, s32 item);
s32 Inventory_CountFar(s32 item);
void UiText_DrawResourceFar(s32 message, void *window, s32 x, s32 y);

void Shop_DrawItemCount(void *window, s32 item) {
  if (window != NULL) {
    RenderOutput_PrepareForRedrawFar(window);
    Func_0810bf98(window, 8, 0, item);
    if (Inventory_CountFar(item) == 0)
#if (LANGUAGE == LANGUAGE_ITALIAN)
      UiText_DrawResourceFar((s32)MsgNoItems, window, 20, 12);
#else
      UiText_DrawResourceFar((s32)MsgNoItems, window, 8, 20);
#endif
  }
}

struct ShopCursor {
  struct RenderOutput *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct ShopSellWork {
  u8 unk000[0x4dc];
  struct ShopCursor cursor;
  u8 unk4ec[0x19];
  u8 mode;
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

void *UnitGet(s32 unitId);
s32 Shop_GetSellPrice(s32 item_id);
s32 Inventory_GetQuantityFar(s32 unitId, s32 slot);
void Func_081084f4(s32 messageId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Func_081096f8(s32 minimum, s32 maximum, s32 price);
void WaitFrames(s32 frames);
void UiIcon_PrepareObjectFar(struct RenderOutput *object);

#if (LANGUAGE == LANGUAGE_GERMAN)
#define QUANTITY_CURSOR_X 120
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                    \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define QUANTITY_CURSOR_X 104
#else
#define QUANTITY_CURSOR_X 128
#endif

/// Asks how many of a stackable item to sell, when the unit holds more than
/// one, and returns the count; 1 otherwise. The Broken Seal's
/// Shop_SelectSellNum.
s32 Func_0810a03c(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 price;
  s32 count;
  s32 selection;
  struct ItemDefinition *item;
  struct ShopSellWork *shop;
  struct ShopUnit *unit;

  shop = Ram_HeapSlots->menuRuntime;
  unit = UnitGet(unitId);
  item = Item_Get(unit->inventory[slot]);
  result = 1;
  price = Shop_GetSellPrice(unit->inventory[slot]);
  count = Inventory_GetQuantityFar(unitId, slot);
  selection = count;
  if ((item->flags & ITEM_STACKABLE) && count > 1) {
    Func_081084f4((s32)MsgHowManyToSell);
    savedX = shop->cursor.targetX;
    savedY = shop->cursor.targetY;
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    Shop_PlaceCursor(NULL, QUANTITY_CURSOR_X, 0x30);
    result = Func_081096f8(0, selection, price);
    WaitFrames(1);
    UiIcon_PrepareObjectFar(shop->cursor.anchor);
    Shop_PlaceCursor(NULL, savedX, savedY);
  }
  return result;
}
