/// @file
/// The item picker: which party member's item to drop when every pack is full.
// Part of shop.c.

#include "shop_internal.h"

/// Lets the player choose a party member, then one of their items that may be
/// dropped, in the selling screen without its shopkeeper. Returns 0 with the
/// choice in selectedUnit and selectedSlot, or -1 for B with both -1.
s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot) {
  struct ShopState *shop;
  struct WindowSprite *cursorAnchor;
  struct Window *partyWindow;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;
  s32 itemSlot;
  s32 result = 0;

  Shop_InitializeState();
  shop = gMenuState;
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
  partyWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  cursorAnchor = WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16,
                                        partyWindow, 0, 0);
  cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  cursorAnchor->kind = WINDOW_SPRITE_GLYPH;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  Menu_CreatePartyFiguresFar(partyWindow, 2, 0, 8, 0);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(partyWindow, memberIndex * 24 - 12, 0);
      shop->cursorFrames = 3;
      Shop_DrawParty(partyWindow, memberIndex, 0);
      Shop_DrawMemberItems(shop->itemWindow, unitId);
    }

    wait(1);
    if ((gKeysPressed & KEY_A) != 0) {
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        continue;
      }

      Sound_PlayFar(SE_MENU_CONFIRM);
      itemSlot = ItemPicker_SelectItem(unitId);
      if (itemSlot == -1) {
        shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
        shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
        redraw = 1;
        continue;
      }
      *selectedUnit = unitId;
      *selectedSlot = itemSlot;
      result = 0;
      break;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      *selectedUnit = -1;
      *selectedSlot = -1;
      result = -1;
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
  }

  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(partyWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  wait(1);
  Shop_ReleaseState();
  return result;
}

/// Where the line under the item draws "Price", the stack's sale value and the
/// coins word, in pixels, and how far back from MsgPriceHeading the coins word
/// is: MsgCoins, or in Japanese the message just before.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_PICKER_PRICE_LABEL_X 8
#define ITEM_PICKER_PRICE_NUMBER_X 40
#define ITEM_PICKER_COINS_LABEL_X 80
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_GERMAN
#define ITEM_PICKER_PRICE_LABEL_X 0
#define ITEM_PICKER_PRICE_NUMBER_X 32
#define ITEM_PICKER_COINS_LABEL_X 72
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_SPANISH
#define ITEM_PICKER_PRICE_LABEL_X 8
#define ITEM_PICKER_PRICE_NUMBER_X 40
#define ITEM_PICKER_COINS_LABEL_X 88
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_FRENCH
#define ITEM_PICKER_PRICE_LABEL_X 8
#define ITEM_PICKER_PRICE_NUMBER_X 32
#define ITEM_PICKER_COINS_LABEL_X 72
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#else
#define ITEM_PICKER_PRICE_LABEL_X 8
#define ITEM_PICKER_PRICE_NUMBER_X 40
#define ITEM_PICKER_COINS_LABEL_X 80
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 1
#endif

/// Lets the player choose one of the party member's items to drop, refusing an
/// indispensable one with a message and an equipped cursed one with only the
/// cancel sound (the game's bug; BUGFIX shows its message, see
/// SHOP_CANNOT_REMOVE_STATUS). Returns the slot, or -1 when cancelled.
s32 ItemPicker_SelectItem(s32 unitId) {
  struct ShopState *shop = gMenuState;
  struct Window *descriptionWindow;
  struct Window *priceWindow;
  struct Unit *unit;
  s32 slot;
  s32 redraw;
  s32 inventoryCount;
  s32 selectionResult;
  s32 discardStatus;
  s32 itemId;
  struct Window *window;
  s32 x;
  s32 y;

  unit = Unit_GetFar(unitId);
  inventoryCount = 1;
  selectionResult = 0;
  redraw = 1;
  priceWindow =
      Window_CreateFar(ITEM_PICKER_WINDOW_X, 8, ITEM_PICKER_WINDOW_WIDTH, 4,
                       WINDOW_FLAG_INSTANT);
  descriptionWindow = Window_CreateFar(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  slot = 0;

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      inventoryCount = Inventory_CountFar(unitId);
      if (slot > inventoryCount - 1)
        slot = inventoryCount - 1;

      itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
      window = shop->itemWindow;
      x = slot % SHOP_INVENTORY_COLUMNS << 4;
      y = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
      MenuCursor_Place(window, x, y);
      shop->cursorFrames = 3;
      ItemPicker_DrawItem(priceWindow, unitId, slot);
      Shop_DrawMessage(descriptionWindow, itemId + MsgItemPlainName);
    }

    wait(1);

    if ((gKeysPressed & KEY_A) != 0) {
      discardStatus = Inventory_CheckDiscardFar(unitId, slot);
      if (discardStatus == INVENTORY_DISCARD_ALLOWED) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        selectionResult = slot;
        break;
      }
      // BUG: default code selects the error using selectionResult, still zero.
      if (discardStatus == INVENTORY_DISCARD_INDISPENSABLE) {
        // "You can't drop it!"
        Message_OpenWindowFar(MsgCannotDropIt, 8, 1, MESSAGE_WINDOW_UNREAD);
      } else if (SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) ==
                 INVENTORY_DISCARD_STUCK) {
        // "You can't remove it!"
        Message_OpenWindowFar(MsgCannotRemoveIt, 8, 1, MESSAGE_WINDOW_UNREAD);
      }
      Sound_PlayFar(SE_MENU_CANCEL);
      while (Message_AllTypedFar() == 0)
        wait(1);
      Message_CloseFinishedWindowsFar();
      continue;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      slot--;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      slot++;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_UP) != 0) {
      slot -= SHOP_INVENTORY_COLUMNS;
      if (slot < 0)
        slot += INVENTORY_SLOTS;
      while (slot >= inventoryCount)
        slot -= SHOP_INVENTORY_COLUMNS;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_DOWN) != 0) {
      slot += SHOP_INVENTORY_COLUMNS;
      if (slot >= inventoryCount)
        slot -= INVENTORY_SLOTS;
      while (slot < 0)
        slot += SHOP_INVENTORY_COLUMNS;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
  }

  Window_CloseFar(descriptionWindow, 2);
  Window_CloseFar(priceWindow, 2);
  wait(1);
  return selectionResult;
}

/// Draws the slot's item name and, below it, why it cannot be dropped or what
/// the whole stack would sell for.
void ItemPicker_DrawItem(struct Window *window, s32 unitId, s32 slot) {
  struct Unit *unit = Unit_GetFar(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  u32 inventoryEntry = unit->inventory[slot];
  s32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

  if (window != 0) {
    s32 discardStatus;

    WindowSprite_RedrawSavedRectFar(window);
    Text_DrawMessageAsTilesFar(itemId + MsgItemName, window, 0, 0);

    discardStatus = Inventory_CheckDiscardFar(unitId, slot);
    if (discardStatus == INVENTORY_DISCARD_INDISPENSABLE) {
      Text_DrawMessageAsTilesFar(MsgCannotDrop, window, SHOP_ITEM_NOTE_X, 8);
    } else if (discardStatus == INVENTORY_DISCARD_STUCK) {
      Text_DrawMessageAsTilesFar(MsgCannotRemove, window, SHOP_ITEM_NOTE_X, 8);
    } else {
      s32 unitSalePrice;
      s32 stackSalePrice;

      unitSalePrice = Shop_SalePrice(unit->inventory[slot]);
      stackSalePrice = quantity * unitSalePrice;

      Text_DrawMessageAsTilesFar(MsgPriceHeading, window,
                                 ITEM_PICKER_PRICE_LABEL_X, 8);
      Text_DrawNumberInWindowFar(stackSalePrice, 5, window,
                                 ITEM_PICKER_PRICE_NUMBER_X, 8);
      Text_DrawMessageAsTilesFar(MsgPriceHeading -
                                     ITEM_PICKER_COINS_MESSAGE_OFFSET,
                                 window, ITEM_PICKER_COINS_LABEL_X, 8);
    }
  }
}

#undef ITEM_PICKER_PRICE_LABEL_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef ITEM_PICKER_COINS_LABEL_X
#undef ITEM_PICKER_COINS_MESSAGE_OFFSET
