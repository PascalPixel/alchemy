#include "party_state.h"
#include "battle_system.h"
extern struct ShopRuntime *gMenuWork;
extern u8 gData_03001f2c[];

extern u8 MsgShopRepairConfirm[];
extern u8 MsgCannotRepairKind[];
extern u8 MsgNotDamaged[];
extern u8 MsgCursedWontComeOff[];
extern u8 MsgNotEnoughMoney[];

struct ItemDefinition *Item_Get(s32 item);
s32 Inventory_FindEquippedFar(s32 unitId, u8 kind);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Func_08077068(s32 unitId, s32 slot);
void Party_AdjustCoinsFar(s32 amount);

void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & 0x1ff;
  struct ItemDefinition *item = Item_Get(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 price = Shop_RepairPrice(unit->inventory[slot]);
  s32 message;
  u8 kind;
  u32 saved;

  kind = item->useType;
  if (kind != 2) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState((s32)MsgCannotRepairKind);
    return;
  }
  if (!(unit->inventory[slot] & 0x400)) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState((s32)MsgNotDamaged);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState((s32)MsgCursedWontComeOff);
    return;
  }
  if (price > gGameState.money) {
    UiMessage_ShowAndRestoreState((s32)MsgNotEnoughMoney);
    return;
  }
  UiWork_PushValueSlotFar(itemId, 2);
  UiWork_PushValueSlotFar(price, 5);
  message = (s32)MsgShopRepairConfirm;
  UiMessage_ShowAndRestoreState(message);
  if (UiMessage_ShowChoice(0) != 0) {
    UiMessage_ShowAndRestoreState(message + 1);
    return;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, 2);
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
  Func_08077068(unitId, slot);
  Party_AdjustCoinsFar(-price);
  Shop_DrawMoney();
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, 2);
  UiMessage_ShowAndRestoreState(message + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
