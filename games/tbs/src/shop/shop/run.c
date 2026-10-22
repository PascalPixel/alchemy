/// @file
/// Running a shop, its cursor work, and the message helpers every counter uses.
// Part of shop.c.

#include "../shop_internal.h"

/// Lists the current shop's artifacts: every item of the shop's type whose
/// shared stock counter is above zero. Writes them to the work block's stock
/// list and returns how many. Called by Shop_Run when the player picks
/// Artifacts, and by Shop_SelectBuy to refresh the list after a purchase.
s32 Shop_ListAvailableArtifacts(void) {
  struct ShopRuntime *shop;
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
  } while (itemId <= ITEM_ID_MASK);
  shop->stockItemIds[count] = 0;
  shop->stockCount = count;
  return count;
}

/// Advances the hand cursor one frame. Runs every frame as a scheduler task
/// while a counter is open.
void Shop_StepCursor(void) {
  ShopCursor_Advance(&((struct ShopRuntime *)gMenuWork)->cursor);
}

/// Allocates and clears the work block in the menu's heap slot, lists the
/// party, loads the six cursor icons into the VRAM cache and starts the
/// cursor's task. Every counter opens with it: Shop_Run, the inn, the
/// sanctum and the item picker. The block, the icons' cache entries and the
/// task last until Shop_ReleaseCursorWork; as HEAP_SLOT_MENU is the field
/// menu's slot too, a counter and the field menu cannot be open together.
void Shop_InitializeCursorWork(void) {
  struct ShopRuntime *shop;
  s32 resourceSlot;

  shop = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, sizeof(struct ShopRuntime));
  Field_EndBusyAndRevealFar();
  Dma_Fill32(0, shop, sizeof(struct ShopRuntime));
  // The hand cursor's glides take 12 frames until a screen changes it
  shop->mode = 12;
  shop->partyMemberCount = Party_ListActiveOwnersFar(shop->partyMemberIds);
  // Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite
  resourceSlot = Resource_FindFreeEntry();
  shop->cursorIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_HandTiles);
  resourceSlot = Resource_FindFreeEntry();
  shop->previousPageIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_UpArrowTiles);
  resourceSlot = Resource_FindFreeEntry();
  shop->nextPageIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_DownArrowTiles);
  resourceSlot = Resource_FindFreeEntry();
  shop->gemIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_GemTiles);
  resourceSlot = Resource_FindFreeEntry();
  shop->statDownIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_SmallDownArrowTiles);
  resourceSlot = Resource_FindFreeEntry();
  shop->statUpIcon = resourceSlot;
  VramCache_Load(resourceSlot, 128, gShop_SmallUpArrowTiles);
  Scheduler_AddOrUpdateCallback(Shop_StepCursor, SCHEDULER_KEY_MAIN_LOOP);
}

/// Stops the cursor task, frees the six cursor icons and the work block.
/// Every counter closes with it.
void Shop_ReleaseCursorWork(void) {
  struct ShopRuntime *state;

  state = gMenuWork;
  Scheduler_RemoveCallback(Shop_StepCursor);
  UiWork_FinalizePendingCoreFar();
  Resource_ResetEntry(state->cursorIcon);
  Resource_ResetEntry(state->previousPageIcon);
  Resource_ResetEntry(state->nextPageIcon);
  Resource_ResetEntry(state->gemIcon);
  Resource_ResetEntry(state->statUpIcon);
  Resource_ResetEntry(state->statDownIcon);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
}

/// Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
/// until the player leaves. `stockRow` picks the row of sStockTable
/// (SHOP_STOCK_ROW_EMPTY for a row out of range) and `keeperId` the object
/// whose portrait the messages show. Seeds the row's artifacts on the first
/// visit and holds the work block while it runs. Called by the town scenes'
/// shopkeepers through SHOP.S, and by the debug shop. Returns 0.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopRuntime *shop;
  struct RenderOutput *anchor;
  struct UiWindow *window;
  s32 action = 0;

  if (stockRow >= ShopStock_GetRowCount() || stockRow < 0)
    stockRow = SHOP_STOCK_ROW_EMPTY;
  // Seed this location's artifact stock before creating the shop UI.
  ShopStock_AddArtifacts(stockRow);
  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->shopType = ShopStock_GetType(stockRow);
  // Xian's shops are the warriors' shops, which greet differently.
  if (stockRow == SHOP_STOCK_ROW_XIAN_WEAPONS)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ARMOR)
    shop->warriorShop = 1;
  if (stockRow == SHOP_STOCK_ROW_XIAN_ITEMS)
    shop->warriorShop = 1;
  shop->keeperResource =
      ((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(
                                      keeperId))
           ->animation)
          ->entries[0]
          ->animId;
  // The shopkeeper's portrait window, or an empty one off the left edge when
  // there is no portrait.
  window = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
  if (window == 0)
    window = UiWindow_CreateFar(-5, 0, 5, 5, UI_WINDOW_INSTANT);
  anchor = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  anchor->active = UI_SPRITE_STILL;
  anchor->kind = 0;
  // The hand waits off the left edge until a screen places it
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  UiMessage_ShowAndWait(MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->partyAction = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = ShopStock_CopyItems(stockRow, shop->stockItemIds);
      UiMessage_ShowAndWait(MsgWhatWouldYouLike);
      Shop_SelectBuy();
    } else if (action == SHOP_ACTION_SELL) {
      UiMessage_ShowAndWait(MsgWhatToSell);
      Shop_PickUnit();
    } else if (action == SHOP_ACTION_ARTIFACTS) {
      if (Shop_ListAvailableArtifacts() != 0) {
        UiMessage_ShowAndWait(MsgConnoisseur);
        Shop_SelectBuy();
      } else {
        UiMessage_ShowAndWait(MsgOutOfStock);
        WaitFrames(1);
      }
    } else if (action == SHOP_ACTION_REPAIR) {
      UiMessage_ShowAndWait(MsgFixDamaged);
      Shop_SelectUnit();
    } else {
      break;
    }
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    UiMessage_ShowAndWait(MsgAnythingElse);
  }
  UiMessage_ShowAndWait(MsgShopFarewell);
  UiWork_FinalizeFar(window, 2);
  Shop_ReleaseCursorWork();
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
  UnitGetFar(MIA)->poison = 1;
  UnitGetFar(JENNA)->poison = 1;
  UnitGetFar(IVAN)->haunt = 1;
  Shop_Run(SHOP_DEBUG_STOCK_ROW, 0x1e);
  return 0;
}

/// Waits one frame at a time until the sound driver has finished the last
/// command. Called by the inn's sleep, the sanctum and two scenes that wait
/// for a fanfare.
void AudioCommand_WaitForStateByteClear(void) {
  while (AudioCommand_GetStateByteFar() != 0) {
    WaitFrames(1);
  }
}

/// Byte view of the six colour writes in a quantity-bar segment, laid over a
/// 4-bit tile at the segment's byte column (Shop_FillSelector, cursor.c).
struct SelectorRecord {
  u8 filler0[4]; ///< the 4 bytes before the segment's byte in row 1, untouched
  /// Rows 1 to 6: index 4n is the segment's byte in row n + 1.
  u8 values[21];
};

/// Shows a shopkeeper's line and waits for its window to finish, one more
/// frame included. `messageId` is the weapon shop's line; the armor, item and
/// warriors' shops say theirs, at the same distance from their own welcome.
/// Called by every counter.
void UiMessage_ShowAndWait(s32 messageId) {
  // A shared message-resolution inline added four bytes to each dialogue caller
  // in the English native build; both callers retain their direct resolution.
  struct ShopRuntime *shop = gMenuWork;
  s32 portraitResource = FieldEffect_GetResourceIdFar(shop->keeperResource);
  s32 resolvedMessage = messageId;
  s8 shopType;

  UiWork_FinalizePendingCoreFar();
  shopType = shop->shopType;
  if (shopType == SHOP_TYPE_ARMOR)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == SHOP_TYPE_ITEMS)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  // A warriors' shop adds its distance on top of its type's
  if ((s8)shop->warriorShop != 0)
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
  UiText_OpenMessageWindowFar(resolvedMessage, 5, 0,
                              (portraitResource << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

/// Shows a shopkeeper's line as UiMessage_ShowAndWait does, hiding the hand
/// cursor while it shows and putting back its motion after.
void UiMessage_ShowAndRestoreState(s32 messageId) {
  s32 portraitResource;
  s32 resolvedMessage;
  s8 shopType;
  s8 warriorShop;
  u8 savedCursorMode;
  struct ShopRuntime *shop;
  struct RenderOutput **cursorSlot;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->active;
  resolvedMessage = messageId;
  portraitResource = FieldEffect_GetResourceIdFar(shop->keeperResource);
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
  (*cursorSlot)->active = UI_SPRITE_HIDDEN;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(resolvedMessage, 5, 0,
                              (portraitResource << 0x10) | 0x22);
  while (UiWork_IsCompleteFar() == 0) {
    WaitFrames(1);
  }
  WaitFrames(1);
  shop->cursor.anchor->active = savedCursorMode;
}

/// Runs the Yes or No choice beside the shopkeeper's line and returns it: 0
/// for Yes, 1 for No or B. `choice` is where the cursor starts. Keeps the
/// hand cursor's motion. Called by the counters before they buy, sell, equip
/// or repair, and by the inn.
s32 UiMessage_ShowChoice(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->active = savedCursorMode;
  return choice;
}

/// Runs the Yes or No choice at the sanctum's place on the screen and
/// returns it, keeping the hand cursor's motion. Called by
/// Sanctum_RunPartyService.
s32 UiMessage_ShowChoiceVariant(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(7, 7, choice);
  (*cursorSlot)->active = savedCursorMode;
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
