#include "SHOP.H"
#include "BATTLE_RUNTIME.H"

/* Sells count of one inventory slot (count -1 sells the whole stack as
   one): refuses worthless and cursed-and-equipped items, picks the offer
   text for broken, several, rare or ordinary items, and on agreement
   removes the items and pays out. */

extern u8 Value_00000cab[], Value_00000cac[], Value_00000cae[], Value_00000caf[];
extern u8 Value_00000cb0[], Value_00000cb1[], Value_00000cb2[], Value_00000cb3[];
extern u8 Value_00000cb4[], Value_00000cb5[], Value_00000cb6[];

struct ItemDefinition *Item_Get(s32 item);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void Audio_PlayCue(s32 cue);
void Func_080772b0(s32 unit_id, s32 slot);
void Party_AdjustSixDigitCounterAFar(s32 amount);

void Shop_SellItem(s32 unit_id, s32 slot, s32 count)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    struct BattleUnit *unit = BattleUnit_Get(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_Get(item_id);
    u8 rare = item->flags & 4;
    s32 all = 0;
    s32 total;
    s32 message;
    s32 i;

    if (count == -1) {
        all = 1;
        count = 1;
    }
    total = Shop_ComputeSalePrice(unit->inventory[slot]) * count;
    if (total == 0) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)Value_00000cac);
        return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)Value_00000cab);
        return;
    }
    if (all)
        message = (s32)Value_00000cb2;
    else if (unit->inventory[slot] & 0x400)
        message = (s32)Value_00000cb1;
    else if (count > 1)
        message = (s32)Value_00000cb0;
    else if (rare)
        message = (s32)Value_00000caf;
    else
        message = (s32)Value_00000cae;
    UiWork_PushValueSlotFar(item_id, 2);
    UiWork_PushValueSlotFar(total, 5);
    UiMessage_ShowAndRestoreState(message);
    if (UiMessage_ShowChoice(0) != 0) {
        if (rare || all)
            message = (s32)Value_00000cb6;
        else
            message = (s32)Value_00000cb4;
        UiMessage_ShowAndRestoreState(message);
        return;
    }
    Audio_PlayCue(102);
    for (i = 0; i < count; i++)
        Func_080772b0(unit_id, slot);
    Party_AdjustSixDigitCounterAFar(total);
    Shop_DrawMoney();
    Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
    if (rare || all)
        message = (s32)Value_00000cb5;
    else
        message = (s32)Value_00000cb3;
    UiMessage_ShowAndRestoreState(message);
}
