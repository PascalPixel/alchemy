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
#include "ITEM.H"
#include "UI.H"
#include "text/MSG_IDS.H"
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
struct ItemDefinition *Item_GetDirectFar(s32 item);
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
struct ItemDefinition *Item_GetDirectFar(s32 item);

extern struct ShopRuntime *gMenuWork;

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);

void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BtlUnit *unit = UnitGetFar(unit_id);
    struct ItemDefinition *item = Item_GetDirectFar(item_id);
    s32 replaced = -1;
    s32 slot;
    s32 saved;
    s32 i;
    s32 without[4];
    s32 with[4];

    if (window == 0)
        return;
    RenderOutput_PrepareForRedrawFar(window);
    if (!Item_CanOwnerEquip(unit_id, item_id)) {
        UiText_DrawResourceFar((s32)MsgShopCannotEquip, window, 8, 24);
        return;
    }
    slot = Inventory_FindEquippedFar(unit_id, item->type);
    if (slot == -1) {
        for (i = 0; i <= 14; i++) {
            if (!(unit->inventory[i] & 0x200))
                break;
        }
        if (i == 15) {
            for (i = 0; i <= 14; i++) {
                if (Item_GetDirectFar(unit->inventory[i])->type == 6)
                    break;
            }
            if (i == 15)
                i = 0;
        }
        slot = i;
    } else {
        replaced = unit->inventory[slot] & 0x1ff;
    }
    saved = unit->inventory[slot];
    unit->inventory[slot] = item_id | 0x200;
    UnitCalcFar(unit_id);
    with[0] = unit->atk;
    with[1] = unit->def;
    with[2] = unit->agi;
    with[3] = unit->luck;
    unit->inventory[slot] = saved;
    UnitCalcFar(unit_id);
    without[0] = unit->atk;
    without[1] = unit->def;
    without[2] = unit->agi;
    without[3] = unit->luck;
    for (i = 0; i <= 2; i++) {
        if (without[i] > with[i])
            RenderOutput_CreateFar(shop->stat_down_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)->kind = 0;
        else if (without[i] < with[i])
            RenderOutput_CreateFar(shop->stat_up_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)->kind = 0;
        UiText_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
        if (without[i] != with[i])
            UiText_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
        UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
    }
    if (replaced != -1)
        UiText_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}
