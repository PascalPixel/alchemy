/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
// Part of shop.c.

#include "../shop_internal.h"

// Shop_DrawCarriedItem reads the unit and writes its icons as bytes; these are
// the offsets of the unit's inventory and the icon's depth.
#define SHOP_INVENTORY_OFFSET ((s32)offsetof(struct Unit, inventory))
#define SHOP_ICON_DEPTH_OFFSET ((s32)offsetof(struct WindowSprite, sentinel))

#define SHOP_STACK_LIMIT 30 ///< maximum quantity accepted by Inventory_AddItem

/// Runs the Buy and Artifacts counters: the player browses the stock seven
/// items a page, picks one, chooses who gets it and how many, and pays. Checks
/// room in the bag and coins first, and warns when the party member cannot
/// equip it. An artifact bought takes its stock counter down, and the list is
/// rebuilt. Repeats until B. Called by Shop_Run.
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
  shop->moneyWindow = Window_CreateFar(
      30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();
  priceWindow =
      Window_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
  // Browse stock first; recipient and quantity selection follow confirmation.
  selectionIndex = selected;
  stockWindow = Window_CreateFar(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
  shop->mode = 12;
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
      shop->mode = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMessage(messageWindow, itemId + MsgItemPlainName);
      WindowSprite_RedrawSavedRectFar(priceWindow);
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

  Window_CloseFar(messageWindow, 2);
  Window_CloseFar(stockWindow, 2);
  WaitFrames(1);
  if (selectionResult != 0)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  Shop_ShowKeeperMessage(MsgWhoGetsThis);
  selectedDefinition = Item_GetDirectFar(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryActorsFar(partyWindow, 2, 0, 8, 0);
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
      shop->mode = 3;
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
        Sound_PlaySongFar(SE_MENU_CANCEL);
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          Shop_ShowKeeperMessage(MsgPackFull);
        else
          Shop_ShowKeeperMessage(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u32)selectedDefinition->price > gAdventure.coins) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        Shop_ShowKeeperMessageHidingCursor(MsgBuyNotEnoughMoney);
        selectionResult = -1;
        break;
      }
      if (Item_IsCompatibleWithUnitFar(unitId, shop->selectedItem) == 0) {
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        Shop_ShowKeeperMessage(MsgCannotEquipStillWant);
        if (Shop_AskYesNo(0) != 0) {
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

  Menu_ReleaseEntryActorsFar();
  Window_CloseFar(itemWindow, 2);
  Window_CloseFar(partyWindow, 2);
  WaitFrames(1);
  // Artifact quantities are consumed only after the purchase completes.
  if (selectionResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (Shop_ListAvailableArtifacts() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  Shop_ShowKeeperMessage(MsgBuyAnythingElse);
  goto next_page;

close:
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  WaitFrames(1);
  return 0;
}

/// Draws a page of seven stock items with their prices, the page arrows, and
/// the chosen item lifted and pulsing. Called by Shop_SelectBuy each time the
/// cursor moves.
void Shop_DrawStock(struct Window *window, s32 selected) {

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
  struct WindowSprite *icon;

  shop = gMenuWork;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected - selected % SHOP_STOCK_PAGE_ITEMS;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, OUTPUT_SIZE_16X16,
                                    window, 216, -16);
      zero = 0;
      icon->kind = zero;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
      icon->animationStep = zero;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, OUTPUT_SIZE_16X16,
                                    window, 216, 24);
      zero = 0;
      icon->kind = zero;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
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
            WindowSprite_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
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
  struct Window *window;

  shop = gMenuWork;
  window = shop->moneyWindow;
  if (window != 0) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    Text_DrawMessageAsTilesFar(MsgYourCoins, window, 0, 0);
    Text_DrawNumberInWindowFar(gAdventure.coins, 6, window, 0x20, 8);
#else
    s32 msg = MsgYourCoins;

    Text_DrawMessageAsTilesFar(msg, window, 0, 0);
    Text_DrawNumberInWindowFar(gAdventure.coins, 6, window, 0, 8);
    Text_DrawMessageAsTilesFar(msg - 2, window, 48, 8);
#endif
  }
}

/// Draws the item's name and below it its price. With a price of 0 it says
/// instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or
/// that it is not broken (SHOP_PRICE_KIND_REPAIR). Called by the buy, sell and
/// repair lists.
void Shop_DrawItemPrice(struct Window *window, s32 itemId, s32 price,
                        s32 priceKind) {
  // A structured switch with early returns reduced the English native extent
  // from 0x98 to 0x90, so the existing shared draw tail remains.
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
  labelMessage -= 3;
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
  return;
}

/// Shows `message`, an item's description, in the help window; a NULL window
/// is ignored. Called by the counters' lists each time the cursor moves.
void Shop_DrawMessage(struct Window *window, s32 message) {
  if (window != 0) {
#if TBS_PATCH_1
    WindowSprite_RedrawSavedRectFar(window);
#else
    WindowSprite_ClearListFar(window);
    WaitFrames(1);
#endif
    SHOP_DRAW_MESSAGE(message, window, 0, 0);
  }
}

/// Shows the chosen party member walking and the others standing, and draws at
/// four fifths of their size the party members who cannot equip `itemId`.
/// Called by every counter's party member choice.
void Shop_DrawParty(struct Window *window, s32 selected, s32 itemId) {
  struct ShopWork *shop = gMenuWork;
  s32 memberIndex;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        Sprite_StartAnimFar(shop->partyMemberIcons[memberIndex], 30);
      else
        Sprite_StartAnimFar(shop->partyMemberIcons[memberIndex], 1);
      shop->partyMemberScale[memberIndex] = Q16_ONE;
      if (!Item_IsCompatibleWithUnitFar(shop->partyMemberIds[memberIndex],
                                        itemId))
        shop->partyMemberScale[memberIndex] = 0xcccc;
    }
  }
}

/// Shows how Attack, Defense and Agility would change if the party member wore
/// the item: equips it in place of what it would replace, recomputes, puts the
/// slot back and recomputes again, then draws both values with an arrow and
/// the name of the item it would replace. Called by Shop_SelectBuy for
/// weapons and armor.
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
      WindowSprite_CreateFar(shop->statDownIcon, OUTPUT_SIZE_16X16, window, 56,
                             index * 16 - 4)
          ->kind = 0;
    else if (baseStats[index] < previewStats[index])
      WindowSprite_CreateFar(shop->statUpIcon, OUTPUT_SIZE_16X16, window, 56,
                             index * 16 - 4)
          ->kind = 0;
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
/// Called by Shop_SelectBuy for items that are not equipment.
void Shop_DrawCarriedItem(struct Window *window, s32 unitId, s32 itemId) {
  // A typed WindowSprite icon store kept the English text extent but changed 76
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
    WindowSprite_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + SHOP_INVENTORY_OFFSET;
      MessageArgument_PushFar(
          (*(u16 *)(unitBytes + quantityOffset) >> INVENTORY_QUANTITY_SHIFT) +
              1,
          MESSAGE_ARGUMENT_NUMBER);
      Text_DrawMessageAsTilesFar(MsgYouHave, window, 0, 0);
    } else {
      Text_DrawMessageAsTilesFar(MsgShopNoneInStock, window, 0, 0);
    }
    itemIndex = 0;
    firstOffset = SHOP_INVENTORY_OFFSET;
    if (*(u16 *)(unitBytes + firstOffset) != 0) {
      for (;;) {
        itemOffset = itemIndex * 2 + SHOP_INVENTORY_OFFSET;
        icon = (u8 *)WindowSprite_CreateLoadedFar(
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
/// buy, the artifact's stock and a stack of 30 less what the party member
/// carries. Returns the count (1 for other items), or -1 when cancelled. Called
/// by Shop_SelectBuy.
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
    Shop_ShowKeeperMessage(MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
    } else {
      carriedQuantity = 0;
    }

    quantityLimit = SHOP_STACK_LIMIT;
    if (item->price != 0)
      quantityLimit = (u32)gAdventure.coins / item->price;

    if (shop->action == SHOP_ACTION_ARTIFACTS) {
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
    MenuCursor_Place(0, SHOP_QUANTITY_CURSOR_X, 0x30);
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
  struct Window *window;
  s32 redraw;
  s32 resourceSlot;
  s32 quantityIndex;
  s32 selectedQuantity;
  struct WindowSprite *quantitySprite;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 unitMessageBase;
#endif

  tileBuffer = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  redraw = 1;
  maximum -= minimum;
  selectedQuantity = -1;
  quantityIndex = 0;
  window = Window_CreateFar(7, 4, 23, 3, WINDOW_FLAG_INSTANT);
  if (window != 0) {
    resourceSlot = VramCache_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      // The bar is one 8-tile entry shown as two 32x8 sprites (shape and size
      // bits 0x40004000); the second starts at the entry's tile 4
      VramCache_Load(resourceSlot, 0x100, 0);
      WindowSprite_CreateFar(resourceSlot, OUTPUT_SIZE_32X8, window, 0, 0);
      quantitySprite =
          WindowSprite_CreateFar(resourceSlot, OUTPUT_SIZE_32X8, window, 32, 0);
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
          QuantityBar_Fill(SHOP_STACK_LIMIT, 14, tileBuffer);
          QuantityBar_Fill(minimum + maximum, 0, tileBuffer);
          QuantityBar_Fill(minimum + quantityIndex + 1, 10, tileBuffer);
          QuantityBar_Fill(minimum, 2, tileBuffer);
          VramCache_Load(resourceSlot, 0x100, tileBuffer);
          selectedQuantity = quantityIndex + 1;
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
          Text_DrawMessageAsTilesFar(unitMessageBase - 1, window, 144, 0);
#endif
        }
        WaitFrames(1);
      }
      WaitFrames(1);
      Window_CloseFar(window, 2);
    }
  }
  FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  return selectedQuantity;
}

/// Adds each item bought to the party member, charging its price and adding it
/// to the shop credit one at a time, then offers to equip it and, if equipped,
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
  Shop_ShowKeeperMessageHidingCursor(MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, lastAddedSlot) != 0) {
    Shop_SellOld(unitId, replacedSlot);
  }
}

/// Offers to equip the item just bought, unless it is equipped already, the
/// party member cannot equip it, or what it would replace is cursed on. Equips
/// it on Yes, redraws the grid and warns of a curse. Returns 1 when it was
/// equipped. Called by Shop_BuyDone.
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
    struct ItemDefinition *oldInfo =
        Item_GetDirectFar(unit->inventory[replacedSlot]);

    if (oldInfo->flags & ITEM_CANNOT_UNEQUIP)
      return 0;
  }

  MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  Shop_ShowKeeperMessage(MsgEquipNowPrompt);
  if (Shop_AskYesNo(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  itemWindow = shop->itemWindow;
  if (itemWindow != 0)
    Shop_DrawMemberItems(itemWindow, unitId);

  if (item->flags & ITEM_CURSED) {
    Sound_PlaySongFar(SE_CURSE);
    TextPrinter_CloseFinishedWindowsFar();
    Message_OpenWindowFar(MsgBecameCursed, 8, 4, 2);
    while (TextPrinter_AllFinishedFar() == 0) {
      WaitFrames(1);
    }
  }

  Shop_ShowKeeperMessageHidingCursor(MsgLookBolder);
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
