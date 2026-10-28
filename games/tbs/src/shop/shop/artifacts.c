/// @file
/// The Game Ticket counter, the gift items and every shop's stock.
// Part of shop.c.

#include "../shop_internal.h"

/// The shop credit each Game Ticket costs, by how many have been given: five
/// at each price: 300, then 500 doubling to 8000. The gift count stops at 28
/// (Party_AdjustShopGifts), so every ticket from the 29th costs entry 28's
/// 8000 and entry 29 is never read.
// clang-format off
static const s16 sGameTicketCreditCosts[30] = {
    300, 300, 300, 300, 300,
    500, 500, 500, 500, 500,
    1000, 1000, 1000, 1000, 1000,
    2000, 2000, 2000, 2000, 2000,
    4000, 4000, 4000, 4000, 4000,
    8000, 8000, 8000, 8000, 8000,
};

/// Gift items, terminated by 0; no function reads this table. They are the
/// Long Sword to the Muramasa, the Short Sword, Mint, the Demon Axe, Asura's
/// Armor, the Cotton Shirt, the Travel Vest, Herb, Nut and an unused row.
const s16 gShop_GiftItemIds[21] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    16, 195, 38, 84, 89, 90, 180, 181, 190, 0,
};
// clang-format on

/// Gives a Game Ticket once the party's shop credit (what it has spent in
/// shops) reaches the next ticket's cost, and lets the player choose who
/// takes it. Spends the credit and counts the gift, not coins. Called by
/// Shop_SelectBuy after each purchase.
void Shop_BuySpecialItem(struct Window *window, struct Window *itemWindow) {
  struct ShopWork *shop = gMenuWork;
  s32 creditCost;
  u32 savedSelectedItem;
  s32 redraw;
  s32 unitId;
  s32 memberIndex;
  s32 messageBase;
  s32 slot;

  redraw = 1;
  unitId = 0;
  savedSelectedItem = shop->selectedItem;
  creditCost = sGameTicketCreditCosts[gAdventure.shopGifts];
  memberIndex = 0;

  // Gifts spend accumulated shop credit; they do not debit the party's coins.
  if (creditCost > gAdventure.shopCredit)
    return;
  shop->selectedItem = ITEM_GAME_TICKET;
  MessageArgument_PushFar(ITEM_GAME_TICKET, MESSAGE_ARGUMENT_ITEM);
  messageBase = MsgTokenGift;
  Shop_ShowKeeperMessageHidingCursor(messageBase);
  MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
  Shop_ShowKeeperMessageHidingCursor(messageBase + 1);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(window, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(window, memberIndex, shop->selectedItem);
      Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      // Probe capacity before the keeper's confirmation; the gift is committed
      // only after that line finishes.
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          Shop_ShowKeeperMessage(MsgPackFull);
        else
          Shop_ShowKeeperMessage(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      Sound_PlayFar(SE_SHOP_PURCHASE);
      Shop_ShowKeeperMessageHidingCursor(MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Shop_ShowKeeperMessageHidingCursor(MsgDontWantIt);
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }
  shop->selectedItem = savedSelectedItem;
}

/// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts
/// (each list ending with 0 when shorter), then the shop's type.
enum {
  SHOP_STOCK_ITEMS = 0,          ///< the first ordinary item's column
  SHOP_STOCK_ITEM_COUNT = 24,    ///< ordinary items a row holds
  SHOP_STOCK_ARTIFACTS = 24,     ///< the first artifact's column
  SHOP_STOCK_ARTIFACT_COUNT = 8, ///< artifacts a row holds
  /// The shop's type (enum ShopType).
  SHOP_STOCK_TYPE = 32,
  SHOP_STOCK_COLUMNS = 33,
  /// Rows, by the stock row an event passes to Shop_Run; SHOP_STOCK_ROW_EMPTY
  /// stands in for a row out of range.
  SHOP_STOCK_ROWS = 35
};

// clang-format off
static const s16 sStockTable[SHOP_STOCK_ROWS][SHOP_STOCK_COLUMNS] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 16, 43, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        89, 90, 103, 118, 127, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 16, 31, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        56, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        90, 118, 127, 136, 156, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        57, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 90, 104, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        58, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 92, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 17, 32, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 92, 119, 128, 137, 146, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        92, 105, 120, 137, 146, 157, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        106, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        3, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        59, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        77, 92, 105, 120, 129, 137, 147, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        3, 18, 33, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        60, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        77, 107, 120, 129, 138, 147, 158, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        61, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        78, 94, 107, 121, 129, 138, 148, 158, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 189, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 20, 34, 62, 0, 0, 0, 0,
        1,
    },
    {
        79, 139, 149, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        109, 131, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 183, 189, 186, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 21, 47, 63, 0, 0, 0, 0,
        1,
    },
    {
        80, 150, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        110, 122, 140, 159, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 183, 189, 186, 0, 0, 0, 0,
        0,
    },
};
// clang-format on

/// Returns how many stock rows there are; Shop_Run refuses a row past them.
s32 ShopStock_GetRowCount(void) { return SHOP_STOCK_ROWS; }

/// Adds a shop's artifacts to the shared stock counters once, on its first
/// visit, and sets the shop's flag (FLAG_SHOP_ARTIFACTS_STOCKED + row) to
/// remember it. Called by Shop_Run as a shop opens. Each artifact adds one,
/// so an item listed by two shops can be bought twice once both are
/// visited.
void ShopStock_AddArtifacts(s32 stockRow) {
  const s16 *stockEntries;
  const s16 *artifactCursor;
  s32 itemId;
  s32 artifactIndex;
  s32 initializedFlag;

  initializedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
  if (GameFlag_GetFar(initializedFlag) == 0) {
    GameFlag_SetFar(initializedFlag);
    artifactIndex = 0;
    itemId = sStockTable[stockRow][SHOP_STOCK_ARTIFACTS];
    if (itemId != 0) {
      stockEntries = sStockTable[stockRow];
      artifactCursor = stockEntries + SHOP_STOCK_ARTIFACTS;
      do {
        Item_AdjustCounterFar(itemId, 1);
        artifactIndex++;
        if (artifactIndex > SHOP_STOCK_ARTIFACT_COUNT - 1)
          break;
        artifactCursor++;
        itemId = *artifactCursor;
      } while (itemId != 0);
    }
  }
}

/// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0
/// after, and returns how many. Called by Shop_Run for Buy.
s32 ShopStock_CopyItems(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (sStockTable[stockRow][SHOP_STOCK_ITEMS] != 0) {
    dst = output;
    src = sStockTable[stockRow];
    do {
      /* FAKEMATCH(volatile): only the copy read is volatile, so it stays apart
       * from the signed sentinel read; as *dst = *src, 19 of 30 instructions
       * differ in tbs-en. */
      *dst = *(const volatile s16 *)src;
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
/// ShopType). Called by Shop_Run.
s32 ShopStock_GetType(s32 stockRow) {
  return sStockTable[stockRow][SHOP_STOCK_TYPE];
}
