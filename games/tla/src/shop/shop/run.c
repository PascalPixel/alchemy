/// @file
/// Running a shop: the artifacts it lists, opening its work, the counter's
/// loop and its questions.
// Part of shop.c.

#include "../shop_internal.h"
#include "gamepak/far_call/window.h"
#include "module/shop/shop.h"
#include "module/tla.h"

/// Lists the current shop's artifacts: every item of the shop's type whose
/// shared stock counter is above zero, or of any type at a general store.
/// Writes them to the work block's stock list and returns how many. Called by
/// Shop_Run when the player picks Artifacts, and by Shop_SelectBuy to refresh
/// the list after a purchase.
s32 Shop_ListAvailableArtifacts(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 count = 0;
  s32 itemId;

  // Every 9-bit id is tried.
  for (itemId = 0; itemId <= ITEM_ID_MASK; itemId++) {
    s32 shopType = shop->shopType;

    if (shopType != SHOP_TYPE_GENERAL_STORE) {
      // XXX: no stock row has type 4; a row of the stock table or an event
      // passing it would settle what it was for.
      if (shopType != 4) {
        if (shopType != Item_GetEquipmentGroupFar(itemId))
          continue;
      }
    }
    if (Item_AdjustCounterFar(itemId, 0) != 0)
      shop->stockItemIds[count++] = itemId;
  }
  shop->stockItemIds[count] = 0;
  shop->stockCount = count;
  return count;
}

/// Advances the hand cursor one frame. Runs every frame as a task while a
/// counter is open.
void MenuCursor_Step(void) {
  MenuCursor_Advance(
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor);
}

/// Allocates and clears the work block in the menu's heap slot, lists the
/// party, loads the six cursor icons into the VRAM cache, starts the cursor's
/// task and readies the item icon grid. Every counter opens with it: Shop_Run,
/// the inn, the sanctum, the forge and the item picker. The block, the icons'
/// cache entries and the task last until Shop_ReleaseWork.
void Shop_InitializeWork(void) {
  struct ShopRuntime *work;
  s32 slot;

  work = HeapSlot_Alloc(220, 0xcec);
  Event_ResolveAllPendingActionsFar();
  Dma_Fill32(0, work, 0xcec);
  // The hand cursor's glides take 12 frames until a screen changes it
  work->mode = 12;
  work->partyMemberCount = Party_ListMembersFar(work->partyMemberIds);
  Palette_CopyObjectBankToBackground14Far();
  // Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite
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
  Task_Add(MenuCursor_Step, TASK_PASS_VBLANK);
  ItemIconGrid_Initialize();
}

/// Releases the item icon grid, stops the cursor task, frees the six cursor
/// icons and the work block. Every counter closes with it.
void Shop_ReleaseWork(void) {
  struct ShopRuntime *state;

  state = Ram_HeapSlots->menuRuntime;
  ItemIconGrid_Release();
  Task_Remove(MenuCursor_Step);
  Message_CloseFinishedWindowsFar();
  Resource_ResetEntry(state->cursorIcon);
  Resource_ResetEntry(state->previousPageIcon);
  Resource_ResetEntry(state->nextPageIcon);
  Resource_ResetEntry(state->gemIcon);
  Resource_ResetEntry(state->statUpIcon);
  Resource_ResetEntry(state->statDownIcon);
  HeapSlot_Release(220);
}

/// Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
/// until the player leaves. `stockRow` picks the row of the stock table (the
/// empty row 0 for one out of range) and `keeperId` the actor whose portrait
/// the messages show. Seeds the row's artifacts on the first visit and holds
/// the work block while it runs. Called by the town scenes' shopkeepers through
/// the far-call table, and by the debug shop. Returns 0.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopRuntime *shop;
  struct WindowSprite *anchor;
  struct RenderInput *window;
  s32 action = 0;

  if (stockRow >= ShopStock_GetRowCount() || stockRow < 0)
    stockRow = 0;
  // Seed this place's artifact stock before creating the shop UI.
  ShopStock_AddArtifacts(stockRow);
  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuRuntime;
  shop->shopType = ShopStock_GetType(stockRow);
  shop->keeperResource =
      **(u16 **)(*(u8 **)((u8 *)Actor_GetByIdFar__shop(keeperId) + 80) + 40);
  // The shopkeeper's portrait window, or an empty one off the right edge when
  // there is no portrait.
  window = Portrait_OpenWindowFar(shop->keeperResource, 0, 0, 0);
  if (window == 0)
    window = Window_CreateFar(30, 0, 2, 2, 2);
  anchor =
      WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16, window, 0, 0);
  anchor->sentinel = 255;
  anchor->active = 1;
  anchor->kind = 0;
  // The hand waits off the left edge until a screen places it
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  Shop_ShowKeeperMessage((s32)MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->partyAction = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = ShopStock_CopyItems(stockRow, shop->stockItemIds);
      Shop_ShowKeeperMessage((s32)MsgWhatWouldYouLike);
      Shop_SelectBuy();
    } else if (action == SHOP_ACTION_SELL) {
      Shop_ShowKeeperMessage((s32)MsgWhatToSell);
      Shop_ChooseMember();
    } else if (action == SHOP_ACTION_ARTIFACTS) {
      if (Shop_ListAvailableArtifacts() != 0) {
        Shop_ShowKeeperMessage((s32)MsgConnoisseur);
        Shop_SelectBuy();
      } else {
        Shop_ShowKeeperMessage((s32)MsgOutOfStock);
        wait(1);
      }
    } else if (action == SHOP_ACTION_REPAIR) {
      Shop_ShowKeeperMessage((s32)MsgFixDamaged);
      Shop_RunRepairMemberChoice();
    } else {
      break;
    }
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Shop_ShowKeeperMessage((s32)MsgAnythingElse);
  }
  Shop_ShowKeeperMessage((s32)MsgShopFarewell);
  Window_CloseFar(window, 2);
  Shop_ReleaseWork();
  return 0;
}

/// Opens the debug shop with 200000 coins, the ticket counter at its cap,
/// broken equipment for repairs and poisoned or haunted party members for the
/// sanctum. It opens the empty stock row 0.
s32 Shop_OpenDebugShop(void) {
  gMainRoutineWork.coins = 200000;
  gMainRoutineWork.counterC = 28;
  // A broken Virtuous Armlet (141), a broken Muramasa (11) and a Bone (231)
  Inventory_EquipFar__shop(GARET,
                           Inventory_AddItemFar(GARET, INVENTORY_BROKEN | 141));
  Inventory_EquipFar__shop(ISAAC,
                           Inventory_AddItemFar(ISAAC, INVENTORY_BROKEN | 11));
  Inventory_AddItemFar(IVAN, 231);
  ((struct BattleUnit *)Unit_Get(MIA))->poison = 1;
  ((struct BattleUnit *)Unit_Get(JENNA))->poison = 1;
  ((struct BattleUnit *)Unit_Get(IVAN))->haunt = 1;
  Shop_Run(0, 30);
  return 0;
}

/// Waits one frame at a time until the sound driver has finished the last
/// command. Called by the inn's sleep, the sanctum and events that wait for a
/// jingle.
void Sound_WaitForJingle(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    wait(1);
  }
}

void Shop_ShowKeeperMessage(s32 messageId) {
  struct ShopRuntime *state = Ram_HeapSlots->menuRuntime;
  s32 message;
  s32 value;
  s8 mode;

  message = messageId;
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
s32 Shop_AskYesNo(s32 choice) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor.anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  choice = YesNoMenu_RunAtFar(6, 5, choice);
#else
  choice = YesNoMenu_RunAtFar(7, 5, choice);
#endif
  (*slot)->active = saved;
  return choice;
}

/// Runs the Yes or No choice at the sanctum's place on the screen and
/// returns it, keeping the hand cursor's motion. Called by
/// Sanctum_RunPartyService.
s32 Sanctum_AskYesNo(s32 choice) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->cursor.anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
  choice = YesNoMenu_RunAtFar(7, 7, choice);
  (*slot)->active = saved;
  return choice;
}

/// Moves a cursor over `count` entries laid out `columns` to a row by the
/// d-pad: left and right wrap round, up and down stop at the ends. Plays the
/// cursor sound when it moves. Returns 0 when it stayed, 1 when it moved
/// within its row, 2 when it changed row. Called by every counter's lists.
s32 MenuGrid_MoveCursor(s32 *cursor, s32 count, s32 columns) {
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
