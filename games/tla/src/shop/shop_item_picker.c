/// @file
/// The forge's counter, and the item picker it and events use to choose one
/// of a party member's items.
// Part of shop.c.

#include "shop_internal.h"
#include "sounds.h"
#include "shop/shop.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/game.h"

/// Runs the forge: the smith's line, then a party member's materials to
/// forge, until the player leaves. `actorId` is the actor whose portrait the
/// messages show. Called through the far-call table; returns 0.
s32 Forge_Run(s32 actorId) {
  struct ShopState *shop;
  struct WindowSprite *icon;
  struct Window *window;
  s32 messageBase;
  s32 result;

  Shop_InitializeState();
  shop = Ram_HeapSlots->menuState;
  shop->shopType = SHOP_TYPE_WEAPONS;
  shop->action = SHOP_ACTION_FORGE;
  shop->keeperGraphics = ((struct Sprite *)Actor_GetByIdFar(actorId)->sprite)
                             ->entries[0]
                             ->animationId;
  window = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  icon = WindowSprite_CreateFar(shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16,
                                window, 0, 0);
  icon->depth = OAM_DEPTH_FRONT;
  icon->motion = WINDOW_SPRITE_MOTION_STILL;
  icon->kind = WINDOW_SPRITE_PLAIN;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  messageBase = (s32)MsgRightWhenI;
  // "Right when I saw those materials, I had to work!"
  Shop_ShowKeeperMessage(messageBase);
  // "Which item needs my special attention?"
  Shop_ShowKeeperMessage(messageBase + 1);
  result = Forge_ChooseMember();
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  if (result == -1)
    // "Aren't you the ones mucking about earlier?"
    Shop_ShowKeeperMessage(messageBase + 2);
  Window_CloseFar(window, 2);
  Shop_ReleaseState();
  return 0;
}

/// Runs the forge's party member choice, the sell counter's.
s32 Forge_ChooseMember(void) { return Shop_ChooseMember(); }

/// Lets the player choose the party member's item to forge, on the sell
/// counter's grid.
s32 Forge_SelectItem(s32 unitId) { return Shop_SelectSell(unitId); }

/// The forge's offer for one inventory item: refuses an item it has no recipe
/// for, otherwise asks (a rusted weapon takes its own lines), and on yes takes
/// the item, redraws the grid and stores the rolled result. Returns 0 when the
/// item was taken, else -1.
s32 Forge_OfferItem(s32 unitId, s32 slot) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct Unit *unit = Unit_Get(unitId);
  u32 inventoryEntry = unit->inventory[slot];
  s32 itemId;
  s32 condition;
  s32 result;

  itemId = INVENTORY_ITEM_ID_MASK;
  itemId &= inventoryEntry;
  condition = Forge_GetItemCondition(itemId);
  result = -1;

  if (Forge_GetItemCondition(itemId) == 0) {
    Shop_ShowKeeperMessageHidingCursor((s32)MsgNoThatSNoGood);
    return result;
  }
  MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
  Shop_ShowKeeperMessageHidingCursor(condition == FORGE_CONDITION_PROCESSED + 1
                                         ? (s32)MsgItSOkIfIWork
                                         : (s32)MsgYouWantItToShine);
  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessageHidingCursor(
        condition == FORGE_CONDITION_PROCESSED + 1 ? (s32)MsgHmThatSNotRight
                                                   : (s32)MsgWellHmIWonder);
  } else {
    Shop_ShowKeeperMessageHidingCursor(condition ==
                                               FORGE_CONDITION_PROCESSED + 1
                                           ? (s32)MsgWellAllRightIM
                                           : (s32)MsgALittleElbowGrease);
    Inventory_RemoveFar(unitId, slot);
    Shop_DrawMemberItems(shop->itemWindow, unitId);
    itemId = Forge_RollResult(itemId);
    Forge_SetItem(itemId);
    result = 0;
  }
  return result;
}

/// Runs the item picker for an item to show, refusing one that cannot be
/// unequipped: Naribwe's fortune teller reads the party's fortune from it.
s32 ItemPicker_RunToShow(s32 *selectedUnit, s32 *selectedSlot) {
  return ItemPicker_Run(selectedUnit, selectedSlot, ITEM_PICKER_SHOW);
}

/// Runs the item picker for an item to drop, showing each item's sale price.
s32 ItemPicker_RunToDrop(s32 *selectedUnit, s32 *selectedSlot) {
  return ItemPicker_Run(selectedUnit, selectedSlot, ITEM_PICKER_DROP);
}

/// Lets the player choose a party member, then one of its items, four party
/// members a page, returning the unit and slot through the pointers (both -1
/// when cancelled). `itemMode` is what the item is for (enum ItemPickerMode).
/// Holds the shop's state while it runs.
s32 ItemPicker_Run(s32 *selectedUnit, s32 *selectedSlot, s32 itemMode) {
  struct ShopState *shop;
  s32 unitId = 0;
  s32 redraw = 1;
  s32 memberIndex = 0;
  struct Window *partyWindow;
  struct Window *cursorWindow;
  struct WindowSprite *cursorAnchor;
  s32 itemSlot;
  s32 result = 0;

  Shop_InitializeState();
  shop = Ram_HeapSlots->menuState;
  shop->itemMode = itemMode;
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
  partyWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  shop->listWindow = partyWindow;
  cursorWindow = Window_CreateFar(30, 0, 1, 1, WINDOW_FLAG_INSTANT);
  cursorAnchor = WindowSprite_CreateFar(
      shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, cursorWindow, 0, 0);
  cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  cursorAnchor->kind = WINDOW_SPRITE_PLAIN;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  shop->cursorFrames = 1;
  Menu_ShowPartyFiguresFar(partyWindow, 2, 0);
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(partyWindow,
                       (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
      shop->cursorFrames = 3;
      if (redraw == MENU_GRID_CHANGED_PAGE) {
        Menu_CreatePartyFiguresFar(memberIndex / SHOP_PARTY_PAGE_MEMBERS *
                                   SHOP_PARTY_PAGE_MEMBERS);
        wait(1);
      }
      Shop_DrawParty(partyWindow, memberIndex, 0);
      Shop_DrawMemberItems(shop->itemWindow, unitId);
      redraw = 0;
    }
    wait(1);
    if (gInput.pressed & KEY_A) {
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(partyWindow);
      Sound_PlayFar(SE_MENU_CONFIRM);
      itemSlot = ItemPicker_SelectItem(unitId);
      if (itemSlot == -1) {
        shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
        shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
        redraw = MENU_GRID_CHANGED_PAGE;
        continue;
      }
      *selectedUnit = unitId;
      *selectedSlot = itemSlot;
      result = 0;
      break;
    } else if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      *selectedUnit = -1;
      *selectedSlot = -1;
      result = -1;
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount,
                                   SHOP_PARTY_PAGE_MEMBERS);
    }
  }
  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(cursorWindow, 2);
  Window_CloseFar(partyWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  wait(1);
  Shop_ReleaseState();
  return result;
}

/// The price window and the item details window fit their edition's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_PICKER_PRICE_WINDOW_X 16
#define ITEM_PICKER_PRICE_WINDOW_WIDTH 14
#elif LANGUAGE == LANGUAGE_GERMAN
#define ITEM_PICKER_PRICE_WINDOW_X 13
#define ITEM_PICKER_PRICE_WINDOW_WIDTH 17
#else
#define ITEM_PICKER_PRICE_WINDOW_X 14
#define ITEM_PICKER_PRICE_WINDOW_WIDTH 16
#endif

/// The item details window's width, in tiles, fitted to its edition's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_PICKER_DETAILS_WINDOW_WIDTH 13
#elif LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_FRENCH
#define ITEM_PICKER_DETAILS_WINDOW_WIDTH 16
#else
#define ITEM_PICKER_DETAILS_WINDOW_WIDTH 18
#endif

/// Lets the player choose one of the party member's items, refusing with a
/// message one that cannot be dropped or removed (Inventory_CheckDiscard or
/// Inventory_CheckUnequip, by the picker's mode). Returns the slot, or -1 when
/// cancelled.
s32 ItemPicker_SelectItem(s32 unitId) {
  struct Window *descriptionWindow;
  struct ShopState *shop;
  struct Window *priceWindow;
  struct Unit *unit;
  s32 slot;
  s32 inventoryCount;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 refusal;
  struct Window *window;
  s32 column;
  s32 row;

  shop = Ram_HeapSlots->menuState;
  unit = Unit_Get(unitId);
  inventoryCount = 1;
  redraw = 1;
  priceWindow =
      Window_CreateFar(ITEM_PICKER_PRICE_WINDOW_X, 8,
                       ITEM_PICKER_PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
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
      itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
      window = shop->itemWindow;
      column = slot % SHOP_INVENTORY_COLUMNS;
      row = slot / SHOP_INVENTORY_COLUMNS;
      MenuCursor_Place(window, column * 16, row * 16 + 8);
      shop->cursorFrames = 3;
      ItemPicker_DrawItem(priceWindow, unitId, slot);
      Shop_DrawMessage(descriptionWindow, itemId + (s32)MsgItemDescription);
    }
    wait(1);
    if (gInput.pressed & KEY_A) {
      wait(1);
      if (shop->itemMode == ITEM_PICKER_DROP)
        refusal = Inventory_CheckDiscardFar(unitId, slot);
      else
        refusal = Inventory_CheckUnequipFar(unitId, slot);
      if (refusal == 0) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = slot;
        break;
      }
      if (refusal & INVENTORY_REFUSE_CANNOT_UNEQUIP)
        Message_OpenWindowFar((s32)MsgCannotRemoveIt, 8, 1, 0);
      else if (refusal & INVENTORY_REFUSE_INDISPENSABLE)
        Message_OpenWindowFar((s32)MsgCannotDropIt, 8, 1, 0);
      Sound_PlayFar(SE_MENU_CANCEL);
      while (Message_AllTypedFar() == 0)
        wait(1);
      Message_CloseFinishedWindowsFar();
      continue;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gInput.held & KEY_SELECT) {
      u32 inventoryEntry = unit->inventory[slot];
      struct Window *detailsWindow;

      Sound_PlayFar(SE_RECOVERY);
      detailsWindow = Window_CreateFar(0, 9, ITEM_PICKER_DETAILS_WINDOW_WIDTH,
                                       10, WINDOW_FLAG_INSTANT);
      ItemMenu_DrawItemDetailsFar(detailsWindow, inventoryEntry);
      shop->memberSpritesShown = 0;
      while (gInput.held & KEY_SELECT)
        wait(1);
      shop->memberSpritesShown = 1;
      Window_CloseFar(detailsWindow, 2);
      WindowSprite_PrepareForRedrawFar(shop->listWindow);
      wait(1);
      // German, Spanish and Italian also redraw the item window here.
#if TLA_PATCH_4 || (TLA_PATCH_2 && !TLA_PATCH_3)
      Shop_DrawMemberItems(shop->itemWindow, unitId);
      wait(1);
#endif
      // Japanese does not redraw the cursor or the two windows here.
#if TLA_PATCH_1
      redraw = 1;
#endif
      continue;
    }
    if (gInput.repeat & KEY_LEFT) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      slot--;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if (gInput.repeat & KEY_RIGHT) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      slot++;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if (gInput.repeat & KEY_UP) {
      slot -= SHOP_INVENTORY_COLUMNS;
      if (slot < 0)
        slot += INVENTORY_SLOTS;
      while (slot >= inventoryCount)
        slot -= SHOP_INVENTORY_COLUMNS;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
    if (gInput.repeat & KEY_DOWN) {
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
  return result;
}

#undef ITEM_PICKER_PRICE_WINDOW_X
#undef ITEM_PICKER_PRICE_WINDOW_WIDTH
#undef ITEM_PICKER_DETAILS_WINDOW_WIDTH

/// The note sits under the item's name; the price line reads "Price", the
/// price, then "Coins". Japanese indents the note by 8 pixels; Japanese and
/// French draw the price and the coins label 8 pixels further left, Spanish
/// the coins label 8 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_PRICE_NOTE_X 8
#else
#define SHOP_PRICE_NOTE_X 0
#endif
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_FRENCH
#define ITEM_PICKER_PRICE_NUMBER_X 32
#else
#define ITEM_PICKER_PRICE_NUMBER_X 40
#endif
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_COINS_X 72
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_PRICE_COINS_X 88
#else
#define SHOP_PRICE_COINS_X 80
#endif

/// Draws the slot's item name and, below it, why it cannot be dropped or
/// removed, or, for an item to drop, what the whole stack would sell for.
/// Called by ItemPicker_SelectItem each time the cursor moves.
void ItemPicker_DrawItem(struct Window *window, s32 unitId, s32 slot) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct Unit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
  u32 inventoryEntry = unit->inventory[slot];
  s32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;
  s32 refusal;
  s32 price;
  s32 labelMessage;

  if (window == 0)
    return;
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawMessageAsTilesFar(itemId + (s32)MsgItemName, window, 0, 0);
  if (shop->itemMode == ITEM_PICKER_DROP)
    refusal = Inventory_CheckDiscardFar(unitId, slot);
  else
    refusal = Inventory_CheckUnequipFar(unitId, slot);
  if (refusal & INVENTORY_REFUSE_INDISPENSABLE) {
    Text_DrawMessageAsTilesFar((s32)MsgCannotDrop, window, SHOP_PRICE_NOTE_X,
                               8);
  } else if (refusal & INVENTORY_REFUSE_CANNOT_UNEQUIP) {
    Text_DrawMessageAsTilesFar((s32)MsgCannotRemove, window, SHOP_PRICE_NOTE_X,
                               8);
  } else if (shop->itemMode == ITEM_PICKER_DROP) {
    price = Shop_SalePrice(unit->inventory[slot]) * quantity;
    labelMessage = (s32)MsgPriceHeading;
    Text_DrawMessageAsTilesFar(labelMessage, window, 8, 8);
    Text_DrawNumberInWindowFar(price, 5, (s32)window,
                               ITEM_PICKER_PRICE_NUMBER_X, 8);
    // Five messages before the price heading: MsgCoins.
    Text_DrawMessageAsTilesFar(labelMessage - 5, window, SHOP_PRICE_COINS_X, 8);
  }
}

#undef SHOP_PRICE_NOTE_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef SHOP_PRICE_COINS_X

/// Queues each loaded icon's sprite while the grid is visible. Runs every
/// frame as a task.
void ItemIconGrid_Draw(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;

  if (shop->iconsVisible) {
    struct IconSprite *sprite = shop->iconSprites;
    u16 *iconSlot = shop->iconSlots;
    s32 i;

    for (i = 0; i < INVENTORY_SLOTS; i++) {
      if (*iconSlot++ != ICON_NONE)
        Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW);
      sprite++;
    }
  }
}

/// Empties the grid, hides it and starts its drawing task.
void ItemIconGrid_Initialize(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 i;

  for (i = 0; i < INVENTORY_SLOTS; i++)
    shop->iconSlots[i] = ICON_NONE;
  {
    s32 hidden = 0;
    shop->iconsVisible = hidden;
  }
  Task_Add(ItemIconGrid_Draw, TASK_PASS_FRAME + 10);
}

/// Stops the drawing task and frees the icons' VRAM cache entries.
void ItemIconGrid_Release(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 i;

  Task_Remove(ItemIconGrid_Draw);
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (shop->iconSlots[i] != ICON_NONE)
      VramCache_FreeEntry(shop->iconSlots[i]);
  }
}

/// Places icon `index` at (x, y) screen pixels.
void ItemIconGrid_SetPosition(s32 index, s32 x, s32 y) {
  struct ShopState *shop = Ram_HeapSlots->menuState;

  shop->iconSprites[index].attr.x = x;
  shop->iconSprites[index].attr.y = y;
}

/// Shows the grid (1) or hides it (0).
void ItemIconGrid_SetVisible(s32 visible) {
  struct ShopState *shop = Ram_HeapSlots->menuState;

  shop->iconsVisible = visible;
}

/// Releases the item icons' VRAM entries, then loads the icons of up to count
/// items with the ICON_ layers in `mode` (an empty item keeps its old entry)
/// and gives each its sprite attributes and tile.
void ItemIconGrid_LoadItems(u16 *items, s32 count, s32 mode) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  struct IconSprite *sprite;
  s32 entry;
  s32 i;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (shop->iconSlots[i] != ICON_NONE) {
      VramCache_FreeEntry(shop->iconSlots[i]);
      shop->iconSlots[i] = ICON_NONE;
    }
  }
  sprite = shop->iconSprites;
  for (i = 0; i < INVENTORY_SLOTS && i != count; i++, sprite++) {
    if (items[i] != 0) {
      entry = VramCache_FindFreeEntry();
      if (entry < VRAM_CACHE_ENTRY_COUNT) {
        u32 *attr;

        ItemIcon_LoadLayersIntoSlotFar(items[i], mode, entry);
        attr = (u32 *)&sprite->attr;
        *attr++ = WINDOW_SPRITE_SIZE_16X16;
        *attr = 0;
        sprite->attr.tile = gVramCacheEntries[entry].offset >> 5;
      }
      shop->iconSlots[i] = entry;
    }
  }
}

/// Shows a party member's INVENTORY_SLOTS inventory icons at (x, y) pixels
/// inside `window`, SHOP_INVENTORY_COLUMNS to a row, 16 pixels apart.
void ItemIconGrid_ShowInventory(struct Window *window, s32 x, s32 y,
                                s32 unitId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 left = window->x * 8 + x + 8;
  s32 top = window->y * 8 + y + 8;
  s32 i;

  ItemIconGrid_LoadItems(unit->inventory, INVENTORY_SLOTS,
                         ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN |
                             ICON_EQUIPPED);
  for (i = 0; i < INVENTORY_SLOTS; i++)
    ItemIconGrid_SetPosition(i, left + i % SHOP_INVENTORY_COLUMNS * 16,
                             top + i / SHOP_INVENTORY_COLUMNS * 16);
  ItemIconGrid_SetVisible(1);
}
