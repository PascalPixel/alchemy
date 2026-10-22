/// @file
/// Selling: choosing a party member, the item grid and the sale.
// Part of shop.c.

#include "../shop_internal.h"

/// Lets the player choose whose items to sell or repair, with the coins
/// window and the member's item grid, and runs Shop_SelectSell or
/// Shop_SelectRepair for each member chosen until B. Called by Shop_Run for
/// Sell and Repair.
s32 Shop_PickUnit(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *listWindow;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;

  shop->moneyWindow =
      UiWindow_CreateFar(0, 9, SHOP_MONEY_WINDOW_WIDTH, 4, UI_WINDOW_INSTANT);
  Shop_DrawMoney();
  shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, UI_WINDOW_INSTANT);
  listWindow = UiWindow_CreateFar(0, 14, 13, 3, UI_WINDOW_INSTANT);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(listWindow, memberIndex, 0);
      Shop_DrawUnitGrid(shop->itemWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      if (Inventory_CountFar(unitId) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
      } else {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        if (shop->partyAction == SHOP_ACTION_SELL)
          Shop_SelectSell(unitId);
        else
          Shop_SelectRepair(unitId);
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        redraw = 1;
      }
      continue;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      Menu_ReleaseEntryObjectsFar();
      UiWork_FinalizeFar(listWindow, 2);
      UiWork_FinalizeFar(shop->itemWindow, 2);
      UiWork_FinalizeFar(shop->moneyWindow, 2);
      WaitFrames(1);
      return 0;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }
}

/// Lets the player choose items to sell from the member's grid, one at a
/// time, with each one's price and description, until B or the inventory is
/// empty. Returns -1 when cancelled, 0 when the inventory ran out. Called by
/// Shop_PickUnit.
s32 Shop_SelectSell(s32 unitId) {
  // Shared previous/next-row inlines added four bytes to each sell, repair and
  // use picker in the English native build; the direct row arithmetic remains.
  struct UiWindow *descriptionWindow;
  struct ShopRuntime *shop;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 inventoryCount;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 quantity;
  struct UiWindow *window;
  s32 x;

  shop = gMenuWork;
  unit = Unit_GetFar(unitId);
  inventoryCount = 1;
  priceWindow =
      UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, UI_WINDOW_INSTANT);
  slot = 0;

  for (;;) {
    descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, UI_WINDOW_INSTANT);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
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
        Shop_PlaceCursor(window, x, slot / SHOP_INVENTORY_COLUMNS * 16 + 8);
        shop->mode = 3;
        Shop_DrawItemPrice(priceWindow, itemId,
                           Shop_SalePrice(unit->inventory[slot]),
                           SHOP_PRICE_KIND_SELL);
        Shop_DrawMessage(descriptionWindow, itemId + MsgItemPlainName);
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
      // Up from the top row wraps to the bottom row of the 15-slot grid, then
      // steps up a row at a time onto a filled slot; down onto an empty slot
      // wraps to the same column's top row, which may be where it started
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

    quantity = Shop_SelectSellCount(unitId, slot);
    if (quantity != -1)
      Shop_SellItem(unitId, slot, quantity);
    UiMessage_ShowAndWait(MsgSellAnythingElse);
    if (Inventory_CountFar(unitId) == 0)
      break;
  }

  UiWork_FinalizeFar(priceWindow, 2);
  return result;
}

/// Draws the member's items as icons in rows of five, or a line saying it has
/// none. Called by the sell, repair and drop screens each time the
/// member changes, and after a sale.
void Shop_DrawUnitGrid(struct UiWindow *window, s32 unitId) {
  struct BattleUnit *unit;
  s32 x;
  s32 y;
  s32 slot;
  struct RenderOutput *icon;

  unit = Unit_GetFar(unitId);
  x = 8;
  y = 0;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (Inventory_CountFar(unitId) == 0) {
      UiText_DrawResourceFar(MsgNoItems, window, SHOP_EMPTY_GRID_X,
                             SHOP_EMPTY_GRID_Y);
    } else {
      slot = 0;
      do {
        if (unit->inventory[slot] != 0) {
          icon = RenderOutput_CreateLoadedFar(unit->inventory[slot], 27, window,
                                              x, y);
          icon->sentinel = 252;
        }
        x += 16;
        if (slot == 4) {
          x = 8;
          y += 16;
        }
        if (slot == 9) {
          x = 8;
          y += 16;
        }
        slot++;
      } while (slot < INVENTORY_SLOTS);
    }
  }
}

/// Asks how many to sell when the item is a stack of more than one, and
/// returns the count (1 otherwise, -1 when cancelled), putting the cursor
/// back after. Called by Shop_SelectSell.
s32 Shop_SelectSellCount(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 unitSalePrice;
  s32 stackQuantity;
  s32 quantityLimit;
  struct ItemDefinition *item;
  struct ShopRuntime *shop;
  struct BattleUnit *unit;

  shop = gMenuWork;
  unit = Unit_GetFar(unitId);
  item = Item_GetDirectFar(unit->inventory[slot]);
  result = 1;
  unitSalePrice = Shop_SalePrice(unit->inventory[slot]);
  // Shop_GetSelectionState is GAME.S's name for Inventory_GetQuantity
  stackQuantity = Shop_GetSelectionState(unitId, slot);
  quantityLimit = stackQuantity;
  if ((item->flags & ITEM_STACKABLE) && stackQuantity > 1) {
    UiMessage_ShowAndWait(MsgHowManyToSell);
    savedX = shop->cursor.targetX;
    savedY = shop->cursor.targetY;
    shop->cursor.anchor->active = 4;
    shop->mode = 0xc;
    Shop_PlaceCursor(NULL, SHOP_QUANTITY_CURSOR_X, 0x30);
    result = Shop_SelectQuantity(0, quantityLimit, unitSalePrice);
    WaitFrames(1);
    UiIcon_PrepareObjectFar(shop->cursor.anchor);
    Shop_PlaceCursor(NULL, savedX, savedY);
  }
  return result;
}

/// Sells `count` of the item in `slot`: refuses an item the shop does not buy
/// or one equipped and cursed on, asks with the offer that fits (an old item
/// the player just replaced, a broken one, a stack, a rare one), then
/// discards them, pays the coins and redraws. `count` -1 is the old
/// equipment the equip prompt offers to sell (Shop_SellOld). Called by
/// Shop_SelectSell and Shop_SellOld. Each item goes through
/// Inventory_Discard, so one with a stock counter (an artifact) goes back on
/// it, and the shops of its type may sell it again.
void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = Unit_GetFar(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  u8 rare = item->flags & ITEM_RARE;
  s32 sellingReplacedItem = 0;
  s32 total;
  s32 message;
  s32 soldCount;

  if (count == -1) {
    sellingReplacedItem = 1;
    count = 1;
  }
  total = Shop_SalePrice(unit->inventory[slot]) * count;
  if (total == 0) {
    UiWork_PushValueSlotFar(itemId, MESSAGE_ARGUMENT_ITEM);
    UiMessage_ShowAndRestoreState(MsgWeDoNotBuy);
    return;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) &&
      (item->flags & ITEM_CANNOT_UNEQUIP)) {
    UiWork_PushValueSlotFar(itemId, MESSAGE_ARGUMENT_ITEM);
    UiMessage_ShowAndRestoreState(MsgCannotSellEquipped);
    return;
  }
  if (sellingReplacedItem)
    message = MsgOldItemSellOffer;
  else if (unit->inventory[slot] & INVENTORY_BROKEN)
    message = MsgDamagedSellOffer;
  else if (count > 1)
    message = MsgSellTotal;
  else if (rare)
    message = MsgRareSellOffer;
  else
    message = MsgSellOffer;
  UiWork_PushValueSlotFar(itemId, MESSAGE_ARGUMENT_ITEM);
  UiWork_PushValueSlotFar(total, MESSAGE_ARGUMENT_NUMBER);
  UiMessage_ShowAndRestoreState(message);
  if (UiMessage_ShowChoice(0) != 0) {
    if (rare || sellingReplacedItem)
      message = MsgKeepIt;
    else
      message = MsgChangedMind;
    UiMessage_ShowAndRestoreState(message);
    return;
  }
  // Discard each item before crediting coins and rebuilding the inventory grid.
  AudioCommand_PlayFar(SE_SHOP_SELL);
  for (soldCount = 0; soldCount < count; soldCount++)
    Inventory_DiscardFar(unitId, slot);
  Party_AdjustCoinsFar(total);
  Shop_DrawMoney();
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  if (rare || sellingReplacedItem)
    message = MsgSold;
  else
    message = MsgDeal;
  UiMessage_ShowAndRestoreState(message);
}
