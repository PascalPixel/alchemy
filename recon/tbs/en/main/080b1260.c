#include "SHOP.H"
#include "BATTLE_RUNTIME.H"

/* main:080b1260 Shop_DrawEquipComparison - hand-written draft, 192 of 264
   halfwords differ (500 of 528 bytes). Left: the ROM dumps two literal
   pools mid-function (one holding the 0x200 equipped bit and the 0x1ff
   item mask), keeps the tried item in its parameter register, and keeps
   the luck pointer on the stack; the stat loop's registers differ.

   Draws how equipping an item would change a member's attack, defense
   and agility: the stat with the item tried on and without it, an arrow
   for each change, and the name of the item it would replace. */

extern u8 Value_00000c98[];
extern u8 Value_00000182[];

struct ItemDefinition *Item_Get(s32 item);
void Func_08015060(s32 window);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
s32 Item_CanOwnerEquip(s32 unit_id, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unit_id, u8 kind);
struct ShopCursorAnchor *RenderOutput_CreateFar(u32 resource, u32 flags, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);

void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    struct BattleUnit *unit = BattleUnit_Get(unit_id);
    struct ItemDefinition *item = Item_Get(item_id);
    s32 replaced = -1;
    s32 slot;
    u32 saved;
    s32 i;
    s32 y;
    s32 row;
    s32 x;
    s32 with[4];
    s32 without[4];

    if (window == 0)
        return;
    Func_08015060(window);
    if (!Item_CanOwnerEquip(unit_id, item_id)) {
        Func_08015078(0xc8e, window, 8, 24);
        return;
    }
    slot = Inventory_FindEquippedFar(unit_id, item->type);
    if (slot == replaced) {
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
    item_id |= 0x200;
    saved = unit->inventory[slot];
    unit->inventory[slot] = item_id;
    BattleUnit_Recalculate(unit_id);
    with[0] = unit->attack;
    with[1] = unit->defense;
    with[2] = unit->agility;
    with[3] = unit->luck;
    unit->inventory[slot] = saved;
    BattleUnit_Recalculate(unit_id);
    without[0] = unit->attack;
    without[1] = unit->defense;
    without[2] = unit->agility;
    without[3] = unit->luck;
    row = 2;
    for (i = 0, y = 0; i <= 2; i++, y += 16) {
        if (without[i] > with[i]) {
            RenderOutput_CreateFar(shop->stat_up_icon, 0x40000000, window, 56, y - 4)->unknown_00[4] = 0;
        } else if (without[i] < with[i]) {
            RenderOutput_CreateFar(shop->stat_down_icon, 0x40000000, window, 56, y - 4)->unknown_00[4] = 0;
        }
        UiText_DrawNumberInWindowFar(without[i], 3, window, 32, y);
        if (without[i] != with[i])
            UiText_DrawNumberInWindowFar(with[i], 3, window, 72, y);
        UiText_DrawCharacterAtOffsetFar((s32)Value_00000c98 + i, window, 0, y);
        UiWindow_DrawDividerLineFar(window, 0, row, 13, row);
        row += 2;
    }
    if (replaced != -1)
        UiText_DrawCharacterAtOffsetFar(replaced + (s32)Value_00000182, window, 0, 48);
}
