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
    ITEM_001, ITEM_002, ITEM_CLAYMORE, ITEM_004, ITEM_SHAMSHIR,
    ITEM_SILVER_BLADE, ITEM_MASAMUNE, ITEM_ARCTIC_BLADE, ITEM_GAIA_BLADE,
    ITEM_010, ITEM_MURAMASA, ITEM_SHORT_SWORD, ITEM_MINT, ITEM_DEMON_AXE,
    ITEM_ASURAS_ARMOR, ITEM_COTTON_SHIRT, ITEM_090, ITEM_HERB, ITEM_NUT,
    ITEM_190, 0,
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
  creditCost = sGameTicketCreditCosts[gMainRoutineWork.shopGifts];
  memberIndex = 0;

  // Gifts spend accumulated shop credit; they do not debit the party's coins.
  if (creditCost > gMainRoutineWork.shopCredit)
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
    wait(1);
  }
  shop->selectedItem = savedSelectedItem;
}

/// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts
/// (each list ending with 0 when shorter), then the shop's type, which an item
/// shop, SHOP_TYPE_ITEMS, leaves at 0.
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
    {0},
    {
        [SHOP_STOCK_ITEMS] = ITEM_001, ITEM_SHORT_SWORD, ITEM_043, ITEM_055,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_COTTON_SHIRT, ITEM_090, ITEM_103, ITEM_118,
        ITEM_PADDED_GLOVES, ITEM_156,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_001, ITEM_SHORT_SWORD, ITEM_031, ITEM_043,
        [SHOP_STOCK_ARTIFACTS] = ITEM_056,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_090, ITEM_118, ITEM_PADDED_GLOVES, ITEM_136,
        ITEM_156, ITEM_166,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_001, ITEM_002, ITEM_HUNTERS_SWORD, ITEM_031,
        ITEM_044,
        [SHOP_STOCK_ARTIFACTS] = ITEM_057,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_075, ITEM_090, ITEM_104, ITEM_119, ITEM_128,
        ITEM_136, ITEM_145, ITEM_157, ITEM_166,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_002, ITEM_HUNTERS_SWORD, ITEM_031, ITEM_044,
        [SHOP_STOCK_ARTIFACTS] = ITEM_058,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_075, ITEM_092, ITEM_119, ITEM_128, ITEM_136,
        ITEM_145, ITEM_157, ITEM_166,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_002, ITEM_HUNTERS_SWORD, ITEM_032, ITEM_044,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_075, ITEM_092, ITEM_119, ITEM_128, ITEM_137,
        ITEM_146, ITEM_157,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_002, ITEM_018, ITEM_032, ITEM_045,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_092, ITEM_105, ITEM_IRON_SHIELD, ITEM_137,
        ITEM_146, ITEM_157, ITEM_167,
        [SHOP_STOCK_ARTIFACTS] = ITEM_CHINA_DRESS,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_018, ITEM_032, ITEM_045,
        [SHOP_STOCK_ARTIFACTS] = ITEM_059,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_CHAIN_MAIL, ITEM_092, ITEM_105,
        ITEM_IRON_SHIELD, ITEM_129, ITEM_137, ITEM_147, ITEM_167,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_018, ITEM_033, ITEM_045,
        [SHOP_STOCK_ARTIFACTS] = ITEM_060,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_CHAIN_MAIL, ITEM_107, ITEM_IRON_SHIELD,
        ITEM_129, ITEM_138, ITEM_147, ITEM_158, ITEM_167,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_004, ITEM_019, ITEM_033, ITEM_046,
        [SHOP_STOCK_ARTIFACTS] = ITEM_ANGELIC_ANKH,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_078, ITEM_094, ITEM_107, ITEM_121, ITEM_129,
        ITEM_138, ITEM_148, ITEM_158, ITEM_168,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_WATER_OF_LIFE,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_004, ITEM_019, ITEM_033, ITEM_046,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SHAMSHIR, ITEM_020, ITEM_034, ITEM_062,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_079, ITEM_139, ITEM_149, ITEM_169,
        [SHOP_STOCK_ARTIFACTS] = ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_POTION, ITEM_WATER_OF_LIFE,
        ITEM_PSY_CRYSTAL,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_004, ITEM_019, ITEM_033, ITEM_046,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SILVER_BLADE, ITEM_021, ITEM_047,
        ITEM_CRYSTAL_ROD,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_080, ITEM_150, ITEM_169,
        [SHOP_STOCK_ARTIFACTS] = ITEM_110, ITEM_122, ITEM_SPIRIT_ARMLET,
        ITEM_159,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR,
        ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_POTION, ITEM_WATER_OF_LIFE,
        ITEM_PSY_CRYSTAL,
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
  s32 copying;
  s16 *dst;
  s32 count;

  count = 0;
  if (sStockTable[stockRow][SHOP_STOCK_ITEMS] != 0) {
    dst = output;
    copying = 1;
    src = sStockTable[stockRow];
    do {
      /* FAKEMATCH(dead): the copy under a test of copying, always 1, which
       * emits nothing but keeps the copy's read apart from the signed sentinel
       * read; as the copy and increments without the test, 19 of 30
       * instructions differ in tbs-en. */
      if (copying) {
        *dst = *src;
        count++;
        src++;
        dst++;
      }
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
