#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

// What every part of the shop object shares: its includes, the shop actions
// and sanctum services, and the values that differ by edition. shop.c
// includes it before the parts, and each part includes it too, so that a
// part read on its own still resolves.

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
#include "heap_state.h"

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

// The x of the note below an item's name, in pixels: buy.c and
// item_picker.c both draw it.
#if TBS_PATCH_1
#define SHOP_ITEM_NOTE_X 0
#else
#define SHOP_ITEM_NOTE_X 8
#endif

// The quantity cursor's x, in pixels: buy.c and sell.c both place it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define SHOP_QUANTITY_CURSOR_X 120
#else
#define SHOP_QUANTITY_CURSOR_X 128
#endif

#endif
