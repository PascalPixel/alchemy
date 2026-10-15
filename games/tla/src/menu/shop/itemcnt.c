#include "types.h"
#include "item.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

extern u8 MsgNoItems[];
void RenderOutput_PrepareForRedrawFar(void *window);
void Func_0810bf98(void *window, s32 x, s32 y, s32 item);
s32 Inventory_CountFar(s32 item);
void UiText_DrawResourceFar(s32 message, void *window, s32 x, s32 y);

#if !defined(TLA_EDITION_JA) && !defined(TLA_EDITION_DE) && !defined(TLA_EDITION_ES) /* their copies differ and stay on their scaffold (S4) */
void Shop_DrawItemCount(void *window, s32 item)
{
    if (window != NULL) {
        RenderOutput_PrepareForRedrawFar(window);
        Func_0810bf98(window, 8, 0, item);
        if (Inventory_CountFar(item) == 0)
#if defined(TLA_EDITION_IT)
            UiText_DrawResourceFar((s32)MsgNoItems, window, 20, 12);
#else
            UiText_DrawResourceFar((s32)MsgNoItems, window, 8, 20);
#endif
    }
}
#endif

struct ShopCursor {
    struct RenderOutput *anchor;
    u16 x;
    u16 y;
    s16 target_x;
    s16 target_y;
    s8 active;
    s8 kind;
};

struct ShopSellWork {
    u8 unknown_000[0x4dc];
    struct ShopCursor cursor;
    u8 unknown_4ec[0x19];
    u8 mode;
};

struct ShopUnit {
    u8 unknown_00[216];
    u16 inventory[15];
};

void *UnitGet(s32 unit_id);
s32 Shop_GetSellPrice(s32 item_id);
s32 Inventory_GetQuantityFar(s32 unit_id, s32 slot);
void Func_081084f4(s32 message_id);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Func_081096f8(s32 minimum, s32 maximum, s32 price);
void WaitFrames(s32 frames);
void UiIcon_PrepareObjectFar(struct RenderOutput *object);

#if defined(TLA_EDITION_DE)
#define QUANTITY_CURSOR_X 120
#elif defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define QUANTITY_CURSOR_X 104
#else
#define QUANTITY_CURSOR_X 128
#endif

// Asks how many of a stackable item to sell, when the unit holds more than
// one, and returns the count; 1 otherwise. The Broken Seal's
// Shop_SelSellNum.
s32 Func_0810a03c(s32 unit_id, s32 slot)
{
    s32 result;
    s16 saved_x;
    s16 saved_y;
    s32 price;
    s32 count;
    s32 selection;
    struct ItemDefinition *item;
    struct ShopSellWork *shop;
    struct ShopUnit *unit;

    shop = Ram_HeapSlots->menu_runtime;
    unit = UnitGet(unit_id);
    item = Item_Get(unit->inventory[slot]);
    result = 1;
    price = Shop_GetSellPrice(unit->inventory[slot]);
    count = Inventory_GetQuantityFar(unit_id, slot);
    selection = count;
    if ((item->flags & ITEM_STACKABLE) && count > 1) {
        Func_081084f4((s32)MsgHowManyToSell);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        Shop_PlaceCursor(NULL, QUANTITY_CURSOR_X, 0x30);
        result = Func_081096f8(0, selection, price);
        WaitFrames(1);
        UiIcon_PrepareObjectFar(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}
