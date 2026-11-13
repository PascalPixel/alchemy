/// @file
/// Repairing items: the price and the repair.
// Part of shop.c.

#include "../shop_internal.h"

s32 Shop_GetItemPrice(s32 item) {
  s32 price = (u16)Item_Get(item)->price >> 2;

  if ((item & 0x400) == 0)
    price = 0;
  return price;
}

s32 Func_0810a2c8(void) { return Func_08109ad8(); }

s32 Func_0810a2d0(s32 unitId) { return Func_08109cac(unitId); }

extern u8 MsgRepairNotHandled[];     // MsgCannotRepairKind
extern u8 MsgRepairNotDamaged[];     // MsgNotDamaged
extern u8 MsgRepairStuckOn[];        // MsgCursedWontComeOff
extern u8 MsgRepairNotEnoughMoney[]; // MsgNotEnoughMoney
extern u8 MsgRepairOffer[]; // MsgShopRepairConfirm, then its three replies

void Inventory_RepairFar(s32 unitId, s32 slot);

void Func_0810a2d8(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & 0x1ff;
  struct ItemDefinition *item = Item_Get(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 price;
  s32 message;
  u8 kind;
  u32 saved;

  price = Shop_GetItemPrice(unit->inventory[slot]);
  kind = item->useType;
  if (kind != 2) {
    MessageArgument_PushFar(itemId, 2);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotHandled);
    return;
  }
  if (!(unit->inventory[slot] & 0x400)) {
    MessageArgument_PushFar(itemId, 2);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotDamaged);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
    MessageArgument_PushFar(itemId, 2);
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairStuckOn);
    return;
  }
  if (price > gMainRoutineWork.coins) {
    Shop_ShowKeeperMessageHidingCursor((s32)MsgRepairNotEnoughMoney);
    return;
  }
  MessageArgument_PushFar(itemId, 2);
  MessageArgument_PushFar(price, 5);
  message = (s32)MsgRepairOffer;
  Shop_ShowKeeperMessageHidingCursor(message);
  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessageHidingCursor(message + 1);
    return;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawItemCount(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, 2);
  Shop_ShowKeeperMessageHidingCursor(message + 2);
  Message_CloseFinishedWindowsFar();
  wait(10);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(100);
  wait(110);
  Sound_PlayFar(112);
  wait(20);
  unit->inventory[slot] = saved;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-price);
  Func_08109188();
  Shop_DrawItemCount(shop->itemWindow, unitId);
  MessageArgument_PushFar(itemId, 2);
  Shop_ShowKeeperMessageHidingCursor(message + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
