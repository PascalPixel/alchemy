/// @file
/// The Game Ticket counter, the gift items and every shop's stock.
/// Part of shop.c, the shop object, compiled only through it.

#include "../shop_internal.h"

/// Gift prices
// clang-format off
static const s16 sGameTicketCreditCosts[30] = {
    300, 300, 300, 300, 300,
    500, 500, 500, 500, 500,
    1000, 1000, 1000, 1000, 1000,
    2000, 2000, 2000, 2000, 2000,
    4000, 4000, 4000, 4000, 4000,
    8000, 8000, 8000, 8000, 8000,
};

/// Gift items, terminated by 0; no function reads this table.
const s16 gShop_GiftItemIds[21] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    16, 195, 38, 84, 89, 90, 180, 181, 190, 0,
};
// clang-format on

/// Offers a Game Ticket for accumulated shop credit and lets the player choose
/// its recipient.
void Shop_BuySpecialItem(struct UiWindow *window, struct UiWindow *itemWindow) {
  struct ShopRuntime *shop = gMenuWork;
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
  creditCost = sGameTicketCreditCosts[gGameState.shopGifts];
  memberIndex = 0;

  // Gifts spend accumulated shop credit; they do not debit the party's coins.
  if (creditCost > gGameState.shopCredit)
    return;
  shop->selectedItem = ITEM_GAME_TICKET;
  UiWork_PushValueSlotFar(ITEM_GAME_TICKET, 2);
  messageBase = MsgTokenGift;
  UiMessage_ShowAndRestoreState(messageBase);
  UiWork_PushValueSlotFar(shop->selectedItem, 2);
  UiMessage_ShowAndRestoreState(messageBase + 1);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(window, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(window, memberIndex, shop->selectedItem);
      Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiWork_PushValueSlotFar(unitId, 1);
        UiWork_PushValueSlotFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          UiMessage_ShowAndWait(MsgPackFull);
        else
          UiMessage_ShowAndWait(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      AudioCommand_PlayFar(SE_SHOP_PURCHASE);
      UiMessage_ShowAndRestoreState(MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      UiMessage_ShowAndRestoreState(MsgDontWantIt);
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      break;
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
  shop->selectedItem = savedSelectedItem;
}

/// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts
/// (each list ending with 0 when shorter), then the shop's type.
enum {
  SHOP_STOCK_ITEMS = 0,
  SHOP_STOCK_ITEM_COUNT = 24,
  SHOP_STOCK_ARTIFACTS = 24,
  SHOP_STOCK_ARTIFACT_COUNT = 8,
  SHOP_STOCK_TYPE = 32,
  SHOP_STOCK_COLUMNS = 33,
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

/// Returns the number of rows in the shop stock table.
s32 EventTable_GetRowLimit(void) { return SHOP_STOCK_ROWS; }

/// Adds a location's initial artifacts to shared stock once, before its first
/// shop visit.
void EventTable_ApplyRowAbilities(s32 stockRow) {
  const s16 *stockEntries;
  const s16 *artifactCursor;
  s32 itemId;
  s32 artifactIndex;
  s32 initializedFlag;

  initializedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
  if (GameFlag_TestFar(initializedFlag) == 0) {
    GameFlag_SetBitFar(initializedFlag);
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

/// Copies up to 24 ordinary stock items and appends a zero terminator.
s32 EventTable_CopyRowHeader(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (sStockTable[stockRow][SHOP_STOCK_ITEMS] != 0) {
    dst = output;
    src = sStockTable[stockRow];
    do {
      /* FAKEMATCH: only the copy read is volatile, so its lifetime stays
       * separate from the signed sentinel read. Without volatile: differ 41;
       * indexed and while forms: 34 to 54; read or copied as u16 (2026-10-13):
       * differ 41, the sentinel read is reused for the copy. */
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

/// Reads the shop type from the last word of its stock row.
s32 EventTable_GetRowType(s32 stockRow) {
  return sStockTable[stockRow][SHOP_STOCK_TYPE];
}
