#include "types.h"
#include "item.h"
#include "ram_buffer.h"
#include "party_state.h"

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

void *UnitGet(s32 unitId);
s32 Inventory_FindFar(s32 unitId, s32 item_id);
s32 Item_AdjustCounterFar(s32 item_id, s32 amount);
void Func_081084f4(s32 messageId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Func_081096f8(s32 minimum, s32 maximum, s32 price);
s32 __udivsi3(s32, s32);
extern u8 MsgHowManyToBuy[];

#if defined(TLA_EDITION_DE)
#define QUANTITY_CURSOR_Y 120
#elif defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define QUANTITY_CURSOR_Y 104
#else
#define QUANTITY_CURSOR_Y 128
#endif

s32 Shop_SelectBuyNum(s32 unitId, s32 item_id)
{
    struct ShopWork *shop;
    struct ShopUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = Ram_HeapSlots->menuRuntime;
    unit = UnitGet(unitId);
    item = Item_Get(item_id);
    result = 1;
    if (item->flags & 0x10) {
        Func_081084f4((s32)MsgHowManyToBuy);
        slot = Inventory_FindFar(unitId, item_id);
        if (slot != -1)
            quantity = (unit->inventory[slot] >> 11) + 1;
        else
            quantity = 0;

        chance = 30;
        if ((u16)item->price != 0)
            chance = __udivsi3(gPartyState.coins, (u16)item->price);

        if (shop->partyAction == 2) {
            maximum = Item_AdjustCounterFar(item_id, 0);
            if (chance > maximum)
                maximum = Item_AdjustCounterFar(item_id, 0);
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
