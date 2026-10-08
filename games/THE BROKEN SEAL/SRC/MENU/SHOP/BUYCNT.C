#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SHOP.H"
#include "BTLSYS.H"
#include "PARTY_STATE.H"
#include "TBS_EDITION.H"
#include "UI.H"
#include "text/MSG_IDS.H"
void RenderOutput_PrepareForRedrawFar(s32 window);
s32 Inventory_FindFar(s32 unit_id, s32 item_id);
struct ItemDefinition *Item_GetDirectFar(s32 item);
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);

#define RenderOutput_CreateLoadedFar ((struct RenderOutput * (*)(u16,s32,s32,s32,s32))RenderOutput_CreateLoadedFar)

void UiWork_PushValueSlotFar(s32 kosuu, s32 style);

extern struct ShopRuntime *gMenuWork;

void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id)
{

    u8 *unit;
    s32 x;
    s32 y;
    s32 item_index;
    s32 slot;
    s32 off;
    s32 first_offset;
    s32 item_offset;
    s32 next_offset;
    u8 *icon;

    unit = (u8 *)UnitGetFar(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        RenderOutput_PrepareForRedrawFar(window);
        slot = Inventory_FindFar(unit_id, item_id);
        if (slot != -1) {
            off = slot * 2 + 216;
            UiWork_PushValueSlotFar((*(u16 *)(unit + off) >> 11) + 1, 5);
            UiText_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
        } else {
            UiText_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
        }
        item_index = 0;
        first_offset = 216;
        if (*(u16 *)(unit + first_offset) != 0) {
            for (;;) {
                item_offset = item_index * 2 + 216;
                icon = (u8 *)RenderOutput_CreateLoadedFar(*(u16 *)(unit + item_offset), 27,
                                     window, x, y);
                icon[15] = 252;
                x += 16;
                if (item_index == 4) {
                    x = 8;
                    y += 16;
                }
                if (item_index == 9) {
                    x = 8;
                    y += 16;
                }
                item_index++;
                if (item_index > 14)
                    break;
                next_offset = item_index * 2 + 216;
                if (*(u16 *)(unit + next_offset) == 0)
                    break;
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
    struct BtlUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = gMenuWork;
    unit = UnitGetFar(unit_id);
    item = Item_GetDirectFar(item_id);
    result = 1;
    if (item->flags & 0x10) {
        UiMessage_ShowAndWait((s32)MsgHowManyToBuy);
        slot = Inventory_FindFar(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = (u32)(gGameState.coins) / item->price;

        if (shop->party_action == 2) {
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
        Shop_PlaceCursor(0, ACTION_Y, 0x30);
        result = Shop_SelectQuantity(quantity, chance, item->price);
    }
    return result;
}
