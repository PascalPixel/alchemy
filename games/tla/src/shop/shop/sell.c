/// @file
/// Selling: choosing a party member, the item grid and the sale.
// Part of shop.c.

#include "../shop_internal.h"

// The Japanese money window is a tile narrower, as in Func_08108b70.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MONEY_WINDOW_WIDTH 11
#else
#define MONEY_WINDOW_WIDTH 12
#endif

s32 Func_08109cac(s32 unitId);
s32 Func_0810a2d0(s32 unitId);
s32 Func_0810b6c4(s32 unitId);

s32 Func_08109ad8(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 unitId = 0;
  s32 memberIndex = 0;
  void *listWindow;
  s32 redraw = 1;
  s32 result;

  shop->moneyWindow = Window_CreateFar(0, 9, MONEY_WINDOW_WIDTH, 4, 2);
  Func_08109188();
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, 2);
  listWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->listWindow = listWindow;
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  Func_080f8060(listWindow, 2, 0);
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Func_0810928c(listWindow, memberIndex, 0);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      wait(1);
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(113);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      Sound_PlayFar(112);
      if (shop->partyAction == 1) {
        Func_08109cac(unitId);
      } else if (shop->partyAction == 3) {
        Func_0810a2d0(unitId);
      } else {
        result = Func_0810b6c4(unitId);
        if (result == 0)
          break;
      }
      shop->cursor.anchor->active = 4;
      shop->mode = 12;
      redraw = 2;
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  /* FAKEMATCH(barrier): an empty asm clobbering r8 at the loop exit, which
   * keeps listWindow (live here) out of r8 and leaves it to unitId (dead
   * here); without it, 24 of 205 instructions differ in tla-en. */
  __asm__("" : : : "r8");
  Func_0810bea8(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return result;
}

#undef MONEY_WINDOW_WIDTH
#undef MONEY_WINDOW_WIDTH

// The price window and the item details window are sized to their text.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PRICE_WINDOW_X 16
#define PRICE_WINDOW_WIDTH 14
#define DETAILS_WINDOW_WIDTH 13
#elif (LANGUAGE == LANGUAGE_ENGLISH)
#define PRICE_WINDOW_X 15
#define PRICE_WINDOW_WIDTH 15
#define DETAILS_WINDOW_WIDTH 16
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define PRICE_WINDOW_X 14
#define PRICE_WINDOW_WIDTH 16
#define DETAILS_WINDOW_WIDTH 18
#else
#define PRICE_WINDOW_X 15
#define PRICE_WINDOW_WIDTH 15
#define DETAILS_WINDOW_WIDTH 18
#endif

// Unnamed in the catalog yet: 4699, 4723 and 4862 take placeholders here.
extern u8 MsgSellAnythingElse[];

s32 Shop_GetItemPrice(s32 item);
s32 Forge_GetMaterialChance(s32 item);

s32 Func_0810a03c(s32 unitId, s32 slot);
void Func_0810a2d8(s32 unitId, s32 slot);
s32 Func_0810b6cc(s32 unitId, s32 slot);

s32 Func_08109cac(s32 unitId) {
  struct ShopRuntime *shop;
  void *nameWindow;
  void *priceWindow;
  struct ShopUnit *unit;
  s32 inventoryCount;
  s32 slot;
  void *window;
  s32 redraw;
  s32 itemId;
  s32 result;
  s32 count;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  slot = 0;
  inventoryCount = 1;
  priceWindow = Window_CreateFar(PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4, 2);
  do {
    nameWindow = Window_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    redraw = 1;
    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = 0x1ff & unit->inventory[slot];
        Shop_PlaceCursor(shop->itemWindow, slot % 5 * 16, slot / 5 * 16 + 8);
        shop->mode = 3;
        if (shop->partyAction == 1)
          Func_081091cc(priceWindow, itemId,
                        Shop_GetSellPrice(unit->inventory[slot]), 1);
        else if (shop->partyAction == 3)
          Func_081091cc(priceWindow, itemId,
                        Shop_GetItemPrice(unit->inventory[slot]), 2);
        else
          Func_081091cc(priceWindow, itemId, Forge_GetMaterialChance(itemId),
                        3);
        Shop_DrawMsg(nameWindow, itemId + (s32)MsgItemPlainName);
      }
      if (gInput.pressed & 1) {
        Sound_PlayFar(112);
        result = 0;
        break;
      }
      if (gInput.pressed & 2) {
        Sound_PlayFar(113);
        result = -1;
        break;
      }
      if (gInput.held & 4) {
        u32 item = unit->inventory[slot];

        Sound_PlayFar(126);
        window = Window_CreateFar(0, 9, DETAILS_WINDOW_WIDTH, 10, 2);
        ItemMenu_DrawItemDetailsFar(window, item);
        // Through a byte pointer the store may alias any memory, so the
        // input address saved across the calls is restored after it, as
        // the native does; as shop->detailsHidden = 0, the restore moves
        // above it (6 of 374 instructions differ in tla-en).
        *(u8 *)&shop->detailsHidden = 0;
        while (gInput.held & 4)
          wait(1);
        shop->detailsHidden = 1;
        Window_CloseFar(window, 2);
        Func_08109188();
        WindowSprite_PrepareForRedrawFar(shop->listWindow);
#if TLA_PATCH_2
        Shop_DrawItemCount(shop->itemWindow, unitId);
#endif
        wait(1);
        // The Japanese release goes on to the next frame without redrawing
        // the cursor, price and name.
#if TLA_PATCH_1
        redraw = 1;
        continue;
#endif
      } else {
        if (gInput.repeat & 32) {
          Sound_PlayFar(111);
          slot--;
          slot = (inventoryCount + slot) % inventoryCount;
          redraw = 1;
        }
        if (gInput.repeat & 16) {
          Sound_PlayFar(111);
          slot++;
          slot = (inventoryCount + slot) % inventoryCount;
          redraw = 1;
        }
        if (gInput.repeat & 64) {
          slot -= 5;
          if (slot < 0)
            slot += 15;
          while (slot >= inventoryCount)
            slot -= 5;
          Sound_PlayFar(111);
          redraw = 1;
        }
        if (gInput.repeat & 128) {
          slot += 5;
          if (slot >= inventoryCount)
            slot -= 15;
          while (slot < 0)
            slot += 5;
          Sound_PlayFar(111);
          redraw = 1;
        }
        wait(1);
      }
    }
    Window_CloseFar(nameWindow, 2);
    wait(1);
    if (result != 0)
      break;
    if (shop->partyAction == 1) {
      count = Func_0810a03c(unitId, slot);
      if (count != -1)
        Shop_SellItem(unitId, slot, count);
      Shop_ShowKeeperMessage((s32)MsgSellAnythingElse);
    } else if (shop->partyAction == 3) {
      Func_0810a2d8(unitId, slot);
      Shop_ShowKeeperMessage((s32)MsgRepairAnythingElse);
    } else {
      result = Func_0810b6cc(unitId, slot);
      if (result == 0)
        break;
      Shop_ShowKeeperMessage((s32)MsgWhichItemNeedsAttention);
    }
  } while (Inventory_CountFar(unitId) != 0);
  Window_CloseFar(priceWindow, 2);
  return result;
}

#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH
#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH
#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH
#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH

void Shop_DrawItemCount(void *window, s32 item) {
  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
    Func_0810bf98(window, 8, 0, item);
    if (Inventory_CountFar(item) == 0)
#if (LANGUAGE == LANGUAGE_ITALIAN)
      Text_DrawMessageFar((s32)MsgNoItems, window, 20, 12);
#else
      Text_DrawMessageFar((s32)MsgNoItems, window, 8, 20);
#endif
  }
}

struct ShopSellWork {
  u8 unk000[0x4dc];
  struct ShopCursor cursor;
  u8 unk4ec[0x19];
  u8 mode;
};

s32 Inventory_GetQuantityFar(s32 unitId, s32 slot);
void Icon_PrepareActorFar(struct WindowSprite *object);

#if (LANGUAGE == LANGUAGE_GERMAN)
#define QUANTITY_CURSOR_X 120
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||       \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define QUANTITY_CURSOR_X 104
#else
#define QUANTITY_CURSOR_X 128
#endif

/// Asks how many of a stackable item to sell, when the unit holds more than
/// one, and returns the count; 1 otherwise.
// The Broken Seal's Shop_SelectSellCount.
s32 Func_0810a03c(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 price;
  s32 count;
  s32 selection;
  struct ItemDefinition *item;
  struct ShopSellWork *shop;
  struct ShopUnit *unit;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  item = Item_Get(unit->inventory[slot]);
  result = 1;
  price = Shop_GetSellPrice(unit->inventory[slot]);
  count = Inventory_GetQuantityFar(unitId, slot);
  selection = count;
  if ((item->flags & ITEM_STACKABLE) && count > 1) {
    Shop_ShowKeeperMessage((s32)MsgHowManyToSell);
    savedX = shop->cursor.targetX;
    savedY = shop->cursor.targetY;
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    Shop_PlaceCursor(NULL, QUANTITY_CURSOR_X, 0x30);
    result = Func_081096f8(0, selection, price);
    wait(1);
    Icon_PrepareActorFar(shop->cursor.anchor);
    Shop_PlaceCursor(NULL, savedX, savedY);
  }
  return result;
}

#undef QUANTITY_CURSOR_X

void Inventory_DiscardFar(s32 unitId, s32 slot);
void Unit_RefreshPsynergyFar(s32 unitId);

void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop;

  shop = Ram_HeapSlots->menuRuntime;
  // The inner block gives its locals their pseudos after the item id's
  // offset, which puts the stack slots in the ROM's order.
  {
    struct BattleUnit *unit = Unit_Get(unitId);
    s32 itemId = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_Get(itemId);
    u8 rare = item->flags & 4;
    s32 all = 0;
    s32 total;
    s32 message;
    s32 i;

    if (count == -1) {
      all = 1;
      count = 1;
    }
    total = Shop_GetSellPrice(unit->inventory[slot]) * count;
    if (total == 0) {
      MessageArgument_PushFar(itemId, 2);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgWeDoNotBuy);
      return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
      MessageArgument_PushFar(itemId, 2);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgCannotSellEquipped);
      return;
    }
    if (all)
      message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & 0x400)
      message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
      message = (s32)MsgSellTotal;
    else if (rare)
      message = (s32)MsgRareSellOffer;
    else
      message = (s32)MsgSellOffer;
    MessageArgument_PushFar(itemId, 2);
    MessageArgument_PushFar(total, 5);
    Shop_ShowKeeperMessageHidingCursor(message);
    if (Shop_AskYesNo(0) != 0) {
      if (rare || all)
        message = (s32)MsgKeepIt;
      else
        message = (s32)MsgChangedMind;
      Shop_ShowKeeperMessageHidingCursor(message);
      return;
    }
    Sound_PlayFar(102);
    for (i = 0; i < count; i++)
      Inventory_DiscardFar(unitId, slot);
    Unit_RefreshPsynergyFar(unitId);
    Unit_CalculateFar(unitId);
    Party_AdjustCoinsFar(total);
    Func_08109188();
    Shop_DrawItemCount(shop->itemWindow, unitId);
    if (rare || all)
      message = (s32)MsgSold;
    else
      message = (s32)MsgDeal;
    Shop_ShowKeeperMessageHidingCursor(message);
  }
}
