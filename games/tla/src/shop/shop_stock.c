/// @file
/// The Game Ticket counter and every shop's stock.
// Part of shop.c.

#include "shop_internal.h"
#include "item_ids.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "shop/shop.h"
#include "gamepak/far_call/menu.h"

/// Gives a Game Ticket once the party's spending in shops reaches the next
/// ticket's cost, and lets the player choose who takes it.
void Shop_GiveGameTicket(struct Window *window, struct Window *itemWindow) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  u32 savedSelectedItem;
  s32 memberIndex;
  s32 unitId;
  s32 creditCost;
  s32 redraw;
  s32 slot;
  s32 messageBase;

  memberIndex = 0;
  savedSelectedItem = shop->selectedItem;
  unitId = 0;
  creditCost = gShop_GameTicketCreditCosts[gMainRoutineState.shopGifts];

  // A ticket is paid from the coins spent in shops, not from the party's
  // coins.
  if (creditCost > gMainRoutineState.shopCredit)
    return;
  shop->selectedItem = ITEM_GAME_TICKET;
  MessageArgument_PushFar(ITEM_GAME_TICKET, MESSAGE_ARGUMENT_ITEM);
  messageBase = (s32)MsgLetMeGiveYouThis;
  // "Let me give you this <item> as a token of my gratitude."
  Shop_ShowKeeperMessageHidingCursor(messageBase);
  MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
  // "So who gets this <item>?"
  Shop_ShowKeeperMessageHidingCursor(messageBase + 1);
  redraw = MENU_GRID_CHANGED_PAGE;
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(window,
                       (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
      shop->cursorFrames = 3;
      if (redraw == MENU_GRID_CHANGED_PAGE) {
        Menu_CreatePartyFiguresFar(memberIndex / SHOP_PARTY_PAGE_MEMBERS *
                                   SHOP_PARTY_PAGE_MEMBERS);
        wait(1);
      }
      Shop_DrawParty(window, memberIndex, shop->selectedItem);
      Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
      redraw = 0;
    }
    if (gInput.pressed & KEY_A) {
      // Try the ticket in the bag to see that it fits; it is given only after
      // "Then here you go!".
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          Shop_ShowKeeperMessage((s32)MsgPackFull);
        else
          Shop_ShowKeeperMessage((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      Sound_PlayFar(SE_SHOP_PURCHASE);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    } else if (gInput.pressed & KEY_B) {
      Shop_ShowKeeperMessageHidingCursor((s32)MsgDontWantIt);
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount,
                                   SHOP_PARTY_PAGE_MEMBERS);
      wait(1);
    }
  }
  shop->selectedItem = savedSelectedItem;
}

/// Returns how many stock rows there are; Shop_Run refuses a row past them.
s32 ShopStock_GetRowCount(void) { return SHOP_STOCK_ROWS; }

/// Adds a shop's artifacts to the shared stock on its first visit, so an item
/// two shops list can be bought twice.
void ShopStock_AddArtifacts(s32 stockRow) {
  s32 stockedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
  s32 artifactIndex;

  if (GameFlag_Get(stockedFlag) == 0) {
    GameFlag_Set(stockedFlag);
#if TLA_PATCH_1
    // From patch 1 an empty entry is skipped; the Japanese release stops at the
    // row's first one.
    for (artifactIndex = 0; artifactIndex < SHOP_STOCK_ARTIFACT_COUNT;
         artifactIndex++) {
      s16 itemId =
          gShop_StockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIndex];

      if (itemId != 0)
        Item_AdjustRareItemStockFar(itemId, 1);
    }
#else
    for (artifactIndex = 0; artifactIndex < SHOP_STOCK_ARTIFACT_COUNT;
         artifactIndex++) {
      s16 itemId =
          gShop_StockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIndex];

      if (itemId == 0)
        break;
      Item_AdjustRareItemStockFar(itemId, 1);
    }
#endif
  }
}

/// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0
/// after, and returns how many. Called by Shop_Run for Buy.
s32 ShopStock_CopyItems(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;
  u32 item;

  count = 0;
  if (gShop_StockTable[stockRow][SHOP_STOCK_ITEMS] != 0) {
    dst = output;
    src = gShop_StockTable[stockRow];
    do {
      item = (u16)*src;
      *dst = item;
      count++;
      src++;
      dst++;
      if (count > SHOP_STOCK_ITEM_COUNT - 1)
        break;
    } while (*src != 0);
  }
  output[count] = 0;
  return count;
}

/// Returns the shop's type from the last column of its stock row (enum
/// ShopType).
s32 ShopStock_GetType(s32 stockRow) {
  return gShop_StockTable[stockRow][SHOP_STOCK_TYPE];
}
