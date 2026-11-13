/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
// Part of shop.c.

#include "../shop_internal.h"

// The Select window is wider outside English and narrower in Japanese, the
// price window wider in German, French and Italian and narrower in Japanese;
// the Japanese money window is narrower too.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELD_WINDOW_X 10
#define HELD_WINDOW_WIDTH 20
#elif LANGUAGE == LANGUAGE_JAPANESE
#define HELD_WINDOW_X 14
#define HELD_WINDOW_WIDTH 13
#else
#define HELD_WINDOW_X 13
#define HELD_WINDOW_WIDTH 17
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define PRICE_WINDOW_WIDTH 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define PRICE_WINDOW_WIDTH 14
#else
#define PRICE_WINDOW_WIDTH 15
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define MONEY_WINDOW_X 19
#define MONEY_WINDOW_WIDTH 11
#else
#define MONEY_WINDOW_X 18
#define MONEY_WINDOW_WIDTH 12
#endif

struct ShopCursorSprite {
  u8 unk00[5];
  u8 active;
};

s32 Item_IsCompatibleWithOwnerFar(s32 unit, s32 item);

s32 PartyInventory_CountItemFar(s32 item);

void Shop_DrawMessage(s32 win, s32 message);
s32 Shop_SelectBuyCount(s32 unit, s32 item);
void Shop_BuyDone(s32 unit, s32 item, s32 quantity);
void Shop_DrawStock(struct RenderInput *window, s32 selected);
void Shop_DrawItemPrice(s32 window, s32 itemId, s32 price, s32 priceKind);
void Func_080f8040(void *window, s32 item, s32 count);
void Shop_DrawParty(struct RenderInput *window, s32 selected, s32 itemId);
void Shop_DrawCarriedItem(struct RenderInput *window, s32 unitId, s32 itemId);
void Shop_DrawEquipComparison(s32 window, s32 unitId, s32 itemId);
void Shop_GiveGameTicket(void *partyWindow, void *itemWindow);

s32 Shop_SelectBuy(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  void *priceWindow = 0;
  void *stockWindow;
  void *messageWindow;
  void *partyWindow;
  s32 selected = 0;
  s32 quantity;
  s32 stockCount;
  s16 *stockIds;
  struct ItemDefinition *heldDefinition;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  void *itemWindow;
  s32 redraw;
  s32 selectionIndex;
  s32 itemId;
  struct ItemDefinition *stockDefinition;
  s32 selectionResult;
  s32 purchaseResult;
  s32 unitId;
  s32 capacitySlot;
  s32 removedCount;
  /* FAKEMATCH(dead): both A-button tests also store into key, which nothing
   * reads; as if (gInput.pressed & 1) at both, 8 of 540 instructions differ in
   * tla-en. */
  s32 key;

  shop->itemWindow = 0;
  shop->moneyWindow =
      Window_CreateFar(MONEY_WINDOW_X, 7, MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();
  priceWindow = Window_CreateFar(0, 8, PRICE_WINDOW_WIDTH, 4, 2);

next_page:
  selectionIndex = selected;
  stockWindow = Window_CreateFar(0, 12, 30, 4, 2);
  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  messageWindow = Window_CreateFar(0, 17, 30, 3, 2);
  redraw = 1;
  stockIds = shop->stockItemIds;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_Get(itemId);
      MenuCursor_Place(stockWindow, selectionIndex % 7 * 32 - 8, 8);
      shop->mode = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMessage(messageWindow, itemId + (s32)MsgItemPlainName);
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, (u16)stockDefinition->price, 0);
      redraw = 0;
    }
    if (key = gInput.pressed & 1) {
      selected = selectionIndex;
      Sound_PlayFar(112);
      selectionResult = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      selectionResult = -1;
      break;
    }
    if (gInput.held & 4) {
      s32 held;
      void *window;
#if !TLA_PATCH_1
      s32 heldItem;
#endif

#if TLA_PATCH_1
      stockCount = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(stockCount);
      heldDefinition = Item_Get(stockCount);
#else
      heldItem = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(heldItem);
#endif
      Sound_PlayFar(126);
      window = Window_CreateFar(HELD_WINDOW_X, 3, HELD_WINDOW_WIDTH, 10, 2);
#if TLA_PATCH_1
      Func_080f8040(window, stockCount, held);
#else
      Func_080f8040(window, heldItem, held);
#endif
      while (gInput.held & 4)
        wait(1);
      Window_CloseFar(window, 2);
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMoney();
#if TLA_PATCH_1
      // Japanese does not redraw the price window here.
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, stockCount, (u16)heldDefinition->price, 0);
#endif
    } else {
      redraw = MenuGrid_MoveCursor(&selectionIndex, stockCount, 7);
    }
    wait(1);
  }

  Window_CloseFar(messageWindow, 2);
  Window_CloseFar(stockWindow, 2);
  wait(1);
  if (selectionResult == -1)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  Shop_ShowKeeperMessage((s32)MsgWhoGetsThis);
  selectedDefinition = Item_Get(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  Func_080f8060(partyWindow, 2, 0);
  itemWindow = Window_CreateFar(16, 11, 14, 9, 2);
  redraw = 2;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      Shop_ShowKeeperMessage((s32)MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      unitId = shop->partyMemberIds[selectionIndex];
      MenuCursor_Place(partyWindow, selectionIndex % 4 * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(selectionIndex / 4 * 4);
        wait(1);
      }
      Shop_DrawParty(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == 0)
        Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if (key = gInput.pressed & 1) {
      redraw = 2;
      WindowSprite_PrepareForRedrawFar(partyWindow);
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        Sound_PlayFar(113);
        MessageArgument_PushFar(unitId, 1);
        MessageArgument_PushFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == 15)
          Shop_ShowKeeperMessage((s32)MsgPackFull);
        else
          Shop_ShowKeeperMessage((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u16)selectedDefinition->price > (u32)gMainRoutineWork.coins) {
        Sound_PlayFar(113);
        Shop_ShowKeeperMessageHidingCursor((s32)MsgBuyNotEnoughMoney);
        purchaseResult = -1;
        break;
      }
      if (Item_IsCompatibleWithOwnerFar(unitId, shop->selectedItem) == 0) {
        MessageArgument_PushFar(unitId, 1);
        Shop_ShowKeeperMessage((s32)MsgCannotEquipStillWant);
        if (Shop_AskYesNo(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      Sound_PlayFar(112);
      wait(1);
      quantity = Shop_SelectBuyCount(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Shop_GiveGameTicket(partyWindow, itemWindow);
      purchaseResult = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      purchaseResult = -1;
      break;
    }
    redraw = MenuGrid_MoveCursor(&selectionIndex, shop->partyMemberCount, 4);
    wait(1);
  }

  ItemIconGrid_SetVisible(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(itemWindow, 2);
  Window_CloseFar(partyWindow, 2);
  wait(1);
  if (purchaseResult == 0 && shop->partyAction == 2) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (Shop_ListAvailableArtifacts() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  Shop_ShowKeeperMessage((s32)MsgBuyAnythingElse);
  goto next_page;

close:
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return 0;
}

#undef PRICE_WINDOW_WIDTH
#undef PRICE_WINDOW_WIDTH
#undef PRICE_WINDOW_WIDTH
#undef MONEY_WINDOW_WIDTH
#undef MONEY_WINDOW_WIDTH

struct WindowSprite *WindowSprite_CreateLoadedFar(s32 item, s32 mode,
                                                  struct RenderInput *window,
                                                  s32 x, s32 y);

void Shop_DrawStock(struct RenderInput *window, s32 selected) {
  struct ShopRuntime *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  s32 pageSlot;
  s32 priceX;
  s32 zero;
  s32 highlightFrame;
  s16 itemId;
  struct ItemDefinition *definition;
  struct WindowSprite *icon;

  shop = Ram_HeapSlots->menuRuntime;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected / 7 * 7;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    216, -16);
      zero = 0;
      icon->kind = zero;
      icon->active = 17;
      icon->animStep = zero;
    }
    if (stockIndex + 7 < itemCount) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, 0x40000000, window, 216,
                                    24);
      zero = 0;
      icon->kind = zero;
      icon->active = 15;
      icon->animStep = zero;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (; (u32)pageSlot < 7 && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        definition = Item_Get(itemId);
        icon =
            WindowSprite_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->active = 9;
          highlightFrame = 10;
          icon->animStep = highlightFrame;
          icon->sentinel = 253;
        }
        // Read unsigned (ldrh), as the buy screen reads the price elsewhere.
        icon = Shop_CreatePriceSprite((u16)definition->price, window, priceX, 0);
        icon->sentinel = 251;
      }
    }
  }
}

struct ShopWork_08109188 {
  u8 unk00[12];
  void *coinWindow;
};

void Shop_DrawMoney(void) {
  void *window =
      ((struct ShopWork_08109188 *)Ram_HeapSlots->menuRuntime)->coinWindow;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 message;
#endif

  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
#if LANGUAGE == LANGUAGE_JAPANESE
    // "Coins" (MsgCoins) after the count, two messages before MsgYourCoins.
    message = (s32)MsgYourCoins;
    Text_DrawCharacterAtOffsetFar(message, window, 0, 0);
    message -= 2;
    Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6, (s32)window, 0, 8);
    Text_DrawCharacterAtOffsetFar(message, window, 48, 8);
#else
    Text_DrawCharacterAtOffsetFar((s32)MsgYourCoins, window, 0, 0);
    Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6, (s32)window, 32, 8);
#endif
  }
}

// The price line reads "Price", the price, then "Coins". Japanese indents
// the line and its notes by 8 pixels, French draws the price 8 pixels nearer
// its label, Spanish and Italian the coins label 8 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_PRICE_LABEL_X 8
#else
#define SHOP_PRICE_LABEL_X 0
#endif
#if LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_NUMBER_X 24
#else
#define SHOP_PRICE_NUMBER_X 32
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_PRICE_COINS_X 80
#else
#define SHOP_PRICE_COINS_X 72
#endif

/// MsgItemCondition starts a list: "----------", "Processed", "Rusted".
extern u8 MsgCannotBuyThat[];

/// Draws the shop item's name and price, or its cannot-buy or not-broken note;
/// kind 3 draws the item condition message numbered by the price instead.
// The twin of ☀️'s Shop_DrawItemPrice (games/tbs/src/shop/shop.c), in the
// twin's goto layout, with kind 3 added. It matches with the switch on the
// kind as the twin has it (if-gotos give differ 46).
void Shop_DrawItemPrice(s32 window, s32 itemId, s32 price, s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  Text_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window,
                                  SHOP_PRICE_LABEL_X, 8);
  return;
not_broken:
  Text_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, SHOP_PRICE_LABEL_X,
                                  8);
  return;
show_price:
  labelMessage = (s32)MsgPrice;
  Text_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  labelMessage -= 3;
  Text_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
  Text_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_COINS_X, 8);
  return;
draw:
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawCharacterAtOffsetFar(itemId + (s32)MsgItemName, window, 0, 0);
  if (price == 0) {
    switch (priceKind) {
    case 1:
      goto cannot_buy;
    case 2:
      goto not_broken;
    }
  }
  if (priceKind == 3)
    Text_DrawCharacterAtOffsetFar(price + (s32)MsgItemCondition, window, 8,
                                    8);
  else
    goto show_price;
}

#undef SHOP_PRICE_NUMBER_X
#undef SHOP_PRICE_COINS_X

#define OWN_Text_DrawCharacterAtOffsetFar
//--------------------------------------------------------------------------
// Shop: replace a window's message.
//--------------------------------------------------------------------------

#if !TLA_PATCH_1
void WindowSprite_ClearListFar();
#define WindowSprite_ClearListFar ((void (*)(s32))WindowSprite_ClearListFar)
#endif

/// Redraws win and writes message from the top left; does nothing if win is
/// 0.
/// The Japanese release clears win and waits a frame; from patch 1 it redraws
/// the window's saved rectangle.
void Shop_DrawMessage(s32 win, s32 message) {
  if (win != 0) {
#if TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar(win);
#else
    WindowSprite_ClearListFar(win);
    wait(1);
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
    Text_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
    Text_DrawMessageFar(message, win, 0, 0);
#endif
  }
}

#undef WindowSprite_ClearListFar

void Shop_DrawParty(struct RenderInput *window, s32 selected, s32 itemId) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 page = selected / 4;
  s32 first = page * 4;
  s32 count = shop->partyMemberCount;
  struct WindowSprite *icon;
  s32 i;

  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (first != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    88, -12);
      icon->kind = 0;
      icon->active = 17;
      icon->animStep = 0;
    }
    if (first + 4 < count) {
      icon =
          WindowSprite_CreateFar(shop->nextPageIcon, 0x40000000, window, 88, 0);
      icon->kind = 0;
      icon->active = 15;
      icon->animStep = 0;
    }
    for (i = 0; i <= 3; i++) {
      s32 owner = shop->partyMemberIds[first + i];
      void *figure = shop->partyMemberIcons[i];

      if (figure != NULL) {
        if (first + i == selected)
          Animation_ApplyChildArgumentFar(figure, 30);
        else
          Animation_ApplyChildArgumentFar(figure, 1);
        shop->partyMemberScale[i] = 0x10000;
        if (!Item_IsCompatibleWithOwnerFar(owner, itemId))
          shop->partyMemberScale[i] = 0xcccc;
      }
    }
  }
}

struct ShopWork {
  u8 unk000[0x24];
  void *itemWindow;
  u8 unk028[0x4cc];
  u16 statUpIcon;
  u16 statDownIcon;
  u8 unk4f8[0xd];
  u8 mode;
  u8 unk506;
  s8 partyAction;
};

/// Shows how Attack, Defense and Agility would change if the party member wore
/// the item: equips it in place of what it would replace, recomputes, puts the
/// slot back and recomputes again, then draws both values with an arrow and
/// the name of the item it would replace. Called by Shop_SelectBuy for
/// weapons and armor.
void Shop_DrawEquipComparison(s32 window, s32 unitId, s32 itemId) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  struct ItemDefinition *item = Item_Get(itemId);
  s32 replaced = -1;
  s32 slot;
  s32 saved;
  s32 i;
  s32 without[4];
  s32 with[4];

  if (window == 0)
    return;
  WindowSprite_PrepareForRedrawFar(window);
  if (!Item_CanOwnerEquip(unitId, itemId)) {
    Text_DrawMessageFar((s32)MsgShopCannotEquip, window, 8, 24);
    return;
  }
  slot = Inventory_FindEquippedFar(unitId, item->type);
  if (slot == -1) {
    for (i = 0; i <= 14; i++) {
      if (!(unit->inventory[i] & 0x200))
        break;
    }
    if (i == 15) {
      for (i = 0; i <= 14; i++) {
        if (Item_Get(unit->inventory[i])->type == 6)
          break;
      }
      if (i == 15)
        i = 0;
    }
    slot = i;
  } else {
    replaced = unit->inventory[slot] & 0x1ff;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = itemId | 0x200;
  Unit_CalculateFar(unitId);
  with[0] = unit->atk;
  with[1] = unit->def;
  with[2] = unit->agi;
  with[3] = unit->luck;
  unit->inventory[slot] = saved;
  Unit_CalculateFar(unitId);
  without[0] = unit->atk;
  without[1] = unit->def;
  without[2] = unit->agi;
  without[3] = unit->luck;
  for (i = 0; i <= 2; i++) {
    if (without[i] > with[i])
      WindowSprite_CreateFar(shop->statDownIcon, 0x40000000,
                             (struct RenderInput *)window, 56, i * 16 - 4)
          ->kind = 0;
    else if (without[i] < with[i])
      WindowSprite_CreateFar(shop->statUpIcon, 0x40000000,
                             (struct RenderInput *)window, 56, i * 16 - 4)
          ->kind = 0;
    Text_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
    if (without[i] != with[i])
      Text_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
    Text_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
    Window_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
  }
  if (replaced != -1)
    Text_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}

s32 Inventory_FindFar(s32 unitId, s32 item_id);

/// Shows how many of an item the member already carries and draws the
/// member's inventory icons through ItemIconGrid_ShowInventory at (8, 8).
// The twin of ☀️'s Shop_DrawUnitItem (games/tbs/src/shop/shop.c), whose icon
// loop ⚓️ moved into that helper. It matches with the twin's quantityOffset
// local (differ 10 with the offset inside the read).
void Shop_DrawCarriedItem(struct RenderInput *window, s32 unitId, s32 itemId) {
  u8 *unitBytes;
  s32 slot;
  s32 quantityOffset;

  unitBytes = (u8 *)Unit_Get(unitId);
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + 216;
      MessageArgument_PushFar((*(u16 *)(unitBytes + quantityOffset) >> 11) + 1, 5);
      Text_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
    } else {
      Text_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
    }
    ItemIconGrid_ShowInventory(window, 8, 8, unitId);
  }
}

s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
s32 __udivsi3(s32, s32);

#if (LANGUAGE == LANGUAGE_GERMAN)
#define QUANTITY_CURSOR_Y 120
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||                    \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define QUANTITY_CURSOR_Y 104
#else
#define QUANTITY_CURSOR_Y 128
#endif

s32 Shop_SelectBuyCount(s32 unitId, s32 itemId) {
  struct ShopWork *shop;
  struct ShopUnit *unit;
  struct ItemDefinition *item;
  s32 quantity;
  s32 chance;
  s32 slot;
  s32 maximum;
  s32 result;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  item = Item_Get(itemId);
  result = 1;
  if (item->flags & 0x10) {
    Shop_ShowKeeperMessage((s32)MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1)
      quantity = (unit->inventory[slot] >> 11) + 1;
    else
      quantity = 0;

    chance = 30;
    if ((u16)item->price != 0)
      chance = __udivsi3(gMainRoutineWork.coins, (u16)item->price);

    if (shop->partyAction == 2) {
      maximum = Item_AdjustCounterFar(itemId, 0);
      if (chance > maximum)
        maximum = Item_AdjustCounterFar(itemId, 0);
      else
        maximum = chance;
      chance = maximum;
    }

    chance += quantity;
    if (chance > 30)
      chance = 30;

    shop->mode = 12;
    MenuCursor_Place(0, QUANTITY_CURSOR_Y, 0x30);
    result = Shop_SelectQuantity(quantity, chance, (u16)item->price);
  }
  return result;
}

#undef QUANTITY_CURSOR_Y
#undef QUANTITY_CURSOR_Y
#undef QUANTITY_CURSOR_Y

// The quantity window: outside the English edition the count sits further left
// and the price and coins label move with their words; the Spanish, French and
// Italian windows are wider; Japanese draws the counter コ after the count.
#if LANGUAGE == LANGUAGE_ENGLISH
#define QUANTITY_WINDOW_X 7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X 72
#define QUANTITY_PRICE_X 88
#define QUANTITY_COINS_X 136
#elif LANGUAGE == LANGUAGE_JAPANESE
#define QUANTITY_WINDOW_X 7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X 64
#define QUANTITY_PIECE_X 80
#define QUANTITY_PRICE_X 96
#define QUANTITY_COINS_X 144
#elif LANGUAGE == LANGUAGE_GERMAN
#define QUANTITY_WINDOW_X 7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 80
#define QUANTITY_COINS_X 128
#elif LANGUAGE == LANGUAGE_SPANISH
#define QUANTITY_WINDOW_X 5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 104
#define QUANTITY_COINS_X 160
#else
#define QUANTITY_WINDOW_X 5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X 64
#define QUANTITY_PRICE_X 88
#define QUANTITY_COINS_X 144
#endif

#ifndef MODERN
// A current GCC refuses the volatile redeclaration and reads input.h's.

#endif
extern const u8 gShop_QuantityTiles[];

struct UiSprite {
  u8 unk00[24];
  u16 tile : 10;
  u16 flags : 6;
};

/// Lets the player pick a quantity with left and right in a small window that
/// shows it and its price in coins; returns it, or -1 when B cancels. The
/// Broken Seal's Shop_SelectQuantity.
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price) {
  u8 *buffer;
  s32 window;
  s32 changed;
  s32 slot;
  s32 count;
  s32 result;
  struct UiSprite *sprite;
  s32 selected;

  buffer = HeapSlot_AllocEwram(56, 0x400);
  changed = 1;
  maximum -= minimum;
  result = -1;
  count = 0;
  window = Window_CreateFar(QUANTITY_WINDOW_X, 4, QUANTITY_WINDOW_WIDTH, 3, 2);
  if (window != 0) {
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
      VramCache_Load(slot, 0x100, 0);
      WindowSprite_CreateFar(slot, OUTPUT_SIZE_32X8,
                             (struct RenderInput *)window, 0, 0);
      sprite = (struct UiSprite *)WindowSprite_CreateFar(
          slot, OUTPUT_SIZE_32X8, (struct RenderInput *)window, 32, 0);
      sprite->tile += 4;
      for (;;) {
        if (gInput.pressed & 1) {
          Sound_PlayFar(112);
          result = count + 1;
          break;
        }
        if (gInput.pressed & 2) {
          Sound_PlayFar(113);
          result = -1;
          break;
        }
        if (gInput.repeat & 0x20) {
          Sound_PlayFar(111);
          changed = 1;
          count--;
        }
        if (gInput.repeat & 0x10) {
          Sound_PlayFar(111);
          changed = 1;
          count++;
        }
        if (changed) {
          changed = 0;
          count = (count + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, buffer, DMA_COPY_WORDS(64), REG_DMA3);
          QuantityBar_Fill(30, 14, buffer);
          QuantityBar_Fill(minimum + maximum, 0, buffer);
          QuantityBar_Fill(minimum + count + 1, 10, buffer);
          /* FAKEMATCH(temporary): the quantity in its own local before the
           * last fill; as result = count + 1 after VramCache_Load, 28 of 192
           * instructions differ in tla-en. */
          selected = count + 1;
          QuantityBar_Fill(minimum, 2, buffer);
          VramCache_Load(slot, 0x100, buffer);
          result = selected;
          Text_DrawNumberInWindowFar(result, 2, window, QUANTITY_COUNT_X, 0);
#if LANGUAGE == LANGUAGE_JAPANESE
          // MsgPiece, the counter after the count, written as the message after
          // MsgCoins: the coins label below then shares its literal-pool word.
          Text_DrawCharacterAtOffsetFar((s32)MsgCoins + 1, window,
                                        QUANTITY_PIECE_X, 0);
#endif
          Text_DrawNumberInWindowFar(price * result, 6, window,
                                     QUANTITY_PRICE_X, 0);
          Text_DrawCharacterAtOffsetFar((s32)MsgCoins, window, QUANTITY_COINS_X,
                                        0);
        }
        wait(1);
      }
      wait(1);
      Window_CloseFar(window, 2);
    }
  }
  HeapSlot_Release(56);
  return result;
}

#undef QUANTITY_COINS_X
#undef QUANTITY_COINS_X
#undef QUANTITY_COINS_X
#undef QUANTITY_COINS_X
#undef QUANTITY_COINS_X

s32 Party_AdjustShopCreditFar(s32 amount);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);

void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 remaining;
  s32 addedSlot;
  struct ItemDefinition *item;

  remaining = quantity;
  item = Item_Get(itemId);
  addedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  Sound_PlayFar(101);
  if (addedSlot < remaining) {
    do {
      addedSlot = Inventory_AddItemFar(unitId, itemId);
      Party_AdjustCoinsFar(0 - (u16)item->price);
      remaining -= 1;
      Party_AdjustShopCreditFar((u16)item->price);
      Shop_DrawMoney();
    } while (remaining != 0);
  }
  Shop_ShowKeeperMessageHidingCursor((s32)MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, addedSlot) != 0)
    Shop_SellOld(unitId, replacedSlot);
}

void Shop_DrawMemberItems(void *window, s32 item);

s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  u32 raw = unit->inventory[slot];
  s32 itemId;
  struct ItemDefinition *info;
  s32 replaced;
  void *window;

  itemId = 0x1ff;
  itemId &= raw;
  info = Item_Get(itemId);

  if (unit->inventory[slot] & 0x200)
    return 0;

  if (Item_CanOwnerEquip(unitId, itemId) == 0)
    return 0;

  replaced = Inventory_FindEquippedFar(unitId, info->type);
  if (replaced != -1) {
    struct ItemDefinition *oldInfo = Item_Get(unit->inventory[replaced]);

    if (oldInfo->flags & 2)
      return 0;
  }

  MessageArgument_PushFar(unitId, 1);
  Shop_ShowKeeperMessage((s32)MsgEquipNowPrompt);
  if (Shop_AskYesNo(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  window = shop->itemWindow;
  if (window != 0)
    Shop_DrawMemberItems(window, unitId);

  if (info->flags & 1) {
    Sound_PlayFar(103);
    Message_CloseFinishedWindowsFar();
    {
      s32 x = 8;
      s32 y = 4;
      s32 flags = 2;

      /* FAKEMATCH(barrier): an empty asm on x, y and flags, so the three are
       * set before the message loads; as Message_OpenWindowFar(
       * (s32)MsgBecameCursed, 8, 4, 2), 2 of 106 instructions differ in
       * tla-en. */
      asm volatile("" : "+l"(x), "+l"(y), "+l"(flags));
      Message_OpenWindowFar((s32)MsgBecameCursed, x, y, flags);
    }
    while (Message_AllTypedFar() == 0)
      wait(1);
  }

  Shop_ShowKeeperMessageHidingCursor((s32)MsgLookBolder);
  return 1;
}

s32 Shop_SellOld(s32 unitId, s32 slot) {
  u8 *unit;
  s32 itemOffset;
  s32 itemId;
  u32 raw;

  unit = Unit_Get(unitId);
  if (slot == -1)
    return 0;
  itemOffset = slot * 2 + 216;
  raw = *(u16 *)(unit + itemOffset);
  itemId = 0x1ff;
  itemId &= raw;
  if (Item_Get(itemId)->type == 6)
    return 0;
  if (Item_Get(itemId)->flags & 8)
    return 0;
  Shop_SellItem(unitId, slot, -1);
  return 1;
}

s32 Shop_SalePrice(s32 item_id) {
  s32 price;

  price = (u16)Item_Get(item_id)->price;
  if (Item_Get(item_id)->flags & 8)
    price = 0;
  else if (item_id & 0x400)
    price /= 2;
  else
    price = price * 3 / 4;
  return price;
}
