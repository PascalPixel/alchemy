#include "BATTLE_RUNTIME.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 Data_03001c94[];
extern u8 gKeysRepeat[];

s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_FindEquippedFar(s32, u8);
s32 Party_AdjustSixDigitCounterAFar(s32);
s32 Party_AdjustSixDigitCounterBFar(s16);
void UiMessage_ShowAndRestoreState(s32 message);
void Audio_PlayCue(s32);
extern char MsgHereYouGo;

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_Get(item_id);
    added_slot = 0;
    replaced_slot = Inventory_FindEquippedFar(unit_id, item->type);
    Audio_PlayCue(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Inventory_AddItemFar(unit_id, item_id);
            Party_AdjustSixDigitCounterAFar(0 - item->price);
            remaining -= 1;
            Party_AdjustSixDigitCounterBFar(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    UiMessage_ShowAndRestoreState((s32)&MsgHereYouGo);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0) {
        Shop_SellOld(unit_id, replaced_slot);
    }
}
