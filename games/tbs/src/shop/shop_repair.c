/// @file
/// Repairing broken items.
// Part of shop.c.

#include "shop_internal.h"

/// Returns what a repair costs: a quarter of the price for a broken item,
/// nothing for one that is not broken.
s32 Shop_RepairPrice(s32 inventoryEntry) {
  s32 repairCost = Item_GetDirectFar(inventoryEntry)->price / 4;

  if ((inventoryEntry & INVENTORY_BROKEN) == 0) {
    repairCost = 0;
  }
  return repairCost;
}

/// Runs the repair counter's party member choice; always returns 0.
s32 Shop_RunRepairMemberChoice(void) {
  Shop_ChooseMember();
  return 0;
}

/// Lets the player choose items to repair from the party member's grid, one at
/// a time, with each one's repair price, until B or the inventory is empty.
/// Returns -1 when cancelled, 0 when the inventory ran out.
s32 Shop_SelectRepair(s32 unitId) {
  struct ShopState *shop;
  s32 inventoryCount;
  struct Window *descriptionWindow;
  struct Window *priceWindow;
  struct Unit *unit;
  s32 slot;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 repairCost;
  struct Window *window;
  s32 x;
  s32 y;

  shop = gMenuState;
  unit = Unit_GetFar(unitId);
  inventoryCount = 1;
  priceWindow = Window_CreateFar(SHOP_SELL_PRICE_X, 8, SHOP_SELL_PRICE_WIDTH, 4,
                                 WINDOW_FLAG_INSTANT);
  slot = 0;

  for (;;) {
    descriptionWindow = Window_CreateFar(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
    shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
    redraw = 1;

    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
        window = shop->itemWindow;
        x = slot % SHOP_INVENTORY_COLUMNS * 16;
        y = slot / SHOP_INVENTORY_COLUMNS * 16 + 8;
        MenuCursor_Place(window, x, y);
        shop->cursorFrames = 3;
        repairCost = Shop_RepairPrice(unit->inventory[slot]);
        Shop_DrawItemPrice(priceWindow, itemId, repairCost,
                           SHOP_PRICE_KIND_REPAIR);
        Shop_DrawMessage(descriptionWindow, itemId + MsgItemPlainName);
      }
      if ((gKeysPressed & KEY_A) != 0) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        slot--;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        slot++;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_UP) != 0) {
        slot -= SHOP_INVENTORY_COLUMNS;
        if (slot < 0)
          slot += INVENTORY_SLOTS;
        while (slot >= inventoryCount)
          slot -= SHOP_INVENTORY_COLUMNS;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        slot += SHOP_INVENTORY_COLUMNS;
        if (slot >= inventoryCount)
          slot -= INVENTORY_SLOTS;
        while (slot < 0)
          slot += SHOP_INVENTORY_COLUMNS;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      wait(1);
    }

    Window_CloseFar(descriptionWindow, 2);
    wait(1);
    if (result != 0)
      break;

    Shop_RepairItem(unitId, slot);
    // "Anything else need repairing?"
    Shop_ShowKeeperMessage(MsgRepairAnythingElse);
    if (Inventory_CountFar(unitId) == 0)
      break;
  }

  Window_CloseFar(priceWindow, 2);
  return result;
}

/// Repairs the item in `slot`: refuses one that cannot break, is not broken or
/// is equipped and cursed on, or that the coins do not cover; asks, then takes
/// the item away while three clangs sound, gives it back mended, charges the
/// price and offers to equip it.
void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopState *shop = gMenuState;
  struct Unit *unit = Unit_GetFar(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 repairCost = Shop_RepairPrice(unit->inventory[slot]);
  s32 messageBase;
  u8 useType;
  u32 savedEntry;

  useType = item->useType;
  if (useType != ITEM_USE_MAY_BREAK) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // "Sorry, but we don't handle (item)s."
    Shop_ShowKeeperMessageHidingCursor(MsgCannotRepairKind);
    return;
  }
  if (!(unit->inventory[slot] & INVENTORY_BROKEN)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // "Huh? That (item) isn't damaged at all."
    Shop_ShowKeeperMessageHidingCursor(MsgNotDamaged);
    return;
  }
  // useType is ITEM_USE_MAY_BREAK here, so this tests ITEM_CANNOT_UNEQUIP.
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flags)) {
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    // "This (item) won't come off. Bring it back after you get it off."
    Shop_ShowKeeperMessageHidingCursor(MsgCursedWontComeOff);
    return;
  }
  if (repairCost > gMainRoutineState.holdings.coins) {
    // "Uh-oh, you don't seem to have enough money."
    Shop_ShowKeeperMessageHidingCursor(MsgNotEnoughMoney);
    return;
  }
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  MessageArgument_PushFar(repairCost, MESSAGE_ARGUMENT_NUMBER);
  messageBase = MsgShopRepairConfirm;
  // "I'll fix your (item) for (price) coins, OK?"
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
  // "Yes, sir! I'll take your (item), then."
  Shop_ShowKeeperMessageHidingCursor(messageBase + 2);
  Message_CloseFinishedWindowsFar();
  // Six seconds of work: three clangs 110 frames apart, then the menu's
  // confirm sound.
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
  // "I've repaired your (item). Here it is."
  Shop_ShowKeeperMessageHidingCursor(messageBase + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
