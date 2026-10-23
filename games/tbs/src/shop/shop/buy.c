/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
// Part of shop.c.

#include "../shop_internal.h"

// Shop_DrawUnitItem reads the unit and writes its icons as bytes; these are
// the offsets of the unit's inventory and the icon's depth.
#define SHOP_INVENTORY_OFFSET ((s32)offsetof(struct Unit, inventory))
#define SHOP_ICON_DEPTH_OFFSET ((s32)offsetof(struct RenderOutput, sentinel))

#define SHOP_STACK_LIMIT 30 ///< maximum quantity accepted by Inventory_AddItem

/// Runs the Buy and Artifacts counters: the player browses the stock seven
/// items a page, picks one, chooses who gets it and how many, and pays. Checks
/// room in the bag and coins first, and warns when the member cannot equip
/// it. An artifact bought takes its stock counter down, and the list is
/// rebuilt. Repeats until B. Called by Shop_Run.
s32 Shop_SelectBuy(void) {
  struct ShopWork *shop = gMenuWork;
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
  shop->moneyWindow = UiWindow_CreateFar(
      30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, UI_WINDOW_INSTANT);
  Shop_DrawMoney();
  priceWindow =
      UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, UI_WINDOW_INSTANT);

next_page:
  // Browse stock first; recipient and quantity selection follow confirmation.
  selectionIndex = selected;
  stockWindow = UiWindow_CreateFar(0, 12, 30, 4, UI_WINDOW_INSTANT);
  shop->cursor.anchor->active = UI_SPRITE_HOP_DOWN_LEFT;
  shop->mode = 12;
  messageWindow = UiWindow_CreateFar(0, 17, 30, 3, UI_WINDOW_INSTANT);
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
      Shop_DrawMessage(messageWindow, itemId + MsgItemPlainName);
      RenderOutput_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, stockDefinition->price,
                         SHOP_PRICE_KIND_BUY);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      selected = selectionIndex;
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex--;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex++;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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
  partyWindow = UiWindow_CreateFar(0, 14, 13, 3, UI_WINDOW_INSTANT);
  shop->cursor.anchor->active = UI_SPRITE_WOBBLE;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(partyWindow, 2, 0, 8, 0);
  itemWindow = UiWindow_CreateFar(16, 11, 14, 9, UI_WINDOW_INSTANT);
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
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == EQUIPMENT_GROUP_NONE)
        Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      // Test for room by adding one and taking it out again, before the
      // coins are checked; a full bag and a full stack get different lines
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        UiWork_PushValueSlotFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          UiMessage_ShowAndWait(MsgPackFull);
        else
          UiMessage_ShowAndWait(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u32)selectedDefinition->price > gGameState.coins) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        UiMessage_ShowAndRestoreState(MsgBuyNotEnoughMoney);
        selectionResult = -1;
        break;
      }
      if (Item_IsCompatibleWithUnitFar(unitId, shop->selectedItem) == 0) {
        UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        UiMessage_ShowAndWait(MsgCannotEquipStillWant);
        if (UiMessage_ShowChoice(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      WaitFrames(1);
      quantity = Shop_SelectBuyCount(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Shop_BuySpecialItem(partyWindow, itemWindow);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selectionIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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

/// Draws a page of seven stock items with their prices, the page arrows, and
/// the chosen item lifted and pulsing. Called by Shop_SelectBuy each time the
/// cursor moves.
void Shop_DrawStock(struct UiWindow *window, s32 selected) {

  struct ShopWork *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  s32 pageSlot;
  s32 priceX;
  s32 zero;
  s32 highlightFrame;
  s16 itemId;
  struct ItemDefinition *item;
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
      icon->active = UI_SPRITE_HOP_UP;
      icon->animationStep = zero;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = RenderOutput_CreateFar(shop->nextPageIcon, 0x40000000, window, 216,
                                    24);
      zero = 0;
      icon->kind = zero;
      icon->active = UI_SPRITE_HOP_DOWN_2;
      icon->animationStep = zero;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (;
           (u32)pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        item = Item_GetDirectFar(itemId);
        // Depth slots: the items 252, the chosen one 253, the prices 251
        icon =
            RenderOutput_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->active = UI_SPRITE_SWELL;
          highlightFrame = 10;
          icon->animationStep = highlightFrame;
          icon->sentinel = 253;
        }
        icon = Shop_CreatePriceSprite(item->price, window, priceX, 0);
        icon->sentinel = 251;
      }
    }
  }
}

/// Draws the party's coins in the coins window, in the edition's layout.
/// Called when a counter opens and after each purchase, sale or repair.
void Shop_DrawMoney(void) {
  struct ShopWork *shop;
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

/// Draws the item's name and below it its price. With a price of 0 it says
/// instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or
/// that it is not broken (SHOP_PRICE_KIND_REPAIR). Called by the buy, sell and
/// repair lists.
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
  case SHOP_PRICE_KIND_SELL:
    goto cannot_buy;
  case SHOP_PRICE_KIND_REPAIR:
    UiText_DrawCharacterAtOffsetFar(MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
    break;
  default:
    goto show_price;
  }
  return;
}

/// Shows `message`, an item's description, in the help window; a NULL window
/// is ignored. Called by the counters' lists each time the cursor moves.
void Shop_DrawMessage(struct UiWindow *window, s32 message) {
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

/// Shows the chosen member walking and the others standing, and draws at four
/// fifths of their size the members who cannot equip `itemId`. Called by
/// every counter's member choice.
void Shop_DrawParty(struct UiWindow *window, s32 selected, s32 itemId) {
  struct ShopWork *shop = gMenuWork;
  s32 memberIndex;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObject_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                           30);
      else
        AnimationObject_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                           1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Item_IsCompatibleWithUnitFar(shop->partyMemberIds[memberIndex],
                                        itemId))
        shop->partyMemberScale[memberIndex] = 0xcccc;
    }
  }
}

/// Shows how Attack, Defense and Agility would change if the member wore the
/// item: equips it in place of what it would replace, recomputes, puts the
/// slot back and recomputes again, then draws both values with an arrow and
/// the name of the item it would replace. Called by Shop_SelectBuy for
/// weapons and armor.
void Shop_DrawEquipComparison(struct UiWindow *window, s32 unitId, s32 itemId) {
  struct ShopWork *shop = gMenuWork;
  struct Unit *unit = Unit_GetFar(unitId);
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
  if (!Item_CanUnitEquip(unitId, itemId)) {
    UiText_DrawResourceFar(MsgShopCannotEquip, window, 8, 24);
    return;
  }
  previewSlot = Inventory_FindEquippedFar(unitId, item->type);
  // With nothing of the type equipped, borrow the first slot that is not
  // equipped, else the first Psynergy item's, else slot 0.
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
    replacedItemId = unit->inventory[previewSlot] & INVENTORY_ITEM_ID_MASK;
  }
  // Borrow a live slot so Unit_Calculate includes every equipment effect;
  // restore both the slot and the stats before returning to the counter.
  savedEntry = unit->inventory[previewSlot];
  unit->inventory[previewSlot] = itemId | INVENTORY_EQUIPPED;
  Unit_CalculateFar(unitId);
  previewStats[0] = unit->atk;
  previewStats[1] = unit->def;
  previewStats[2] = unit->agi;
  previewStats[3] = unit->luck;
  unit->inventory[previewSlot] = savedEntry;
  Unit_CalculateFar(unitId);
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
/// Shows the quantity already carried and the member's inventory icons.
/// Called by Shop_SelectBuy for items that are not equipment.
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

  unitBytes = (u8 *)Unit_GetFar(unitId);
  x = 8;
  y = 8;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + SHOP_INVENTORY_OFFSET;
      UiWork_PushValueSlotFar(
          (*(u16 *)(unitBytes + quantityOffset) >> INVENTORY_QUANTITY_SHIFT) +
              1,
          MESSAGE_ARGUMENT_NUMBER);
      UiText_DrawCharacterAtOffsetFar(MsgYouHave, window, 0, 0);
    } else {
      UiText_DrawCharacterAtOffsetFar(MsgShopNoneInStock, window, 0, 0);
    }
    itemIndex = 0;
    firstOffset = SHOP_INVENTORY_OFFSET;
    if (*(u16 *)(unitBytes + firstOffset) != 0) {
      for (;;) {
        itemOffset = itemIndex * 2 + SHOP_INVENTORY_OFFSET;
        icon = (u8 *)RenderOutput_CreateLoadedFar(
            *(u16 *)(unitBytes + itemOffset), 27, window, x, y);
        icon[SHOP_ICON_DEPTH_OFFSET] = 252;
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
        if (itemIndex >= INVENTORY_SLOTS)
          break;
        nextOffset = itemIndex * 2 + SHOP_INVENTORY_OFFSET;
        if (*(u16 *)(unitBytes + nextOffset) == 0)
          break;
      }
    }
  }
}

/// Asks how many to buy when the item stacks, limited by what the coins
/// buy, the artifact's stock and a stack of 30 less what the member carries.
/// Returns the count (1 for other items), or -1 when cancelled. Called by
/// Shop_SelectBuy.
s32 Shop_SelectBuyCount(s32 unitId, s32 itemId) {
  struct ShopWork *shop;
  struct Unit *unit;
  struct ItemDefinition *item;
  s32 carriedQuantity;
  s32 quantityLimit;
  s32 slot;
  s32 artifactLimit;
  s32 selectedQuantity;

  shop = gMenuWork;
  unit = Unit_GetFar(unitId);
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

    quantityLimit = SHOP_STACK_LIMIT;
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
    if (quantityLimit > SHOP_STACK_LIMIT)
      quantityLimit = SHOP_STACK_LIMIT;

    shop->mode = 12;
    Shop_PlaceCursor(0, SHOP_QUANTITY_CURSOR_X, 0x30);
    selectedQuantity =
        Shop_SelectQuantity(carriedQuantity, quantityLimit, item->price);
  }
  return selectedQuantity;
}

// The quantity window's count and the coins word after its total, in pixels:
// German and French move both 8 left, Spanish the coins word 8 right.
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

/// Chooses how many items to buy or sell, returning the quantity above
/// `minimum` or -1 when cancelled. The bar's fixed prefix is `minimum`
/// (items already carried when buying); `maximum` must exceed it so at least
/// one can be chosen. Holds HEAP_SLOT_TILE_SCRATCH for the bar's tiles. Called
/// by the buy and sell quantity pickers.
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

  tileBuffer = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  redraw = 1;
  maximum -= minimum;
  selectedQuantity = -1;
  quantityIndex = 0;
  window = UiWindow_CreateFar(7, 4, 23, 3, UI_WINDOW_INSTANT);
  if (window != 0) {
    resourceSlot = Resource_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      // The bar is one 8-tile entry shown as two 32x8 sprites (shape and size
      // bits 0x40004000); the second starts at the entry's tile 4
      VramCache_Load(resourceSlot, 0x100, 0);
      RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 0, 0);
      quantitySprite =
          RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 32, 0);
      quantitySprite->oam.index += 4;
      for (;;) {
        if (gKeysPressed & KEY_A) {
          Sound_PlaySongFar(SE_MENU_CONFIRM);
          selectedQuantity = quantityIndex + 1;
          break;
        }
        if (gKeysPressed & KEY_B) {
          Sound_PlaySongFar(SE_MENU_CANCEL);
          selectedQuantity = -1;
          break;
        }
        if (gKeysRepeat & KEY_LEFT) {
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex--;
        }
        if (gKeysRepeat & KEY_RIGHT) {
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex++;
        }
        if (redraw) {
          redraw = 0;
          quantityIndex = (quantityIndex + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, tileBuffer,
                  (DMA_ENABLE | DMA_32BIT) << 16 | 0x40, REG_DMA3);
          // Paint capacity, available stock, selection and carried items in
          // that order; each shorter prefix overwrites the previous colour.
          Shop_FillSelector(SHOP_STACK_LIMIT, 14, tileBuffer);
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
  FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  return selectedQuantity;
}

/// Adds each item bought to the member, charging its price and adding it to
/// the shop credit one at a time, then offers to equip it and, if equipped,
/// to sell what it replaced. Called by Shop_SelectBuy, which has already
/// checked the room and the coins. The artifact counters are not touched
/// here: Shop_SelectBuy takes them down after.
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
  Sound_PlaySongFar(SE_SHOP_PURCHASE);
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

/// Offers to equip the item just bought, unless it is equipped already, the
/// member cannot equip it, or what it would replace is cursed on. Equips it
/// on Yes, redraws the grid and warns of a curse. Returns 1 when it was
/// equipped. Called by Shop_BuyDone.
s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopWork *shop = gMenuWork;
  struct Unit *unit = Unit_GetFar(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 replacedSlot;
  struct UiWindow *itemWindow;

  if (unit->inventory[slot] & INVENTORY_EQUIPPED)
    return 0;

  if (Item_CanUnitEquip(unitId, itemId) == 0)
    return 0;

  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  if (replacedSlot != -1) {
    struct ItemDefinition *oldInfo =
        Item_GetDirectFar(unit->inventory[replacedSlot]);

    if (oldInfo->flags & ITEM_CANNOT_UNEQUIP)
      return 0;
  }

  UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  UiMessage_ShowAndWait(MsgEquipNowPrompt);
  if (UiMessage_ShowChoice(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  itemWindow = shop->itemWindow;
  if (itemWindow != 0)
    Shop_DrawUnitGrid(itemWindow, unitId);

  if (item->flags & ITEM_CURSED) {
    Sound_PlaySongFar(SE_103);
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(MsgBecameCursed, 8, 4, 2);
    while (UiWork_IsCompleteFar() == 0) {
      WaitFrames(1);
    }
  }

  UiMessage_ShowAndRestoreState(MsgLookBolder);
  return 1;
}

/// Offers to sell the item the new one replaced, unless there is none or it
/// is a Psynergy item or indispensable. Returns 1 when it offered. Called by
/// Shop_BuyDone.
s32 Shop_SellOld(s32 unitId, s32 slot) {
  struct Unit *unit;
  s32 itemId;

  unit = Unit_GetFar(unitId);
  if (slot == -1) {
    return 0;
  }
  itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  if (Item_GetDirectFar(itemId)->type == ITEM_TYPE_PSYNERGY) {
    return 0;
  }
  if (Item_GetDirectFar(itemId)->flags & ITEM_INDISPENSABLE) {
    return 0;
  }
  Shop_SellItem(unitId, slot, -1);
  return 1;
}

/// Returns what the shops pay for an inventory entry: nothing for an
/// indispensable item, half the price for a broken one, three quarters
/// otherwise.
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
#undef SHOP_INVENTORY_OFFSET
#undef SHOP_ICON_DEPTH_OFFSET
#undef SHOP_STACK_LIMIT
