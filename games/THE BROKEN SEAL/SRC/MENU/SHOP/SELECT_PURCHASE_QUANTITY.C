#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "TBS_EDITION.H"

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

extern u8 Value_00000ca0;
extern void UiMessage_ShowAndWait(s32);
extern s32 Item_FindSlot(s32, s32);
extern s32 Math_DivU(s32, s32);
extern s32 Ability_GetMaximum(s32, s32);
extern s32 Shop_RunQuantitySelector(s32, s32, s32);

s32 Shop_SelBuyNum(s32 unit_id, s32 item_id)
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
        UiMessage_ShowAndWait((s32)&Value_00000ca0);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = Math_DivU(SHOP_PARTY_STATE.money, item->price);

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
        result = Shop_RunQuantitySelector(quantity, chance, item->price);
    }
    return result;
}
