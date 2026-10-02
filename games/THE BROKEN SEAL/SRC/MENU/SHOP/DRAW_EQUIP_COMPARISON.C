#include "SHOP.H"
#include "BATTLE_RUNTIME.H"

extern struct ShopRuntime *gMenuWork;
extern u8 MsgItemName[];
extern u8 MsgShopCannotEquip[];
extern u8 MsgShopStatLabel[];

struct ItemDefinition *Item_Get(s32 item);
void RenderOutput_PrepareForRedrawFar(s32 window);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
s32 Item_CanOwnerEquip(s32 unit_id, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unit_id, s32 kind);
struct ShopCursorAnchor *RenderOutput_CreateFar(u32 resource, u32 flags, s32 window, s32 x, s32 y);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Owner_RecalculateStatsFar(s32 unit_id);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);

/* Draws how equipping an item would change a member's attack, defense and
 * agility: each stat without the item and, when it changes, with the item
 * tried on, an arrow for each change, and the name of the item it would
 * replace. The item is tried in the slot of the equipped item of its type,
 * else the first unequipped slot, else the first slot holding a type-6 item,
 * else the first slot. */
void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BattleUnit *unit = Owner_GetStateFar(unit_id);
    struct ItemDefinition *item = Item_Get(item_id);
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
                if (Item_Get(unit->inventory[i])->type == 6)
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
    Owner_RecalculateStatsFar(unit_id);
    with[0] = unit->attack;
    with[1] = unit->defense;
    with[2] = unit->agility;
    with[3] = unit->luck;
    unit->inventory[slot] = saved;
    Owner_RecalculateStatsFar(unit_id);
    without[0] = unit->attack;
    without[1] = unit->defense;
    without[2] = unit->agility;
    without[3] = unit->luck;
    for (i = 0; i <= 2; i++) {
        if (without[i] > with[i])
            RenderOutput_CreateFar(shop->stat_up_icon, 0x40000000, window, 56, i * 16 - 4)->unknown_00[4] = 0;
        else if (without[i] < with[i])
            RenderOutput_CreateFar(shop->stat_down_icon, 0x40000000, window, 56, i * 16 - 4)->unknown_00[4] = 0;
        UiNumber_DrawAt(without[i], 3, window, 32, i * 16);
        if (without[i] != with[i])
            UiNumber_DrawAt(with[i], 3, window, 72, i * 16);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
        UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
    }
    if (replaced != -1)
        UiText_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}
