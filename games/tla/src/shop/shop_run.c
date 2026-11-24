/// @file
/// Running a shop: the artifacts it lists, opening its state, the counter's
/// loop and the message helpers every counter uses.
// Part of shop.c.

#include "shop_internal.h"
#include "resource_ids.h"
#include "item_ids.h"
#include "sounds.h"
#include "gamepak/far_call/window.h"
#include "shop/shop.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/field_sequence.h"
#include "window/cursor.h"

/// Lists the current shop's artifacts: every item of the shop's type whose
/// shared stock counter is above zero, or of any type at a general store.
/// Writes them to the shop state's stock list and returns how many. Called by
/// Shop_Run when the player picks Artifacts, and by Shop_SelectBuy to refresh
/// the list after a purchase.
s32 Shop_ListAvailableArtifacts(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 count = 0;
  s32 itemId;

  // Every 9-bit id is tried.
  for (itemId = 0; itemId <= INVENTORY_ITEM_ID_MASK; itemId++) {
    s32 shopType = shop->shopType;

    if (shopType != SHOP_TYPE_GENERAL_STORE) {
      if (shopType != SHOP_TYPE_EVERY_GROUP) {
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
  MenuCursor_Advance(&((struct ShopState *)Ram_HeapSlots->menuState)->cursor);
}

/// Allocates and clears the shop's state in the menu's heap slot, lists the
/// party, loads the six cursor icons into the VRAM cache, starts the cursor's
/// task and readies the item icon grid. Every counter opens with it: Shop_Run,
/// the inn, the sanctum, the forge and the item picker. The block, the icons'
/// cache entries and the task last until Shop_ReleaseState.
void Shop_InitializeState(void) {
  struct ShopState *shop;
  s32 resourceSlot;

  shop = HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ShopState));
  Event_ResolveAllPendingActionsFar();
  Dma_Fill32(0, shop, sizeof(struct ShopState));
  // The hand cursor's glides take SHOP_CURSOR_ENTER_FRAMES until a screen
  // changes it.
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  shop->partyMemberCount = Party_ListMembersFar(shop->partyMemberIds);
  Palette_LoadIconsIntoHighlightFar();
  // Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite
  resourceSlot = VramCache_FindFreeEntry();
  shop->cursorIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128, RESOURCE_UI_ICON_TILES);
  resourceSlot = VramCache_FindFreeEntry();
  shop->previousPageIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128, RESOURCE_SHOP_UP_ARROW_TILES);
  resourceSlot = VramCache_FindFreeEntry();
  shop->nextPageIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128, RESOURCE_SHOP_DOWN_ARROW_TILES);
  resourceSlot = VramCache_FindFreeEntry();
  shop->gemIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128, RESOURCE_SHOP_GEM_TILES);
  resourceSlot = VramCache_FindFreeEntry();
  shop->statDownIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128,
                         RESOURCE_MENU_SMALL_DOWN_ARROW_TILES);
  resourceSlot = VramCache_FindFreeEntry();
  shop->statUpIcon = resourceSlot;
  VramBlock_LoadResource(resourceSlot, 128, RESOURCE_MENU_ARROW_TILES);
  Task_Add(MenuCursor_Step, TASK_PASS_FRAME);
  ItemIconGrid_Initialize();
}

/// Releases the item icon grid, stops the cursor task, frees the six cursor
/// icons and the shop's state. Every counter closes with it.
void Shop_ReleaseState(void) {
  struct ShopState *shop;

  shop = Ram_HeapSlots->menuState;
  ItemIconGrid_Release();
  Task_Remove(MenuCursor_Step);
  Message_CloseFinishedWindowsFar();
  VramCache_FreeEntry(shop->cursorIcon);
  VramCache_FreeEntry(shop->previousPageIcon);
  VramCache_FreeEntry(shop->nextPageIcon);
  VramCache_FreeEntry(shop->gemIcon);
  VramCache_FreeEntry(shop->statUpIcon);
  VramCache_FreeEntry(shop->statDownIcon);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
/// until the player leaves. `stockRow` picks the row of the stock table (the
/// empty row 0 for one out of range) and `keeperId` the actor whose portrait
/// the messages show. Seeds the row's artifacts on the first visit and holds
/// the shop's state while it runs. Called by the town scenes' shopkeepers
/// through the far-call table, and by the debug shop. Returns 0.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopState *shop;
  struct WindowSprite *anchor;
  struct Window *window;
  s32 action = 0;

  if (stockRow >= ShopStock_GetRowCount() || stockRow < 0)
    stockRow = SHOP_STOCK_ROW_EMPTY;
  ShopStock_AddArtifacts(stockRow);
  Shop_InitializeState();
  shop = Ram_HeapSlots->menuState;
  shop->shopType = ShopStock_GetType(stockRow);
  shop->keeperGraphics = ((struct Sprite *)Actor_GetByIdFar(keeperId)->sprite)
                             ->entries[0]
                             ->animationId;
  // The shopkeeper's portrait window, or an empty one off the right edge when
  // there is no portrait.
  window = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (window == 0)
    window = Window_CreateFar(30, 0, 2, 2, WINDOW_FLAG_INSTANT);
  anchor =
      WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16, window, 0, 0);
  anchor->depth = OAM_DEPTH_FRONT;
  anchor->motion = WINDOW_SPRITE_MOTION_STILL;
  anchor->kind = 0;
  // The hand waits off the left edge until a screen places it
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  Shop_ShowKeeperMessage((s32)MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->action = action;
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
  Shop_ReleaseState();
  return 0;
}

/// The coins the debug shop gives.
#define DEBUG_SHOP_COINS 200000
/// The actor whose sprite picks the debug shop keeper's portrait, in whatever
/// room it opens.
#define DEBUG_SHOP_KEEPER 30

/// Unused: opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at
/// its cap, broken equipment for repairs and poisoned or haunted party members
/// for the sanctum, on the empty stock row. Nothing calls it or its far-call
/// export; The Broken Seal's debug menu opened its own.
s32 Shop_OpenDebugShop(void) {
  gMainRoutineState.coins = DEBUG_SHOP_COINS;
  gMainRoutineState.shopGifts = SHOP_GIFTS_MAX;
  Inventory_EquipFar(
      GARET,
      Inventory_AddItemFar(GARET, INVENTORY_BROKEN | ITEM_VIRTUOUS_ARMLET));
  Inventory_EquipFar(
      ISAAC, Inventory_AddItemFar(ISAAC, INVENTORY_BROKEN | ITEM_MURAMASA));
  Inventory_AddItemFar(IVAN, ITEM_BONE);
  Unit_Get(MIA)->poison = POISON_NORMAL;
  Unit_Get(JENNA)->poison = POISON_NORMAL;
  Unit_Get(IVAN)->haunt = 1;
  Shop_Run(SHOP_STOCK_ROW_EMPTY, DEBUG_SHOP_KEEPER);
  return 0;
}

/// Waits until the jingle playing has ended and the music it interrupted is
/// back: the inn's night, the sanctum's healing and events that wait for a
/// jingle.
void Sound_WaitForJingle(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    wait(1);
  }
}

/// Shows the shopkeeper's line `messageId` and waits until it is typed out.
/// Lines are given as the weapon shop's: the armor shop, the item shop and the
/// general store each say their own version, as many messages on.
void Shop_ShowKeeperMessage(s32 messageId) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 resolvedMessage;
  s32 voice;
  s8 shopType;

  resolvedMessage = messageId;
  voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  Message_CloseFinishedWindowsFar();
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_GENERAL_STORE)
    resolvedMessage += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ARMOR)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ITEMS)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  Message_OpenWindowFar(resolvedMessage, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
}

/// Shows a shopkeeper's line as Shop_ShowKeeperMessage does, hiding the hand
/// cursor while it shows and putting back its motion after.
void Shop_ShowKeeperMessageHidingCursor(s32 messageId) {
  s32 voice;
  s32 resolvedMessage;
  s8 shopType;
  u8 savedCursorMode;
  struct ShopState *shop;
  struct WindowSprite **cursorSlot;

  shop = Ram_HeapSlots->menuState;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->motion;
  resolvedMessage = messageId;
  voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_GENERAL_STORE)
    resolvedMessage += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ARMOR)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ITEMS)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  (*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(resolvedMessage, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

/// The shop's Yes/No menu: one column further left in German, Spanish and
/// Italian.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define SHOP_CONFIRM_MENU_X 6
#else
#define SHOP_CONFIRM_MENU_X 7
#endif

/// Runs the Yes or No choice beside the shopkeeper's line and returns it: 0
/// for Yes, 1 for No or B. `choice` is where the cursor starts. Keeps the
/// hand cursor's motion. Called by the counters before they buy, sell, equip
/// or repair, and by the inn.
s32 Shop_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopState *)Ram_HeapSlots->menuState)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareActorFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

#undef SHOP_CONFIRM_MENU_X

/// Runs the Yes or No choice at the sanctum's place on the screen and
/// returns it, keeping the hand cursor's motion. Called by
/// Sanctum_RunPartyService.
s32 Sanctum_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopState *)Ram_HeapSlots->menuState)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareActorFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(7, 7, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

/// Moves a cursor over `count` entries laid out `columns` to a row by the
/// d-pad: left and right wrap round, up and down stop at the ends. Plays the
/// cursor sound when it moves. Returns how it moved (enum MenuGridMove), a page
/// being one row of `columns`. Called by every counter's lists.
s32 MenuGrid_MoveCursor(s32 *cursor, s32 count, s32 columns) {
  s32 changed = MENU_GRID_STAYED;
  s32 previous = *cursor;
  s32 position = previous;

  if (gInput.repeat & KEY_LEFT) {
    position--;
    position = (position + count) % count;
  }
  if (gInput.repeat & KEY_RIGHT) {
    position++;
    position = (position + count) % count;
  }
  if (gInput.repeat & KEY_UP) {
    if (position - columns >= 0)
      position -= columns;
  }
  if (gInput.repeat & KEY_DOWN) {
    if (position + columns < (columns + count - 1) / columns * columns)
      position += columns;
    if (position > count - 1)
      position = count - 1;
  }
  if (previous != position) {
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    changed = MENU_GRID_MOVED;
  }
  if (previous / columns * columns != position / columns * columns)
    changed = MENU_GRID_CHANGED_PAGE;
  *cursor = position;
  return changed;
}
