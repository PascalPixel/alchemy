/// @file
/// Running a shop, its cursor work, and the message helpers every counter uses.
/// Part of shop.c, the shop object, compiled only through it.

#include "../shop_internal.h"

/// Lists the current shop's artifacts whose shared stock counters are nonzero.
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
  do {
    equipmentGroup = shop->shopType;
    if (equipmentGroup == Item_GetEquipmentGroupFar(itemId) &&
        Item_AdjustCounterFar(itemId, 0) != 0) {
      *stockItems = itemId;
      count++;
      stockItems++;
    }
    itemId++;
  } while (itemId <= 0x1ff);
  shop->stockItemIds[count] = 0;
  shop->stockCount = count;
  return count;
}

/// Advances the cursor in the shared menu state once per scheduled update.
void Shop_StepCursor(void) {
  ShopCursor_Advance(&((struct ShopRuntime *)gMenuWork)->cursor);
}

/// Allocates the shared menu state, lists the party, loads six cursor icons and
/// starts its task.
void Shop_InitializeCursorWork(void) {
  struct ShopRuntime *shop;
  s32 resourceSlot;

  shop = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
  Battle_ResetEffectCounterFar();
  Dma_Fill32(0, shop, 0x29c * 4);
  shop->mode = 12;
  shop->partyMemberCount = Party_ListActiveOwnersFar(shop->partyMemberIds);
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
  Scheduler_AddOrUpdateCallback(Shop_StepCursor, 0xc80);
}

/// Stops the shared cursor task and releases the shop, sanctum or inn menu
/// resources.
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

/// Runs the shop's buy, sell, artifacts and repair menu with the selected
/// shopkeeper.
s32 Shop_Run(s32 stockRow, s32 keeperId) {
  struct ShopRuntime *shop;
  struct RenderOutput *anchor;
  struct UiWindow *window;
  s32 action = 0;

  if (stockRow >= EventTable_GetRowLimit() || stockRow < 0)
    stockRow = 0;
  // Seed this location's artifact stock before creating the shop UI.
  EventTable_ApplyRowAbilities(stockRow);
  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->shopType = EventTable_GetRowType(stockRow);
  if (stockRow == 16)
    shop->warriorShop = 1;
  if (stockRow == 17)
    shop->warriorShop = 1;
  if (stockRow == 18)
    shop->warriorShop = 1;
  shop->keeperResource =
      ((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(
                                      keeperId))
           ->animation)
          ->entries[0]
          ->animId;
  window = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
  if (window == 0)
    window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
  anchor = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  anchor->active = 1;
  anchor->kind = 0;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  UiMessage_ShowAndWait(MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->partyAction = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = EventTable_CopyRowHeader(stockRow, shop->stockItemIds);
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

#if TBS_PATCH_1
#define SHOP_DEBUG_STOCK_ROW 1
#else
#define SHOP_DEBUG_STOCK_ROW 2
#endif

/// Sets up the original debug shop scenario with money, equipment and party
/// ailments.
s32 Battle_ApplyPresetItemsAndFlags(s32 unused) {
  gGameState.coins = 0x30d40;
  gGameState.shopGifts = 0x1c;
  Inventory_EquipFar(GARET, Inventory_AddItemFar(GARET, 0x48d));
  Inventory_EquipFar(ISAAC, Inventory_AddItemFar(ISAAC, 0x40b));
  Inventory_AddItemFar(IVAN, ITEM_BONE);
  UnitGetFar(MIA)->poison = 1;
  UnitGetFar(JENNA)->poison = 1;
  UnitGetFar(IVAN)->haunt = 1;
  Shop_Run(SHOP_DEBUG_STOCK_ROW, 0x1e);
  return 0;
}

/// Waits one frame at a time until the audio command completes.
void AudioCommand_WaitForStateByteClear(void) {
  while (AudioCommand_GetStateByteFar() != 0) {
    WaitFrames(1);
  }
}

struct SelectorRecord {
  u8 filler0[4];
  u8 values[21];
};

/// Resolves shop dialogue and waits for its window to finish, including the
/// final frame.
/// A shared message-resolution inline added four bytes to each dialogue caller
/// in the English native build; both callers retain their direct resolution.
void UiMessage_ShowAndWait(s32 messageId) {
  struct ShopRuntime *shop = gMenuWork;
  s32 portraitResource = BattleFx_GetResourceIdFar(shop->keeperResource);
  s32 resolvedMessage = messageId;
  s8 shopType;

  UiWork_FinalizePendingCoreFar();
  shopType = shop->shopType;
  if (shopType == 2)
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (shopType == 0)
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  if ((s8)shop->warriorShop != 0)
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
  UiText_OpenMessageWindowFar(resolvedMessage, 5, 0,
                              (portraitResource << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

/// Resolves shop dialogue, hides the cursor while it is shown and restores the
/// saved cursor mode.
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
  portraitResource = BattleFx_GetResourceIdFar(shop->keeperResource);
  shopType = shop->shopType;
  if (shopType == 2) {
    resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  }
  if (shopType == 0) {
    resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
  }
  warriorShop = shop->warriorShop;
  if (warriorShop != 0) {
    resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
  }
  (*cursorSlot)->active = 13;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(resolvedMessage, 5, 0,
                              (portraitResource << 0x10) | 0x22);
  while (UiWork_IsCompleteFar() == 0) {
    WaitFrames(1);
  }
  WaitFrames(1);
  shop->cursor.anchor->active = savedCursorMode;
}

/// Prepares the localized confirmation menu and restores the cursor's saved
/// mode.
s32 UiMessage_ShowChoice(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->active = savedCursorMode;
  return choice;
}

/// Runs the sanctum-position confirmation menu and restores the cursor's saved
/// mode.
s32 UiMessage_ShowChoiceVariant(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(7, 7, choice);
  (*cursorSlot)->active = savedCursorMode;
  return choice;
}

/// Unused original list walker: follows the requested number of next links.
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
