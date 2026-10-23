/// @file
/// Running a shop, its cursor work, and the message helpers every counter uses.
// Part of shop.c.

#include "../shop_internal.h"

/// Lists the current shop's artifacts: every item of the shop's type whose
/// shared stock counter is above zero. Writes them to the work block's stock
/// list and returns how many. Called by Shop_Run when the player picks
/// Artifacts, and by Shop_SelectBuy to refresh the list after a purchase.
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
  // 9-bit id is tried, past the 324 rows of items.tsv too.
  do {
    equipmentGroup = shop->shopType;
    if (equipmentGroup == Item_GetEquipmentGroupFar(itemId) &&
        Item_AdjustCounterFar(itemId, 0) != 0) {
      *stockItems = itemId;
      count++;
      stockItems++;
    }
    itemId++;
  } while (itemId <= INVENTORY_ITEM_ID_MASK);
  shop->stockItemIds[count] = 0;
  shop->stockCount = count;
  return count;
}

/// Advances the hand cursor one frame. Runs every frame as a task
/// while a counter is open.
void MenuCursor_Step(void) {
  MenuCursor_Advance(&((struct ShopWork *)gMenuWork)->cursor);
}

/// Allocates and clears the work block in the menu's heap slot, lists the
/// party, loads the six cursor icons into the VRAM cache and starts the
/// cursor's task. Every counter opens with it: Shop_Run, the inn, the
/// sanctum and the item picker. The block, the icons' cache entries and the
/// task last until Shop_ReleaseWork; as HEAP_SLOT_MENU is the field
/// menu's slot too, a counter and the field menu cannot be open together.
void Shop_InitializeWork(void) {
  struct ShopWork *shop;
  s32 resourceSlot;

  shop = AllocHeapSlot(HEAP_SLOT_MENU, sizeof(struct ShopWork));
  Field_EndBusyAndRevealFar();
  Dma_Fill32(0, shop, sizeof(struct ShopWork));
  // The hand cursor's glides take 12 frames until a screen changes it
  shop->mode = 12;
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
  Task_Add(MenuCursor_Step, TASK_PRIORITY_MAIN_LOOP);
}

/// Stops the cursor task, frees the six cursor icons and the work block.
/// Every counter closes with it.
void Shop_ReleaseWork(void) {
  struct ShopWork *state;

  state = gMenuWork;
  Task_Remove(MenuCursor_Step);
  TextPrinter_CloseFinishedWindowsFar();
  VramCache_FreeEntry(state->cursorIcon);
  VramCache_FreeEntry(state->previousPageIcon);
  VramCache_FreeEntry(state->nextPageIcon);
  VramCache_FreeEntry(state->gemIcon);
  VramCache_FreeEntry(state->statUpIcon);
  VramCache_FreeEntry(state->statDownIcon);
  FreeHeapSlot(HEAP_SLOT_MENU);
}

/// Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
/// until the player leaves. `stockRow` picks the row of sStockTable
/// (SHOP_STOCK_ROW_EMPTY for a row out of range) and `keeperId` the object
/// whose portrait the messages show. Seeds the row's artifacts on the first
/// visit and holds the work block while it runs. Called by the town maps'
/// shopkeepers through SHOP.S, and by the debug shop. Returns 0.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopWork *shop;
  struct WindowSprite *anchor;
  struct Window *window;
  s32 action = 0;

  if (stockRow >= ShopStock_GetRowCount() || stockRow < 0)
    stockRow = SHOP_STOCK_ROW_EMPTY;
  // Seed this location's artifact stock before creating the shop UI.
  ShopStock_AddArtifacts(stockRow);
  Shop_InitializeWork();
  shop = gMenuWork;
  shop->shopType = ShopStock_GetType(stockRow);
  // Xian's shops are the warriors' shops, which greet differently.
  if (stockRow == SHOP_STOCK_ROW_XIAN_WEAPONS)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ARMOR)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ITEMS)
    shop->warriorShop = 1;
  shop->keeperGraphics =
      ((struct Sprite *)((struct Object *)ObjectEvent_GetByIdFar(keeperId))
           ->animation)
          ->entries[0]
          ->animationId;
  // The shopkeeper's portrait window, or an empty one off the left edge when
  // there is no portrait.
  window = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (window == 0)
    window = Window_CreateFar(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
  anchor = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  anchor->motion = WINDOW_SPRITE_MOTION_STILL;
  anchor->kind = 0;
  // The hand waits off the left edge until a screen places it
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  Shop_ShowKeeperMessage(MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->action = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = ShopStock_CopyItems(stockRow, shop->stockItemIds);
      Shop_ShowKeeperMessage(MsgWhatWouldYouLike);
      Shop_SelectBuy();
    } else if (action == SHOP_ACTION_SELL) {
      Shop_ShowKeeperMessage(MsgWhatToSell);
      Shop_ChooseMember();
    } else if (action == SHOP_ACTION_ARTIFACTS) {
      if (Shop_ListAvailableArtifacts() != 0) {
        Shop_ShowKeeperMessage(MsgConnoisseur);
        Shop_SelectBuy();
      } else {
        Shop_ShowKeeperMessage(MsgOutOfStock);
        WaitFrames(1);
      }
    } else if (action == SHOP_ACTION_REPAIR) {
      Shop_ShowKeeperMessage(MsgFixDamaged);
      Shop_RunRepairMemberChoice();
    } else {
      break;
    }
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Shop_ShowKeeperMessage(MsgAnythingElse);
  }
  Shop_ShowKeeperMessage(MsgShopFarewell);
  Window_CloseFar(window, 2);
  Shop_ReleaseWork();
  return 0;
}

// The debug shop sells Vale's weapons outside Japan and its armor in Japan.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SHOP_DEBUG_STOCK_ROW SHOP_STOCK_ROW_VALE_WEAPONS
#else
#define SHOP_DEBUG_STOCK_ROW SHOP_STOCK_ROW_VALE_ARMOR
#endif

/// Opens the debug shop with 200000 coins, the ticket counter at its cap,
/// broken equipment for repairs and poisoned or haunted members for the
/// sanctum. Called by DebugMenu_RunPresetShop.
s32 Shop_OpenDebugShop(s32 unused) {
  gGameState.coins = 200000;
  gGameState.shopGifts = 28;
  Inventory_EquipFar(
      GARET,
      Inventory_AddItemFar(GARET, INVENTORY_BROKEN | ITEM_VIRTUOUS_ARMLET));
  Inventory_EquipFar(
      ISAAC, Inventory_AddItemFar(ISAAC, INVENTORY_BROKEN | ITEM_MURAMASA));
  Inventory_AddItemFar(IVAN, ITEM_BONE);
  Unit_GetFar(MIA)->poison = 1;
  Unit_GetFar(JENNA)->poison = 1;
  Unit_GetFar(IVAN)->haunt = 1;
  Shop_Run(SHOP_DEBUG_STOCK_ROW, 0x1e);
  return 0;
}

/// Waits one frame at a time until the sound driver has finished the last
/// command. Called by the inn's sleep, the sanctum and two scripts that wait
/// for a fanfare.
void Sound_WaitForFanfare(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    WaitFrames(1);
  }
}

/// Byte view of the six colour writes in a quantity-bar segment, laid over a
/// 4-bit tile at the segment's byte column (QuantityBar_Fill, cursor.c).
struct SelectorRecord {
  u8 filler0[4]; ///< the 4 bytes before the segment's byte in row 1, untouched
  /// Rows 1 to 6: index 4n is the segment's byte in row n + 1.
  u8 values[21];
};

/// Shows a shopkeeper's line and waits for its window to finish, one more
/// frame included. `messageId` is the weapon shop's line; the armor, item and
/// warriors' shops say theirs, at the same distance from their own welcome.
/// Called by every counter.
void Shop_ShowKeeperMessage(s32 messageId) {
  // A shared message-resolution inline added four bytes to each dialogue caller
  // in the English native build; both callers retain their direct resolution.
  struct ShopWork *shop = gMenuWork;
  s32 portraitResource = Speaker_GetVoiceFar(shop->keeperGraphics);
  s32 resolvedMessage = messageId;
  s8 shopType;

  TextPrinter_CloseFinishedWindowsFar();
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_ARMOR)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ITEMS)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  // A warriors' shop adds its distance on top of its type's
  if ((s8)shop->warriorShop != 0)
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
  Message_OpenWindowFar(resolvedMessage, 5, 0, (portraitResource << 16) | 0x22);
  while (TextPrinter_AllFinishedFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

/// Shows a shopkeeper's line as Shop_ShowKeeperMessage does, hiding the hand
/// cursor while it shows and putting back its motion after.
void Shop_ShowKeeperMessageHidingCursor(s32 messageId) {
  s32 portraitResource;
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
  portraitResource = Speaker_GetVoiceFar(shop->keeperGraphics);
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
  TextPrinter_CloseFinishedWindowsFar();
  Message_OpenWindowFar(resolvedMessage, 5, 0,
                        (portraitResource << 0x10) | 0x22);
  while (TextPrinter_AllFinishedFar() == 0) {
    WaitFrames(1);
  }
  WaitFrames(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

/// Runs the Yes or No choice beside the shopkeeper's line and returns it: 0
/// for Yes, 1 for No or B. `choice` is where the cursor starts. Keeps the
/// hand cursor's motion. Called by the counters before they buy, sell, equip
/// or repair, and by the inn.
s32 Shop_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopWork *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareObjectFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

/// Runs the Yes or No choice at the sanctum's place on the screen and
/// returns it, keeping the hand cursor's motion. Called by
/// Sanctum_RunPartyService.
s32 Sanctum_AskYesNo(s32 choice) {
  struct WindowSprite **cursorSlot =
      &((struct ShopWork *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->motion;
  Icon_PrepareObjectFar(*cursorSlot);
  choice = YesNoMenu_RunAtFar(7, 7, choice);
  (*cursorSlot)->motion = savedCursorMode;
  return choice;
}

/// Returns the node `state->count` next links on from `state->node`. Nothing
/// calls it.
UNUSED struct NodeChainNode *
NodeChain_GetNodeAtIndex(struct NodeChainState *state) {
  struct NodeChainNode *node = state->node;
  s32 linkIndex;

  for (linkIndex = 0; linkIndex != state->count; ++linkIndex) {
    node = node->next;
  }
  return node;
}

#undef SHOP_DEBUG_STOCK_ROW
