#include "btlsys.h"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];

extern u8 MsgCannotSellEquipped[], MsgWeDoNotBuy[], MsgSellOffer[], MsgRareSellOffer[];
extern u8 MsgSellTotal[], MsgDamagedSellOffer[], MsgOldItemSellOffer[], MsgDeal[];
extern u8 MsgChangedMind[], MsgSold[], MsgKeepIt[];

struct ItemDefinition *Item_Get(s32 item);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void Audio_PlayCue(s32 cue);
void Func_080772b0(s32 unit_id, s32 slot);
void Party_AdjustCoinsFar(s32 amount);

void Shop_SellItem(s32 unit_id, s32 slot, s32 count)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BtlUnit *unit = UnitGetFar(unit_id);
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
    total = Shop_SalePrice(unit->inventory[slot]) * count;
    if (total == 0) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgWeDoNotBuy);
        return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgCannotSellEquipped);
        return;
    }
    if (all)
        message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & 0x400)
        message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
        message = (s32)MsgSellTotal;
    else if (rare)
        message = (s32)MsgRareSellOffer;
    else
        message = (s32)MsgSellOffer;
    UiWork_PushValueSlotFar(item_id, 2);
    UiWork_PushValueSlotFar(total, 5);
    UiMessage_ShowAndRestoreState(message);
    if (UiMessage_ShowChoice(0) != 0) {
        if (rare || all)
            message = (s32)MsgKeepIt;
        else
            message = (s32)MsgChangedMind;
        UiMessage_ShowAndRestoreState(message);
        return;
    }
    Audio_PlayCue(102);
    for (i = 0; i < count; i++)
        Func_080772b0(unit_id, slot);
    Party_AdjustCoinsFar(total);
    Shop_DrawMoney();
    Shop_DrawUnitGrid(shop->item_window, unit_id);
    if (rare || all)
        message = (s32)MsgSold;
    else
        message = (s32)MsgDeal;
    UiMessage_ShowAndRestoreState(message);
}
