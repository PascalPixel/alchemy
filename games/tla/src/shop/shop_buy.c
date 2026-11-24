/// @file
/// Buying: the stock list, the money and party windows, quantities and prices.
// Part of shop.c.

#include "shop_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "shop/shop.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/field.h"

/// The Select window is wider outside English and narrower in Japanese, the
/// price window wider in German, French and Italian and narrower in Japanese.
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
#define BUY_PRICE_WINDOW_WIDTH 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define BUY_PRICE_WINDOW_WIDTH 14
#else
#define BUY_PRICE_WINDOW_WIDTH 15
#endif

/// Runs the Buy and Artifacts counters: the player browses the stock seven
/// items a page, picks one, chooses who gets it and how many, and pays. Checks
/// room in the bag and coins first, and warns when the party member cannot
/// equip it. An artifact bought takes its stock counter down, and the list is
/// rebuilt. Holding Select shows the item's details. Repeats until B.
s32 Shop_SelectBuy(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct Window *priceWindow = 0;
  struct Window *stockWindow;
  struct Window *descriptionWindow;
  struct Window *partyWindow;
  s32 selected = 0;
  s32 quantity;
  s32 stockCount;
  s16 *stockIds;
  struct ItemDefinition *heldDefinition;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  struct Window *itemWindow;
  s32 redraw;
  s32 selectionIndex;
  s32 itemId;
  struct ItemDefinition *stockDefinition;
  s32 selectionResult;
  s32 purchaseResult;
  s32 unitId;
  s32 capacitySlot;
  s32 removedCount;
  /* FAKEMATCH(dead): both A-button tests also store into key. */
  s32 key;

  shop->itemWindow = 0;
  shop->moneyWindow =
      Window_CreateFar(30 - SHOP_COIN_WINDOW_WIDTH, 7, SHOP_COIN_WINDOW_WIDTH,
                       4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();
  priceWindow =
      Window_CreateFar(0, 8, BUY_PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
  selectionIndex = selected;
  stockWindow = Window_CreateFar(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  descriptionWindow = Window_CreateFar(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
  redraw = 1;
  stockIds = shop->stockItemIds;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_GetDirectFar(itemId);
      MenuCursor_Place(stockWindow,
                       selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
      shop->cursorFrames = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMessage(descriptionWindow, itemId + (s32)MsgItemDescription);
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, (u16)stockDefinition->price,
                         SHOP_PRICE_KIND_BUY);
      redraw = 0;
    }
    if (key = gInput.pressed & KEY_A) {
      selected = selectionIndex;
      Sound_PlayFar(SE_MENU_CONFIRM);
      selectionResult = 0;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if (gInput.held & KEY_SELECT) {
      s32 held;
      struct Window *window;
#if !TLA_PATCH_1
      s32 heldItem;
#endif

#if TLA_PATCH_1
      /* FAKEMATCH(temporary): the item shown is kept in stockCount, which the
       * loop's top reloads. */
      stockCount = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(stockCount);
      heldDefinition = Item_GetDirectFar(stockCount);
#else
      heldItem = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(heldItem);
#endif
      Sound_PlayFar(SE_RECOVERY);
      window = Window_CreateFar(HELD_WINDOW_X, 3, HELD_WINDOW_WIDTH, 10,
                                WINDOW_FLAG_INSTANT);
#if TLA_PATCH_1
      ItemMenu_DrawItemDetailsWithCountFar(window, stockCount, held);
#else
      ItemMenu_DrawItemDetailsWithCountFar(window, heldItem, held);
#endif
      while (gInput.held & KEY_SELECT)
        wait(1);
      Window_CloseFar(window, 2);
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMoney();
#if TLA_PATCH_1
      // Japanese does not redraw the price window here.
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, stockCount, (u16)heldDefinition->price,
                         SHOP_PRICE_KIND_BUY);
#endif
    } else {
      redraw = MenuGrid_MoveCursor(&selectionIndex, stockCount,
                                   SHOP_STOCK_PAGE_ITEMS);
    }
    wait(1);
  }

  Window_CloseFar(descriptionWindow, 2);
  Window_CloseFar(stockWindow, 2);
  wait(1);
  if (selectionResult == -1)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  Shop_ShowKeeperMessage((s32)MsgWhoGetsThis);
  selectedDefinition = Item_GetDirectFar(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  Menu_ShowPartyFiguresFar(partyWindow, 2, 0);
  itemWindow = Window_CreateFar(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
  redraw = MENU_GRID_CHANGED_PAGE;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      Shop_ShowKeeperMessage((s32)MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      unitId = shop->partyMemberIds[selectionIndex];
      MenuCursor_Place(partyWindow,
                       selectionIndex % SHOP_PARTY_PAGE_MEMBERS * 24 - 12, 0);
      shop->cursorFrames = 3;
      if (redraw == MENU_GRID_CHANGED_PAGE) {
        Menu_CreatePartyFiguresFar(selectionIndex / SHOP_PARTY_PAGE_MEMBERS *
                                   SHOP_PARTY_PAGE_MEMBERS);
        wait(1);
      }
      Shop_DrawParty(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == EQUIPMENT_GROUP_NONE)
        Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if (key = gInput.pressed & KEY_A) {
      redraw = MENU_GRID_CHANGED_PAGE;
      WindowSprite_PrepareForRedrawFar(partyWindow);
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          Shop_ShowKeeperMessage((s32)MsgPackFull);
        else
          Shop_ShowKeeperMessage((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u16)selectedDefinition->price > (u32)gMainRoutineState.coins) {
        Sound_PlayFar(SE_MENU_CANCEL);
        Shop_ShowKeeperMessageHidingCursor((s32)MsgBuyNotEnoughMoney);
        purchaseResult = -1;
        break;
      }
      if (Item_IsCompatibleWithUnitFar(unitId, shop->selectedItem) == 0) {
        MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
        Shop_ShowKeeperMessage((s32)MsgCannotEquipStillWant);
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
      purchaseResult = 0;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      purchaseResult = -1;
      break;
    }
    redraw = MenuGrid_MoveCursor(&selectionIndex, shop->partyMemberCount,
                                 SHOP_PARTY_PAGE_MEMBERS);
    wait(1);
  }

  ItemIconGrid_SetVisible(0);
  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(itemWindow, 2);
  Window_CloseFar(partyWindow, 2);
  wait(1);
  if (purchaseResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustRareItemStockFar(shop->selectedItem, -1);
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

#undef HELD_WINDOW_X
#undef HELD_WINDOW_WIDTH
#undef BUY_PRICE_WINDOW_WIDTH

/// Draws a page of seven stock items with their prices, the page arrows, and
/// the chosen item lifted and pulsing.
void Shop_DrawStock(struct Window *window, s32 selected) {
  struct ShopState *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  s32 pageSlot;
  s32 priceX;
  s16 itemId;
  struct ItemDefinition *item;
  struct WindowSprite *icon;

  shop = Ram_HeapSlots->menuState;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected / SHOP_STOCK_PAGE_ITEMS * SHOP_STOCK_PAGE_ITEMS;
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon,
                                    WINDOW_SPRITE_SIZE_16X16, window, 216, -16);
      icon->kind = WINDOW_SPRITE_PLAIN;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
      icon->animationStep = 0;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon,
                                    WINDOW_SPRITE_SIZE_16X16, window, 216, 24);
      icon->kind = WINDOW_SPRITE_PLAIN;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
      icon->animationStep = 0;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      /* FAKEMATCH(signature): the page's first stock item found as a number,
       * the list's address plus two bytes an item. */
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (;
           (u32)pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        item = Item_GetDirectFar(itemId);
        icon =
            WindowSprite_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->depth = OAM_DEPTH_FRONT - 3;
        if (stockIndex == selected) {
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
          icon->animationStep = 10;
          icon->depth = OAM_DEPTH_FRONT - 2;
        }
        icon = Shop_CreatePriceSprite((u16)item->price, window, priceX, 0);
        icon->depth = OAM_DEPTH_FRONT - 4;
      }
    }
  }
}

/// Draws the party's coins in the coins window, in the edition's layout.
void Shop_DrawMoney(void) {
  struct Window *window =
      ((struct ShopState *)Ram_HeapSlots->menuState)->moneyWindow;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 message;
#endif

  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
#if LANGUAGE == LANGUAGE_JAPANESE
    // "Coins" (MsgCoins) after the count, two messages before MsgYourCoins.
    message = (s32)MsgYourCoins;
    Text_DrawMessageAsTilesFar(message, window, 0, 0);
    message -= 2;
    Text_DrawNumberInWindowFar(gMainRoutineState.coins, 6, (s32)window, 0, 8);
    Text_DrawMessageAsTilesFar(message, window, 48, 8);
#else
    Text_DrawMessageAsTilesFar((s32)MsgYourCoins, window, 0, 0);
    Text_DrawNumberInWindowFar(gMainRoutineState.coins, 6, (s32)window, 32, 8);
#endif
  }
}

/// The price line reads "Price", the price, then "Coins". Japanese indents
/// the line and its notes by 8 pixels, French draws the price 8 pixels nearer
/// its label, Spanish and Italian the coins label 8 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_ITEM_NOTE_X 8
#else
#define SHOP_ITEM_NOTE_X 0
#endif
#if LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_NUMBER_X 24
#else
#define SHOP_PRICE_NUMBER_X 32
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_PRICE_LABEL_X 80
#else
#define SHOP_PRICE_LABEL_X 72
#endif

/// Draws the item's name and below it its price. With a price of 0 it says
/// instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or that it is
/// not broken (SHOP_PRICE_KIND_REPAIR); at the forge (SHOP_PRICE_KIND_FORGE)
/// `price` is the item's condition, drawn as its word.
void Shop_DrawItemPrice(struct Window *window, s32 itemId, s32 price,
                        s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  Text_DrawMessageAsTilesFar((s32)MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X,
                             8);
  return;
not_broken:
  Text_DrawMessageAsTilesFar((s32)MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
  return;
show_price:
  labelMessage = (s32)MsgPrice;
  Text_DrawMessageAsTilesFar(labelMessage, window, SHOP_ITEM_NOTE_X, 8);
  // "Coins", three messages before "Price".
  labelMessage -= 3;
  Text_DrawNumberInWindowFar(price, 5, (s32)window, SHOP_PRICE_NUMBER_X, 8);
  Text_DrawMessageAsTilesFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  return;
draw:
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawMessageAsTilesFar(itemId + (s32)MsgItemName, window, 0, 0);
  if (price == 0) {
    switch (priceKind) {
    case SHOP_PRICE_KIND_SELL:
      goto cannot_buy;
    case SHOP_PRICE_KIND_REPAIR:
      goto not_broken;
    }
  }
  // MsgItemCondition starts a list: "----------", "Processed", "Rusted".
  if (priceKind == SHOP_PRICE_KIND_FORGE)
    Text_DrawMessageAsTilesFar(price + (s32)MsgItemCondition, window, 8, 8);
  else
    goto show_price;
}

#undef SHOP_ITEM_NOTE_X
#undef SHOP_PRICE_NUMBER_X
#undef SHOP_PRICE_LABEL_X

void WindowSprite_ClearListFar(void *window);

/// Shows `message`, an item's description, in the help window; a NULL window
/// is ignored. The Japanese release clears the window and waits a frame; from
/// patch 1 it redraws the window's saved rectangle.
void Shop_DrawMessage(struct Window *window, s32 message) {
  if (window != 0) {
#if TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar(window);
#else
    WindowSprite_ClearListFar(window);
    wait(1);
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
    Text_DrawMessageAsTilesFar(message, window, 0, 0);
#else
    Text_DrawMessageFar(message, (s32)window, 0, 0);
#endif
  }
}

/// Shows the chosen party member running in place and the others standing,
/// four a page with its arrows, and draws at four fifths of their size the
/// party members who cannot equip `itemId`.
void Shop_DrawParty(struct Window *window, s32 selected, s32 itemId) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 page = selected / SHOP_PARTY_PAGE_MEMBERS;
  s32 first = page * SHOP_PARTY_PAGE_MEMBERS;
  s32 count = shop->partyMemberCount;
  struct WindowSprite *icon;
  s32 i;

  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (first != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon,
                                    WINDOW_SPRITE_SIZE_16X16, window, 88, -12);
      icon->kind = WINDOW_SPRITE_PLAIN;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
      icon->animationStep = 0;
    }
    if (first + SHOP_PARTY_PAGE_MEMBERS < count) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon,
                                    WINDOW_SPRITE_SIZE_16X16, window, 88, 0);
      icon->kind = WINDOW_SPRITE_PLAIN;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
      icon->animationStep = 0;
    }
    for (i = 0; i < SHOP_PARTY_PAGE_MEMBERS; i++) {
      s32 unitId = shop->partyMemberIds[first + i];
      void *figure = shop->partyMemberIcons[i];

      if (figure != NULL) {
        if (first + i == selected)
          Sprite_StartAnimationFar(figure, ANIM_PARTY_RUN);
        else
          Sprite_StartAnimationFar(figure, ANIM_STAND);
        shop->partyMemberScale[i] = FIXED_ONE;
        if (!Item_IsCompatibleWithUnitFar(unitId, itemId))
          shop->partyMemberScale[i] = FIXED_ONE * 4 / 5;
      }
    }
  }
}

/// Shows how Attack, Defense and Agility would change if the party member wore
/// the item: equips it in place of what it would replace, recomputes, puts the
/// slot back and recomputes again, then draws both values with an arrow and
/// the name of the item it would replace. Called by Shop_SelectBuy for
/// weapons and armor.
void Shop_DrawEquipComparison(struct Window *window, s32 unitId, s32 itemId) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct Unit *unit = Unit_Get(unitId);
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
  previewStats[0] = unit->attack;
  previewStats[1] = unit->defense;
  previewStats[2] = unit->agility;
  previewStats[3] = unit->luck;
  unit->inventory[previewSlot] = savedEntry;
  Unit_CalculateFar(unitId);
  baseStats[0] = unit->attack;
  baseStats[1] = unit->defense;
  baseStats[2] = unit->agility;
  baseStats[3] = unit->luck;
  for (index = 0; index <= 2; index++) {
    if (baseStats[index] > previewStats[index])
      WindowSprite_CreateFar(shop->statDownIcon, WINDOW_SPRITE_SIZE_16X16,
                             window, 56, index * 16 - 4)
          ->kind = WINDOW_SPRITE_PLAIN;
    else if (baseStats[index] < previewStats[index])
      WindowSprite_CreateFar(shop->statUpIcon, WINDOW_SPRITE_SIZE_16X16, window,
                             56, index * 16 - 4)
          ->kind = WINDOW_SPRITE_PLAIN;
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

/// Shows how many of the item the party member already carries and the party
/// member's inventory icons.
void Shop_DrawCarriedItem(struct Window *window, s32 unitId, s32 itemId) {
  struct Unit *unit;
  s32 slot;

  unit = Unit_Get(unitId);
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      MessageArgument_PushFar(
          (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
          MESSAGE_ARGUMENT_NUMBER);
      Text_DrawMessageAsTilesFar((s32)MsgYouHave, window, 0, 0);
    } else {
      Text_DrawMessageAsTilesFar((s32)MsgShopNoneInStock, window, 0, 0);
    }
    ItemIconGrid_ShowInventory(window, 8, 8, unitId);
  }
}

/// Asks how many to buy when the item stacks, limited by what the coins buy,
/// the artifact's stock and a stack of 30 less what the party member carries.
/// Returns the count (1 for other items), or -1 when cancelled.
s32 Shop_SelectBuyCount(s32 unitId, s32 itemId) {
  struct ShopState *shop;
  struct Unit *unit;
  struct ItemDefinition *item;
  s32 carriedQuantity;
  s32 quantityLimit;
  s32 slot;
  s32 artifactsInStock;
  s32 result;

  shop = Ram_HeapSlots->menuState;
  unit = Unit_Get(unitId);
  item = Item_GetDirectFar(itemId);
  result = 1;
  if (item->flags & ITEM_STACKABLE) {
    Shop_ShowKeeperMessage((s32)MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1)
      carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
    else
      carriedQuantity = 0;

    quantityLimit = SHOP_STACK_LIMIT;
    if ((u16)item->price != 0)
      quantityLimit = (u32)gMainRoutineState.coins / (u16)item->price;

    if (shop->action == SHOP_ACTION_ARTIFACTS) {
      artifactsInStock = Item_AdjustRareItemStockFar(itemId, 0);
      if (quantityLimit > artifactsInStock)
        artifactsInStock = Item_AdjustRareItemStockFar(itemId, 0);
      else
        artifactsInStock = quantityLimit;
      quantityLimit = artifactsInStock;
    }

    quantityLimit += carriedQuantity;
    if (quantityLimit > SHOP_STACK_LIMIT)
      quantityLimit = SHOP_STACK_LIMIT;

    shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
    MenuCursor_Place(0, SHOP_QUANTITY_CURSOR_X, 48);
    result =
        Shop_SelectQuantity(carriedQuantity, quantityLimit, (u16)item->price);
  }
  return result;
}

/// The quantity window: outside the English edition the count sits further left
/// and the price and coins label move with their words; the Spanish, French and
/// Italian windows are wider; Japanese draws the counter コ after the count.
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

/// Lets the player pick a quantity with left and right in a small window that
/// shows it and its price in coins: a bar of 30 segments, the ones already
/// carried in one colour and the ones chosen in another. Returns the count
/// above `minimum`, the items already carried, or -1 when B cancels.
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price) {
  u8 *tileBuffer;
  struct Window *window;
  s32 changed;
  s32 resourceSlot;
  s32 count;
  s32 result;
  struct WindowSprite *rightHalf;
  s32 selected;

  tileBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  changed = 1;
  maximum -= minimum;
  result = -1;
  count = 0;
  window = Window_CreateFar(QUANTITY_WINDOW_X, 4, QUANTITY_WINDOW_WIDTH, 3,
                            WINDOW_FLAG_INSTANT);
  if (window != 0) {
    resourceSlot = VramCache_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      VramCache_Load(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
      // The bar is two 32x8 sprites, the right one four tiles on.
      WindowSprite_CreateFar(resourceSlot, WINDOW_SPRITE_SIZE_32X8, window, 0,
                             0);
      rightHalf = WindowSprite_CreateFar(resourceSlot, WINDOW_SPRITE_SIZE_32X8,
                                         window, 32, 0);
      rightHalf->sprite.oam.index += 4;
      for (;;) {
        if (gInput.pressed & KEY_A) {
          Sound_PlayFar(SE_MENU_CONFIRM);
          result = count + 1;
          break;
        }
        if (gInput.pressed & KEY_B) {
          Sound_PlayFar(SE_MENU_CANCEL);
          result = -1;
          break;
        }
        if (gInput.repeat & KEY_LEFT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          changed = 1;
          count--;
        }
        if (gInput.repeat & KEY_RIGHT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          changed = 1;
          count++;
        }
        if (changed) {
          changed = 0;
          count = (count + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, tileBuffer,
                  DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
          // Paint capacity, available stock, selection and carried items in
          // that order; each shorter prefix overwrites the previous colour.
          QuantityBar_Fill(SHOP_STACK_LIMIT, QUANTITY_BAR_OUT_OF_REACH,
                           tileBuffer);
          QuantityBar_Fill(minimum + maximum, QUANTITY_BAR_OPEN, tileBuffer);
          QuantityBar_Fill(minimum + count + 1, QUANTITY_BAR_CHOSEN,
                           tileBuffer);
          selected = count + 1;
          QuantityBar_Fill(minimum, QUANTITY_BAR_HELD, tileBuffer);
          VramCache_Load(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuffer);
          result = selected;
          Text_DrawNumberInWindowFar(result, 2, (s32)window, QUANTITY_COUNT_X,
                                     0);
#if LANGUAGE == LANGUAGE_JAPANESE
          // The counter コ after the count: MsgPiece, which the ROM reaches
          // from MsgCoins, the message before it.
          Text_DrawMessageAsTilesFar((s32)MsgCoins + 1, window,
                                     QUANTITY_PIECE_X, 0);
#endif
          Text_DrawNumberInWindowFar(price * result, 6, (s32)window,
                                     QUANTITY_PRICE_X, 0);
          Text_DrawMessageAsTilesFar((s32)MsgCoins, window, QUANTITY_COINS_X,
                                     0);
        }
        wait(1);
      }
      wait(1);
      Window_CloseFar(window, 2);
    }
  }
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  return result;
}

#undef QUANTITY_WINDOW_X
#undef QUANTITY_WINDOW_WIDTH
#undef QUANTITY_COUNT_X
#undef QUANTITY_PIECE_X
#undef QUANTITY_PRICE_X
#undef QUANTITY_COINS_X

/// Gives the party member each item bought, one at a time, taking its price
/// and counting it toward the next Game Ticket; then offers to equip it and to
/// sell what it replaced.
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 remainingQuantity;
  s32 lastAddedSlot;
  struct ItemDefinition *item;

  remainingQuantity = quantity;
  item = Item_GetDirectFar(itemId);
  lastAddedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  Sound_PlayFar(SE_SHOP_PURCHASE);
  /* FAKEMATCH(temporary): lastAddedSlot, still 0, stands for 0 in the first
   * test. */
  if (lastAddedSlot < remainingQuantity) {
    do {
      lastAddedSlot = Inventory_AddItemFar(unitId, itemId);
      Party_AdjustCoinsFar(0 - item->price);
      remainingQuantity--;
      Party_AdjustShopCreditFar(item->price);
      Shop_DrawMoney();
    } while (remainingQuantity != 0);
  }
  Shop_ShowKeeperMessageHidingCursor(MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, lastAddedSlot) != 0) {
    Shop_SellOld(unitId, replacedSlot);
  }
}

/// Offers to equip the item just bought, and warns of a curse. Returns 1 when
/// it was equipped.
s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct Unit *unit = Unit_Get(unitId);
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
    Sound_PlayFar(SE_CURSE);
    Message_CloseFinishedWindowsFar();
    Message_OpenWindowFar(MsgBecameCursed, 8, 4, MESSAGE_WINDOW_UNREAD);
    while (Message_AllTypedFar() == 0) {
      wait(1);
    }
  }

  Shop_ShowKeeperMessageHidingCursor(MsgLookBolder);
  return 1;
}

/// Offers to sell the item the new one replaced, unless there is none or it is
/// a Psynergy item or indispensable. Returns 1 when it offered.
s32 Shop_SellOld(s32 unitId, s32 slot) {
  struct Unit *unit;
  s32 itemId;

  unit = Unit_Get(unitId);
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
