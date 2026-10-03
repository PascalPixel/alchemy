#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "PARTY_STATE.H"
#include "TBS_EDITION.H"

extern u8 MsgYouHave;
extern u8 MsgShopNoneInStock;
void UiWindow_Clear(s32 window);
s32 Item_FindSlot(s32 unit_id, s32 item_id);
void UiWork_PushValueSlotFar(s32 kosuu, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
struct RenderOutput *UiIcon_Draw(u16 no, s32 kind, s32 window, s32 x, s32 y);

extern struct ShopRuntime *gMenuWork;
extern u8 MsgHowManyToBuy;
extern void UiMessage_ShowAndWait(s32);
extern s32 Item_FindSlot(s32, s32);
extern s32 Ability_GetMaximum(s32, s32);
extern s32 Shop_SelectQuantity(s32, s32, s32);

/* Draw the member's inventory in three rows of five icons, with the
   number already owned above them when the chosen item is in the bag. */
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id)
{
    struct BattleUnit *unit;
    struct RenderOutput *icon;
    s32 x;
    s32 y;
    s32 index;
    s32 slot;

    unit = Owner_GetStateFar(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        UiWindow_Clear(window);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            UiWork_PushValueSlotFar((unit->inventory[slot] >> 11) + 1, 5);
            UiText_DrawCharacterAtOffsetFar((s32)&MsgYouHave, window, 0, 0);
        } else {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgShopNoneInStock, window, 0, 0);
        }
        for (index = 0; index < 15 && unit->inventory[index] != 0; index++) {
            icon = UiIcon_Draw(unit->inventory[index], 27, window, x, y);
            icon->sentinel = 252;
            x += 16;
            if (index == 4 || index == 9) {
                x = 8;
                y += 16;
            }
        }
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

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

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
    item = Item_Get(item_id);
    result = 1;
    if (item->flags & 0x10) {
        UiMessage_ShowAndWait((s32)&MsgHowManyToBuy);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = (u32)(gGameState.coins) / item->price;

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
        result = Shop_SelectQuantity(quantity, chance, item->price);
    }
    return result;
}
