#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes, the shop actions
/// and sanctum services, and the values that differ by edition. shop.c
/// includes it before the parts, and each part includes it too, so that a
/// part read on its own still resolves.

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
#include "field_offset.h"
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
#include "object_event.h"
#include "heap_state.h"
#include "flags.h"

/// What the player chose at a shop's counter, in the order of its menu's
/// Buy, Sell, Artifacts and Repair, held in shop->action while it runs.
enum ShopAction {
  SHOP_ACTION_BUY,
  SHOP_ACTION_SELL,
  SHOP_ACTION_ARTIFACTS,
  SHOP_ACTION_REPAIR
};

/// A shop's type, the last column of its stock row: the equipment group it
/// sells, as Item_GetEquipmentGroup sorts items. It picks the artifacts the
/// shop lists and which of the keepers' lines it says.
enum ShopType {
  SHOP_TYPE_ITEMS = EQUIPMENT_GROUP_NONE, ///< an item shop
  SHOP_TYPE_WEAPONS = EQUIPMENT_GROUP_WEAPON,
  SHOP_TYPE_ARMOR = EQUIPMENT_GROUP_ARMOR
};

/// The inn the inn's own code names (Inn_CheckIn's innId, a row of
/// sInnPricesPerGuest); each town's map script names its own inn where it opens
/// it.
#define INN_XIAN 5

/// The stock rows the shop code itself names; each town's map script names its
/// own shops' rows where it opens them.
enum ShopStockRow {
  SHOP_STOCK_ROW_EMPTY = 0,        ///< sells nothing; stands in for a bad row
  SHOP_STOCK_ROW_VALE_WEAPONS = 1, ///< Vale's weapon shop
  SHOP_STOCK_ROW_VALE_ARMOR = 2,   ///< Vale's armor shop
  /// Xian's weapon, armor and item shops, the warriors' shops, which greet
  /// the party with lines of their own (ShopWork.warriorShop).
  SHOP_STOCK_ROW_XIAN_WEAPONS = 16,
  SHOP_STOCK_ROW_XIAN_ARMOR = 17,
  SHOP_STOCK_ROW_XIAN_ITEMS = 18
};

/// What a sparkle of the sanctum's burst is doing (FieldEffectSprite.state,
/// Sanctum_UpdateBurstSparkle).
enum SanctumSparkleState {
  /// Placed on a ring around the burst's centre and aimed inside it.
  SANCTUM_SPARKLE_START = 0,
  SANCTUM_SPARKLE_FLYING = 1, ///< flying in; starts again on arrival
  /// Set as the burst ends: freed on arrival.
  SANCTUM_SPARKLE_LAST_FLIGHT = 2
};

/// What the sanctum does for one party member; each kind picks the member's
/// status it needs, its fee and its cure (sanctum.c).
enum SanctumService {
  SANCTUM_SERVICE_REVIVE,      ///< for a downed member (HP 0)
  SANCTUM_SERVICE_CURE_POISON, ///< for poison or deadly poison
  SANCTUM_SERVICE_REPEL_EVIL,  ///< for a haunted member
  SANCTUM_SERVICE_REMOVE_CURSE ///< for any curse bit set
};

enum {
  /// Icons a row in a member's 15-slot inventory grid, 16 pixels apart.
  SHOP_INVENTORY_COLUMNS = 5,
  /// Stock items a page of the buy screen, 32 pixels apart.
  SHOP_STOCK_PAGE_ITEMS = 7
};

// BUG: the original item picker tests its selection result rather than the
// discard error, so it never shows the cannot-remove message for error -3.
#ifdef BUGFIX
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult)              \
  (discardStatus)
#else
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult)              \
  (selectionResult)
#endif

// Edition-specific text and window layout values. Window positions and sizes
// are in tiles, text positions in pixels.

// The shop's Yes/No menu (run.c): one column further left in German.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define SHOP_CONFIRM_MENU_X 6
#else
#define SHOP_CONFIRM_MENU_X 7
#endif

// An item's price and the word after it in the price window that the buy,
// sell and repair lists share (Shop_DrawItemPrice, buy.c), in pixels: French
// moves both 8 left, Spanish the word 8 right.
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

// Where the item grid (Shop_DrawMemberItems, sell.c), shared by the sell,
// repair and drop screens and the equip prompt, writes MsgNoItems for a member
// with no items, in pixels.
#if (LANGUAGE == LANGUAGE_ITALIAN)
#define SHOP_EMPTY_GRID_X 20
#define SHOP_EMPTY_GRID_Y 12
#else
#define SHOP_EMPTY_GRID_X 8
#define SHOP_EMPTY_GRID_Y 20
#endif

// Outside Japan the coins window is a tile wider, and every counter's item
// description (Shop_DrawMessage, buy.c) is drawn as tiles
// (Text_DrawMessageAsTilesFar) rather than into a text canvas.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SHOP_MONEY_WINDOW_WIDTH 12
#define SHOP_DRAW_MESSAGE Text_DrawMessageAsTilesFar
#else
#define SHOP_MONEY_WINDOW_WIDTH 11
#define SHOP_DRAW_MESSAGE Text_DrawMessageFar
#endif

// The sanctum's fee window (sanctum.c), wider for the German and French text.
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

// The item picker's small window (item_picker.c): two tiles wider, and two
// further left, outside Japan.
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_PICKER_WINDOW_X 14
#define ITEM_PICKER_WINDOW_WIDTH 16
#else
#define ITEM_PICKER_WINDOW_X 16
#define ITEM_PICKER_WINDOW_WIDTH 14
#endif

// The x of the note below an item's name, in pixels: buy.c and
// item_picker.c both draw it.
#if LANGUAGE != LANGUAGE_JAPANESE
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
