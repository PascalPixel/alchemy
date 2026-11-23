/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
// Part of shop.c.

#include "shop_internal.h"

/// Runs the Buy and Artifacts counters: the player browses the stock seven
/// items a page, picks one, chooses who gets it and how many, and pays. Checks
/// room in the pack and coins first, and warns when the party member cannot
/// equip it. An artifact bought takes its stock counter down, and the list is
/// rebuilt. Repeats until B.
s32 Shop_SelectBuy(void) {
  struct ShopWork *shop = gMenuWork;
  struct Window *priceWindow = 0;
  struct Window *stockWindow;
  struct Window *messageWindow;
  struct Window *partyWindow;
  s32 selected = 0;
  s32 quantity;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  struct Window *itemWindow;
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
      Window_CreateFar(30 - SHOP_MONEY_WINDOW_WIDTH, 7, SHOP_MONEY_WINDOW_WIDTH,
                       4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();
  priceWindow =
      Window_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
  selectionIndex = selected;
  stockWindow = Window_CreateFar(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  messageWindow = Window_CreateFar(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
  redraw = 1;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      stockIds = shop->stockItemIds;
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_GetDirectFar(itemId);
      redraw = 0;
      MenuCursor_Place(stockWindow,
                       selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
      shop->cursorFrames = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMessage(messageWindow, itemId + MsgItemPlainName);
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, stockDefinition->price,
                         SHOP_PRICE_KIND_BUY);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      selected = selectionIndex;
      Sound_PlayFar(SE_MENU_CONFIRM);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex--;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex++;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
    wait(1);
  }

  Window_CloseFar(messageWindow, 2);
  Window_CloseFar(stockWindow, 2);
  wait(1);
  if (selectionResult != 0)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  // "Which one of you gets this?"
  Shop_ShowKeeperMessage(MsgWhoGetsThis);
  selectedDefinition = Item_GetDirectFar(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  Menu_CreatePartyFiguresFar(partyWindow, 2, 0, 8, 0);
  itemWindow = Window_CreateFar(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
  redraw = 1;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      Shop_ShowKeeperMessage(MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      redraw = 0;
      selectionIndex =
          (selectionIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[selectionIndex];
      MenuCursor_Place(partyWindow, selectionIndex * 24 - 12, 0);
      shop->cursorFrames = 3;
      Shop_DrawParty(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == EQUIPMENT_GROUP_NONE)
        Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      // Test for room by adding one and taking it out again, before the
      // coins are checked; a full bag and a full stack get different lines
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          // "Your pack is full, (the member). Who will you give this to?"
          Shop_ShowKeeperMessage(MsgPackFull);
        else
          // "You can't carry any more of this item, (the member), so who
          // gets it?"
          Shop_ShowKeeperMessage(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u32)selectedDefinition->price > gMainRoutineWork.holdings.coins) {
        Sound_PlayFar(SE_MENU_CANCEL);
        // "Oh! That's a fine choice, but you don't have enough money."
        Shop_ShowKeeperMessageHidingCursor(MsgBuyNotEnoughMoney);
        selectionResult = -1;
        break;
      }
      if (Item_IsCompatibleWithUnitFar(unitId, shop->selectedItem) == 0) {
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        // "You can't equip this, (the member). Do you still want it?"
        Shop_ShowKeeperMessage(MsgCannotEquipStillWant);
        if (Shop_AskYesNo(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      Sound_PlayFar(SE_MENU_CONFIRM);
      wait(1);
      quantity = Shop_SelectBuyCount(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Shop_GiveGameTicket(partyWindow, itemWindow);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex++;
      redraw = 1;
    }
    wait(1);
  }

  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(itemWindow, 2);
  Window_CloseFar(partyWindow, 2);
  wait(1);
  // An artifact bought leaves the world's stock, one for each bought, and the
  // list is made again without what is gone.
  if (selectionResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (Shop_ListAvailableArtifacts() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  // "Anything else?"
  Shop_ShowKeeperMessage(MsgBuyAnythingElse);
  goto next_page;

close:
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return 0;
}

/// Draws a page of seven stock items with their prices, the page arrows, and
/// the chosen item lifted and pulsing.
void Shop_DrawStock(struct Window *window, s32 selected) {
  struct ShopWork *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  u32 pageSlot;
  s32 priceX;
  s16 itemId;
  struct ItemDefinition *item;
  struct WindowSprite *icon;

  shop = gMenuWork;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected - selected % SHOP_STOCK_PAGE_ITEMS;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    // The arrows show the shop's own icons, which closing the window leaves.
    if (stockIndex != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, OUTPUT_SIZE_16X16,
                                    window, 216, -16);
      icon->kind = WINDOW_SPRITE_GLYPH;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
      icon->animationStep = 0;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, OUTPUT_SIZE_16X16,
                                    window, 216, 24);
      icon->kind = WINDOW_SPRITE_GLYPH;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
      icon->animationStep = 0;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (; pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        item = Item_GetDirectFar(itemId);
        icon = (u8 *)WindowSprite_CreateLoadedFar(itemId, ICON_BASE, window,
                                                  pageSlot * 32, 0);
        icon->depth = OAM_DEPTH_FRONT - 3;
        if (stockIndex == selected) {
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
          icon->animationStep = 10;
          icon->depth = OAM_DEPTH_FRONT - 2;
        }
        icon = Shop_CreatePriceSprite(item->price, window, priceX, 0);
        icon->depth = OAM_DEPTH_FRONT - 4;
      }
    }
  }
}

/// Draws the party's coins in the coins window, in the edition's layout.
void Shop_DrawMoney(void) {
  struct ShopWork *shop;
  struct Window *window;

  shop = gMenuWork;
  window = shop->moneyWindow;
  if (window != 0) {
#if LANGUAGE != LANGUAGE_JAPANESE
    Text_DrawMessageAsTilesFar(MsgYourCoins, window, 0, 0);
    Text_DrawNumberInWindowFar(gMainRoutineWork.holdings.coins, 6, window, 32,
                               8);
#else
    s32 msg = MsgYourCoins;

    Text_DrawMessageAsTilesFar(msg, window, 0, 0);
    Text_DrawNumberInWindowFar(gMainRoutineWork.holdings.coins, 6, window, 0,
                               8);
    Text_DrawMessageAsTilesFar(msg - 2, window, 48, 8); // MsgCoins
#endif
  }
}

/// Draws the item's name and below it its price. With a price of 0 it says
/// instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or that it is
/// not broken (SHOP_PRICE_KIND_REPAIR).
void Shop_DrawItemPrice(struct Window *window, s32 itemId, s32 price,
                        s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  Text_DrawMessageAsTilesFar(MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X, 8);
  return;
show_price:
  labelMessage = MsgPrice;
  Text_DrawMessageAsTilesFar(labelMessage, window, SHOP_ITEM_NOTE_X, 8);
  labelMessage -= 3; // MsgCoins
  Text_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
  Text_DrawMessageAsTilesFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  return;
draw:
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawMessageAsTilesFar(itemId + MsgItemName, window, 0, 0);
  if (price != 0)
    goto show_price;
  switch (priceKind) {
  case SHOP_PRICE_KIND_SELL:
    goto cannot_buy;
  case SHOP_PRICE_KIND_REPAIR:
    Text_DrawMessageAsTilesFar(MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
    break;
  default:
    goto show_price;
  }
}

/// Shows `message`, an item's description, in the help window; a NULL window is
/// ignored.
void Shop_DrawMessage(struct Window *window, s32 message) {
  if (window != 0) {
#if TBS_PATCH_1
    WindowSprite_RedrawSavedRectFar(window);
#else
    WindowSprite_ClearListFar(window);
    wait(1);
#endif
    SHOP_DRAW_MESSAGE(message, window, 0, 0);
  }
}

/// Shows the chosen party member running in place and the others standing, and
/// draws at four fifths of their size the party members who cannot equip
/// `itemId`.
void Shop_DrawParty(struct Window *window, s32 selected, s32 itemId) {
  struct ShopWork *shop = gMenuWork;
  s32 memberIndex;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        Sprite_StartAnimationFar(shop->partyMemberIcons[memberIndex],
                                 ANIM_PARTY_RUN);
      else
        Sprite_StartAnimationFar(shop->partyMemberIcons[memberIndex],
                                 ANIM_STAND);
      shop->partyMemberScale[memberIndex] = FIXED_ONE;
      if (!Item_IsCompatibleWithUnitFar(shop->partyMemberIds[memberIndex],
                                        itemId))
        shop->partyMemberScale[memberIndex] = FIXED_ONE * 4 / 5;
    }
  }
}

/// Shows how Attack, Defense and Agility would change if the party member wore
/// the item, with an arrow up or down beside each that changes and the name of
/// the item it would replace. The item is tried on in that item's slot and the
/// slot put back.
void Shop_DrawEquipComparison(struct Window *window, s32 unitId, s32 itemId) {
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
  WindowSprite_PrepareForRedrawFar(window);
  if (!Item_CanUnitEquip(unitId, itemId)) {
    Text_DrawMessageFar(MsgShopCannotEquip, window, 8, 24);
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
  // Luck is worked out with the rest but not shown.
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
  // Attack, Defense and Agility, each with the shop's own arrow icon.
  for (index = 0; index <= 2; index++) {
    if (baseStats[index] > previewStats[index])
      WindowSprite_CreateFar(shop->statDownIcon, OUTPUT_SIZE_16X16, window, 56,
                             index * 16 - 4)
          ->kind = WINDOW_SPRITE_GLYPH;
    else if (baseStats[index] < previewStats[index])
      WindowSprite_CreateFar(shop->statUpIcon, OUTPUT_SIZE_16X16, window, 56,
                             index * 16 - 4)
          ->kind = WINDOW_SPRITE_GLYPH;
    Text_DrawNumberInWindowFar(baseStats[index], 3, window, 32, index * 16);
    if (baseStats[index] != previewStats[index])
      Text_DrawNumberInWindowFar(previewStats[index], 3, window, 72,
                                 index * 16);
    Text_DrawMessageAsTilesFar(MsgShopStatLabel + index, window, 0, index * 16);
    Window_DrawDividerLineFar(window, 0, index * 2 + 2, 13, index * 2 + 2);
  }
  if (replacedItemId != -1)
    Text_DrawMessageAsTilesFar(replacedItemId + MsgItemName, window, 0, 48);
}
/// Shows the quantity already carried and the party member's inventory icons.
void Shop_DrawCarriedItem(struct Window *window, s32 unitId, s32 itemId) {
  struct Unit *unit;
  s32 x;
  s32 y;
  s32 itemIndex;
  s32 slot;
  u8 *icon;

  unit = Unit_GetFar(unitId);
  x = 8;
  y = 8;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      MessageArgument_PushFar(
          (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
          MESSAGE_ARGUMENT_NUMBER);
      Text_DrawMessageAsTilesFar(MsgYouHave, window, 0, 0);
    } else {
      Text_DrawMessageAsTilesFar(MsgShopNoneInStock, window, 0, 0);
    }
    itemIndex = 0;
    if (unit->inventory[itemIndex] != 0) {
      for (;;) {
        icon = (u8 *)WindowSprite_CreateLoadedFar(
            unit->inventory[itemIndex],
            ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_EQUIPPED,
            window, x, y);
        icon[offsetof(struct WindowSprite, depth)] = OAM_DEPTH_FRONT - 3;
        x += 16;
        if (itemIndex == SHOP_INVENTORY_COLUMNS - 1) {
          x = 8;
          y += 16;
        }
        if (itemIndex == SHOP_INVENTORY_COLUMNS * 2 - 1) {
          x = 8;
          y += 16;
        }
        itemIndex++;
        if (itemIndex >= INVENTORY_SLOTS)
          break;
        if (unit->inventory[itemIndex] == 0)
          break;
      }
    }
  }
}

/// Asks how many to buy when the item stacks, limited by what the coins buy,
/// the artifact's stock and a full stack less what the party member carries.
/// Returns the count (1 for other items), or -1 when cancelled.
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
    // "How many do you want?"
    Shop_ShowKeeperMessage(MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
    } else {
      carriedQuantity = 0;
    }

    quantityLimit = INVENTORY_STACK_MAX;
    if (item->price != 0)
      quantityLimit = (u32)gMainRoutineWork.holdings.coins / item->price;

    if (shop->action == SHOP_ACTION_ARTIFACTS) {
      artifactLimit = Item_AdjustCounterFar(itemId, 0);
      if (quantityLimit > artifactLimit)
        artifactLimit = Item_AdjustCounterFar(itemId, 0);
      else
        artifactLimit = quantityLimit;
      quantityLimit = artifactLimit;
    }

    quantityLimit += carriedQuantity;
    if (quantityLimit > INVENTORY_STACK_MAX)
      quantityLimit = INVENTORY_STACK_MAX;

    shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
    MenuCursor_Place(0, SHOP_QUANTITY_CURSOR_X, 48);
    selectedQuantity =
        Shop_SelectQuantity(carriedQuantity, quantityLimit, item->price);
  }
  return selectedQuantity;
}

/// The quantity window's count and the coins word after its total, in pixels:
/// German and French move both 8 left, Spanish the coins word 8 right.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
#define SHOP_QUANTITY_NUMBER_X 64
#define SHOP_QUANTITY_COIN_LABEL_X 128
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 144
#else
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 136
#endif

/// Lets the player choose how many to buy or sell on the quantity bar, from 1
/// to `maximum` less the `minimum` already carried, with the coins they come
/// to. Returns the number, or -1 for B.
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price) {
  void *tileBuffer;
  struct Window *window;
  s32 redraw;
  s32 resourceSlot;
  s32 quantityIndex;
  s32 selectedQuantity;
  struct WindowSprite *quantitySprite;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 unitMessageBase;
#endif

  tileBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  redraw = 1;
  maximum -= minimum;
  selectedQuantity = -1;
  quantityIndex = 0;
  window = Window_CreateFar(7, 4, 23, 3, WINDOW_FLAG_INSTANT);
  if (window != 0) {
    resourceSlot = VramCache_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      // The bar is one 8-tile entry shown as two 32x8 sprites (OBJ_SIZE_32X8);
      // the second starts at the entry's tile 4
      VramCache_Load(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
      WindowSprite_CreateFar(resourceSlot, OUTPUT_SIZE_32X8, window, 0, 0);
      quantitySprite =
          WindowSprite_CreateFar(resourceSlot, OUTPUT_SIZE_32X8, window, 32, 0);
      quantitySprite->oam.index += 4;
      for (;;) {
        if (gKeysPressed & KEY_A) {
          Sound_PlayFar(SE_MENU_CONFIRM);
          selectedQuantity = quantityIndex + 1;
          break;
        }
        if (gKeysPressed & KEY_B) {
          Sound_PlayFar(SE_MENU_CANCEL);
          selectedQuantity = -1;
          break;
        }
        if (gKeysRepeat & KEY_LEFT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex--;
        }
        if (gKeysRepeat & KEY_RIGHT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex++;
        }
        if (redraw) {
          redraw = 0;
          quantityIndex = (quantityIndex + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, tileBuffer,
                  DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
          QuantityBar_Fill(INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH,
                           tileBuffer);
          QuantityBar_Fill(minimum + maximum, QUANTITY_BAR_CHOOSABLE,
                           tileBuffer);
          QuantityBar_Fill(minimum + quantityIndex + 1, QUANTITY_BAR_CHOSEN,
                           tileBuffer);
          QuantityBar_Fill(minimum, QUANTITY_BAR_HELD, tileBuffer);
          VramCache_Load(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuffer);
          selectedQuantity = quantityIndex + 1;
#if LANGUAGE != LANGUAGE_JAPANESE
          Text_DrawNumberInWindowFar(selectedQuantity, 2, window,
                                     SHOP_QUANTITY_NUMBER_X, 0);
          Text_DrawNumberInWindowFar(price * selectedQuantity, 6, window,
                                     SHOP_QUANTITY_NUMBER_X + 16, 0);
          Text_DrawMessageAsTilesFar(MsgCoins, window,
                                     SHOP_QUANTITY_COIN_LABEL_X, 0);
#else
          Text_DrawNumberInWindowFar(selectedQuantity, 2, window, 64, 0);
          unitMessageBase = MsgPiece;
          Text_DrawMessageAsTilesFar(unitMessageBase, window, 80, 0);
          Text_DrawNumberInWindowFar(price * selectedQuantity, 6, window, 96,
                                     0);
          Text_DrawMessageAsTilesFar(unitMessageBase - 1, window, 144,
                                     0); // MsgCoins
#endif
        }
        wait(1);
      }
      wait(1);
      Window_CloseFar(window, 2);
    }
  }
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  return selectedQuantity;
}

/// Gives the party member each item bought and takes its price, then offers to
/// equip it and to sell what it replaced.
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 i;
  s32 lastAddedSlot;
  struct ItemDefinition *item;

  item = Item_GetDirectFar(itemId);
  lastAddedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  // Each one is paid for on its own, and its price counts toward the next
  // Game Ticket.
  Sound_PlayFar(SE_SHOP_PURCHASE);
  for (i = 0; i < quantity; i++) {
    lastAddedSlot = Inventory_AddItemFar(unitId, itemId);
    Party_AdjustCoinsFar(0 - item->price);
    Party_AdjustShopCreditFar(item->price);
    Shop_DrawMoney();
  }
  // "Then here you go!"
  Shop_ShowKeeperMessageHidingCursor(MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, lastAddedSlot) != 0) {
    Shop_SellOld(unitId, replacedSlot);
  }
}

/// Offers to equip the item just bought, and warns of a curse. Returns 1 when
/// it was equipped.
s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopWork *shop = gMenuWork;
  struct Unit *unit = Unit_GetFar(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 replacedSlot;
  struct Window *itemWindow;

  if (unit->inventory[slot] & INVENTORY_EQUIPPED)
    return 0;

  if (Item_CanUnitEquip(unitId, itemId) == 0)
    return 0;

  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  if (replacedSlot != -1) {
    struct ItemDefinition *equippedItem =
        Item_GetDirectFar(unit->inventory[replacedSlot]);

    if (equippedItem->flags & ITEM_CANNOT_UNEQUIP)
      return 0;
  }

  MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  // "Would you like to equip it now, (the member)?"
  Shop_ShowKeeperMessage(MsgEquipNowPrompt);
  if (Shop_AskYesNo(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  itemWindow = shop->itemWindow;
  if (itemWindow != 0)
    Shop_DrawMemberItems(itemWindow, unitId);

  if (item->flags & ITEM_CURSED) {
    Sound_PlayFar(SE_CURSE);
    Message_CloseFinishedWindowsFar();
    // "You've been cursed!"
    Message_OpenWindowFar(MsgBecameCursed, 8, 4, MESSAGE_WINDOW_UNREAD);
    while (Message_AllTypedFar() == 0) {
      wait(1);
    }
  }

  // "You look bolder already!"
  Shop_ShowKeeperMessageHidingCursor(MsgLookBolder);
  return 1;
}

/// Offers to sell the item the new one replaced, unless there is none or it is
/// a Psynergy item or indispensable. Returns 1 when it offered.
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
