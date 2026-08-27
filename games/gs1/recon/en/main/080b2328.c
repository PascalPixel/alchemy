#include "battle_runtime.h"
#include "item.h"
#include "shop.h"

s32 Func_08077228(s32 unit_id, u8 type);
s32 Func_08077068(s32 unit_id, s32 slot);
s32 Func_08077230(s32 amount);
s32 Func_080b1868(s32 unit_id, s32 slot);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);
void UiText_DrawQuantity(u32 value, u32 mode);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 mode);
void UiWork_FinalizePending(void);

extern char Value_00000cba;
extern char Value_00000cbb;
extern char Value_00000cbc;
extern char Value_00000cbd;
extern char Value_00000cbe;

#define Shop_RepairItem Func_080b2328

void Shop_RepairItem(s32 unit_id, s32 slot)
{
    struct ShopRuntime *shop;
    s32 item_id;
    struct BattleUnit *unit;
    struct ItemDefinition *item;
    u16 raw_item;
    u16 saved_item;
    s32 price;
    s32 replaced_slot;
    s32 message;
    s32 confirm;

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    item_id = unit->inventory[slot] & 0x1ff;
    item = Item_Get(item_id);
    replaced_slot = Func_08077228(unit_id, item->type);
    raw_item = unit->inventory[slot];
    price = Shop_ComputeRepairPrice(raw_item);

    if (item->use_type != 2) {
        UiText_DrawQuantity(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)&Value_00000cba);
        return;
    }
    if ((raw_item & 0x400) == 0) {
        UiText_DrawQuantity(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)&Value_00000cbb);
        return;
    }
    if ((raw_item & 0x200) != 0 && (item->flags & item->use_type) != 0) {
        UiText_DrawQuantity(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)&Value_00000cbc);
        return;
    }
    if (price > SHOP_PARTY_STATE.money) {
        UiMessage_ShowAndRestoreState((s32)&Value_00000cbd);
        return;
    }

    UiText_DrawQuantity(item_id, 2);
    UiText_DrawQuantity(price, 5);
    message = (s32)&Value_00000cbe;
    UiMessage_ShowAndRestoreState(message);
    confirm = UiMessage_ShowChoice(0);
    if (confirm != 0) {
        UiMessage_ShowAndRestoreState(message + 1);
        return;
    }

    saved_item = unit->inventory[slot];
    unit->inventory[slot] = confirm;
    Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
    UiText_DrawQuantity(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 2);
    UiWork_FinalizePending();
    WaitFrames(10);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(112);
    WaitFrames(20);
    unit->inventory[slot] = saved_item;
    Func_08077068(unit_id, slot);
    Func_08077230(-price);
    Shop_DrawMoney();
    Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
    UiText_DrawQuantity(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 3);
    if (Func_080b1868(unit_id, slot) != 0) {
        Shop_SellReplacedItem(unit_id, replaced_slot);
    }
}
