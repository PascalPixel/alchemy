/// @file
/// Repairing broken items.
// Part of shop.c.

#include "../shop_internal.h"

/// Returns what a repair costs: a quarter of the price for a broken item,
/// nothing for one that is not broken. Called by the repair list and
/// Shop_RepairItem.
s32 Shop_RepairPrice(s32 inventoryEntry) {
  s32 repairCost = Item_GetDirectFar(inventoryEntry)->price / 4;

  if ((inventoryEntry & INVENTORY_BROKEN) == 0) {
    repairCost = 0;
  }
  return repairCost;
}

/// Runs the repair counter's member choice; always returns 0. Called by
/// Shop_Run for Repair.
s32 Shop_SelectUnit(void) {
  Shop_PickUnit();
  return 0;
}

/// Lets the player choose items to repair from the member's grid, one at a
/// time, with each one's repair price, until B or the inventory is empty.
/// Returns -1 when cancelled, 0 when the inventory ran out. Called by
/// Shop_PickUnit.
s32 Shop_SelectRepair(s32 unitId) {
  struct ShopRuntime *shop;
  s32 inventoryCount;
  struct UiWindow *descriptionWindow;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 repairCost;
  struct UiWindow *window;
  s32 x;
  s32 y;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  inventoryCount = 1;
  priceWindow =
      UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, UI_WINDOW_INSTANT);
  slot = 0;

  for (;;) {
    descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, UI_WINDOW_INSTANT);
    shop->cursor.anchor->active = UI_SPRITE_HOP_DOWN_LEFT;
    shop->mode = 12;
    redraw = 1;

    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = ITEM_ID_MASK & unit->inventory[slot];
        window = shop->itemWindow;
        x = slot % SHOP_INVENTORY_COLUMNS * 16;
        y = slot / SHOP_INVENTORY_COLUMNS * 16 + 8;
        Shop_PlaceCursor(window, x, y);
        shop->mode = 3;
        repairCost = Shop_RepairPrice(unit->inventory[slot]);
        Shop_DrawItemPrice(priceWindow, itemId, repairCost,
                           SHOP_PRICE_KIND_REPAIR);
        Shop_DrawMsg(descriptionWindow, itemId + MsgItemPlainName);
      }
      if ((gKeysPressed & KEY_A) != 0) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot -= 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot += 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_UP) != 0) {
        slot -= SHOP_INVENTORY_COLUMNS;
        if (slot < 0)
          slot += INVENTORY_SLOTS;
        while (slot >= inventoryCount)
          slot -= SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        slot += SHOP_INVENTORY_COLUMNS;
        if (slot >= inventoryCount)
          slot -= INVENTORY_SLOTS;
        while (slot < 0)
          slot += SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      WaitFrames(1);
    }

    UiWork_FinalizeFar(descriptionWindow, 2);
    WaitFrames(1);
    if (result != 0)
      break;

    Shop_RepairItem(unitId, slot);
    UiMessage_ShowAndWait(MsgRepairAnythingElse);
    if (Inventory_CountFar(unitId) == 0)
      break;
  }

  UiWork_FinalizeFar(priceWindow, 2);
  return result;
}

/// Repairs the item in `slot`: refuses one that cannot break, is not broken
/// or is equipped and cursed on, or that the coins do not cover; asks, then
/// takes the item away for the smith's three hammer blows, gives it back
/// mended, charges the price and offers to equip it. Called by
/// Shop_SelectRepair.
void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 repairCost = Shop_RepairPrice(unit->inventory[slot]);
  s32 messageBase;
  u8 useType;
  u32 savedEntry;

  useType = item->useType;
  if (useType != ITEM_USE_MAY_BREAK) {
    UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
    UiMessage_ShowAndRestoreState(MsgCannotRepairKind);
    return;
  }
  if (!(unit->inventory[slot] & INVENTORY_BROKEN)) {
    UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
    UiMessage_ShowAndRestoreState(MsgNotDamaged);
    return;
  }
  // useType is ITEM_USE_MAY_BREAK here, so this tests ITEM_CANNOT_UNEQUIP.
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flags)) {
    UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
    UiMessage_ShowAndRestoreState(MsgCursedWontComeOff);
    return;
  }
  if (repairCost > gGameState.coins) {
    UiMessage_ShowAndRestoreState(MsgNotEnoughMoney);
    return;
  }
  UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
  UiWork_PushValueSlotFar(repairCost, MSG_ARG_NUMBER);
  messageBase = MsgShopRepairConfirm;
  UiMessage_ShowAndRestoreState(messageBase);
  if (UiMessage_ShowChoice(0) != 0) {
    UiMessage_ShowAndRestoreState(messageBase + 1);
    return;
  }
  // Hide the item during the work sequence without compacting its slot.
  savedEntry = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
  UiMessage_ShowAndRestoreState(messageBase + 2);
  UiWork_FinalizePendingCoreFar();
  // The hammer blows are 110 frames apart; the item stays hidden throughout,
  // 360 frames (six seconds) from the first wait to the last.
  WaitFrames(10);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_MENU_CONFIRM);
  WaitFrames(20);
  // Restore the entry, repair it, charge the fee and offer equipment changes.
  unit->inventory[slot] = savedEntry;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-repairCost);
  Shop_DrawMoney();
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, MSG_ARG_ITEM);
  UiMessage_ShowAndRestoreState(messageBase + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
