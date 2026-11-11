/// @file
/// The forge's counter, and the item picker it and events use to choose one
/// of a party member's items.
// Part of shop.c.

#include "../shop_internal.h"

extern u8 MsgRightWhenI[];

void Func_08108148(void);
void Func_0810824c(void);

void Func_081084f4(s32 message);
s32 Func_0810b6bc(void);

s32 Func_0810b5c4(s32 objectId) {
  struct ShopRuntime *shop;
  struct WindowSprite *icon;
  void *window;
  s32 message;
  s32 result;

  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  shop->shopType = 1;
  shop->partyAction = 4;
  shop->keeperResource = *((struct FieldObject *)Actor_GetByIdFar(objectId))->sprite->portrait;
  window = Func_080380f8(shop->keeperResource, 0, 0, 0);
  icon = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  icon->sentinel = 255;
  icon->active = 1;
  icon->kind = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  message = (s32)MsgRightWhenI;
  Func_081084f4(message);
  Func_081084f4(message + 1);
  result = Func_0810b6bc();
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  if (result == -1)
    Func_081084f4(message + 2);
  Window_CloseFar(window, 2);
  Func_0810824c();
  return 0;
}

s32 Func_08109ad8(void);

s32 Func_0810b6bc(void) { return Func_08109ad8(); }

s32 Func_08109cac(s32 unitId);

s32 Func_0810b6c4(s32 unitId) { return Func_08109cac(unitId); }

s32 Forge_GetMaterialChance(s32 item);
s32 Forge_RollResult(s32 item);
void Forge_SetItem(s32 item);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void MessageArgument_PushFar(s32 value, s32 slot);
s32 Inventory_RemoveFar(s32 unitId, s32 slot);
void Shop_DrawItemCount(void *window, s32 unitId);

/// The forge's offer for one inventory item: refuses an item that is no
/// material, otherwise asks (a rare material, chance 1, takes its own lines),
/// and on yes takes the item, redraws the count and stores the rolled result.
/// Returns 0 when the item was taken, else -1.
s32 Func_0810b6cc(s32 unitId, s32 slot) {
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
    Func_0810857c((s32)MsgNoThatSNoGood);
  } else {
    MessageArgument_PushFar(item, 2);
    Func_0810857c(chance == 1 ? (s32)MsgItSOkIfIWork
                              : (s32)MsgYouWantItToShine);
    if (Func_08108630(0) != 0) {
      Func_0810857c(chance == 1 ? (s32)MsgHmThatSNotRight
                                : (s32)MsgWellHmIWonder);
    } else {
      Func_0810857c(chance == 1 ? (s32)MsgWellAllRightIM
                                : (s32)MsgALittleElbowGrease);
      Inventory_RemoveFar(unitId, slot);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      item = Forge_RollResult(item);
      Forge_SetItem(item);
      result = 0;
    }
  }
  return result;
}

s32 Func_0810b7b4(s32 *outUnit, s32 *outSlot, s32 itemMode);

void Func_0810b79c(s32 a, s32 b) { Func_0810b7b4(a, b, 1); }

void Func_0810b7a8(s32 a, s32 b) { Func_0810b7b4(a, b, 0); }

// Func_0810b7b4 picks a party member and then one of their items, four members
// a page, returning the unit and slot through the pointers (both -1 when
// cancelled): the twin of The Broken Seal's Shop_PickUnitItem.

void Func_08108148(void);
void Func_0810824c(void);

void Func_080f8060(void *window, s32 a, s32 b);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void wait(s32 frames);

void Shop_DrawItemCount(void *window, s32 unitId);
s32 Inventory_CountFar(s32 unitId);
void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedrawFar(void *window);
s32 Func_0810b9c0(s32 unitId);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);
void Menu_ReleaseEntryObjectsFar(void);

s32 Func_0810b7b4(s32 *outUnit, s32 *outSlot, s32 itemMode) {
  struct ShopRuntime *shop;
  s32 unitId = 0;
  s32 redraw = 1;
  s32 memberIndex = 0;
  void *listWindow;
  void *iconWindow;
  struct WindowSprite *icon;
  s32 slot;
  s32 result;

  Func_08108148();
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
      Shop_PlaceCursor(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Func_0810928c(listWindow, memberIndex, 0);
      Shop_DrawItemCount(shop->itemWindow, unitId);
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
      slot = Func_0810b9c0(unitId);
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
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
    }
  }
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(iconWindow, 2);
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  wait(1);
  Func_0810824c();
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

// Unnamed in the catalog yet: 4676 and 4675 take placeholders here.
extern u8 MsgCannotRemoveIt[];
extern u8 MsgCannotDropIt[];
extern u8 MsgItemPlainName[];

s32 Inventory_CountFar(s32 unitId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_0810bca0(void *window, s32 unitId, s32 slot);

void wait(s32 frames);
s32 Func_080ad268(s32 unitId, s32 slot);
s32 Func_080ad270(s32 unitId, s32 slot);
void Sound_PlayFar(s32 cue);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 Message_AllTypedFar(void);
void Message_CloseFinishedWindowsFar(void);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);

void WindowSprite_PrepareForRedrawFar(void *window);
void Shop_DrawItemCount(void *window, s32 unitId);

s32 Func_0810b9c0(s32 unitId) {
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
      Shop_PlaceCursor(window, x * 16, y * 16 + 8);
      shop->mode = 3;
      Func_0810bca0(priceWindow, unitId, slot);
      Shop_DrawMsg(nameWindow, itemId + (s32)MsgItemPlainName);
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
      Shop_DrawItemCount(shop->itemWindow, unitId);
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

extern u8 MsgItemName[];
extern u8 MsgCannotDrop[];
extern u8 MsgCannotRemove[];
extern u8 MsgPriceHeading[];

void WindowSprite_RedrawSavedRectFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

s32 Func_080ad268(s32 unitId, s32 slot);
s32 Func_080ad270(s32 unitId, s32 slot);
s32 Shop_GetSellPrice(s32 item);

/// Draws the selected item's panel in the sell and drop lists: its name, then
/// why it cannot be dropped or removed, or, while selling, what the whole
/// stack would sell for. The Broken Seal's Shop_DrawUseItem, with this game's
/// check chosen by the list's mode. Called by Func_0810b9c0 each time the
/// cursor moves.
void Func_0810bca0(void *window, s32 unitId, s32 slot) {
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
  Text_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, 0, 0);
  if (shop->itemMode == 0)
    flags = Func_080ad268(unitId, slot);
  else
    flags = Func_080ad270(unitId, slot);
  if (flags & 2) {
    Text_DrawCharacterAtOffsetFar((s32)MsgCannotDrop, window, SHOP_PRICE_NOTE_X,
                                  8);
  } else if (flags & 1) {
    Text_DrawCharacterAtOffsetFar((s32)MsgCannotRemove, window,
                                  SHOP_PRICE_NOTE_X, 8);
  } else if (shop->itemMode == 0) {
    price = Shop_GetSellPrice(unit->inventory[slot]) * quantity;
    message = (s32)MsgPriceHeading;
    Text_DrawCharacterAtOffsetFar(message, window, 8, 8);
    Text_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
    // Five messages before the price heading: MsgCoins.
    Text_DrawCharacterAtOffsetFar(message - 5, window, SHOP_PRICE_COINS_X, 8);
  }
}

#undef SHOP_PRICE_COINS_X
#undef SHOP_PRICE_COINS_X
#undef SHOP_PRICE_COINS_X

void Func_0810bdb0(void) {
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

void Func_0810bdf4(void) {
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
  Task_Add(Func_0810bdb0, TASK_PASS_VBLANK + 10);
}

void Func_0810be3c(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  Task_Remove(Func_0810bdb0);
  for (i = 0; i < ICON_COUNT; i++) {
    if (work->resources[i] != ICON_NONE)
      Resource_ResetEntry(work->resources[i]);
  }
}

void Func_0810be70(s32 index, s32 x, s32 y) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->sprites[index].attr.x = x;
  work->sprites[index].attr.y = y;
}

void Func_0810bea8(s32 active) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->active = active;
}

/// Releases the item icons' VRAM entries, then loads the icons of up to count
/// items (an empty item keeps its old entry) and gives each its sprite
/// attributes and tile.
void Func_0810bebc(u16 *items, s32 count, s32 mode) {
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

void Func_0810bf98(s16 *window, s32 x, s32 y, s32 owner) {
  u8 *unit = Unit_Get(owner);
  s32 left = window[6] * 8 + x + 8;
  s32 top = window[7] * 8 + y + 8;
  s32 i;

  Func_0810bebc((u16 *)(unit + 216), ICON_COUNT, 27);
  for (i = 0; i < ICON_COUNT; i++)
    Func_0810be70(i, left + i % 5 * 16, top + i / 5 * 16);
  Func_0810bea8(1);
}
