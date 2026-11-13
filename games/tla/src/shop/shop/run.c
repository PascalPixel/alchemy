/// @file
/// Running a shop: the artifacts it lists, opening its work, the counter's
/// loop and its questions.
// Part of shop.c.

#include "../shop_internal.h"

struct ShopItemList {
  u8 unk000[0x270];
  u16 items[0x149];
  u16 count;
  u8 unk504[2];
  s8 group;
};

s32 Func_081080a8(void) {
  struct ShopItemList *menu = Ram_HeapSlots->menuRuntime;
  s32 count = 0;
  s32 item;

  for (item = 0; item < 0x200; item++) {
    s32 group = menu->group;

    if (group != 3) {
      if (group != 4) {
        if (group != Item_GetEquipmentGroupFar(item))
          continue;
      }
    }
    if (Item_AdjustCounterFar(item, 0) != 0)
      menu->items[count++] = item;
  }
  menu->items[count] = 0;
  menu->count = count;
  return count;
}

extern char ResourceId_ShopGemTiles;
extern char ResourceId_MenuSmallDownArrowTiles;
extern char ResourceId_MenuArrowTiles;
extern char ResourceId_ShopUpArrowTiles;
extern char ResourceId_ShopDownArrowTiles;

void ShopCursor_Advance(struct ShopCursor *cursor);
void Event_ResolveAllPendingActionsFar(void);
void Palette_CopyObjectBankToBackground14Far(void);
void VramBlock_LoadResource(s32 slot, s32 size, s32 resource);
void Func_0810bdf4(void);
void Func_0810be3c(void);

void Func_08108130(void) {
  ShopCursor_Advance(
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor);
}

/// Opens the shop's work: lists the party, loads the cursor, page arrow, gem
/// and stat arrow tiles, and starts the cursor's callback.
// The Broken Seal's Shop_InitializeCursorWork.
void Shop_InitializeWork(void) {
  struct ShopRuntime *work;
  s32 slot;

  work = HeapSlot_Alloc(220, 0xcec);
  Event_ResolveAllPendingActionsFar();
  Dma_Fill32(0, work, 0xcec);
  work->mode = 12;
  work->partyMemberCount = Party_ListActiveOwnersFar(work->partyMemberIds);
  Palette_CopyObjectBankToBackground14Far();
  slot = Resource_FindFreeEntry();
  work->cursorIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_UiIconTiles);
  slot = Resource_FindFreeEntry();
  work->previousPageIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopUpArrowTiles);
  slot = Resource_FindFreeEntry();
  work->nextPageIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopDownArrowTiles);
  slot = Resource_FindFreeEntry();
  work->gemIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_ShopGemTiles);
  slot = Resource_FindFreeEntry();
  work->statDownIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuSmallDownArrowTiles);
  slot = Resource_FindFreeEntry();
  work->statUpIcon = slot;
  VramBlock_LoadResource(slot, 128, (s32)&ResourceId_MenuArrowTiles);
  Task_Add(Func_08108130, TASK_PASS_VBLANK);
  Func_0810bdf4();
}

/// Closes the shop: stops the cursor's callback, frees its tiles and the work.
// The Broken Seal's Shop_ReleaseCursorWork.
void Func_0810824c(void) {
  struct ShopRuntime *state;

  state = Ram_HeapSlots->menuRuntime;
  Func_0810be3c();
  Task_Remove(Func_08108130);
  Message_CloseFinishedWindowsFar();
  Resource_ResetEntry(state->cursorIcon);
  Resource_ResetEntry(state->previousPageIcon);
  Resource_ResetEntry(state->nextPageIcon);
  Resource_ResetEntry(state->gemIcon);
  Resource_ResetEntry(state->statUpIcon);
  Resource_ResetEntry(state->statDownIcon);
  HeapSlot_Release(220);
}

// Func_081082c4 runs the shop's buy, sell, artifacts and repair menu with the
// selected shopkeeper; the twin of The Broken Seal's Shop_Run
// (games/tbs/src/shop/shop.c), without its warrior shop rows. The messages take
// the twin's names.

s32 EventTable_GetRowLimit(void);
s32 EventTable_GetRowType(s32 row);
void EventTable_ApplyRowAbilities(s32 row);
struct RenderInput *Func_080380f8(s32 resource, s32 x, s32 y, s32 mode);
struct RenderInput *Window_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                     s32 style);
void MenuCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);
void Shop_ShowKeeperMessage(s32 message);
s32 Menu_SelectShopCommandFar(s32 action);
s32 ShopStock_CopyItems(s32 stockRow, s16 *output);
s32 Func_08108b70(void);
s32 Func_08109ad8(void);
s32 Func_0810a2c8(void);
void Window_CloseFar(struct RenderInput *window, s32 mode);

s32 Func_081082c4(s32 stockRow, s32 keeperId) {
  struct ShopRuntime *shop;
  struct WindowSprite *anchor;
  struct RenderInput *window;
  s32 action = 0;

  if (stockRow >= EventTable_GetRowLimit() || stockRow < 0)
    stockRow = 0;
  EventTable_ApplyRowAbilities(stockRow);
  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuRuntime;
  shop->shopType = EventTable_GetRowType(stockRow);
  shop->keeperResource =
      **(u16 **)(*(u8 **)((u8 *)Actor_GetByIdFar(keeperId) + 80) + 40);
  window = Func_080380f8(shop->keeperResource, 0, 0, 0);
  if (window == 0)
    window = Window_CreateFar(30, 0, 2, 2, 2);
  anchor =
      WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16, window, 0, 0);
  anchor->sentinel = 255;
  anchor->active = 1;
  anchor->kind = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  Shop_ShowKeeperMessage((s32)MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->partyAction = action;
    if (action == 0) {
      shop->stockCount = ShopStock_CopyItems(stockRow, shop->stockItemIds);
      Shop_ShowKeeperMessage((s32)MsgWhatWouldYouLike);
      Func_08108b70();
    } else if (action == 1) {
      Shop_ShowKeeperMessage((s32)MsgWhatToSell);
      Func_08109ad8();
    } else if (action == 2) {
      if (Func_081080a8() != 0) {
        Shop_ShowKeeperMessage((s32)MsgConnoisseur);
        Func_08108b70();
      } else {
        Shop_ShowKeeperMessage((s32)MsgOutOfStock);
        wait(1);
      }
    } else if (action == 3) {
      Shop_ShowKeeperMessage((s32)MsgFixDamaged);
      Func_0810a2c8();
    } else {
      break;
    }
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Shop_ShowKeeperMessage((s32)MsgAnythingElse);
  }
  Shop_ShowKeeperMessage((s32)MsgShopFarewell);
  Window_CloseFar(window, 2);
  Func_0810824c();
  return 0;
}

s32 Debug_ApplyPresetItemsAndFlags(void) {
  gMainRoutineWork.coins = 200000;
  gMainRoutineWork.counterC = 28;
  Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
  Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
  Inventory_AddItemFar(2, 0xe7);
  ((u8 *)Unit_Get(3))[0x131] = 1;
  ((u8 *)Unit_Get(5))[0x131] = 1;
  ((u8 *)Unit_Get(2))[0x140] = 1;
  Func_081082c4(0, 30);
  return 0;
}

void Sound_WaitForJingle(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    wait(1);
  }
}

s32 BattleEffect_GetResourceIdFar(u16 id);

void Shop_ShowKeeperMessage(s32 first) {
  struct ShopRuntime *state = Ram_HeapSlots->menuRuntime;
  s32 message;
  s32 value;
  s8 mode;

  message = first;
  value = BattleEffect_GetResourceIdFar(state->keeperResource);
  Message_CloseFinishedWindowsFar();
  mode = state->shopType;
  if (mode == 3)
    message += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (mode == 2)
    message += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (mode == 0)
    message += MsgItemShopWelcome - MsgWeaponShopWelcome;
  Message_OpenWindowFar(message, 5, 0, (value << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
}

void Shop_ShowKeeperMessageHidingCursor(s32 messageId) {
  s32 variant;
  s32 no;
  s8 mode;
  u8 saved;
  struct ShopRuntime *state;
  struct WindowSprite **slot;

  state = Ram_HeapSlots->menuRuntime;
  slot = &state->cursor.anchor;
  saved = (*slot)->active;
  no = messageId;
  variant = BattleEffect_GetResourceIdFar(state->keeperResource);
  mode = state->shopType;
  if (mode == 3)
    no += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (mode == 2)
    no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (mode == 0)
    no += MsgItemShopWelcome - MsgWeaponShopWelcome;
  (*slot)->active = 0xD;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(no, 5, 0, (variant << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  state->cursor.anchor->active = saved;
}

/// Runs the Yes or No choice beside the shopkeeper's line and returns it: 0
/// for Yes, 1 for No or B. `choice` is where the cursor starts. Keeps the
/// hand cursor's motion. Called by the counters before they buy, sell, equip
/// or repair, and by the inn.
s32 Shop_AskYesNo(s32 arg0) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor.anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
  arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
  (*slot)->active = saved;
  return arg0;
}

/// Runs the Yes or No choice at the sanctum's place on the screen and
/// returns it, keeping the hand cursor's motion. Called by
/// Sanctum_RunPartyService.
s32 Sanctum_AskYesNo(s32 arg0) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor.anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
  arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
  (*slot)->active = saved;
  return arg0;
}

s32 Func_08108690(s32 *cursor, s32 count, s32 columns) {
  s32 changed = 0;
  s32 old = *cursor;
  s32 pos = old;

  if (gInput.repeat & 0x20) {
    pos--;
    pos = (pos + count) % count;
  }
  if (gInput.repeat & 0x10) {
    pos++;
    pos = (pos + count) % count;
  }
  if (gInput.repeat & 0x40) {
    if (pos - columns >= 0)
      pos -= columns;
  }
  if (gInput.repeat & 0x80) {
    if (pos + columns < (columns + count - 1) / columns * columns)
      pos += columns;
    if (pos > count - 1)
      pos = count - 1;
  }
  if (old != pos) {
    Sound_PlayFar(111);
    changed = 1;
  }
  if (old / columns * columns != pos / columns * columns)
    changed = 2;
  *cursor = pos;
  return changed;
}
