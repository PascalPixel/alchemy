/// @file
/// Repairing items: the price and the repair.
// Part of shop.c.

#include "../shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"

/// Returns what a repair costs: a quarter of the price for a broken item,
/// nothing for one that is not broken.
s32 Shop_RepairPrice(s32 item) {
  s32 price = (u16)Item_GetDirectFar(item)->price >> 2;

  if ((item & INVENTORY_BROKEN) == 0)
    price = 0;
  return price;
}

/// Runs the repair counter's party member choice. Called by Shop_Run for
/// Repair.
s32 Shop_RunRepairMemberChoice(void) { return Shop_ChooseMember(); }

/// In The Lost Age the sell screen serves repairs too, by shop->action.
s32 Shop_SelectRepair(s32 unitId) { return Shop_SelectSell(unitId); }

void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 price;
  s32 message;
  u8 kind;
  u32 saved;

  price = Shop_RepairPrice(unit->inventory[slot]);
  kind = item->useType;
  if (kind != 2) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // MsgRepairNotHandled: MsgCannotRepairKind
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotHandled);
    return;
  }
  if (!(unit->inventory[slot] & INVENTORY_BROKEN)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // MsgRepairNotDamaged: MsgNotDamaged
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotDamaged);
    return;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (kind & item->flags)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // MsgRepairStuckOn: MsgCursedWontComeOff
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairStuckOn);
    return;
  }
  if (price > gMainRoutineWork.coins) {
    // MsgRepairNotEnoughMoney: MsgNotEnoughMoney
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotEnoughMoney);
    return;
  }
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  MessageArgument_PushFar(price, MESSAGE_ARGUMENT_NUMBER);
  // MsgRepairOffer: MsgShopRepairConfirm, then its three replies
  message = (s32)MsgRepairOffer;
  Shop_ShowKeeperMessageHidingCursor(message);
  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessageHidingCursor(message + 1);
    return;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawMemberItems(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  Shop_ShowKeeperMessageHidingCursor(message + 2);
  Message_CloseFinishedWindowsFar();
  wait(10);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_MENU_CONFIRM);
  wait(20);
  unit->inventory[slot] = saved;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-price);
  Shop_DrawMoney();
  Shop_DrawMemberItems(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  Shop_ShowKeeperMessageHidingCursor(message + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
