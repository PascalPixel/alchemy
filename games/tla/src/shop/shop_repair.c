/// @file
/// Repairing broken items.
// Part of shop.c.

#include "shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"

/// Returns what a repair costs: a quarter of the price for a broken item,
/// nothing for one that is not broken.
s32 Shop_RepairPrice(s32 inventoryEntry) {
  s32 price = (u16)Item_GetDirectFar(inventoryEntry)->price >> 2;

  if ((inventoryEntry & INVENTORY_BROKEN) == 0)
    price = 0;
  return price;
}

/// Runs the repair counter's party member choice. Called by Shop_Run for
/// Repair.
s32 Shop_RunRepairMemberChoice(void) { return Shop_ChooseMember(); }

/// Lets the player choose items to repair on the sell counter's grid, which
/// serves repairs too by shop->action.
s32 Shop_SelectRepair(s32 unitId) { return Shop_SelectSell(unitId); }

/// Repairs the item in `slot`: refuses one that cannot break, is not broken or
/// is equipped and cursed on, or that the coins do not cover; asks, then takes
/// the item away for the smith's three hammer blows, gives it back mended,
/// charges the price and offers to equip it.
void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;
  struct Unit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 repairCost;
  s32 messageBase;
  u8 useType;
  u32 savedEntry;

  repairCost = Shop_RepairPrice(unit->inventory[slot]);
  useType = item->useType;
  if (useType != ITEM_USE_MAY_BREAK) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotHandled);
    return;
  }
  if (!(unit->inventory[slot] & INVENTORY_BROKEN)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotDamaged);
    return;
  }
  // useType is ITEM_USE_MAY_BREAK here, so this tests ITEM_CANNOT_UNEQUIP.
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flags)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairStuckOn);
    return;
  }
  if (repairCost > gMainRoutineWork.coins) {
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotEnoughMoney);
    return;
  }
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  MessageArgument_PushFar(repairCost, MESSAGE_ARGUMENT_NUMBER);
  messageBase = (s32)MsgRepairOffer;
  // "I'll fix your <item> for <n> coins, OK?"
  Shop_ShowKeeperMessageHidingCursor(messageBase);
  if (Shop_AskYesNo(0) != 0) {
    // "Changed your mind? Whatever you say!"
    Shop_ShowKeeperMessageHidingCursor(messageBase + 1);
    return;
  }
  // Hide the item while it is repaired, without compacting its slot.
  savedEntry = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawMemberItems(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  // "Yes, sir! I'll take your <item>, then."
  Shop_ShowKeeperMessageHidingCursor(messageBase + 2);
  Message_CloseFinishedWindowsFar();
  // The hammer blows are 110 frames apart; the item stays hidden throughout.
  wait(10);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_BELL);
  wait(110);
  Sound_PlayFar(SE_MENU_CONFIRM);
  wait(20);
  unit->inventory[slot] = savedEntry;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-repairCost);
  Shop_DrawMoney();
  Shop_DrawMemberItems(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  // "I've repaired your <item>. Here it is."
  Shop_ShowKeeperMessageHidingCursor(messageBase + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
