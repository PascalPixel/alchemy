/// @file
/// The item picker: which member's item to drop when every bag is full.
// Part of shop.c.

#include "../shop_internal.h"

/// Lets the player choose a party member, then one of its items that may be
/// dropped. Writes the member and inventory slot to `selectedUnit` and
/// `selectedSlot` and returns 0, or writes -1 to both and returns -1 when
/// cancelled. Holds the shop's work block while it runs. Reached through
/// SHOP.S when an item is found with every bag full ("What will you drop?",
/// PartyInventory_GiveItem), by the Elemental Star room's Mythril Bags and by
/// the debug menu.
s32 Shop_PickUnitItem(s32 *selectedUnit, s32 *selectedSlot) {
  struct ShopRuntime *shop;
  struct RenderOutput *cursorAnchor;
  struct UiWindow *partyWindow;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;
  s32 itemSlot;
  s32 result = 0;

  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, UI_WINDOW_INSTANT);
  partyWindow = UiWindow_CreateFar(0, 14, 13, 3, UI_WINDOW_INSTANT);
  cursorAnchor = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000,
                                        partyWindow, 0, result);
  cursorAnchor->active = UI_SPRITE_WOBBLE;
  cursorAnchor->kind = result;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(partyWindow, 2, 0, 8, result);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(partyWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(partyWindow, memberIndex, 0);
      Shop_DrawUnitGrid(shop->itemWindow, unitId);
    }

    WaitFrames(1);
    if ((gKeysPressed & KEY_A) != 0) {
      if (Inventory_CountFar(unitId) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        continue;
      }

      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      itemSlot = Shop_SelectUse(unitId);
      if (itemSlot == -1) {
        shop->cursor.anchor->active = UI_SPRITE_WOBBLE;
        shop->mode = 12;
        redraw = 1;
        continue;
      }
      *selectedUnit = unitId;
      *selectedSlot = itemSlot;
      result = 0;
      break;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      *selectedUnit = -1;
      *selectedSlot = -1;
      result = -1;
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
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(partyWindow, 2);
  UiWork_FinalizeFar(shop->itemWindow, 2);
  WaitFrames(1);
  Shop_ReleaseCursorWork();
  return result;
}

// The sale value line's label, number and coin word, placed for each
// language's words; Japanese names its coins with a different message.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 80
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_GERMAN
#define SHOP_USE_PRICE_LABEL_X 0
#define SHOP_USE_PRICE_NUMBER_X 32
#define SHOP_USE_COIN_LABEL_X 72
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 88
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_FRENCH
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 32
#define SHOP_USE_COIN_LABEL_X 72
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#else
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 80
#define SHOP_USE_COIN_MESSAGE_OFFSET 1
#endif

/// Lets the player choose one of the member's items to drop, refusing an
/// indispensable one with a message and an equipped cursed one with only the
/// cancel sound (the game's bug; BUGFIX shows its message, see
/// SHOP_CANNOT_REMOVE_STATUS). Returns the slot, or -1 when cancelled. Called
/// by Shop_PickUnitItem.
s32 Shop_SelectUse(s32 unitId) {
  struct ShopRuntime *shop = gMenuWork;

  struct UiWindow *descriptionWindow;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 redraw;
  s32 inventoryCount;
  s32 selectionResult;
  s32 discardStatus;
  s32 itemId;
  struct UiWindow *window;
  s32 x;
  s32 y;

  unit = UnitGetFar(unitId);
  inventoryCount = 1;
  selectionResult = 0;
  redraw = 1;
  priceWindow = UiWindow_CreateFar(SHOP_USE_WINDOW_X, 8, SHOP_USE_WINDOW_WIDTH,
                                   4, UI_WINDOW_INSTANT);
  descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, UI_WINDOW_INSTANT);
  shop->cursor.anchor->active = UI_SPRITE_HOP_DOWN_LEFT;
  shop->mode = 12;
  slot = 0;

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      inventoryCount = Inventory_CountFar(unitId);
      if (slot > inventoryCount - 1)
        slot = inventoryCount - 1;

      itemId = unit->inventory[slot] & ITEM_ID_MASK;
      window = shop->itemWindow;
      x = slot % SHOP_INVENTORY_COLUMNS << 4;
      y = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
      Shop_PlaceCursor(window, x, y);
      shop->mode = 3;
      Shop_DrawUseItem(priceWindow, unitId, slot);
      Shop_DrawMsg(descriptionWindow, itemId + MsgItemPlainName);
    }

    WaitFrames(1);

    if ((gKeysPressed & KEY_A) != 0) {
      discardStatus = Inventory_CheckDiscardFar(unitId, slot);
      if (discardStatus == 0) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        selectionResult = slot;
        break;
      }
      // BUG: default code selects the error using selectionResult, still zero.
      if (discardStatus == -4) {
        UiText_OpenMessageWindowFar(MsgCannotDropIt, 8, 1, 2);
      } else if (SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) ==
                 -3) {
        UiText_OpenMessageWindowFar(MsgCannotRemoveIt, 8, 1, 2);
      }
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
      UiWork_FinalizePendingCoreFar();
      continue;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      slot -= 1;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      slot += 1;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_UP) != 0) {
      slot -= SHOP_INVENTORY_COLUMNS;
      if (slot < 0)
        slot += INVENTORY_SLOTS;
      while (slot >= inventoryCount)
        slot -= SHOP_INVENTORY_COLUMNS;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_DOWN) != 0) {
      slot += SHOP_INVENTORY_COLUMNS;
      if (slot >= inventoryCount)
        slot -= INVENTORY_SLOTS;
      while (slot < 0)
        slot += SHOP_INVENTORY_COLUMNS;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
  }

  UiWork_FinalizeFar(descriptionWindow, 2);
  UiWork_FinalizeFar(priceWindow, 2);
  WaitFrames(1);
  return selectionResult;
}

/// Draws the slot's item name and, below it, why it cannot be dropped or what
/// the whole stack would sell for. Called by Shop_SelectUse each time the
/// cursor moves.
void Shop_DrawUseItem(struct UiWindow *window, s32 unitId, s32 slot) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  u32 inventoryEntry = unit->inventory[slot];
  s32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

  if (window != 0) {
    s32 discardStatus;

    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 0, 0);

    discardStatus = Inventory_CheckDiscardFar(unitId, slot);
    if (discardStatus == -4) {
      UiText_DrawCharacterAtOffsetFar(MsgCannotDrop, window, SHOP_ITEM_NOTE_X,
                                      8);
    } else if (discardStatus == -3) {
      UiText_DrawCharacterAtOffsetFar(MsgCannotRemove, window, SHOP_ITEM_NOTE_X,
                                      8);
    } else {
      s32 unitSalePrice;
      s32 stackSalePrice;

      unitSalePrice = Shop_SalePrice(unit->inventory[slot]);
      stackSalePrice = quantity * unitSalePrice;

      UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window,
                                      SHOP_USE_PRICE_LABEL_X, 8);
      UiText_DrawNumberInWindowFar(stackSalePrice, 5, window,
                                   SHOP_USE_PRICE_NUMBER_X, 8);
      UiText_DrawCharacterAtOffsetFar(MsgPriceHeading -
                                          SHOP_USE_COIN_MESSAGE_OFFSET,
                                      window, SHOP_USE_COIN_LABEL_X, 8);
    }
  }
}

#undef SHOP_USE_PRICE_LABEL_X
#undef SHOP_USE_PRICE_NUMBER_X
#undef SHOP_USE_COIN_LABEL_X
#undef SHOP_USE_COIN_MESSAGE_OFFSET
