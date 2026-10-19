/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
/// Part of shop.c, the shop object, compiled only through it.

#include "../shop_internal.h"

/// Browses stock, selects a recipient, checks capacity and funds, then
/// purchases the chosen quantity.
s32 Shop_SelectBuy(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *priceWindow = 0;
  struct UiWindow *stockWindow;
  struct UiWindow *messageWindow;
  struct UiWindow *partyWindow;
  s32 selected = 0;
  s32 quantity;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  struct UiWindow *itemWindow;
  s32 redraw;
  s32 selectionIndex;
  s32 stockCount;
  s32 itemId;
  struct ItemDefinition *stockDefinition;
  s32 selectionResult;
  s32 previousIndex;
  s32 unitId;
  s32 capacitySlot;
  s32 removedCount;
  s16 *stockIds;

  shop->itemWindow = 0;
  shop->moneyWindow =
      UiWindow_CreateFar(30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, 2);
  Shop_DrawMoney();
  priceWindow = UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, 2);

next_page:
  // Browse stock first; recipient and quantity selection follow confirmation.
  selectionIndex = selected;
  stockWindow = UiWindow_CreateFar(0, 12, 30, 4, 2);
  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  messageWindow = UiWindow_CreateFar(0, 17, 30, 3, 2);
  redraw = 1;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      stockIds = shop->stockItemIds;
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_GetDirectFar(itemId);
      redraw = 0;
      Shop_PlaceCursor(stockWindow,
                       selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
      shop->mode = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMsg(messageWindow, itemId + MsgItemPlainName);
      RenderOutput_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, stockDefinition->price, 0);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      selected = selectionIndex;
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex--;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex++;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_UP) != 0 &&
        selectionIndex - SHOP_STOCK_PAGE_ITEMS >= 0) {
      selectionIndex -= SHOP_STOCK_PAGE_ITEMS;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_DOWN) != 0) {
      if (selectionIndex + SHOP_STOCK_PAGE_ITEMS <
          (stockCount + (SHOP_STOCK_PAGE_ITEMS - 1)) / SHOP_STOCK_PAGE_ITEMS *
              SHOP_STOCK_PAGE_ITEMS) {
        selectionIndex += SHOP_STOCK_PAGE_ITEMS;
        redraw = 1;
      }
      if (selectionIndex > stockCount - 1)
        selectionIndex = stockCount - 1;
    }
    WaitFrames(1);
  }

  UiWork_FinalizeFar(messageWindow, 2);
  UiWork_FinalizeFar(stockWindow, 2);
  WaitFrames(1);
  if (selectionResult != 0)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  UiMessage_ShowAndWait(MsgWhoGetsThis);
  selectedDefinition = Item_GetDirectFar(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(partyWindow, 2, 0, 8, 0);
  itemWindow = UiWindow_CreateFar(16, 11, 14, 9, 2);
  redraw = 1;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      UiMessage_ShowAndWait(MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      redraw = 0;
      selectionIndex =
          (selectionIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[selectionIndex];
      Shop_PlaceCursor(partyWindow, selectionIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == 0)
        Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      // Probe capacity with the original add/remove before checking funds.
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiWork_PushValueSlotFar(unitId, 1);
        UiWork_PushValueSlotFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          UiMessage_ShowAndWait(MsgPackFull);
        else
          UiMessage_ShowAndWait(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u32)selectedDefinition->price > gGameState.coins) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiMessage_ShowAndRestoreState(MsgBuyNotEnoughMoney);
        selectionResult = -1;
        break;
      }
      if (Item_IsCompatibleWithOwnerFar(unitId, shop->selectedItem) == 0) {
        UiWork_PushValueSlotFar(unitId, 1);
        UiMessage_ShowAndWait(MsgCannotEquipStillWant);
        if (UiMessage_ShowChoice(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      WaitFrames(1);
      quantity = Shop_SelectBuyNum(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Shop_BuySpecialItem(partyWindow, itemWindow);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(itemWindow, 2);
  UiWork_FinalizeFar(partyWindow, 2);
  WaitFrames(1);
  // Artifact quantities are consumed only after the purchase completes.
  if (selectionResult == 0 && shop->partyAction == SHOP_ACTION_ARTIFACTS) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (Shop_ListAvailableArtifacts() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  UiMessage_ShowAndWait(MsgBuyAnythingElse);
  goto next_page;

close:
  UiWork_FinalizeFar(priceWindow, 2);
  UiWork_FinalizeFar(shop->moneyWindow, 2);
  WaitFrames(1);
  return 0;
}

/// Draws a seven-item stock page, selection highlight, page arrows and price
/// sprites.
void Shop_DrawStock(struct UiWindow *window, s32 selected) {

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
  struct RenderOutput *icon;

  shop = gMenuWork;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected - selected % SHOP_STOCK_PAGE_ITEMS;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = RenderOutput_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    216, -16);
      zero = 0;
      icon->kind = zero;
      icon->active = 17;
      icon->animStep = zero;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = RenderOutput_CreateFar(shop->nextPageIcon, 0x40000000, window, 216,
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
      for (;
           (u32)pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        definition = Item_GetDirectFar(itemId);
        icon =
            RenderOutput_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->active = 9;
          highlightFrame = 10;
          icon->animStep = highlightFrame;
          icon->sentinel = 253;
        }
        icon = Shop_CreatePriceSprite(definition->price, window, priceX, 0);
        icon->sentinel = 251;
      }
    }
  }
}

/// Draws the party's coins using the edition's layout and currency grammar.
void Shop_DrawMoney(void) {
  struct ShopRuntime *shop;
  struct UiWindow *window;

  shop = gMenuWork;
  window = shop->moneyWindow;
  if (window != 0) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    UiText_DrawCharacterAtOffsetFar(MsgYourCoins, window, 0, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0x20, 8);
#else
    s32 msg = MsgYourCoins;

    UiText_DrawCharacterAtOffsetFar(msg, window, 0, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(msg - 2, window, 48, 8);
#endif
  }
}

/// Draws the item name and price, or its cannot-buy or not-broken note.
void Shop_DrawItemPrice(struct UiWindow *window, s32 itemId, s32 price,
                        s32 priceKind) {
  // A structured switch with early returns reduced the English native extent
  // from 0x98 to 0x90, so the existing shared draw tail remains.
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  UiText_DrawCharacterAtOffsetFar(MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X,
                                  8);
  return;
show_price:
  labelMessage = MsgPrice;
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_ITEM_NOTE_X, 8);
  labelMessage -= 3;
  UiText_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  return;
draw:
  RenderOutput_RedrawSavedRectFar(window);
  UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 0, 0);
  if (price != 0)
    goto show_price;
  switch (priceKind) {
  case 1:
    goto cannot_buy;
  case 2:
    UiText_DrawCharacterAtOffsetFar(MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
    break;
  default:
    goto show_price;
  }
  return;
}

/// Clears or restores the message window, then draws its text; a null window
/// is ignored.
void Shop_DrawMsg(struct UiWindow *window, s32 message) {
  if (window != 0) {
#if TBS_PATCH_1
    RenderOutput_RedrawSavedRectFar(window);
#else
    RenderOutput_ClearListFar(window);
    WaitFrames(1);
#endif
    SHOP_DRAW_MESSAGE(message, window, 0, 0);
  }
}

/// Highlights the selected party icon and dims members who cannot equip the
/// candidate item.
void Shop_DrawParty(struct UiWindow *window, s32 selected, s32 itemId) {
  struct ShopRuntime *shop = gMenuWork;
  s32 memberIndex;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            30);
      else
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Item_IsCompatibleWithOwnerFar(shop->partyMemberIds[memberIndex],
                                         itemId))
        shop->partyMemberScale[memberIndex] = 0xcccc;
    }
  }
}

/// Temporarily equips the candidate, recalculates stats, restores the slot and
/// draws the comparison.
void Shop_DrawEquipComparison(struct UiWindow *window, s32 unitId, s32 itemId) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 replacedItemId = -1;
  s32 previewSlot;
  s32 savedEntry;
  s32 index;
  s32 baseStats[4];
  s32 previewStats[4];

  if (window == 0)
    return;
  RenderOutput_PrepareForRedrawFar(window);
  if (!Item_CanOwnerEquip(unitId, itemId)) {
    UiText_DrawResourceFar(MsgShopCannotEquip, window, 8, 24);
    return;
  }
  previewSlot = Inventory_FindEquippedFar(unitId, item->type);
  if (previewSlot == -1) {
    for (index = 0; index < INVENTORY_SLOTS; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        break;
    }
    if (index == INVENTORY_SLOTS) {
      for (index = 0; index < INVENTORY_SLOTS; index++) {
        if (Item_GetDirectFar(unit->inventory[index])->type ==
            ITEM_TYPE_PSYNERGY)
          break;
      }
      if (index == INVENTORY_SLOTS)
        index = 0;
    }
    previewSlot = index;
  } else {
    replacedItemId = unit->inventory[previewSlot] & ITEM_ID_MASK;
  }
  // Recalculate a real temporary equipment change, then restore it exactly.
  savedEntry = unit->inventory[previewSlot];
  unit->inventory[previewSlot] = itemId | INVENTORY_EQUIPPED;
  UnitCalcFar(unitId);
  previewStats[0] = unit->atk;
  previewStats[1] = unit->def;
  previewStats[2] = unit->agi;
  previewStats[3] = unit->luck;
  unit->inventory[previewSlot] = savedEntry;
  UnitCalcFar(unitId);
  baseStats[0] = unit->atk;
  baseStats[1] = unit->def;
  baseStats[2] = unit->agi;
  baseStats[3] = unit->luck;
  for (index = 0; index <= 2; index++) {
    if (baseStats[index] > previewStats[index])
      RenderOutput_CreateFar(shop->statDownIcon, 0x40000000, window, 56,
                             index * 16 - 4)
          ->kind = 0;
    else if (baseStats[index] < previewStats[index])
      RenderOutput_CreateFar(shop->statUpIcon, 0x40000000, window, 56,
                             index * 16 - 4)
          ->kind = 0;
    UiText_DrawNumberInWindowFar(baseStats[index], 3, window, 32, index * 16);
    if (baseStats[index] != previewStats[index])
      UiText_DrawNumberInWindowFar(previewStats[index], 3, window, 72,
                                   index * 16);
    UiText_DrawCharacterAtOffsetFar(MsgShopStatLabel + index, window, 0,
                                    index * 16);
    UiWindow_DrawDividerLineFar(window, 0, index * 2 + 2, 13, index * 2 + 2);
  }
  if (replacedItemId != -1)
    UiText_DrawCharacterAtOffsetFar(replacedItemId + MsgItemName, window, 0,
                                    48);
}
/// Shows the quantity already carried and draws the member's existing inventory
/// icons.
void Shop_DrawUnitItem(struct UiWindow *window, s32 unitId, s32 itemId) {
  // A typed RenderOutput icon store kept the English text extent but changed 76
  // text bytes; retain the existing byte view of the sprite.

  u8 *unitBytes;
  s32 x;
  s32 y;
  s32 itemIndex;
  s32 slot;
  s32 quantityOffset;
  s32 firstOffset;
  s32 itemOffset;
  s32 nextOffset;
  u8 *icon;

  unitBytes = (u8 *)UnitGetFar(unitId);
  x = 8;
  y = 8;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + 216;
      UiWork_PushValueSlotFar(
          (*(u16 *)(unitBytes + quantityOffset) >> INVENTORY_QUANTITY_SHIFT) +
              1,
          5);
      UiText_DrawCharacterAtOffsetFar(MsgYouHave, window, 0, 0);
    } else {
      UiText_DrawCharacterAtOffsetFar(MsgShopNoneInStock, window, 0, 0);
    }
    itemIndex = 0;
    firstOffset = 216;
    if (*(u16 *)(unitBytes + firstOffset) != 0) {
      for (;;) {
        itemOffset = itemIndex * 2 + 216;
        icon = (u8 *)RenderOutput_CreateLoadedFar(
            *(u16 *)(unitBytes + itemOffset), 27, window, x, y);
        icon[15] = 252;
        x += 16;
        if (itemIndex == 4) {
          x = 8;
          y += 16;
        }
        if (itemIndex == 9) {
          x = 8;
          y += 16;
        }
        itemIndex++;
        if (itemIndex > 14)
          break;
        nextOffset = itemIndex * 2 + 216;
        if (*(u16 *)(unitBytes + nextOffset) == 0)
          break;
      }
    }
  }
}

/// Limits the purchase by stack space, available coins and artifact stock
/// before asking its quantity.
s32 Shop_SelectBuyNum(s32 unitId, s32 itemId) {
  struct ShopRuntime *shop;
  struct BattleUnit *unit;
  struct ItemDefinition *item;
  s32 carriedQuantity;
  s32 quantityLimit;
  s32 slot;
  s32 artifactLimit;
  s32 selectedQuantity;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  item = Item_GetDirectFar(itemId);
  selectedQuantity = 1;
  if (item->flags & ITEM_STACKABLE) {
    UiMessage_ShowAndWait(MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
    } else {
      carriedQuantity = 0;
    }

    quantityLimit = 30;
    if (item->price != 0)
      quantityLimit = (u32)gGameState.coins / item->price;

    if (shop->partyAction == SHOP_ACTION_ARTIFACTS) {
      artifactLimit = Item_AdjustCounterFar(itemId, 0);
      if (quantityLimit > artifactLimit)
        artifactLimit = Item_AdjustCounterFar(itemId, 0);
      else
        artifactLimit = quantityLimit;
      quantityLimit = artifactLimit;
    }

    quantityLimit += carriedQuantity;
    if (quantityLimit > 30)
      quantityLimit = 30;

    shop->mode = 12;
    Shop_PlaceCursor(0, SHOP_QUANTITY_CURSOR_X, 0x30);
    selectedQuantity =
        Shop_SelectQuantity(carriedQuantity, quantityLimit, item->price);
  }
  return selectedQuantity;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define SHOP_QUANTITY_NUMBER_X 64
#define SHOP_QUANTITY_COIN_LABEL_X 128
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 144
#else
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 136
#endif

/// Polls a wrapped quantity selector, redraws its amount and price, and returns
/// the count or -1.
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price) {
  void *tileBuffer;
  struct UiWindow *window;
  s32 redraw;
  s32 resourceSlot;
  s32 quantityIndex;
  s32 selectedQuantity;
  struct RenderOutput *quantitySprite;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 unitMessageBase;
#endif

  tileBuffer = Runtime_AllocateBlock(14, 0x400);
  redraw = 1;
  maximum -= minimum;
  selectedQuantity = -1;
  quantityIndex = 0;
  window = UiWindow_CreateFar(7, 4, 23, 3, 2);
  if (window != 0) {
    resourceSlot = Resource_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      VramCache_Load(resourceSlot, 0x100, 0);
      RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 0, 0);
      quantitySprite =
          RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 32, 0);
      quantitySprite->oam.index += 4;
      for (;;) {
        if (gKeysPressed & KEY_A) {
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          selectedQuantity = quantityIndex + 1;
          break;
        }
        if (gKeysPressed & KEY_B) {
          AudioCommand_PlayFar(SE_MENU_CANCEL);
          selectedQuantity = -1;
          break;
        }
        if (gKeysRepeat & KEY_LEFT) {
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex--;
        }
        if (gKeysRepeat & KEY_RIGHT) {
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex++;
        }
        if (redraw) {
          redraw = 0;
          quantityIndex = (quantityIndex + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, tileBuffer,
                  (DMA_ENABLE | DMA_32BIT) << 16 | 0x40, REG_DMA3);
          Shop_FillSelector(30, 14, tileBuffer);
          Shop_FillSelector(minimum + maximum, 0, tileBuffer);
          Shop_FillSelector(minimum + quantityIndex + 1, 10, tileBuffer);
          Shop_FillSelector(minimum, 2, tileBuffer);
          VramCache_Load(resourceSlot, 0x100, tileBuffer);
          selectedQuantity = quantityIndex + 1;
#if (LANGUAGE != LANGUAGE_JAPANESE)
          UiText_DrawNumberInWindowFar(selectedQuantity, 2, window,
                                       SHOP_QUANTITY_NUMBER_X, 0);
          UiText_DrawNumberInWindowFar(price * selectedQuantity, 6, window,
                                       SHOP_QUANTITY_NUMBER_X + 16, 0);
          UiText_DrawCharacterAtOffsetFar(MsgCoins, window,
                                          SHOP_QUANTITY_COIN_LABEL_X, 0);
#else
          UiText_DrawNumberInWindowFar(selectedQuantity, 2, window, 64, 0);
          unitMessageBase = MsgPiece;
          UiText_DrawCharacterAtOffsetFar(unitMessageBase, window, 80, 0);
          UiText_DrawNumberInWindowFar(price * selectedQuantity, 6, window, 96,
                                       0);
          UiText_DrawCharacterAtOffsetFar(unitMessageBase - 1, window, 144, 0);
#endif
        }
        WaitFrames(1);
      }
      WaitFrames(1);
      UiWork_FinalizeFar(window, 2);
    }
  }
  Runtime_ReleaseHeapBlock(14);
  return selectedQuantity;
}

/// Adds each purchased item, charges coins and earns credit before offering to
/// equip it.
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 remainingQuantity;
  s32 lastAddedSlot;
  struct ItemDefinition *item;

  remainingQuantity = quantity;
  item = Item_GetDirectFar(itemId);
  lastAddedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  // Keep each inventory addition, coin debit and shop-credit increment in
  // order.
  AudioCommand_PlayFar(SE_SHOP_PURCHASE);
  if (lastAddedSlot < remainingQuantity) {
    do {
      lastAddedSlot = Inventory_AddItemFar(unitId, itemId);
      Party_AdjustCoinsFar(0 - item->price);
      remainingQuantity -= 1;
      Party_AdjustShopCreditFar(item->price);
      Shop_DrawMoney();
    } while (remainingQuantity != 0);
  }
  UiMessage_ShowAndRestoreState(MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, lastAddedSlot) != 0) {
    Shop_SellOld(unitId, replacedSlot);
  }
}

/// Checks equipment restrictions, asks permission and equips the item,
/// including its curse warning.
s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *definition = Item_GetDirectFar(itemId);
  s32 replacedSlot;
  struct UiWindow *itemWindow;

  if (unit->inventory[slot] & INVENTORY_EQUIPPED)
    return 0;

  if (Item_CanOwnerEquip(unitId, itemId) == 0)
    return 0;

  replacedSlot = Inventory_FindEquippedFar(unitId, definition->type);
  if (replacedSlot != -1) {
    struct ItemDefinition *oldInfo =
        Item_GetDirectFar(unit->inventory[replacedSlot]);

    if (oldInfo->flags & ITEM_CANNOT_UNEQUIP)
      return 0;
  }

  UiWork_PushValueSlotFar(unitId, 1);
  UiMessage_ShowAndWait(MsgEquipNowPrompt);
  if (UiMessage_ShowChoice(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  itemWindow = shop->itemWindow;
  if (itemWindow != 0)
    Shop_DrawUnitGrid(itemWindow, unitId);

  if (definition->flags & ITEM_CURSED) {
    AudioCommand_PlayFar(SE_103);
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(MsgBecameCursed, 8, 4, 2);
    while (UiWork_IsCompleteFar() == 0) {
      WaitFrames(1);
    }
  }

  UiMessage_ShowAndRestoreState(MsgLookBolder);
  return 1;
}

/// Offers to sell one replaced equipment item unless its type or flags prohibit
/// it.
s32 Shop_SellOld(s32 unitId, s32 slot) {
  struct BattleUnit *unit;
  s32 itemId;

  unit = UnitGetFar(unitId);
  if (slot == -1) {
    return 0;
  }
  itemId = unit->inventory[slot] & ITEM_ID_MASK;
  if (Item_GetDirectFar(itemId)->type == ITEM_TYPE_PSYNERGY) {
    return 0;
  }
  if (Item_GetDirectFar(itemId)->flags & ITEM_INDISPENSABLE) {
    return 0;
  }
  Shop_SellItem(unitId, slot, -1);
  return 1;
}

/// Returns zero for indispensable items, half price for broken ones or three
/// quarters otherwise.
s32 Shop_SalePrice(s32 inventoryEntry) {
  s32 salePrice;

  salePrice = Item_GetDirectFar(inventoryEntry)->price;

  if (Item_GetDirectFar(inventoryEntry)->flags & ITEM_INDISPENSABLE) {
    salePrice = 0;
  } else if (inventoryEntry & INVENTORY_BROKEN) {
    salePrice = salePrice / 2;
  } else {
    salePrice = salePrice * 3 / 4;
  }
  return salePrice;
}

#undef SHOP_QUANTITY_NUMBER_X
#undef SHOP_QUANTITY_COIN_LABEL_X
