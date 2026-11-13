/// @file
/// The forge's counter, and the item picker it and events use to choose one
/// of a party member's items.
// Part of shop.c.

#include "../shop_internal.h"
#include "module/shop/shop.h"

/// Runs the forge: the smith's line, then a party member's materials to
/// forge, until the player leaves. `objectId` is the actor whose portrait the
/// messages show. Called through the far-call table; returns 0.
/// ★ Named from the game: "Sunshine's smithy" (message 4879)
s32 Forge_Run(s32 objectId) {
  struct ShopRuntime *shop;
  struct WindowSprite *icon;
  void *window;
  s32 message;
  s32 result;

  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuRuntime;
  shop->shopType = 1;
  shop->partyAction = 4;
  shop->keeperResource = *((struct FieldObject *)Actor_GetByIdFar__shop(objectId))->sprite->portrait;
  window = Func_080380f8(shop->keeperResource, 0, 0, 0);
  icon = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  icon->sentinel = 255;
  icon->active = 1;
  icon->kind = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  message = (s32)MsgRightWhenI;
  Shop_ShowKeeperMessage(message);
  Shop_ShowKeeperMessage(message + 1);
  result = Forge_ChooseMember();
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  if (result == -1)
    Shop_ShowKeeperMessage(message + 2);
  Window_CloseFar(window, 2);
  Shop_ReleaseWork();
  return 0;
}

/// Runs the forge's party member choice, the sell counter's.
s32 Forge_ChooseMember(void) { return Shop_ChooseMember(); }

/// Lets the player choose the party member's item to forge, on the sell
/// counter's grid.
s32 Forge_SelectItem(s32 unitId) { return Shop_SelectSell(unitId); }

/// The forge's offer for one inventory item: refuses an item that is no
/// material, otherwise asks (a rare material, chance 1, takes its own lines),
/// and on yes takes the item, redraws the count and stores the rolled result.
/// Returns 0 when the item was taken, else -1.
s32 Forge_OfferItem(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  u32 raw = unit->inventory[slot];
  s32 item;
  s32 chance;
  /* FAKEMATCH(pin): result held in r8; as a plain local, 16 of 78
   * instructions differ in tla-en. */
  register s32 result asm("r8");

  item = 0x1ff;
  item &= raw;
  chance = Forge_GetMaterialChance(item);
  result = -1;

  if (Forge_GetMaterialChance(item) == 0) {
    Shop_ShowKeeperMessageHidingCursor((s32)MsgNoThatSNoGood);
  } else {
    MessageArgument_PushFar(item, 2);
    Shop_ShowKeeperMessageHidingCursor(chance == 1 ? (s32)MsgItSOkIfIWork
                              : (s32)MsgYouWantItToShine);
    if (Shop_AskYesNo(0) != 0) {
      Shop_ShowKeeperMessageHidingCursor(chance == 1 ? (s32)MsgHmThatSNotRight
                                : (s32)MsgWellHmIWonder);
    } else {
      Shop_ShowKeeperMessageHidingCursor(chance == 1 ? (s32)MsgWellAllRightIM
                                : (s32)MsgALittleElbowGrease);
      Inventory_RemoveFar(unitId, slot);
      Shop_DrawMemberItems(shop->itemWindow, unitId);
      item = Forge_RollResult(item);
      Forge_SetItem(item);
      result = 0;
    }
  }
  return result;
}

// XXX: what the item picker's mode 1 chooses an item for; the callers through
// the far-call table and Func_080ad270 would settle it.
void Func_0810b79c(s32 a, s32 b) { ItemPicker_Run(a, b, 1); }

/// Runs the item picker for an item to drop, showing each item's sale price.
void ItemPicker_RunToDrop(s32 a, s32 b) { ItemPicker_Run(a, b, 0); }

// ItemPicker_Run picks a party member and then one of their items, four members
// a page, returning the unit and slot through the pointers (both -1 when
// cancelled): the twin of The Broken Seal's Shop_PickUnitItem.

/// ItemPicker_Run picks a party member and then one of their items, four members
/// a page, returning the unit and slot through the pointers (both -1 when
/// cancelled): the twin of The Broken Seal's Shop_PickUnitItem.
s32 ItemPicker_Run(s32 *outUnit, s32 *outSlot, s32 itemMode) {
  struct ShopRuntime *shop;
  s32 unitId = 0;
  s32 redraw = 1;
  s32 memberIndex = 0;
  void *listWindow;
  void *iconWindow;
  struct WindowSprite *icon;
  s32 slot;
  s32 result;

  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuRuntime;
  shop->itemMode = itemMode;
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, 2);
  listWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->listWindow = listWindow;
  iconWindow = Window_CreateFar(30, 0, 1, 1, 2);
  result = 0;
  icon = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000,
                                (struct RenderInput *)iconWindow, 0, result);
  icon->active = 4;
  icon->kind = result;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  shop->mode = 1;
  Func_080f8060(listWindow, 2, 0);
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Shop_DrawParty(listWindow, memberIndex, 0);
      Shop_DrawMemberItems(shop->itemWindow, unitId);
      redraw = 0;
    }
    wait(1);
    if (gInput.pressed & 1) {
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(113);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      Sound_PlayFar(112);
      slot = ItemPicker_SelectItem(unitId);
      if (slot == -1) {
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        redraw = 2;
        continue;
      }
      *outUnit = unitId;
      *outSlot = slot;
      result = 0;
      break;
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      *outUnit = -1;
      *outSlot = -1;
      result = -1;
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount, 4);
    }
  }
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(iconWindow, 2);
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  wait(1);
  Shop_ReleaseWork();
  return result;
}

// The price window and the item details window fit their edition's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PRICE_WINDOW_X 16
#define PRICE_WINDOW_WIDTH 14
#elif LANGUAGE == LANGUAGE_GERMAN
#define PRICE_WINDOW_X 13
#define PRICE_WINDOW_WIDTH 17
#else
#define PRICE_WINDOW_X 14
#define PRICE_WINDOW_WIDTH 16
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define DETAILS_WINDOW_WIDTH 13
#elif LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_FRENCH
#define DETAILS_WINDOW_WIDTH 16
#else
#define DETAILS_WINDOW_WIDTH 18
#endif

s32 ItemPicker_SelectItem(s32 unitId) {
  void *nameWindow;
  struct ShopRuntime *shop;
  void *priceWindow;
  struct ShopUnit *unit;
  s32 slot;
  s32 inventoryCount;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 flags;
  void *window;
  s32 x;
  s32 y;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  inventoryCount = 1;
  redraw = 1;
  priceWindow = Window_CreateFar(PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4, 2);
  nameWindow = Window_CreateFar(0, 5, 30, 3, 2);

  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  slot = 0;
  while (1) {
    if (redraw != 0) {
      redraw = 0;
      inventoryCount = Inventory_CountFar(unitId);
      if (slot > inventoryCount - 1)
        slot = inventoryCount - 1;
      itemId = 0x1ff & unit->inventory[slot];
      window = shop->itemWindow;
      x = slot % 5;
      y = slot / 5;
      MenuCursor_Place(window, x * 16, y * 16 + 8);
      shop->mode = 3;
      ItemPicker_DrawItem(priceWindow, unitId, slot);
      Shop_DrawMessage(nameWindow, itemId + (s32)MsgItemPlainName);
    }
    wait(1);
    if (gInput.pressed & 1) {
      wait(1);
      if (shop->itemMode == 0)
        flags = Func_080ad268(unitId, slot);
      else
        flags = Func_080ad270(unitId, slot);
      if (flags == 0) {
        Sound_PlayFar(112);
        result = slot;
        break;
      }
      if (flags & 1)
        Message_OpenWindowFar((s32)MsgCannotRemoveIt, 8, 1, 0);
      else if (flags & 2)
        Message_OpenWindowFar((s32)MsgCannotDropIt, 8, 1, 0);
      Sound_PlayFar(113);
      while (Message_AllTypedFar() == 0)
        wait(1);
      Message_CloseFinishedWindowsFar();
      continue;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (gInput.held & 4) {
      u32 item = unit->inventory[slot];
      void *details;

      Sound_PlayFar(126);
      details = Window_CreateFar(0, 9, DETAILS_WINDOW_WIDTH, 10, 2);
      ItemMenu_DrawItemDetailsFar(details, item);
      shop->detailsHidden = 0;
      /* FAKEMATCH(barrier): an empty asm after the detailsHidden store, so the
       * input's address is reloaded from the stack after it; without it, 4
       * of 321 instructions differ in tla-en. */
      asm("");
      while (gInput.held & 4)
        wait(1);
      shop->detailsHidden = 1;
      Window_CloseFar(details, 2);
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
    if (gInput.repeat & 32) {
      Sound_PlayFar(111);
      slot--;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if (gInput.repeat & 16) {
      Sound_PlayFar(111);
      slot++;
      slot = (slot + inventoryCount) % inventoryCount;
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
  }
  Window_CloseFar(nameWindow, 2);
  Window_CloseFar(priceWindow, 2);
  wait(1);
  return result;
}

#undef DETAILS_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH

// The note sits under the item's name; the price line reads "Price", the
// price, then "Coins". Japanese indents the note by 8 pixels; Japanese and
// French draw the price and the coins label 8 pixels further left, Spanish
// the coins label 8 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_PRICE_NOTE_X 8
#else
#define SHOP_PRICE_NOTE_X 0
#endif
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_NUMBER_X 32
#else
#define SHOP_PRICE_NUMBER_X 40
#endif
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_COINS_X 72
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_PRICE_COINS_X 88
#else
#define SHOP_PRICE_COINS_X 80
#endif

/// Draws the selected item's panel in the sell and drop lists: its name, then
/// why it cannot be dropped or removed, or, while selling, what the whole
/// stack would sell for. The Broken Seal's Shop_DrawUseItem, with this game's
/// check chosen by the list's mode. Called by ItemPicker_SelectItem each time the
/// cursor moves.
void ItemPicker_DrawItem(void *window, s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  s32 item = unit->inventory[slot] & 0x1ff;
  u32 raw = unit->inventory[slot];
  s32 quantity = (raw >> 11) + 1;
  s32 flags;
  s32 price;
  s32 message;

  if (window == 0)
    return;
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawMessageAsTilesFar__shop(item + (s32)MsgItemName, window, 0, 0);
  if (shop->itemMode == 0)
    flags = Func_080ad268(unitId, slot);
  else
    flags = Func_080ad270(unitId, slot);
  if (flags & 2) {
    Text_DrawMessageAsTilesFar__shop((s32)MsgCannotDrop, window, SHOP_PRICE_NOTE_X,
                                  8);
  } else if (flags & 1) {
    Text_DrawMessageAsTilesFar__shop((s32)MsgCannotRemove, window,
                                  SHOP_PRICE_NOTE_X, 8);
  } else if (shop->itemMode == 0) {
    price = Shop_SalePrice(unit->inventory[slot]) * quantity;
    message = (s32)MsgPriceHeading;
    Text_DrawMessageAsTilesFar__shop(message, window, 8, 8);
    Text_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
    // Five messages before the price heading: MsgCoins.
    Text_DrawMessageAsTilesFar__shop(message - 5, window, SHOP_PRICE_COINS_X, 8);
  }
}

#undef SHOP_PRICE_COINS_X
#undef SHOP_PRICE_COINS_X
#undef SHOP_PRICE_COINS_X

/// The item icon grid: up to 15 item icons drawn as sprites over a window, as
/// a party member's inventory is shown at a counter. It lives in the shop's
/// work block from Shop_InitializeWork to Shop_ReleaseWork.
///
/// Queues each loaded icon's sprite while the grid is visible. Runs every
/// frame as a task.
void ItemIconGrid_Draw(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  if (work->active) {
    struct IconSprite *sprite = work->sprites;
    u16 *resource = work->resources;
    s32 i;

    for (i = ICON_COUNT - 1; i >= 0; i--) {
      if (*resource++ != ICON_NONE)
        Oam_AddSpriteAtDepth(sprite, 240);
      sprite++;
    }
  }
}

/// Empties the grid, hides it and starts its drawing task.
void ItemIconGrid_Initialize(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = ICON_COUNT - 1; i >= 0; i--)
    work->resources[i] = ICON_NONE;
  // The zero in a local of its own sets the registers of the loop before it;
  // as a plain 0, 28 of 29 instructions differ in tla-en.
  {
    s32 value = 0;
    work->active = value;
  }
  Task_Add(ItemIconGrid_Draw, TASK_PASS_VBLANK + 10);
}

/// Stops the drawing task and frees the icons' VRAM cache entries.
void ItemIconGrid_Release(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  Task_Remove(ItemIconGrid_Draw);
  for (i = 0; i < ICON_COUNT; i++) {
    if (work->resources[i] != ICON_NONE)
      Resource_ResetEntry(work->resources[i]);
  }
}

/// Places icon `index` at (x, y) screen pixels.
void ItemIconGrid_SetPosition(s32 index, s32 x, s32 y) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->sprites[index].attr.x = x;
  work->sprites[index].attr.y = y;
}

/// Shows the grid (1) or hides it (0).
void ItemIconGrid_SetVisible(s32 active) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->active = active;
}

/// Releases the item icons' VRAM entries, then loads the icons of up to count
/// items (an empty item keeps its old entry) and gives each its sprite
/// attributes and tile.
void ItemIconGrid_LoadItems(u16 *items, s32 count, s32 mode) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  struct IconSprite *sprite;
  s32 entry;
  s32 i;

  for (i = 0; i < ICON_COUNT; i++) {
    if (work->resources[i] != ICON_NONE) {
      Resource_ResetEntry(work->resources[i]);
      work->resources[i] = ICON_NONE;
    }
  }
  sprite = work->sprites;
  for (i = 0; i < ICON_COUNT && i != count; i++, sprite++) {
    if (items[i] != 0) {
      entry = Resource_FindFreeEntry();
      if (entry <= 95) {
        u32 *attr;

        Icon_CopyResourceToSlotFar(items[i], mode, entry);
        attr = (u32 *)&sprite->attr;
        *attr++ = 0x40000000;
        *attr = 0;
        sprite->attr.tile = gResourceTableEntries[entry].offset >> 5;
      }
      work->resources[i] = entry;
    }
  }
}

/// Shows a party member's 15 inventory icons at (x, y) pixels inside `window`,
/// five to a row, 16 pixels apart.
void ItemIconGrid_ShowInventory(s16 *window, s32 x, s32 y, s32 owner) {
  u8 *unit = Unit_Get(owner);
  s32 left = window[6] * 8 + x + 8;
  s32 top = window[7] * 8 + y + 8;
  s32 i;

  ItemIconGrid_LoadItems((u16 *)(unit + 216), ICON_COUNT, 27);
  for (i = 0; i < ICON_COUNT; i++)
    ItemIconGrid_SetPosition(i, left + i % 5 * 16, top + i / 5 * 16);
  ItemIconGrid_SetVisible(1);
}
