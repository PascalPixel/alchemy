#include "shop.h"
#include "battle_runtime.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

extern u8 Value_00000ca0;
extern void Func_080b04dc(s32);
extern s32 Item_FindSlot(s32, s32);
extern s32 Func_080022f4(s32, s32);
extern s32 Ability_GetMaximum(s32, s32);
extern s32 Func_080b1614(s32, s32, s32);

s32 Shop_SelectPurchaseQuantity(s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop;
    struct BattleUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    item = Item_Get(item_id);
    result = 1;
    if (item->flags & 0x10) {
        Func_080b04dc((s32)&Value_00000ca0);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = Func_080022f4(SHOP_PARTY_STATE.money, item->price);

        if (shop->party_action == 2) {
            maximum = Ability_GetMaximum(item_id, 0);
            if (chance > maximum)
                maximum = Ability_GetMaximum(item_id, 0);
            else
                maximum = chance;
            chance = maximum;
        }

        chance += quantity;
        if (chance > 30)
            chance = 30;

        shop->mode = 12;
        Shop_PlaceCursor(0, ACTION_Y, 0x30);
        result = Func_080b1614(quantity, chance, item->price);
    }
    return result;
}
