#include "SHOP.H"
#include "BATTLE_RUNTIME.H"

/* Repairs one item for a party member at the shop: refuses items that
   cannot be repaired, are not broken or cost more than the party has, and
   otherwise takes the money, plays the smithing sounds and offers to equip
   the mended item. */

extern u8 Value_00000cbe[];

#define MSG_REPAIR_CONFIRM ((s32)Value_00000cbe)

struct ItemDefinition *Item_Get(s32 item);
s32 Inventory_FindEquippedFar(s32 unit_id, u8 kind);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Func_08077068(s32 unit_id, s32 slot);
void Party_AdjustSixDigitCounterAFar(s32 amount);

void Shop_RepairItem(s32 unit_id, s32 slot)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    struct BattleUnit *unit = BattleUnit_Get(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_Get(item_id);
    s32 equipped = Inventory_FindEquippedFar(unit_id, item->type);
    u32 price = Shop_ComputeRepairPrice(unit->inventory[slot]);
    s32 message;
    u8 kind;
    u32 saved;

    kind = item->use_type;
    if (kind != 2) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState(0xcba);
        return;
    }
    if (!(unit->inventory[slot] & 0x400)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState(0xcbb);
        return;
    }
    if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState(0xcbc);
        return;
    }
    if (price > SHOP_PARTY_STATE.money) {
        UiMessage_ShowAndRestoreState(0xcbd);
        return;
    }
    UiWork_PushValueSlotFar(item_id, 2);
    UiWork_PushValueSlotFar(price, 5);
    message = MSG_REPAIR_CONFIRM;
    UiMessage_ShowAndRestoreState(message);
    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndRestoreState(message + 1);
        return;
    }
    saved = unit->inventory[slot];
    unit->inventory[slot] = 0;
    Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
    UiWork_PushValueSlotFar(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 2);
    UiWork_FinalizePendingCoreFar();
    WaitFrames(10);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(112);
    WaitFrames(20);
    unit->inventory[slot] = saved;
    Func_08077068(unit_id, slot);
    Party_AdjustSixDigitCounterAFar(-price);
    Shop_DrawMoney();
    Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
    UiWork_PushValueSlotFar(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 3);
    if (Shop_ConfirmEquipItem(unit_id, slot))
        Shop_SellReplacedItem(unit_id, equipped);
}
