#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/* What every part of the shop object shares: its includes, the shop actions and
   sanctum services, and the values that differ by edition. shop.c includes it
   before the parts, and each part includes it too, so that a part read on its
   own still resolves. */

#include "debug/debug.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "field/scene.h"
#include "graphics/graphics.h"
#include "shop/shop.h"
#include "game/main_routine_state.h"
#include "window/node_chain.h"
#include "text/msg_ids.h"

/* What the player chose at a shop's counter, in the order of its menu's */
/* Buy, Sell, Artifacts and Repair, held in shop->action while it runs. */
typedef enum SHOP_ACTION {
	SHOP_ACTION_BUY,
	SHOP_ACTION_SELL,
	SHOP_ACTION_ARTIFACTS,
	SHOP_ACTION_REPAIR
} SHOP_ACTION;

/* A shop's type, the last column of its stock row: the equipment group it
   sells, as gmItemGetEquipmentGroup sorts items. It picks the artifacts the
   shop lists and which of the keepers' lines it says. */
typedef enum SHOP_TYPE {
	SHOP_TYPE_ITEMS = EQUIPMENT_GROUP_NONE,
	SHOP_TYPE_WEAPONS = EQUIPMENT_GROUP_WEAPON,
	SHOP_TYPE_ARMOR = EQUIPMENT_GROUP_ARMOR
} SHOP_TYPE;

/* The inn the inn's own code names (mnInnCheckIn's innId, a row of
   InnPricesPerGuest); each town's script names its own inn where it opens
   it. */
#define INN_XIAN 5

/* The stock rows the shop code itself names; each town's script names its own
   shops' rows where it opens them. */
typedef enum SHOP_STOCK_ROW {
	SHOP_STOCK_ROW_EMPTY = 0, /* sells nothing; stands in for a bad row */
	SHOP_STOCK_ROW_VALE_WEAPONS = 1,
	SHOP_STOCK_ROW_VALE_ARMOR = 2,
	/* Xian's weapon, armor and item shops, the warriors' shops, which greet the
	   party with lines of their own (SHOP_STATE.warriorShop). */
	SHOP_STOCK_ROW_XIAN_WEAPONS = 16,
	SHOP_STOCK_ROW_XIAN_ARMOR = 17,
	SHOP_STOCK_ROW_XIAN_ITEMS = 18
} SHOP_STOCK_ROW;

/* What a sparkle of the sanctum's burst is doing (FIELD_PARTICLE.state,
   mnSanctumUpdateBurstSparkle). */
typedef enum SANCTUM_SPARKLE_STATE {
	/* Placed on a ring around the burst's centre and aimed inside it. */
	SANCTUM_SPARKLE_START = 0,
	SANCTUM_SPARKLE_FLYING = 1, /* flying in; starts again on arrival */
	/* Set as the burst ends: freed on arrival. */
	SANCTUM_SPARKLE_LAST_FLIGHT = 2
} SANCTUM_SPARKLE_STATE;

/* What the sanctum does for one party member; each kind picks the party
   member's status it needs, its fee and its cure (shop_sanctum.c). */
typedef enum SANCTUM_SERVICE {
	SANCTUM_SERVICE_REVIVE,		 /* for a downed party member (HP 0) */
	SANCTUM_SERVICE_CURE_POISON, /* for poison or deadly poison */
	SANCTUM_SERVICE_REPEL_EVIL,	 /* for a haunted party member */
	SANCTUM_SERVICE_REMOVE_CURSE /* for any curse bit set */
} SANCTUM_SERVICE;

enum {
	/* Icons a row in a party member's 15-slot inventory grid, 16 pixels
	   apart. */
	SHOP_INVENTORY_COLUMNS = 5,
	/* Stock items a page of the buy screen, 32 pixels apart. */
	SHOP_STOCK_PAGE_ITEMS = 7
};

/* BUG: the original item picker tests its selection result rather than the
   discard error, so it never shows the cannot-remove message for error -3. */
#if FIX_DROP_PICKER_CANNOT_REMOVE_MESSAGE
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) \
	(discardStatus)
#else
#define SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) \
	(selectionResult)
#endif

/* Edition-specific text and window layout values. Window positions and sizes
   are in tiles, text positions in pixels. */

/* The shop's Yes/No menu (shop_run.c): one column further left in German. */
#define SHOP_CONFIRM_MENU_X GERMAN_OR(6, 7)

/* An item's price and the word after it in the price window that the buy, sell
   and repair lists share (mnShopDrawItemPrice, shop_buy.c), in pixels: French
   moves both 8 left, Spanish the word 8 right. */
#define SHOP_PRICE_NUMBER_X FRENCH_OR(24, 32)
#define SHOP_PRICE_LABEL_X	BY_LANGUAGE(72, 72, 72, 80, 64, 72)

/* Where the item grid (mnShopDrawMemberItems, shop_sell.c), shared by the sell,
   repair and drop screens and the equip prompt, writes MsgNoItems for a party
   member with no items, in pixels. */
#define SHOP_EMPTY_GRID_X ITALIAN_OR(20, 8)
#define SHOP_EMPTY_GRID_Y ITALIAN_OR(12, 20)

/* Outside Japan the coins window (SHOP_STATE.moneyWindow) is a tile wider, and
   every counter's item description (mnShopDrawMessage, shop_buy.c) is drawn as
   tiles (mnTextDrawMessageAsTilesStub) rather than into a text canvas. */
#define SHOP_MONEY_WINDOW_WIDTH JAPANESE_OR(11, 12)
#define SHOP_DRAW_MESSAGE \
	JAPANESE_OR(mnTextDrawMessageStub, mnTextDrawMessageAsTilesStub)

/* The sanctum's fee window (shop_sanctum.c), wider for the German and French
   text. */
#define SANCTUM_PRICE_WINDOW_X	   GERMAN_OR(0, 1)
#define SANCTUM_PRICE_WINDOW_WIDTH BY_LANGUAGE(23, 23, 30, 23, 25, 23)

/* The item picker's small window (shop_item_picker.c): two tiles wider, and two
   further left, outside Japan. */
#define ITEM_PICKER_WINDOW_X	 JAPANESE_OR(16, 14)
#define ITEM_PICKER_WINDOW_WIDTH JAPANESE_OR(14, 16)

/* The x of the note below an item's name, in pixels: shop_buy.c and
   shop_item_picker.c both draw it. */
#define SHOP_ITEM_NOTE_X JAPANESE_OR(8, 0)

/* The quantity cursor's x, in pixels: shop_buy.c and shop_sell.c both place
   it. */
#define SHOP_QUANTITY_CURSOR_X BY_LANGUAGE(128, 128, 120, 128, 120, 128)

#endif
