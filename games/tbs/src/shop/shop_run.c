/// @file
/// Running a shop, its cursor work, and the message helpers every counter uses.
// Part of shop.c.

#include "shop_internal.h"

/// Lists the current shop's artifacts: every item of the shop's type whose
/// shared stock counter is above zero.
s32 Shop_ListAvailableArtifacts(void) {
  struct ShopWork *shop;
  s16 *stockItems;
  s32 itemId;
  s32 count;
  s8 equipmentGroup;

  shop = gMenuWork;
  count = 0;
  itemId = 0;
  stockItems = shop->stockItemIds;
  // The shop's type is the equipment group it sells (enum ShopType). Every
  // 9-bit id is tried, past the 324 rows of item_table.c too.
  for (; itemId <= INVENTORY_ITEM_ID_MASK; itemId++) {
    equipmentGroup = shop->shopType;
    if (equipmentGroup == Item_GetEquipmentGroupFar(itemId) &&
        Item_AdjustCounterFar(itemId, 0) != 0) {
      *stockItems = itemId;
      count++;
      stockItems++;
    }
  }
  shop->stockItemIds[count] = 0;
  shop->stockCount = count;
  return count;
}

/// Advances the hand cursor one frame. Runs every frame as a task
/// while a counter is open.
void MenuCursor_Step(void) {
  MenuCursor_Advance(&((struct ShopWork *)gMenuWork)->cursor);
}

/// Opens a counter's work: the party list, the cursor icons and the cursor's
/// task, which last until Shop_ReleaseWork. Every counter opens with it.
void Shop_InitializeWork(void) {
  struct ShopWork *shop;
  s32 resourceSlot;

  shop = HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ShopWork));
  Field_EndBusyAndRevealFar();
  Dma_Fill32(0, shop, sizeof(struct ShopWork));
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  shop->partyMemberCount = Party_ListMembersFar(shop->partyMemberIds);
  // Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite
  resourceSlot = VramCache_FindFreeEntry();
  shop->cursorIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gMenuCursor_HandTiles);
  resourceSlot = VramCache_FindFreeEntry();
  shop->previousPageIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_UpArrowTiles);
  resourceSlot = VramCache_FindFreeEntry();
  shop->nextPageIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_DownArrowTiles);
  resourceSlot = VramCache_FindFreeEntry();
  shop->gemIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_GemTiles);
  resourceSlot = VramCache_FindFreeEntry();
  shop->statDownIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_SmallDownArrowTiles);
  resourceSlot = VramCache_FindFreeEntry();
  shop->statUpIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_SmallUpArrowTiles);
  Task_Add(MenuCursor_Step, TASK_PASS_FRAME);
}

/// Stops the cursor task, frees the six cursor icons and the work block.
/// Every counter closes with it.
void Shop_ReleaseWork(void) {
  struct ShopWork *state;

  state = gMenuWork;
  Task_Remove(MenuCursor_Step);
  Message_CloseFinishedWindowsFar();
  VramCache_FreeEntry(state->cursorIcon);
  VramCache_FreeEntry(state->previousPageIcon);
  VramCache_FreeEntry(state->nextPageIcon);
  VramCache_FreeEntry(state->gemIcon);
  VramCache_FreeEntry(state->statUpIcon);
  VramCache_FreeEntry(state->statDownIcon);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
/// until the player leaves. keeperId is the actor whose portrait the lines
/// show.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopWork *shop;
  struct WindowSprite *anchor;
  struct Window *window;
  s32 action = 0;

  if (stockRow >= ShopStock_GetRowCount() || stockRow < 0)
    stockRow = SHOP_STOCK_ROW_EMPTY;
  // A shop's artifacts join the world's stock the first time it opens.
  ShopStock_AddArtifacts(stockRow);
  Shop_InitializeWork();
  shop = gMenuWork;
  shop->shopType = ShopStock_GetType(stockRow);
  // Xian's shops are the warriors' shops, which greet differently.
  // XXX (no evidence): why by row number rather than a column of the row; the
  // stock row's s16 type column could have carried it.
  if (stockRow == SHOP_STOCK_ROW_XIAN_WEAPONS)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ARMOR)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ITEMS)
    shop->warriorShop = 1;
  shop->keeperGraphics =
      (Actor_GetByIdFar(keeperId))->sprite->entries[0]->animationId;
  // The shopkeeper's portrait window, or an empty one off the left edge when
  // there is no portrait.
  window = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (window == 0)
    window = Window_CreateFar(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
  anchor =
      WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16, window, 0, 0);
  anchor->motion = WINDOW_SPRITE_MOTION_STILL;
  // The hand's icon is the shop's, freed by Shop_ReleaseWork, not the sprite's.
  anchor->kind = WINDOW_SPRITE_GLYPH;
  // The hand waits off the left edge until a screen places it
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  // The lines are quoted as a weapon shop words them; "Come in! Welcome! Take
  // a look at our fine weapons."
  Shop_ShowKeeperMessage(MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->action = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = ShopStock_CopyItems(stockRow, shop->stockItemIds);
      // "What would you like?"
      Shop_ShowKeeperMessage(MsgWhatWouldYouLike);
      Shop_SelectBuy();
    } else if (action == SHOP_ACTION_SELL) {
      // "What do you want to sell?"
      Shop_ShowKeeperMessage(MsgWhatToSell);
      Shop_ChooseMember();
    } else if (action == SHOP_ACTION_ARTIFACTS) {
      if (Shop_ListAvailableArtifacts() != 0) {
        // "You must be a connoisseur if those caught your eye."
        Shop_ShowKeeperMessage(MsgConnoisseur);
        Shop_SelectBuy();
      } else {
        // "Sorry, but we're out of stock. Come back later, OK?"
        Shop_ShowKeeperMessage(MsgOutOfStock);
        wait(1);
      }
    } else if (action == SHOP_ACTION_REPAIR) {
      // "You should fix damaged weapons before they break."
      Shop_ShowKeeperMessage(MsgFixDamaged);
      Shop_RunRepairMemberChoice();
    } else {
      break;
    }
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    // "Anything else today?"
    Shop_ShowKeeperMessage(MsgAnythingElse);
  }
  // "Feel free to drop in anytime."
  Shop_ShowKeeperMessage(MsgShopFarewell);
  Window_CloseFar(window, 2);
  Shop_ReleaseWork();
  return 0;
}

/// The debug shop sells Vale's armor in Japanese and its weapons from patch 1.
#if TBS_PATCH_1
#define SHOP_DEBUG_STOCK_ROW SHOP_STOCK_ROW_VALE_WEAPONS
#else
#define SHOP_DEBUG_STOCK_ROW SHOP_STOCK_ROW_VALE_ARMOR
#endif

/// The coins the debug shop gives.
#define DEBUG_SHOP_COINS 200000
/// The actor whose sprite picks the debug shop keeper's portrait, in whatever
/// room it opens.
#define DEBUG_SHOP_KEEPER 30

/// Opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at its cap,
/// broken equipment for repairs and poisoned or haunted party members for the
/// sanctum.
s32 Shop_OpenDebugShop(s32 unused) {
  gMainRoutineWork.holdings.coins = DEBUG_SHOP_COINS;
  gMainRoutineWork.holdings.shopGifts = SHOP_GIFTS_MAX;
  Inventory_EquipFar(
      GARET,
      Inventory_AddItemFar(GARET, INVENTORY_BROKEN | ITEM_VIRTUOUS_ARMLET));
  Inventory_EquipFar(
      ISAAC, Inventory_AddItemFar(ISAAC, INVENTORY_BROKEN | ITEM_MURAMASA));
  Inventory_AddItemFar(IVAN, ITEM_BONE);
  Unit_GetFar(MIA)->poison = POISON_NORMAL;
  Unit_GetFar(JENNA)->poison = POISON_NORMAL;
  Unit_GetFar(IVAN)->haunt = 1;
  Shop_Run(SHOP_DEBUG_STOCK_ROW, DEBUG_SHOP_KEEPER);
  return 0;
}

/// Waits a frame at a time while a jingle holds back the music, as the inn's
/// does through the night.
void Sound_WaitForJingle(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    wait(1);
  }
}

/// Shows a shopkeeper's line and waits until it is typed out. The line is given
/// as a weapon shop's: an armor or item shop says its own, the same distance
/// on in the text, and a warriors' shop its own beyond that.
void Shop_ShowKeeperMessage(s32 messageId) {
  struct ShopWork *shop = gMenuWork;
  s32 voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  s32 resolvedMessage = messageId;
  s8 shopType;

  Message_CloseFinishedWindowsFar();
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_ARMOR)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ITEMS)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  // A warriors' shop adds its distance on top of its type's
  if ((s8)shop->warriorShop != 0)
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
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
  s8 warriorShop;
  u8 savedCursorMode;
  struct ShopWork *shop;
  struct WindowSprite **cursorSlot;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->motion;
  resolvedMessage = messageId;
  voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_ARMOR) {
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  }
  if (shopType == SHOP_TYPE_ITEMS) {
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  }
  warriorShop = shop->warriorShop;
  if (warriorShop != 0) {
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
  }
  (*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(resolvedMessage, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0) {
    wait(1);
  }
  wait(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

/// Runs the Yes or No choice beside the shopkeeper's line: 0 for Yes, 1 for No
/// or B.
s32 Shop_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopWork *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareActorFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

/// Runs the Yes or No choice at the sanctum's place on the screen and returns
/// it, keeping the hand cursor's motion.
s32 Sanctum_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopWork *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareActorFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(7, 7, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

/// Returns the node `state->count` next links on from `state->node`. Nothing
/// calls it.
UNUSED struct SelectionNodeLinkView *
NodeChain_GetNodeAtIndex(struct SelectionMenuCursorView *state) {
  struct SelectionNodeLinkView *node = state->node;
  s32 linkIndex;

  for (linkIndex = 0; linkIndex != state->count; ++linkIndex) {
    node = node->next;
  }
  return node;
}

#undef SHOP_DEBUG_STOCK_ROW
#undef DEBUG_SHOP_COINS
#undef DEBUG_SHOP_KEEPER
