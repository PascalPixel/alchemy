#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "battle_system.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "node_chain.h"
#include "game_state.h"
#include "songs.h"
#include "item.h"
#include "item_ids.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "far_runtime.h"
#include "lib.h"
#include "fixed_point_position.h"
#include "inn_runtime.h"
#include "field_runtime.h"
#include "battle_calc.h"
#include "field_obj.h"

// English menu entries 48..51 establish the four shop actions.
enum ShopAction {
  SHOP_ACTION_BUY,
  SHOP_ACTION_SELL,
  SHOP_ACTION_ARTIFACTS,
  SHOP_ACTION_REPAIR
};

// These service kinds select the existing condition, fee and cure branches.
enum SanctumService {
  SANCTUM_SERVICE_REVIVE,
  SANCTUM_SERVICE_CURE_POISON,
  SANCTUM_SERVICE_EXORCISE,
  SANCTUM_SERVICE_REMOVE_CURSE
};

enum { SHOP_INVENTORY_COLUMNS = 5, SHOP_STOCK_PAGE_ITEMS = 7 };

// BUG: the original item picker tests its selection result rather than the
// discard error, so it never shows the cannot-remove message for error -3.
#ifdef BUGFIX
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult)              \
  (discardStatus)
#else
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult)              \
  (selectionResult)
#endif

// Edition-specific text and window layout values.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define SHOP_CONFIRM_MENU_X 6
#else
#define SHOP_CONFIRM_MENU_X 7
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)
#define SHOP_PRICE_NUMBER_X 0x18
#define SHOP_PRICE_LABEL_X 0x40
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define SHOP_PRICE_NUMBER_X 0x20
#define SHOP_PRICE_LABEL_X 0x50
#else
#define SHOP_PRICE_NUMBER_X 0x20
#define SHOP_PRICE_LABEL_X 0x48
#endif

#if (LANGUAGE == LANGUAGE_ITALIAN)
#define SHOP_EMPTY_GRID_X 20
#define SHOP_EMPTY_GRID_Y 12
#else
#define SHOP_EMPTY_GRID_X 8
#define SHOP_EMPTY_GRID_Y 20
#endif

#if TBS_PATCH_1
#define SHOP_MONEY_WINDOW_WIDTH 12
#define SHOP_DRAW_MESSAGE UiText_DrawCharacterAtOffsetFar
#else
#define SHOP_MONEY_WINDOW_WIDTH 11
#define SHOP_DRAW_MESSAGE UiText_DrawResourceFar
#endif

#if (LANGUAGE == LANGUAGE_GERMAN)
#define SANCTUM_PRICE_WINDOW_X 0
#define SANCTUM_PRICE_WINDOW_WIDTH 30
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SANCTUM_PRICE_WINDOW_X 1
#define SANCTUM_PRICE_WINDOW_WIDTH 25
#else
#define SANCTUM_PRICE_WINDOW_X 1
#define SANCTUM_PRICE_WINDOW_WIDTH 23
#endif

#if TBS_PATCH_1
#define SHOP_USE_WINDOW_X 14
#define SHOP_USE_WINDOW_WIDTH 16
#else
#define SHOP_USE_WINDOW_X 16
#define SHOP_USE_WINDOW_WIDTH 14
#endif

// Lists the current shop's artifacts whose shared stock counters are nonzero.
s32 AbilityMenu_BuildAvailableList(void) {
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

// Advances the cursor in the shared menu state once per scheduled update.
void Shop_StepCursor(void) {
  ShopCursor_Advance(&((struct ShopRuntime *)gMenuWork)->cursor);
}

// Allocates the shared menu state, lists the party, loads six cursor icons and
// starts its task.
void Shop_InitializeCursorWork(void) {
  struct ShopRuntime *shop;
  s32 resourceSlot;

  shop = Runtime_AllocateHeapBlock(55, 0xa70);
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

// Stops the shared cursor task and releases the shop, sanctum or inn menu
// resources.
void Inn_Cleanup(void) {
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
  Runtime_ReleaseHeapBlock(0x37);
}

// Runs the shop's buy, sell, artifacts and repair menu with the selected
// shopkeeper.
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
    action = Menu_SelectEntry11To14Far(action);
    shop->partyAction = action;
    if (action == SHOP_ACTION_BUY) {
      shop->stockCount = EventTable_CopyRowHeader(stockRow, shop->stockItemIds);
      UiMessage_ShowAndWait(MsgWhatWouldYouLike);
      Shop_SelectBuy();
    } else if (action == SHOP_ACTION_SELL) {
      UiMessage_ShowAndWait(MsgWhatToSell);
      Shop_PickUnit();
    } else if (action == SHOP_ACTION_ARTIFACTS) {
      if (AbilityMenu_BuildAvailableList() != 0) {
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
  Inn_Cleanup();
  return 0;
}

#if TBS_PATCH_1
#define SHOP_DEBUG_STOCK_ROW 1
#else
#define SHOP_DEBUG_STOCK_ROW 2
#endif

// Sets up the original debug shop scenario with money, equipment and party
// ailments.
s32 Battle_ApplyPresetItemsAndFlags(s32 unused) {
  gGameState.coins = 0x30d40;
  gGameState.shopGifts = 0x1c;
  Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
  Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
  Inventory_AddItemFar(2, 0xe7);
  UnitGetFar(3)->poison = 1;
  UnitGetFar(5)->poison = 1;
  UnitGetFar(2)->haunt = 1;
  Shop_Run(SHOP_DEBUG_STOCK_ROW, 0x1e);
  return 0;
}

// Waits one frame at a time until the audio command completes.
void AudioCommand_WaitForStateByteClear(void) {
  while (AudioCommand_GetStateByteFar() != 0) {
    WaitFrames(1);
  }
}

struct SelectorRecord {
  u8 filler0[4];
  u8 values[21];
};

// Resolves shop dialogue and waits for its window to finish, including the
// final frame.
// A shared message-resolution inline added four bytes to each dialogue caller
// in the English native build; both callers retain their direct resolution.
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

// Resolves shop dialogue, hides the cursor while it is shown and restores the
// saved cursor mode.
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

// Prepares the localized confirmation menu and restores the cursor's saved
// mode.
s32 UiMessage_ShowChoice(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(SHOP_CONFIRM_MENU_X, 5, choice);
  (*cursorSlot)->active = savedCursorMode;
  return choice;
}

// Runs the sanctum-position confirmation menu and restores the cursor's saved
// mode.
s32 UiMessage_ShowChoiceVariant(s32 choice) {
  struct RenderOutput **cursorSlot =
      &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
  u8 savedCursorMode = (*cursorSlot)->active;
  UiIcon_PrepareObjectFar(*cursorSlot);
  choice = Menu_RunConfirmSelectionAtFar(7, 7, choice);
  (*cursorSlot)->active = savedCursorMode;
  return choice;
}

// Unused original list walker: follows the requested number of next links.
UNUSED struct NodeChainNode *
NodeChain_GetNodeAtIndex(struct NodeChainState *state) {
  struct NodeChainNode *node = state->node;
  s32 linkIndex;

  for (linkIndex = 0; linkIndex != state->count; ++linkIndex) {
    node = node->next;
  }
  return node;
}

// Selection frame tile positions
// clang-format off
static const u16 sSelectorTileOffsets[30] = {
    0x00, 0x01, 0x02, 0x03,
    0x20, 0x21, 0x22, 0x23,
    0x40, 0x41, 0x42, 0x43,
    0x60, 0x61, 0x62, 0x63,
    0x80, 0x81, 0x82, 0x83,
    0xa0, 0xa1, 0xa2, 0xa3,
    0xc0, 0xc1, 0xc2, 0xc3,
    0xe0, 0xe1,
};
// clang-format on

// Fills six selector bytes at each requested offset in the quantity tile
// buffer.
void Shop_FillSelector(s32 itemCount, s32 paletteSelector, u8 *tileBuffer) {
  const u16 *tileOffset;

  paletteSelector = (paletteSelector << 4) + 1;

  if (itemCount > 0) {
    tileOffset = sSelectorTileOffsets;
    do {
      struct SelectorRecord *record =
          (struct SelectorRecord *)(tileBuffer + *tileOffset++);
      record->values[0] = paletteSelector;
      record->values[4] = paletteSelector;
      record->values[8] = paletteSelector;
      record->values[12] = paletteSelector;
      record->values[16] = paletteSelector;
      record->values[20] = paletteSelector;
      itemCount--;
    } while (itemCount != 0);
  }
}

// Number transfer destinations
// clang-format off
static const u16 sPriceDigitTileOffsets[5] = {
    0xc0, 0x90, 0x80, 0x50, 0x40,
};
// clang-format on

// Copies one decimal glyph into its price-tile position, stopping at zero
// source bytes.
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition) {
  u8 *src = gShop_GlyphBytes + (digit << 5);
  u8 *dst = tileBuffer + sPriceDigitTileOffsets[digitPosition] + 2;
  s32 count = 3;

  do {
    if (*src != 0) {
      dst[0] = *src++;
      if (*src != 0) {
        dst[1] = *src++;
        if (*src != 0) {
          dst[30] = *src++;
          if (*src != 0)
            dst[31] = *src++;
        }
      }
    }
    dst += 4;
    count--;
  } while (count >= 0);
}

// Builds up to five decimal digits in scratch tiles, creates their sprite and
// frees the scratch block.
struct RenderOutput *Shop_CreatePriceSprite(s32 value, struct UiWindow *window,
                                            s32 x, s32 y) {
  u8 *tileBuffer;
  s32 resourceSlot;
  struct RenderOutput *priceSprite;

  tileBuffer = Runtime_AllocateBlock(14, 0x400);
  priceSprite = 0;
  Dma_Set(gShop_PriceTiles, tileBuffer, (DMA_ENABLE | DMA_32BIT) << 16 | 0x40,
          REG_DMA3);
  Shop_CopyGlyphs(value % 10, tileBuffer, 0);
  value = value / 10;
  if (value != 0) {
    Shop_CopyGlyphs(value % 10, tileBuffer, 1);
    value = value / 10;
    if (value != 0) {
      Shop_CopyGlyphs(value % 10, tileBuffer, 2);
      value = value / 10;
      if (value != 0) {
        s32 remainingDigits;

        Shop_CopyGlyphs(value % 10, tileBuffer, 3);
        remainingDigits = value / 10;
        if (remainingDigits != 0)
          Shop_CopyGlyphs(remainingDigits % 10, tileBuffer, 4);
      }
    }
  }
  resourceSlot = Resource_FindFreeEntry();
  if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(resourceSlot, 0x100, tileBuffer);
    priceSprite =
        RenderOutput_CreateFar(resourceSlot, 0x80008000, window, x, y);
  }
  Runtime_ReleaseHeapBlock(14);
  return priceSprite;
}

// Restores the saved field color buffers and starts their interpolation.
void Shop_RestoreSceneTiles(s32 layer) {
  // UB: slot 32 lies past scalar gEventWork. Its named load adds eight
  // bytes in the native EN trial (0x54 to 0x5c).
#ifdef UBFIX
  struct BattleEffectBuffers *paletteBuffers = gPaletteBufferWork;
  u8 *eventBytes = (u8 *)gEventWork;
#else
  u32 *workCells = (u32 *)&gEventWork;
  struct BattleEffectBuffers *paletteBuffers =
      (struct BattleEffectBuffers *)workCells[5];
  u8 *eventBytes = (u8 *)workCells[0];
#endif
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(savedPalette, eventBytes + 0x236,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x150, REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2a0, REG_DMA3);
  BattleFx_ApplyColorToTargetBufferFar(layer, 1);
  BattleFx_StartBufferInterpolationFar(16);
}

// Starts restoring the field color buffer after the menu effect.
void Shop_InitEffect(void) {
  BattleFx_ApplyColorToTargetBufferFar((s32)gEventWork + 0x236, 1);
  BattleFx_StartBufferInterpolationFar(0x10);
}

// Interpolates the cursor from its saved position to its target over the
// requested duration.
void ShopCursor_Advance(struct ShopCursor *cursor) {
  struct RenderOutput *sprite;
  s32 durationFrames;
  s32 frame;
  s32 cursorX;
  s32 cursorY;

  if (cursor == NULL)
    return;
  durationFrames = cursor->kind;
  if (durationFrames == 0)
    return;
  sprite = cursor->anchor;
  frame = ++cursor->active;
  cursorX =
      cursor->x + (cursor->targetX - (s16)cursor->x) * frame / durationFrames;
  sprite->x = cursorX;
  sprite->oam.x = cursorX;
  cursorY =
      cursor->y + (cursor->targetY - (s16)cursor->y) * frame / durationFrames;
  sprite->y = cursorY;
  sprite->oam.y = cursorY;
  if (frame == durationFrames) {
    cursor->kind = 0;
    cursor->active = 0;
  }
}

// Moves each cursor axis one quarter of the remaining distance, with a minimum
// one-pixel step.
void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor) {
  struct RenderOutput *sprite;
  s32 delta;
  s32 step;

  sprite = cursor->anchor;
  if (sprite != NULL) {
    delta = sprite->x - cursor->targetX;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->x = sprite->x - step;
      else
        sprite->x = sprite->x - 1;
      sprite->oam.x = sprite->x;
    } else if (delta < 0) {
      if (step != 0)
        sprite->x = sprite->x + step;
      else
        sprite->x = sprite->x + 1;
      sprite->oam.x = sprite->x;
    }

    delta = sprite->y - cursor->targetY;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->y = sprite->y - step;
      else
        sprite->y = sprite->y - 1;
      sprite->oam.y = sprite->y;
    } else if (delta < 0) {
      if (step != 0)
        sprite->y = sprite->y + step;
      else
        sprite->y = sprite->y + 1;
      sprite->oam.y = sprite->y;
    }
  }
}

// Saves the cursor's current position, assigns its target and duration, and
// resets progress.
void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s32 kind) {
  struct RenderOutput *anchor = cursor->anchor;

  cursor->x = anchor->x;
  cursor->y = anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->kind = kind;
  cursor->active = 0;
}

// Places the cursor and its OAM coordinates immediately, preserving the
// one-frame state.
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y) {

  {
    struct RenderOutput *sprite = cursor->anchor;

    cursor->targetX = x;
    cursor->kind = 1;
    cursor->active = 0;
    cursor->x = x;
    sprite->oam.x = sprite->x = x;
  }
  {
    struct RenderOutput *sprite;

    cursor->targetY = y;
    cursor->y = y;
    sprite = cursor->anchor;
    sprite->oam.y = sprite->y = y;
  }
}

// Converts window-relative pixel coordinates to the cursor's absolute target.
void Shop_PlaceCursor(struct UiWindow *window, s32 x, s32 y) {
  s32 cursorX;
  s32 cursorY;
  struct ShopRuntime *shop;

  cursorX = x;
  cursorY = y;
  shop = gMenuWork;
  if (window != NULL) {
    cursorX = cursorX + (window->x * 8) + 8;
    cursorY = cursorY + (window->y * 8) + 8;
  }
  Shop_SetCursor(&shop->cursor, cursorX, cursorY, (s8)shop->mode);
}

// Browses stock, selects a recipient, checks capacity and funds, then purchases
// the chosen quantity.
s32 Shop_SelectBuy(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *priceWindow = 0;
  struct UiWindow *stockWindow;
  struct UiWindow *messageWindow;
  struct UiWindow *partyWindow;
  s32 selected = 0;
  s32 quantity;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  struct UiWindow *itemWindow;
  s32 redraw;
  s32 selectionIndex;
  s32 stockCount;
  s32 itemId;
  struct ItemDefinition *stockDefinition;
  s32 selectionResult;
  s32 previousIndex;
  s32 unitId;
  s32 capacitySlot;
  s32 removedCount;
  s16 *stockIds;

  shop->itemWindow = 0;
  shop->moneyWindow =
      UiWindow_CreateFar(30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, 2);
  Shop_DrawMoney();
  priceWindow = UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, 2);

next_page:
  // Browse stock first; recipient and quantity selection follow confirmation.
  selectionIndex = selected;
  stockWindow = UiWindow_CreateFar(0, 12, 30, 4, 2);
  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  messageWindow = UiWindow_CreateFar(0, 17, 30, 3, 2);
  redraw = 1;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      stockIds = shop->stockItemIds;
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_GetDirectFar(itemId);
      redraw = 0;
      Shop_PlaceCursor(stockWindow,
                       selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
      shop->mode = 4;
      Shop_DrawStock(stockWindow, selectionIndex);
      Shop_DrawMsg(messageWindow, itemId + MsgItemPlainName);
      RenderOutput_RedrawSavedRectFar(priceWindow);
      Shop_DrawItemPrice(priceWindow, itemId, stockDefinition->price, 0);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      selected = selectionIndex;
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex--;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      previousIndex = selectionIndex;
      selectionIndex++;
      selectionIndex = (selectionIndex + stockCount) % stockCount;
      if (previousIndex != selectionIndex) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
    }
    if ((gKeysRepeat & KEY_UP) != 0 &&
        selectionIndex - SHOP_STOCK_PAGE_ITEMS >= 0) {
      selectionIndex -= SHOP_STOCK_PAGE_ITEMS;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_DOWN) != 0) {
      if (selectionIndex + SHOP_STOCK_PAGE_ITEMS <
          (stockCount + (SHOP_STOCK_PAGE_ITEMS - 1)) / SHOP_STOCK_PAGE_ITEMS *
              SHOP_STOCK_PAGE_ITEMS) {
        selectionIndex += SHOP_STOCK_PAGE_ITEMS;
        redraw = 1;
      }
      if (selectionIndex > stockCount - 1)
        selectionIndex = stockCount - 1;
    }
    WaitFrames(1);
  }

  UiWork_FinalizeFar(messageWindow, 2);
  UiWork_FinalizeFar(stockWindow, 2);
  WaitFrames(1);
  if (selectionResult != 0)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  UiMessage_ShowAndWait(MsgWhoGetsThis);
  selectedDefinition = Item_GetDirectFar(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(partyWindow, 2, 0, 8, 0);
  itemWindow = UiWindow_CreateFar(16, 11, 14, 9, 2);
  redraw = 1;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      UiMessage_ShowAndWait(MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      redraw = 0;
      selectionIndex =
          (selectionIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[selectionIndex];
      Shop_PlaceCursor(partyWindow, selectionIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == 0)
        Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
      else
        Shop_DrawEquipComparison(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      // Probe capacity with the original add/remove before checking funds.
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiWork_PushValueSlotFar(unitId, 1);
        UiWork_PushValueSlotFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          UiMessage_ShowAndWait(MsgPackFull);
        else
          UiMessage_ShowAndWait(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if ((u32)selectedDefinition->price > gGameState.coins) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiMessage_ShowAndRestoreState(MsgBuyNotEnoughMoney);
        selectionResult = -1;
        break;
      }
      if (Item_IsCompatibleWithOwnerFar(unitId, shop->selectedItem) == 0) {
        UiWork_PushValueSlotFar(unitId, 1);
        UiMessage_ShowAndWait(MsgCannotEquipStillWant);
        if (UiMessage_ShowChoice(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      WaitFrames(1);
      quantity = Shop_SelectBuyNum(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Shop_BuySpecialItem(partyWindow, itemWindow);
      selectionResult = 0;
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectionIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(itemWindow, 2);
  UiWork_FinalizeFar(partyWindow, 2);
  WaitFrames(1);
  // Artifact quantities are consumed only after the purchase completes.
  if (selectionResult == 0 && shop->partyAction == SHOP_ACTION_ARTIFACTS) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (AbilityMenu_BuildAvailableList() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  UiMessage_ShowAndWait(MsgBuyAnythingElse);
  goto next_page;

close:
  UiWork_FinalizeFar(priceWindow, 2);
  UiWork_FinalizeFar(shop->moneyWindow, 2);
  WaitFrames(1);
  return 0;
}

// Draws a seven-item stock page, selection highlight, page arrows and price
// sprites.
void Shop_DrawStock(struct UiWindow *window, s32 selected) {

  struct ShopRuntime *shop;
  s16 *itemIds;
  s16 *stockCursor;
  s32 itemCount;
  s32 stockIndex;
  s32 pageSlot;
  s32 priceX;
  s32 zero;
  s32 highlightFrame;
  s16 itemId;
  struct ItemDefinition *definition;
  struct RenderOutput *icon;

  shop = gMenuWork;
  itemIds = shop->stockItemIds;
  itemCount = shop->stockCount;
  stockIndex = selected - selected % SHOP_STOCK_PAGE_ITEMS;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (stockIndex != 0) {
      icon = RenderOutput_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    216, -16);
      zero = 0;
      icon->kind = zero;
      icon->active = 17;
      icon->animStep = zero;
    }
    if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
      icon = RenderOutput_CreateFar(shop->nextPageIcon, 0x40000000, window, 216,
                                    24);
      zero = 0;
      icon->kind = zero;
      icon->active = 15;
      icon->animStep = zero;
    }
    pageSlot = 0;
    if (stockIndex < (u32)itemCount) {
      stockCursor = (s16 *)((stockIndex * 2) + (u32)itemIds);
      priceX = 16;
      for (;
           (u32)pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
           priceX += 32, pageSlot++, stockCursor++, stockIndex++) {
        itemId = *stockCursor;
        definition = Item_GetDirectFar(itemId);
        icon =
            RenderOutput_CreateLoadedFar(itemId, 1, window, pageSlot * 32, 0);
        icon->sentinel = 252;
        if (stockIndex == selected) {
          icon->active = 9;
          highlightFrame = 10;
          icon->animStep = highlightFrame;
          icon->sentinel = 253;
        }
        icon = Shop_CreatePriceSprite(definition->price, window, priceX, 0);
        icon->sentinel = 251;
      }
    }
  }
}

// Draws the party's coins using the edition's layout and currency grammar.
void Shop_DrawMoney(void) {
  struct ShopRuntime *shop;
  struct UiWindow *window;

  shop = gMenuWork;
  window = shop->moneyWindow;
  if (window != 0) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    UiText_DrawCharacterAtOffsetFar(MsgYourCoins, window, 0, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0x20, 8);
#else
    s32 msg = MsgYourCoins;

    UiText_DrawCharacterAtOffsetFar(msg, window, 0, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0, 8);
    UiText_DrawCharacterAtOffsetFar(msg - 2, window, 48, 8);
#endif
  }
}

// X position below the item name, in pixels
#if TBS_PATCH_1
#define SHOP_ITEM_NOTE_X 0
#else
#define SHOP_ITEM_NOTE_X 8
#endif

// Draws the item name and price, or its cannot-buy or not-broken note.
// A structured switch with early returns reduced the English native extent
// from 0x98 to 0x90, so the existing shared draw tail remains.
void Shop_DrawItemPrice(struct UiWindow *window, s32 itemId, s32 price,
                        s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  UiText_DrawCharacterAtOffsetFar(MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X,
                                  8);
  return;
show_price:
  labelMessage = MsgPrice;
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_ITEM_NOTE_X, 8);
  labelMessage -= 3;
  UiText_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  return;
draw:
  RenderOutput_RedrawSavedRectFar(window);
  UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 0, 0);
  if (price != 0)
    goto show_price;
  switch (priceKind) {
  case 1:
    goto cannot_buy;
  case 2:
    UiText_DrawCharacterAtOffsetFar(MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
    break;
  default:
    goto show_price;
  }
  return;
}

// Clears or restores the message window, then draws its text; a null window
// is ignored.
void Shop_DrawMsg(struct UiWindow *window, s32 message) {
  if (window != 0) {
#if TBS_PATCH_1
    RenderOutput_RedrawSavedRectFar(window);
#else
    RenderOutput_ClearListFar(window);
    WaitFrames(1);
#endif
    SHOP_DRAW_MESSAGE(message, window, 0, 0);
  }
}

// Highlights the selected party icon and dims members who cannot equip the
// candidate item.
void Shop_DrawParty(struct UiWindow *window, s32 selected, s32 itemId) {
  struct ShopRuntime *shop = gMenuWork;
  s32 memberIndex;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            30);
      else
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Item_IsCompatibleWithOwnerFar(shop->partyMemberIds[memberIndex],
                                         itemId))
        shop->partyMemberScale[memberIndex] = 0xcccc;
    }
  }
}

// Temporarily equips the candidate, recalculates stats, restores the slot and
// draws the comparison.
void Shop_DrawEquipComparison(struct UiWindow *window, s32 unitId, s32 itemId) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 replacedItemId = -1;
  s32 previewSlot;
  s32 savedEntry;
  s32 index;
  s32 baseStats[4];
  s32 previewStats[4];

  if (window == 0)
    return;
  RenderOutput_PrepareForRedrawFar(window);
  if (!Item_CanOwnerEquip(unitId, itemId)) {
    UiText_DrawResourceFar(MsgShopCannotEquip, window, 8, 24);
    return;
  }
  previewSlot = Inventory_FindEquippedFar(unitId, item->type);
  if (previewSlot == -1) {
    for (index = 0; index <= 14; index++) {
      if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
        break;
    }
    if (index == 15) {
      for (index = 0; index <= 14; index++) {
        if (Item_GetDirectFar(unit->inventory[index])->type == 6)
          break;
      }
      if (index == 15)
        index = 0;
    }
    previewSlot = index;
  } else {
    replacedItemId = unit->inventory[previewSlot] & ITEM_ID_MASK;
  }
  // Recalculate a real temporary equipment change, then restore it exactly.
  savedEntry = unit->inventory[previewSlot];
  unit->inventory[previewSlot] = itemId | INVENTORY_EQUIPPED;
  UnitCalcFar(unitId);
  previewStats[0] = unit->atk;
  previewStats[1] = unit->def;
  previewStats[2] = unit->agi;
  previewStats[3] = unit->luck;
  unit->inventory[previewSlot] = savedEntry;
  UnitCalcFar(unitId);
  baseStats[0] = unit->atk;
  baseStats[1] = unit->def;
  baseStats[2] = unit->agi;
  baseStats[3] = unit->luck;
  for (index = 0; index <= 2; index++) {
    if (baseStats[index] > previewStats[index])
      RenderOutput_CreateFar(shop->statDownIcon, 0x40000000, window, 56,
                             index * 16 - 4)
          ->kind = 0;
    else if (baseStats[index] < previewStats[index])
      RenderOutput_CreateFar(shop->statUpIcon, 0x40000000, window, 56,
                             index * 16 - 4)
          ->kind = 0;
    UiText_DrawNumberInWindowFar(baseStats[index], 3, window, 32, index * 16);
    if (baseStats[index] != previewStats[index])
      UiText_DrawNumberInWindowFar(previewStats[index], 3, window, 72,
                                   index * 16);
    UiText_DrawCharacterAtOffsetFar(MsgShopStatLabel + index, window, 0,
                                    index * 16);
    UiWindow_DrawDividerLineFar(window, 0, index * 2 + 2, 13, index * 2 + 2);
  }
  if (replacedItemId != -1)
    UiText_DrawCharacterAtOffsetFar(replacedItemId + MsgItemName, window, 0,
                                    48);
}
// Shows the quantity already carried and draws the member's existing inventory
// icons.
// A typed RenderOutput icon store kept the English text extent but changed
// 76 text bytes; retain the existing byte view of the sprite.
void Shop_DrawUnitItem(struct UiWindow *window, s32 unitId, s32 itemId) {

  u8 *unitBytes;
  s32 x;
  s32 y;
  s32 itemIndex;
  s32 slot;
  s32 quantityOffset;
  s32 firstOffset;
  s32 itemOffset;
  s32 nextOffset;
  u8 *icon;

  unitBytes = (u8 *)UnitGetFar(unitId);
  x = 8;
  y = 8;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + 216;
      UiWork_PushValueSlotFar(
          (*(u16 *)(unitBytes + quantityOffset) >> INVENTORY_QUANTITY_SHIFT) +
              1,
          5);
      UiText_DrawCharacterAtOffsetFar(MsgYouHave, window, 0, 0);
    } else {
      UiText_DrawCharacterAtOffsetFar(MsgShopNoneInStock, window, 0, 0);
    }
    itemIndex = 0;
    firstOffset = 216;
    if (*(u16 *)(unitBytes + firstOffset) != 0) {
      for (;;) {
        itemOffset = itemIndex * 2 + 216;
        icon = (u8 *)RenderOutput_CreateLoadedFar(
            *(u16 *)(unitBytes + itemOffset), 27, window, x, y);
        icon[15] = 252;
        x += 16;
        if (itemIndex == 4) {
          x = 8;
          y += 16;
        }
        if (itemIndex == 9) {
          x = 8;
          y += 16;
        }
        itemIndex++;
        if (itemIndex > 14)
          break;
        nextOffset = itemIndex * 2 + 216;
        if (*(u16 *)(unitBytes + nextOffset) == 0)
          break;
      }
    }
  }
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define SHOP_QUANTITY_CURSOR_X 120
#else
#define SHOP_QUANTITY_CURSOR_X 128
#endif

// Limits the purchase by stack space, available coins and artifact stock before
// asking its quantity.
s32 Shop_SelectBuyNum(s32 unitId, s32 itemId) {
  struct ShopRuntime *shop;
  struct BattleUnit *unit;
  struct ItemDefinition *item;
  s32 carriedQuantity;
  s32 quantityLimit;
  s32 slot;
  s32 artifactLimit;
  s32 selectedQuantity;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  item = Item_GetDirectFar(itemId);
  selectedQuantity = 1;
  if (item->flags & ITEM_STACKABLE) {
    UiMessage_ShowAndWait(MsgHowManyToBuy);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
    } else {
      carriedQuantity = 0;
    }

    quantityLimit = 30;
    if (item->price != 0)
      quantityLimit = (u32)gGameState.coins / item->price;

    if (shop->partyAction == SHOP_ACTION_ARTIFACTS) {
      artifactLimit = Item_AdjustCounterFar(itemId, 0);
      if (quantityLimit > artifactLimit)
        artifactLimit = Item_AdjustCounterFar(itemId, 0);
      else
        artifactLimit = quantityLimit;
      quantityLimit = artifactLimit;
    }

    quantityLimit += carriedQuantity;
    if (quantityLimit > 30)
      quantityLimit = 30;

    shop->mode = 12;
    Shop_PlaceCursor(0, SHOP_QUANTITY_CURSOR_X, 0x30);
    selectedQuantity =
        Shop_SelectQuantity(carriedQuantity, quantityLimit, item->price);
  }
  return selectedQuantity;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define SHOP_QUANTITY_NUMBER_X 64
#define SHOP_QUANTITY_COIN_LABEL_X 128
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 144
#else
#define SHOP_QUANTITY_NUMBER_X 72
#define SHOP_QUANTITY_COIN_LABEL_X 136
#endif

// Polls a wrapped quantity selector, redraws its amount and price, and returns
// the count or -1.
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price) {
  void *tileBuffer;
  struct UiWindow *window;
  s32 redraw;
  s32 resourceSlot;
  s32 quantityIndex;
  s32 selectedQuantity;
  struct RenderOutput *quantitySprite;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 unitMessageBase;
#endif

  tileBuffer = Runtime_AllocateBlock(14, 0x400);
  redraw = 1;
  maximum -= minimum;
  selectedQuantity = -1;
  quantityIndex = 0;
  window = UiWindow_CreateFar(7, 4, 23, 3, 2);
  if (window != 0) {
    resourceSlot = Resource_FindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
      VramCache_Load(resourceSlot, 0x100, 0);
      RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 0, 0);
      quantitySprite =
          RenderOutput_CreateFar(resourceSlot, 0x40004000, window, 32, 0);
      quantitySprite->oam.index += 4;
      for (;;) {
        if (gKeysPressed & KEY_A) {
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          selectedQuantity = quantityIndex + 1;
          break;
        }
        if (gKeysPressed & KEY_B) {
          AudioCommand_PlayFar(SE_MENU_CANCEL);
          selectedQuantity = -1;
          break;
        }
        if (gKeysRepeat & KEY_LEFT) {
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex--;
        }
        if (gKeysRepeat & KEY_RIGHT) {
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
          quantityIndex++;
        }
        if (redraw) {
          redraw = 0;
          quantityIndex = (quantityIndex + maximum) % maximum;
          Dma_Set(gShop_QuantityTiles, tileBuffer,
                  (DMA_ENABLE | DMA_32BIT) << 16 | 0x40, REG_DMA3);
          Shop_FillSelector(30, 14, tileBuffer);
          Shop_FillSelector(minimum + maximum, 0, tileBuffer);
          Shop_FillSelector(minimum + quantityIndex + 1, 10, tileBuffer);
          Shop_FillSelector(minimum, 2, tileBuffer);
          VramCache_Load(resourceSlot, 0x100, tileBuffer);
          selectedQuantity = quantityIndex + 1;
#if (LANGUAGE != LANGUAGE_JAPANESE)
          UiText_DrawNumberInWindowFar(selectedQuantity, 2, window,
                                       SHOP_QUANTITY_NUMBER_X, 0);
          UiText_DrawNumberInWindowFar(price * selectedQuantity, 6, window,
                                       SHOP_QUANTITY_NUMBER_X + 16, 0);
          UiText_DrawCharacterAtOffsetFar(MsgCoins, window,
                                          SHOP_QUANTITY_COIN_LABEL_X, 0);
#else
          UiText_DrawNumberInWindowFar(selectedQuantity, 2, window, 64, 0);
          unitMessageBase = MsgPiece;
          UiText_DrawCharacterAtOffsetFar(unitMessageBase, window, 80, 0);
          UiText_DrawNumberInWindowFar(price * selectedQuantity, 6, window, 96,
                                       0);
          UiText_DrawCharacterAtOffsetFar(unitMessageBase - 1, window, 144, 0);
#endif
        }
        WaitFrames(1);
      }
      WaitFrames(1);
      UiWork_FinalizeFar(window, 2);
    }
  }
  Runtime_ReleaseHeapBlock(14);
  return selectedQuantity;
}

// Adds each purchased item, charges coins and earns credit before offering to
// equip it.
void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 remainingQuantity;
  s32 lastAddedSlot;
  struct ItemDefinition *item;

  remainingQuantity = quantity;
  item = Item_GetDirectFar(itemId);
  lastAddedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  // Keep each inventory addition, coin debit and shop-credit increment in
  // order.
  AudioCommand_PlayFar(SE_SHOP_PURCHASE);
  if (lastAddedSlot < remainingQuantity) {
    do {
      lastAddedSlot = Inventory_AddItemFar(unitId, itemId);
      Party_AdjustCoinsFar(0 - item->price);
      remainingQuantity -= 1;
      Party_AdjustShopCreditFar(item->price);
      Shop_DrawMoney();
    } while (remainingQuantity != 0);
  }
  UiMessage_ShowAndRestoreState(MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, lastAddedSlot) != 0) {
    Shop_SellOld(unitId, replacedSlot);
  }
}

// Checks equipment restrictions, asks permission and equips the item, including
// its curse warning.
s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *definition = Item_GetDirectFar(itemId);
  s32 replacedSlot;
  struct UiWindow *itemWindow;

  if (unit->inventory[slot] & INVENTORY_EQUIPPED)
    return 0;

  if (Item_CanOwnerEquip(unitId, itemId) == 0)
    return 0;

  replacedSlot = Inventory_FindEquippedFar(unitId, definition->type);
  if (replacedSlot != -1) {
    struct ItemDefinition *oldInfo =
        Item_GetDirectFar(unit->inventory[replacedSlot]);

    if (oldInfo->flags & ITEM_CANNOT_UNEQUIP)
      return 0;
  }

  UiWork_PushValueSlotFar(unitId, 1);
  UiMessage_ShowAndWait(MsgEquipNowPrompt);
  if (UiMessage_ShowChoice(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  itemWindow = shop->itemWindow;
  if (itemWindow != 0)
    Shop_DrawUnitGrid(itemWindow, unitId);

  if (definition->flags & ITEM_CURSED) {
    AudioCommand_PlayFar(SE_103);
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(MsgBecameCursed, 8, 4, 2);
    while (UiWork_IsCompleteFar() == 0) {
      WaitFrames(1);
    }
  }

  UiMessage_ShowAndRestoreState(MsgLookBolder);
  return 1;
}

// Offers to sell one replaced equipment item unless its type or flags prohibit
// it.
s32 Shop_SellOld(s32 unitId, s32 slot) {
  struct BattleUnit *unit;
  s32 itemId;

  unit = UnitGetFar(unitId);
  if (slot == -1) {
    return 0;
  }
  itemId = unit->inventory[slot] & ITEM_ID_MASK;
  if (Item_GetDirectFar(itemId)->type == 6) {
    return 0;
  }
  if (Item_GetDirectFar(itemId)->flags & ITEM_INDISPENSABLE) {
    return 0;
  }
  Shop_SellItem(unitId, slot, -1);
  return 1;
}

// Returns zero for indispensable items, half price for broken ones or three
// quarters otherwise.
s32 Shop_SalePrice(s32 inventoryEntry) {
  s32 salePrice;

  salePrice = Item_GetDirectFar(inventoryEntry)->price;

  if (Item_GetDirectFar(inventoryEntry)->flags & ITEM_INDISPENSABLE) {
    salePrice = 0;
  } else if (inventoryEntry & INVENTORY_BROKEN) {
    salePrice = salePrice / 2;
  } else {
    salePrice = salePrice * 3 / 4;
  }
  return salePrice;
}

// Selects whose inventory to sell or repair, returning to the party list after
// each operation.
s32 Shop_PickUnit(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *listWindow;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;

  shop->moneyWindow = UiWindow_CreateFar(0, 9, SHOP_MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();
  shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, 2);
  listWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(listWindow, memberIndex, 0);
      Shop_DrawUnitGrid(shop->itemWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      if (Inventory_CountFar(unitId) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
      } else {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        if (shop->partyAction == SHOP_ACTION_SELL)
          Shop_SelectSell(unitId);
        else
          Shop_SelectRepair(unitId);
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        redraw = 1;
      }
      continue;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      Menu_ReleaseEntryObjectsFar();
      UiWork_FinalizeFar(listWindow, 2);
      UiWork_FinalizeFar(shop->itemWindow, 2);
      UiWork_FinalizeFar(shop->moneyWindow, 2);
      WaitFrames(1);
      return 0;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }
}

// Selects an inventory slot and quantity to sell, repeating until cancelled or
// the inventory is empty.
// Shared previous/next-row inlines added four bytes to each sell, repair and
// use picker in the English native build; the direct row arithmetic remains.
s32 Shop_SelectSell(s32 unitId) {
  struct UiWindow *descriptionWindow;
  struct ShopRuntime *shop;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 inventoryCount;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 quantity;
  struct UiWindow *window;
  s32 x;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  inventoryCount = 1;
  priceWindow = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
  slot = 0;

  for (;;) {
    descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    redraw = 1;

    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = ITEM_ID_MASK & unit->inventory[slot];
        window = shop->itemWindow;
        x = slot % SHOP_INVENTORY_COLUMNS * 16;
        Shop_PlaceCursor(window, x, slot / SHOP_INVENTORY_COLUMNS * 16 + 8);
        shop->mode = 3;
        Shop_DrawItemPrice(priceWindow, itemId,
                           Shop_SalePrice(unit->inventory[slot]), 1);
        Shop_DrawMsg(descriptionWindow, itemId + MsgItemPlainName);
      }

      if ((gKeysPressed & KEY_A) != 0) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot -= 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot += 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_UP) != 0) {
        slot -= SHOP_INVENTORY_COLUMNS;
        if (slot < 0)
          slot += INVENTORY_SLOTS;
        while (slot >= inventoryCount)
          slot -= SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        slot += SHOP_INVENTORY_COLUMNS;
        if (slot >= inventoryCount)
          slot -= INVENTORY_SLOTS;
        while (slot < 0)
          slot += SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      WaitFrames(1);
    }

    UiWork_FinalizeFar(descriptionWindow, 2);
    WaitFrames(1);
    if (result != 0)
      break;

    quantity = Shop_SelectSellNum(unitId, slot);
    if (quantity != -1)
      Shop_SellItem(unitId, slot, quantity);
    UiMessage_ShowAndWait(MsgSellAnythingElse);
    if (Inventory_CountFar(unitId) == 0)
      break;
  }

  UiWork_FinalizeFar(priceWindow, 2);
  return result;
}

// Draws all fifteen inventory slots in five columns, including any empty
// positions.
void Shop_DrawUnitGrid(struct UiWindow *window, s32 unitId) {
  struct BattleUnit *unit;
  s32 x;
  s32 y;
  s32 slot;
  struct RenderOutput *icon;

  unit = UnitGetFar(unitId);
  x = 8;
  y = 0;
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (Inventory_CountFar(unitId) == 0) {
      UiText_DrawResourceFar(MsgNoItems, window, SHOP_EMPTY_GRID_X,
                             SHOP_EMPTY_GRID_Y);
    } else {
      slot = 0;
      do {
        if (unit->inventory[slot] != 0) {
          icon = RenderOutput_CreateLoadedFar(unit->inventory[slot], 27, window,
                                              x, y);
          icon->sentinel = 252;
        }
        x += 16;
        if (slot == 4) {
          x = 8;
          y += 16;
        }
        if (slot == 9) {
          x = 8;
          y += 16;
        }
        slot++;
      } while (slot <= 14);
    }
  }
}

// Asks how many to sell only for stacks larger than one, restoring the cursor
// afterward.
s32 Shop_SelectSellNum(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 unitSalePrice;
  s32 stackQuantity;
  s32 quantityLimit;
  struct ItemDefinition *item;
  struct ShopRuntime *shop;
  struct BattleUnit *unit;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  item = Item_GetDirectFar(unit->inventory[slot]);
  result = 1;
  unitSalePrice = Shop_SalePrice(unit->inventory[slot]);
  stackQuantity = Shop_GetSelectionState(unitId, slot);
  quantityLimit = stackQuantity;
  if ((item->flags & ITEM_STACKABLE) && stackQuantity > 1) {
    UiMessage_ShowAndWait(MsgHowManyToSell);
    savedX = shop->cursor.targetX;
    savedY = shop->cursor.targetY;
    shop->cursor.anchor->active = 4;
    shop->mode = 0xc;
    Shop_PlaceCursor(NULL, SHOP_QUANTITY_CURSOR_X, 0x30);
    result = Shop_SelectQuantity(0, quantityLimit, unitSalePrice);
    WaitFrames(1);
    UiIcon_PrepareObjectFar(shop->cursor.anchor);
    Shop_PlaceCursor(NULL, savedX, savedY);
  }
  return result;
}

// Checks the offer and restrictions, discards the chosen count, then credits
// coins and redraws.
void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  u8 rare = item->flags & 4;
  s32 sellingReplacedItem = 0;
  s32 total;
  s32 message;
  s32 soldCount;

  if (count == -1) {
    sellingReplacedItem = 1;
    count = 1;
  }
  total = Shop_SalePrice(unit->inventory[slot]) * count;
  if (total == 0) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState(MsgWeDoNotBuy);
    return;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) &&
      (item->flags & ITEM_CANNOT_UNEQUIP)) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState(MsgCannotSellEquipped);
    return;
  }
  if (sellingReplacedItem)
    message = MsgOldItemSellOffer;
  else if (unit->inventory[slot] & INVENTORY_BROKEN)
    message = MsgDamagedSellOffer;
  else if (count > 1)
    message = MsgSellTotal;
  else if (rare)
    message = MsgRareSellOffer;
  else
    message = MsgSellOffer;
  UiWork_PushValueSlotFar(itemId, 2);
  UiWork_PushValueSlotFar(total, 5);
  UiMessage_ShowAndRestoreState(message);
  if (UiMessage_ShowChoice(0) != 0) {
    if (rare || sellingReplacedItem)
      message = MsgKeepIt;
    else
      message = MsgChangedMind;
    UiMessage_ShowAndRestoreState(message);
    return;
  }
  // Discard each item before crediting coins and rebuilding the inventory grid.
  AudioCommand_PlayFar(SE_102);
  for (soldCount = 0; soldCount < count; soldCount++)
    Inventory_DiscardFar(unitId, slot);
  Party_AdjustCoinsFar(total);
  Shop_DrawMoney();
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  if (rare || sellingReplacedItem)
    message = MsgSold;
  else
    message = MsgDeal;
  UiMessage_ShowAndRestoreState(message);
}

// Returns one quarter of the base price for broken items, or zero for intact
// items.
s32 Shop_RepairPrice(s32 inventoryEntry) {
  s32 repairCost = Item_GetDirectFar(inventoryEntry)->price / 4;

  if ((inventoryEntry & INVENTORY_BROKEN) == 0) {
    repairCost = 0;
  }
  return repairCost;
}

// Preserves the original repair-owner wrapper and its zero result.
s32 Shop_SelectUnit(void) {
  Shop_PickUnit();
  return 0;
}

// Selects an inventory slot for repair and repeats until cancelled or the
// inventory is empty.
s32 Shop_SelectRepair(s32 unitId) {
  struct ShopRuntime *shop;
  s32 inventoryCount;
  struct UiWindow *descriptionWindow;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 repairCost;
  struct UiWindow *window;
  s32 x;
  s32 y;

  shop = gMenuWork;
  unit = UnitGetFar(unitId);
  inventoryCount = 1;
  priceWindow = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
  slot = 0;

  for (;;) {
    descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    redraw = 1;

    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = ITEM_ID_MASK & unit->inventory[slot];
        window = shop->itemWindow;
        x = slot % SHOP_INVENTORY_COLUMNS * 16;
        y = slot / SHOP_INVENTORY_COLUMNS * 16 + 8;
        Shop_PlaceCursor(window, x, y);
        shop->mode = 3;
        repairCost = Shop_RepairPrice(unit->inventory[slot]);
        Shop_DrawItemPrice(priceWindow, itemId, repairCost, 2);
        Shop_DrawMsg(descriptionWindow, itemId + MsgItemPlainName);
      }
      if ((gKeysPressed & KEY_A) != 0) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot -= 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        slot += 1;
        slot = (slot + inventoryCount) % inventoryCount;
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_UP) != 0) {
        slot -= SHOP_INVENTORY_COLUMNS;
        if (slot < 0)
          slot += INVENTORY_SLOTS;
        while (slot >= inventoryCount)
          slot -= SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        slot += SHOP_INVENTORY_COLUMNS;
        if (slot >= inventoryCount)
          slot -= INVENTORY_SLOTS;
        while (slot < 0)
          slot += SHOP_INVENTORY_COLUMNS;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
      }
      WaitFrames(1);
    }

    UiWork_FinalizeFar(descriptionWindow, 2);
    WaitFrames(1);
    if (result != 0)
      break;

    Shop_RepairItem(unitId, slot);
    UiMessage_ShowAndWait(MsgRepairAnythingElse);
    if (Inventory_CountFar(unitId) == 0)
      break;
  }

  UiWork_FinalizeFar(priceWindow, 2);
  return result;
}

// Checks the repair, hides the item during the work sequence, restores it and
// charges the fee.
void Shop_RepairItem(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  struct ItemDefinition *item = Item_GetDirectFar(itemId);
  s32 equipped = Inventory_FindEquippedFar(unitId, item->type);
  u32 repairCost = Shop_RepairPrice(unit->inventory[slot]);
  s32 messageBase;
  u8 useType;
  u32 savedEntry;

  useType = item->useType;
  if (useType != 2) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState(MsgCannotRepairKind);
    return;
  }
  if (!(unit->inventory[slot] & INVENTORY_BROKEN)) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState(MsgNotDamaged);
    return;
  }
  if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flags)) {
    UiWork_PushValueSlotFar(itemId, 2);
    UiMessage_ShowAndRestoreState(MsgCursedWontComeOff);
    return;
  }
  if (repairCost > gGameState.coins) {
    UiMessage_ShowAndRestoreState(MsgNotEnoughMoney);
    return;
  }
  UiWork_PushValueSlotFar(itemId, 2);
  UiWork_PushValueSlotFar(repairCost, 5);
  messageBase = MsgShopRepairConfirm;
  UiMessage_ShowAndRestoreState(messageBase);
  if (UiMessage_ShowChoice(0) != 0) {
    UiMessage_ShowAndRestoreState(messageBase + 1);
    return;
  }
  // Hide the item during the work sequence without compacting its slot.
  savedEntry = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, 2);
  UiMessage_ShowAndRestoreState(messageBase + 2);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(10);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_100);
  WaitFrames(110);
  AudioCommand_PlayFar(SE_MENU_CONFIRM);
  WaitFrames(20);
  // Restore the entry, repair it, charge the fee and offer equipment changes.
  unit->inventory[slot] = savedEntry;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-repairCost);
  Shop_DrawMoney();
  Shop_DrawUnitGrid(shop->itemWindow, unitId);
  UiWork_PushValueSlotFar(itemId, 2);
  UiMessage_ShowAndRestoreState(messageBase + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}

// Gift prices
// clang-format off
static const s16 sGameTicketCreditCosts[30] = {
    300, 300, 300, 300, 300,
    500, 500, 500, 500, 500,
    1000, 1000, 1000, 1000, 1000,
    2000, 2000, 2000, 2000, 2000,
    4000, 4000, 4000, 4000, 4000,
    8000, 8000, 8000, 8000, 8000,
};

// Gift items, terminated by 0; no function reads this table.
const s16 gShop_GiftItemIds[21] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    16, 195, 38, 84, 89, 90, 180, 181, 190, 0,
};
// clang-format on

// Offers a Game Ticket for accumulated shop credit and lets the player choose
// its recipient.
void Shop_BuySpecialItem(struct UiWindow *window, struct UiWindow *itemWindow) {
  struct ShopRuntime *shop = gMenuWork;
  s32 creditCost;
  u32 savedSelectedItem;
  s32 redraw;
  s32 unitId;
  s32 memberIndex;
  s32 messageBase;
  s32 slot;

  redraw = 1;
  unitId = 0;
  savedSelectedItem = shop->selectedItem;
  creditCost = sGameTicketCreditCosts[gGameState.shopGifts];
  memberIndex = 0;

  // Gifts spend accumulated shop credit; they do not debit the party's coins.
  if (creditCost > gGameState.shopCredit)
    return;
  shop->selectedItem = ITEM_GAME_TICKET;
  UiWork_PushValueSlotFar(ITEM_GAME_TICKET, 2);
  messageBase = MsgTokenGift;
  UiMessage_ShowAndRestoreState(messageBase);
  UiWork_PushValueSlotFar(shop->selectedItem, 2);
  UiMessage_ShowAndRestoreState(messageBase + 1);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(window, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(window, memberIndex, shop->selectedItem);
      Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
    }
    if ((gKeysPressed & KEY_A) != 0) {
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiWork_PushValueSlotFar(unitId, 1);
        UiWork_PushValueSlotFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == INVENTORY_SLOTS)
          UiMessage_ShowAndWait(MsgPackFull);
        else
          UiMessage_ShowAndWait(MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      AudioCommand_PlayFar(SE_SHOP_PURCHASE);
      UiMessage_ShowAndRestoreState(MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      UiMessage_ShowAndRestoreState(MsgDontWantIt);
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex++;
      redraw = 1;
    }
    WaitFrames(1);
  }
  shop->selectedItem = savedSelectedItem;
}

// Shop stock: 24 regular items, 8 special items (each 0-terminated), then kind.
// clang-format off
static const s16 sStockTable[35][33] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 16, 43, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        89, 90, 103, 118, 127, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 16, 31, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        56, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        90, 118, 127, 136, 156, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        1, 2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        57, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 90, 104, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        58, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 92, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 17, 32, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        75, 92, 119, 128, 137, 146, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        2, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        92, 105, 120, 137, 146, 157, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        106, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        3, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        59, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        77, 92, 105, 120, 129, 137, 147, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        3, 18, 33, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        60, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        77, 107, 120, 129, 138, 147, 158, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        61, 0, 0, 0, 0, 0, 0, 0,
        1,
    },
    {
        78, 94, 107, 121, 129, 138, 148, 158, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 189, 0, 0, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 20, 34, 62, 0, 0, 0, 0,
        1,
    },
    {
        79, 139, 149, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        109, 131, 0, 0, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 183, 189, 186, 0, 0, 0, 0,
        0,
    },
    {
        4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 21, 47, 63, 0, 0, 0, 0,
        1,
    },
    {
        80, 150, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        110, 122, 140, 159, 0, 0, 0, 0,
        2,
    },
    {
        180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        182, 183, 189, 186, 0, 0, 0, 0,
        0,
    },
};
// clang-format on

// Returns the number of rows in the shop stock table.
s32 EventTable_GetRowLimit(void) { return 35; }

// Adds a location's initial artifacts to shared stock once, before its first
// shop visit.
void EventTable_ApplyRowAbilities(s32 stockRow) {
  const s16 *stockEntries;
  const s16 *artifactCursor;
  s32 itemId;
  s32 artifactIndex;
  s32 initializedFlag;

  initializedFlag = stockRow + 0x400;
  if (GameFlag_TestFar(initializedFlag) == 0) {
    GameFlag_SetBitFar(initializedFlag);
    artifactIndex = 0;
    itemId = sStockTable[stockRow][24];
    if (itemId != 0) {
      stockEntries = sStockTable[stockRow];
      artifactCursor = stockEntries + 24;
      do {
        Item_AdjustCounterFar(itemId, 1);
        artifactIndex++;
        if (artifactIndex > 7)
          break;
        artifactCursor++;
        itemId = *artifactCursor;
      } while (itemId != 0);
    }
  }
}

// Copies up to 24 ordinary stock items and appends a zero terminator.
s32 EventTable_CopyRowHeader(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (sStockTable[stockRow][0] != 0) {
    dst = output;
    src = sStockTable[stockRow];
    do {
      /* FAKEMATCH: only the copy read is volatile, so its lifetime stays
       * separate from the signed sentinel read. Without volatile: differ 41;
       * indexed and while forms: 34 to 54; read or copied as u16 (2026-10-13):
       * differ 41, the sentinel read is reused for the copy. */
      *dst = *(const volatile s16 *)src;
      count++;
      src++;
      dst++;
      if (count > 23)
        break;
    } while (*src != 0);
  }
  output[count] = 0;
  return count;
}

// Reads the shop type from the last word of its stock row.
s32 EventTable_GetRowType(s32 stockRow) { return sStockTable[stockRow][32]; }

// Calculates the existing fee for reviving, curing poison, exorcising or
// removing a curse.
s32 Shop_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = UnitGetFar(unitId)->level;
  s32 donation = 0;

  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    donation = level * 20;
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    donation = 10;
  } else if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
    donation = 50;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    donation = level * 10;
  }
  return donation;
}

// Checks whether a member needs the selected sanctum service.
s32 Shop_CanServe(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 eligible = 0;

  if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
      (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
      (serviceKind == SANCTUM_SERVICE_EXORCISE && unit->haunt != 0) ||
      (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->restraint != 0)) {
    eligible = 1;
  }
  return eligible;
}

// Counts party members who need the currently selected sanctum service.
s32 Shop_CountUnits(void) {
  struct ShopRuntime *shop = gMenuWork;
  s32 eligibleCount = 0;
  s32 serviceKind = shop->partyAction;
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind))
      eligibleCount++;
  }
  return eligibleCount;
}

// Selects the matching poison, exorcism or curse-service message from a revive
// message base.
s32 Shop_MsgByMode(s32 messageId) {
  s8 serviceKind = ((struct ShopRuntime *)gMenuWork)->partyAction;

  if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    messageId += MsgCurePoisonService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
    messageId += MsgExorcismService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    messageId += MsgRemoveCurseService - MsgReviveService;
  }
  return messageId;
}

// Resolves service dialogue and waits for completion, including the final
// frame.
void UiMessage_ShowResolvedAndWait(s32 messageId) {
  s32 portraitResource;

  portraitResource = BattleFx_GetResourceIdFar(
      ((struct ShopRuntime *)gMenuWork)->keeperResource);
  UiWork_FinalizePendingCoreFar();
  messageId = Shop_MsgByMode(messageId);
  UiText_OpenMessageWindowFar(messageId, 5, 0,
                              (portraitResource << 0x10) | 0x22);
  while (UiWork_IsCompleteFar() == 0) {
    WaitFrames(1U);
  }
  WaitFrames(1U);
}

// Resolves service dialogue, waits for it and restores the cursor's saved mode.
void UiMessage_ShowResolvedAndRestoreState(s32 messageId) {
  struct ShopRuntime *shop;
  struct RenderOutput **cursorSlot;
  s32 portraitResource;
  u8 savedCursorMode;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->active;
  portraitResource = BattleFx_GetResourceIdFar(shop->keeperResource);
  messageId = Shop_MsgByMode(messageId);
  (*cursorSlot)->active = 13;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(messageId, 5, 0, (portraitResource << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursor.anchor->active = savedCursorMode;
}

// Runs the sanctum service menu using the shared shop cursor and window
// resources.
s32 Shop_ConfirmAct(s32 unitId) {
  s32 serviceSelection = 0;
  struct UiWindow *listWindow = 0;
  struct ShopRuntime *shop;
  struct RenderOutput *cursorAnchor;

  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->partyAction = serviceSelection;

  {
    s32 keeperAnimationId =
        (u16)((struct AnimationObject *)((struct ObjectRuntime *)
                                             Object_GetByIdFar(unitId))
                  ->animation)
            ->entries[0]
            ->animId;
    shop->keeperResource = keeperAnimationId;
  }

  listWindow = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(-5, 0, 5, 5, 2);
  }
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(0, 0, 5, 5, 2);
    SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
  }

  cursorAnchor =
      RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, listWindow, 0, 0);
  cursorAnchor->active = 1;
  cursorAnchor->kind = 0;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  UiMessage_ShowResolvedAndWait(MsgSanctumWelcome);

  shop->moneyWindow = UiWindow_CreateFar(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  for (;;) {
    serviceSelection = Menu_SelectEntry19To1cFar(serviceSelection);
    shop->partyAction = serviceSelection;
    if (serviceSelection == -1)
      break;

    {
      s32 messageBase = MsgReviveService;
      s32 message = messageBase;

      messageBase = 0;
      UiMessage_ShowResolvedAndWait(message);
      if (Shop_CountUnits() == 0) {
        UiMessage_ShowResolvedAndWait(message + 1);
      } else {
        Sanctum_RunPartyService();
      }
    }
    shop->partyAction = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    UiMessage_ShowResolvedAndWait(MsgSanctumMoreAid);
  }

  UiMessage_ShowResolvedAndWait(MsgSanctumFarewell);
  UiWork_FinalizeFar(shop->moneyWindow, 2);
  UiWork_FinalizeFar(listWindow, 2);
  Inn_Cleanup();
  return 0;
}

// Selects an eligible member, confirms the donation and applies the chosen
// service.
s32 Sanctum_RunPartyService(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *priceWindow;
  s32 redraw;
  s32 serviceKind;
  struct UiWindow *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 donation;
  s32 messageBase;

  priceWindow = 0;
  redraw = 1;
  serviceKind = shop->partyAction;
  UiMessage_ShowResolvedAndWait(MsgWhoToRevive);

  listWindow = UiWindow_CreateFar(1, 12, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = redraw;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);
  priceWindow = UiWindow_CreateFar(SANCTUM_PRICE_WINDOW_X, 16,
                                   SANCTUM_PRICE_WINDOW_WIDTH, 3, 2);

  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  while (memberIndex < shop->partyMemberCount) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Shop_CanServe(unitId, serviceKind) != 0)
      break;
    memberIndex++;
  }

  redraw = 1;
  for (;;) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      UiMessage_ShowResolvedAndWait(MsgWhoToRevive);
      redraw = 1;
      memberIndex = 0;
      while (memberIndex < shop->partyMemberCount) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Shop_CanServe(unitId, serviceKind) != 0)
          break;
        memberIndex++;
      }
    }

    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_HiliteUnit(listWindow, memberIndex);
      Shop_DrawSelectMsg(priceWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      donation = Shop_ServicePrice(unitId, serviceKind);
      if (Shop_CanServe(unitId, serviceKind) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      UiWork_PushValueSlotFar(unitId, 1);
      UiWork_PushValueSlotFar(donation, 5);
      messageBase = MsgReviveDonation;
      UiMessage_ShowResolvedAndWait(messageBase);
      if (UiMessage_ShowChoiceVariant(0) != 0) {
        UiMessage_ShowResolvedAndRestoreState(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if (donation > (u32)gGameState.coins) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiMessage_ShowResolvedAndRestoreState(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      UiWork_PushValueSlotFar(unitId, 1);
      UiMessage_ShowResolvedAndWait(messageBase + 3);
      UiWork_FinalizePendingCoreFar();
      // Apply the cure and animation before collecting the donation.
      BattleUnit_ResetStateByMode(unitId, serviceKind);
      Shop_RunPartyMemberIconBurst(memberIndex);
      Party_AdjustCoinsFar(-donation);
      Shop_DrawMoney();
      UiWork_PushValueSlotFar(unitId, 1);
      UiMessage_ShowResolvedAndWait(messageBase + 4);
      if (Shop_CountUnits() != 0) {
        chooseAgain = 1;
        continue;
      }
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex -= 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex += 1;
    }
    WaitFrames(1);
    continue;
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(priceWindow, 2);
  UiWork_FinalizeFar(listWindow, 2);
  WaitFrames(1);
  return 0;
}

// Revives, cures poison, removes a haunting or unequips cursed items for a
// sanctum service.
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit;

  unit = UnitGetFar(unitId);
  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    unit->hp = unit->maxHp;
    UnitRatioFar(unitId);
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    unit->poison = 0;
  } else if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
    unit->haunt = 0;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    s32 inventorySlot;

    for (inventorySlot = 0; inventorySlot < 15; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        if (Item_GetDirectFar(unit->inventory[inventorySlot])->flags &
            ITEM_CURSED) {
          unit->inventory[inventorySlot] ^= INVENTORY_EQUIPPED;
          UnitCalcFar(unitId);
        }
      }
    }
  }
}

// Highlights the selected party icon and dims members who do not need the
// sanctum service.
void Shop_HiliteUnit(struct UiWindow *window, s32 selected) {
  struct ShopRuntime *shop = gMenuWork;
  s32 memberIndex;
  s32 serviceKind = shop->partyAction;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            30);
      else
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind))
        shop->partyMemberScale[memberIndex] = 0xb333;
    }
  }
}

// Shows the selected member's service price or the message that no treatment is
// needed.
void Shop_DrawSelectMsg(struct UiWindow *target, s32 unitId) {
  s32 serviceOrMessage;
  s32 donation;

  serviceOrMessage = ((struct ShopRuntime *)gMenuWork)->partyAction;
  donation = Shop_ServicePrice(unitId, serviceOrMessage);
  if (target != 0) {
    RenderOutput_PrepareForRedrawFar(target);
    if (Shop_CanServe(unitId, serviceOrMessage) != 0) {
      serviceOrMessage = MsgReviveCost;
    } else {
      serviceOrMessage = MsgNoHealingNeeded;
    }
    serviceOrMessage = Shop_MsgByMode(serviceOrMessage);
    UiWork_PushValueSlotFar(donation, 5);
    UiText_DrawResourceFar(serviceOrMessage, target, 0, 0);
  }
}

// Picks random particle positions, moves toward the target and releases
// finished particles.
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect) {
  struct FixedPointPosition position;
  s8 *statePointer;
  s32 state;
  s32 targetPending;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == 0) {
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x280000, Random16(), (s32 *)&position);
    EffectSlot_SetPositionFar(effect, position.x, position.z);
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x40000, Random16(), (s32 *)&position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->maxSpeed = 0x20000;
    effect->acceleration = 0x6666;
    effect->limitTurnStep = state;
    *statePointer = *statePointer + 1;
  } else if (state == 1) {
    targetPending = EffectSlot_HasPendingTargetFar(effect);
    if (targetPending == 0)
      *statePointer = targetPending;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTargetFar(effect) == 0)
      BattleFx_ClearOwnedSlotFar(effect);
  }
}

// Updates all 24 service particles, then randomizes the active member's pose.
void Shop_ResetEffects(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct EffectSlot *effect = shop->effects;
  s32 remainingEffects = 23;
  s8 burstMember;

  do {
    remainingEffects--;
    EffectSlot_UpdateFar(effect);
    effect++;
  } while (remainingEffects >= 0);
  burstMember = shop->burstMember;
  if (burstMember != -1)
    ObjectGroup_SetChildValueUnlessFifteenFar(
        shop->partyMemberIcons[burstMember], (Random16() * 7) >> 16);
}

// Sound effects for each party member action
static const s8 sSanctumServiceCues[4] = {
    0x52,
    0x54,
    0x58,
    0x57,
};

// Animates the selected member and service particles, then restores the menu
// and cursor.
void Shop_RunPartyMemberIconBurst(s32 member) {

  struct ShopRuntime *shop;
  s8 savedCursorMode;
  struct FixedPointPosition position;
  struct EffectSlot *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = gMenuWork;
  savedCursorMode = shop->cursor.anchor->active;
  shop->burstMember = 0xff;
  shop->cursor.anchor->active = 13;
  AudioCommand_PlayFar(sSanctumServiceCues[shop->partyAction]);
  Shop_RestoreSceneTiles(0x00202108);
  AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);
  callbackPriority = 0xc80;
  Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callbackPriority);

  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

  // Spawn eighteen particles while the task continues updating all twenty-four.
  effectIndex = 0;
  effect = shop->effects;
  do {
    EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
    EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
    EffectSlot_SetObjectModeFar(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              (Random16() * 7) >> 16);
    effect->scaleY = 0xb333;
    effect->scaleX = 0xb333;
    WaitFrames(3);
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
    effectIndex++;
    effect++;
  } while (effectIndex <= 17);

  // Let active particles finish, then release remaining slots and restore UI.
  AudioCommand_WaitForStateByteClear();
  {
    u8 activeMode = 2;
    u8 *entry = (u8 *)&shop->effects[0].state;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      if (*(s8 *)(entry + 5) != 0) {
        *entry = activeMode;
      }
      entry += sizeof(struct EffectSlot);
    }
  }

  WaitFrames(20);
  AudioCommand_PlayFar(SE_RECOVERY);
  shop->burstMember = -1;
  ObjectGroup_SetChildValueUnlessFifteenFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct EffectSlot *effectSlot = shop->effects;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += 0x48;
      if (flag != 0) {
        BattleFx_ClearOwnedSlotFar(effectSlot);
      }
      effectSlot++;
    }
  }

  Scheduler_RemoveCallback(Shop_ResetEffects);
  AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 16);
  Shop_InitEffect();
  WaitFrames(30);
  shop->cursor.anchor->active = savedCursorMode;
}

enum InnMessageId {
  INN_MESSAGE_WELCOME = 0xd1c,
  INN_MESSAGE_STAY_COMPLETE,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_GOODBYE,
  INN_MESSAGE_REST_COMPLETE,
};

// Inn price multiplier
// clang-format off
static const s8 sInnPricesPerGuest[12] = {
    2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25,
};
// clang-format on

// Multiplies the inn's price per guest by the number of living party members.
s32 Inn_RoomPrice(s32 innId) {
  struct ShopRuntime *shop = gMenuWork;
  s32 guestCount = 0;
  s32 pricePerGuest = sInnPricesPerGuest[innId];
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (UnitGetFar(shop->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

// Offers an inn stay, checks the fee and restores the innkeeper window after
// resting.
s32 Inn_CheckIn(s32 innId, s32 objectId) {
  struct InnRuntimeState *state;
  struct FieldActor *innkeeper;
  struct UiWindow *innkeeperWindow;
  s32 roomPrice;
  s32 messageBase;

  Shop_InitializeCursorWork();
  state = gMenuWork;
  state->active = 1;
  if (innId == 5)
    state->specialActive = 1;

  innkeeper = Object_GetByIdFar(objectId);
  state->resourceId =
      ((struct AnimationObject *)innkeeper->sprite)->entries[0]->animId;
  innkeeperWindow =
      UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);

  roomPrice = Inn_RoomPrice(innId);
  UiWork_PushValueSlotFar(roomPrice, 5);
  messageBase = MsgInnWelcome;
  UiMessage_ShowAndWait(messageBase);
  state->window = UiWindow_CreateFar(0, 16, SHOP_MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  if (UiMessage_ShowChoice(0) != 0) {
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(state->window, 2);
  } else if (roomPrice > (u32)gGameState.coins) {
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(state->window, 2);
  } else {
    UiWork_FinalizeFar(state->window, 2);
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(innkeeperWindow, 2);
    Inn_PlaySleep(roomPrice);

    innkeeper = Object_GetByIdFar(objectId);
    state->resourceId =
        ((struct AnimationObject *)innkeeper->sprite)->entries[0]->animId;
    innkeeperWindow =
        UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
  }

  UiWork_FinalizeFar(innkeeperWindow, 2);
  Inn_Cleanup();
  return 0;
}

// Charges for the stay, restores living members' HP and PP, then plays the rest
// transition.
void Inn_PlaySleep(s32 roomPrice) {
  s16 partyMemberIds[8];
  s32 memberCount;
  s32 memberIndex;
  struct BattleUnit *unit;
  struct FieldWork *fieldWork;

  memberCount = Party_ListActiveOwnersFar(partyMemberIds);
  // Charge once and restore only party members who are alive.
  Party_AdjustCoinsFar(-roomPrice);

  for (memberIndex = 0; memberIndex < memberCount; memberIndex++) {
    unit = UnitGetFar(partyMemberIds[memberIndex]);
    if (unit->hp != 0) {
      unit->hp = unit->maxHp;
      unit->pp = unit->maxPp;
      UnitRatioFar(partyMemberIds[memberIndex]);
    }
  }

  // Sleep between the two screen-transition halves and the rest sound.
  fieldWork = (struct FieldWork *)gEventWork;
  fieldWork->transition = 0x209;
  fieldWork->transitionFrames = 60;
  WaitFrames(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  AudioCommand_PlayFar(MUS_086);
  AudioCommand_WaitForStateByteClear();
  WaitFrames(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  WaitFrames(30);
  ((struct FieldWork *)gEventWork)->transitionFrames = 16;
}

// Returns the selected party member and inventory slot, or -1 for both when
// cancelled.
s32 Shop_PickUnitItem(s32 *selectedUnit, s32 *selectedSlot) {
  struct ShopRuntime *shop;
  struct RenderOutput *cursorAnchor;
  struct UiWindow *partyWindow;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;
  s32 itemSlot;
  s32 result = 0;

  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, 2);
  partyWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
  cursorAnchor = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000,
                                        partyWindow, 0, result);
  cursorAnchor->active = 4;
  cursorAnchor->kind = result;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  shop->mode = 12;
  PsynergyMenu_InitializeEntryObjectsFar(partyWindow, 2, 0, 8, result);

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(partyWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_DrawParty(partyWindow, memberIndex, 0);
      Shop_DrawUnitGrid(shop->itemWindow, unitId);
    }

    WaitFrames(1);
    if ((gKeysPressed & KEY_A) != 0) {
      if (Inventory_CountFar(unitId) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        continue;
      }

      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      itemSlot = Shop_SelectUse(unitId);
      if (itemSlot == -1) {
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        redraw = 1;
        continue;
      }
      *selectedUnit = unitId;
      *selectedSlot = itemSlot;
      result = 0;
      break;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      *selectedUnit = -1;
      *selectedSlot = -1;
      result = -1;
      break;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      memberIndex++;
      redraw = 1;
    }
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(partyWindow, 2);
  UiWork_FinalizeFar(shop->itemWindow, 2);
  WaitFrames(1);
  Inn_Cleanup();
  return result;
}
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 80
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_GERMAN
#define SHOP_USE_PRICE_LABEL_X 0
#define SHOP_USE_PRICE_NUMBER_X 32
#define SHOP_USE_COIN_LABEL_X 72
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 88
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_FRENCH
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 32
#define SHOP_USE_COIN_LABEL_X 72
#define SHOP_USE_COIN_MESSAGE_OFFSET 5
#else
#define SHOP_USE_PRICE_LABEL_X 8
#define SHOP_USE_PRICE_NUMBER_X 40
#define SHOP_USE_COIN_LABEL_X 80
#define SHOP_USE_COIN_MESSAGE_OFFSET 1
#endif

// Selects a discardable inventory slot, showing the original restriction
// messages on rejection.
s32 Shop_SelectUse(s32 unitId) {
  struct ShopRuntime *shop = gMenuWork;

  struct UiWindow *descriptionWindow;
  struct UiWindow *priceWindow;
  struct BattleUnit *unit;
  s32 slot;
  s32 redraw;
  s32 inventoryCount;
  s32 selectionResult;
  s32 discardStatus;
  s32 itemId;
  struct UiWindow *window;
  s32 x;
  s32 y;

  unit = UnitGetFar(unitId);
  inventoryCount = 1;
  selectionResult = 0;
  redraw = 1;
  priceWindow =
      UiWindow_CreateFar(SHOP_USE_WINDOW_X, 8, SHOP_USE_WINDOW_WIDTH, 4, 2);
  descriptionWindow = UiWindow_CreateFar(0, 5, 30, 3, 2);
  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  slot = 0;

  while (1) {
    if (redraw != 0) {
      redraw = 0;
      inventoryCount = Inventory_CountFar(unitId);
      if (slot > inventoryCount - 1)
        slot = inventoryCount - 1;

      itemId = unit->inventory[slot] & ITEM_ID_MASK;
      window = shop->itemWindow;
      x = slot % SHOP_INVENTORY_COLUMNS << 4;
      y = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
      Shop_PlaceCursor(window, x, y);
      shop->mode = 3;
      Shop_DrawUseItem(priceWindow, unitId, slot);
      Shop_DrawMsg(descriptionWindow, itemId + MsgItemPlainName);
    }

    WaitFrames(1);

    if ((gKeysPressed & KEY_A) != 0) {
      discardStatus = Inventory_CheckDiscardFar(unitId, slot);
      if (discardStatus == 0) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        selectionResult = slot;
        break;
      }
      // BUG: default code selects the error using selectionResult, still zero.
      if (discardStatus == -4) {
        UiText_OpenMessageWindowFar(MsgCannotDropIt, 8, 1, 2);
      } else if (SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) ==
                 -3) {
        UiText_OpenMessageWindowFar(MsgCannotRemoveIt, 8, 1, 2);
      }
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
      UiWork_FinalizePendingCoreFar();
      continue;
    }

    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selectionResult = -1;
      break;
    }

    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      slot -= 1;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      slot += 1;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_UP) != 0) {
      slot -= SHOP_INVENTORY_COLUMNS;
      if (slot < 0)
        slot += INVENTORY_SLOTS;
      while (slot >= inventoryCount)
        slot -= SHOP_INVENTORY_COLUMNS;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
    if ((gKeysRepeat & KEY_DOWN) != 0) {
      slot += SHOP_INVENTORY_COLUMNS;
      if (slot >= inventoryCount)
        slot -= INVENTORY_SLOTS;
      while (slot < 0)
        slot += SHOP_INVENTORY_COLUMNS;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
    }
  }

  UiWork_FinalizeFar(descriptionWindow, 2);
  UiWork_FinalizeFar(priceWindow, 2);
  WaitFrames(1);
  return selectionResult;
}

// Draws the selected slot's item, discard restriction or whole-stack sale
// value.
void Shop_DrawUseItem(struct UiWindow *window, s32 unitId, s32 slot) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 itemId = unit->inventory[slot] & ITEM_ID_MASK;
  u32 inventoryEntry = unit->inventory[slot];
  s32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

  if (window != 0) {
    s32 discardStatus;

    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 0, 0);

    discardStatus = Inventory_CheckDiscardFar(unitId, slot);
    if (discardStatus == -4) {
      UiText_DrawCharacterAtOffsetFar(MsgCannotDrop, window, SHOP_ITEM_NOTE_X,
                                      8);
    } else if (discardStatus == -3) {
      UiText_DrawCharacterAtOffsetFar(MsgCannotRemove, window, SHOP_ITEM_NOTE_X,
                                      8);
    } else {
      s32 unitSalePrice;
      s32 stackSalePrice;

      unitSalePrice = Shop_SalePrice(unit->inventory[slot]);
      stackSalePrice = quantity * unitSalePrice;

      UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window,
                                      SHOP_USE_PRICE_LABEL_X, 8);
      UiText_DrawNumberInWindowFar(stackSalePrice, 5, window,
                                   SHOP_USE_PRICE_NUMBER_X, 8);
      UiText_DrawCharacterAtOffsetFar(MsgPriceHeading -
                                          SHOP_USE_COIN_MESSAGE_OFFSET,
                                      window, SHOP_USE_COIN_LABEL_X, 8);
    }
  }
}
