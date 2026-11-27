#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/* What every part of the shop object shares: its includes, its state, the
   counters and services it runs, the stock table's columns and the tables
   shop_tables.c holds. shop.c includes it before the parts, and each part
   includes it too, so that a part read on its own still resolves. It places no
   bytes. */

#include "types.h"
#include "window/cursor.h"
#include "stddef.h"
#include "static_assert.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "graphics/resource.h"
#include "edition.h"
#include "graphics/render_input.h"
#include "battle/battle.h"
#include "window/window.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "text/msg_ids.h"
#include "menu/shop.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "flags.h"
#include "battle/battle_system.h"
#include "game/game_flags.h"
#include "game/class_djinn.h"
#include "fixed_math.h"
#include "field/field_script.h"
#include "field/field_mode.h"
#include "window/item_icon.h"
#include "game/character_ids.h"
#include "game/stubs.h"
#include "menu/stubs.h"
#include "window/stubs.h"
#include "shop/shop.h"
#include "field/field_particle.h"

void mnWindowSprRedrawSavedRectStub(void *window);

/* What the player chose at a shop's counter, in the order of its menu's */
/* Buy, Sell, Artifacts and Repair, held in shop->action while it runs. The
   forge, which The Lost Age adds, runs the sell counter's screens as a counter
   of its own. */
typedef enum SHOP_ACTION {
	SHOP_ACTION_BUY,
	SHOP_ACTION_SELL,
	SHOP_ACTION_ARTIFACTS,
	SHOP_ACTION_REPAIR,
	SHOP_ACTION_FORGE
} SHOP_ACTION;

/* A shop's type, the last column of its stock row: the equipment group it
   sells, as gmItemGetEquipmentGroup sorts items, or a general store, which The
   Lost Age adds and which lists artifacts of every group. It picks the
   artifacts the shop lists and which of the keepers' lines it says: the inn and
   the forge take the weapon shop's type, whose lines are the messages as
   given. */
typedef enum SHOP_TYPE {
	SHOP_TYPE_ITEMS = EQUIPMENT_GROUP_NONE,
	SHOP_TYPE_WEAPONS = EQUIPMENT_GROUP_WEAPON,
	SHOP_TYPE_ARMOR = EQUIPMENT_GROUP_ARMOR,
	SHOP_TYPE_GENERAL_STORE = 3,
	/* Unused: lists every group's artifacts, as a general store does, but no
	   stock row has it and nothing else sets it. */
	SHOP_TYPE_EVERY_GROUP = 4
} SHOP_TYPE;

/* The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts
   (each list ending with 0 when shorter), then the shop's type. */
enum {
	SHOP_STOCK_ITEMS = 0,		   /* the first ordinary item's column */
	SHOP_STOCK_ITEM_COUNT = 24,	   /* ordinary items a row holds */
	SHOP_STOCK_ARTIFACTS = 24,	   /* the first artifact's column */
	SHOP_STOCK_ARTIFACT_COUNT = 8, /* artifacts a row holds */
	SHOP_STOCK_TYPE = 32,		   /* the shop's type (SHOP_TYPE) */
	SHOP_STOCK_COLUMNS = 33,
	/* Rows, by the stock row a script passes to mnShopRun; row 0 sells nothing
	   and stands in for a row out of range. */
	SHOP_STOCK_ROWS = 32
};

/* The stock row the shop code itself names: it sells nothing and stands in for
   a bad row. */
#define SHOP_STOCK_ROW_EMPTY 0

enum {
	/* Icons a row in a party member's 15-slot inventory grid, 16 pixels
	   apart. */
	SHOP_INVENTORY_COLUMNS = 5,
	/* Stock items a page of the buy screen, 32 pixels apart. */
	SHOP_STOCK_PAGE_ITEMS = 7,
	/* Party members a page of the party row, 24 pixels apart. */
	SHOP_PARTY_PAGE_MEMBERS = 4,
	/* The most of one item a party member can carry, and so buy at once. */
	SHOP_STACK_LIMIT = 30
};

/* What mnGridMoveCursor reports, and so what a list redraws. */
typedef enum MENU_GRID_MOVE {
	MENU_GRID_STAYED = 0,
	MENU_GRID_MOVED = 1, /* within its page: the cursor and the entry */
	/* To another page: the page's party figures are made again too. */
	MENU_GRID_CHANGED_PAGE = 2
} MENU_GRID_MOVE;

/* What mnShopDrawItemPrice says below an item: its price, or when the price is
   0 that the shop does not buy it or that it is not broken; at the forge, the
   item's condition (mnForgeGetItemCondition) in place of a price. */
typedef enum SHOP_PRICE_KIND {
	SHOP_PRICE_KIND_BUY = 0,	/* buying: the price, even when it is 0 */
	SHOP_PRICE_KIND_SELL = 1,	/* selling: the shop does not buy it */
	SHOP_PRICE_KIND_REPAIR = 2, /* repairing: it is not broken */
	SHOP_PRICE_KIND_FORGE = 3	/* forging: "Processed" or "Rusted" */
} SHOP_PRICE_KIND;

/* What the sanctum does for one party member; each kind picks the party
   member's status it needs, its donation and its cure (shop_sanctum.c). */
typedef enum SANCTUM_SERVICE {
	SANCTUM_SERVICE_REVIVE,		 /* for a downed party member (HP 0) */
	SANCTUM_SERVICE_CURE_POISON, /* for poison or deadly poison */
	SANCTUM_SERVICE_REPEL_EVIL,	 /* for a haunted party member */
	SANCTUM_SERVICE_REMOVE_CURSE /* for any curse bit set */
} SANCTUM_SERVICE;

/* What a sparkle of the sanctum's burst is doing (FIELD_PARTICLE.state,
   mnSanctumUpdateBurstSparkle). */
typedef enum SANCTUM_SPARKLE_STATE {
	/* Placed on a ring around the burst's centre and aimed inside it. */
	SANCTUM_SPARKLE_START = 0,
	SANCTUM_SPARKLE_FLYING = 1, /* flying in; starts again on arrival */
	/* Set as the burst ends: freed on arrival. */
	SANCTUM_SPARKLE_LAST_FLIGHT = 2
} SANCTUM_SPARKLE_STATE;

/* The sanctum's particle slots (SHOP_STATE.effs), and the sparkles a burst
   sends of them. */
#define SANCTUM_PARTICLE_COUNT 24
#define SANCTUM_BURST_SPARKLES 18

/* What the item picker chooses an item for (SHOP_STATE.itemMode). */
typedef enum ITEM_PICKER_MODE {
	ITEM_PICKER_DROP = 0, /* an item to drop, by gmInventoryCheckDiscard */
	ITEM_PICKER_SHOW = 1  /* an item to show, by gmInventoryCheckUnequip */
} ITEM_PICKER_MODE;

/* What the forge does with a material, its recipe's second column; the item
   list shows it from MsgItemCondition on, after "----------" for an item the
   forge has no recipe for. */
typedef enum FORGE_CONDITION {
	FORGE_CONDITION_PROCESSED = 0, /* "Processed": a rare material */
	FORGE_CONDITION_RUSTED = 1	   /* "Rusted": a rusty weapon */
} FORGE_CONDITION;

/* What the forge makes of one material: one of its results, by weight out of
   100. A rusted weapon has one result. */
typedef struct FORGE_RECIPE {
	Sint16 item;	   /* the material */
	Sint16 condition;  /* FORGE_CONDITION */
	Sint16 results[8]; /* the items it may become, ended by 0 when fewer */
	Sint16 weights[8]; /* each result's chance, in percent */
} FORGE_RECIPE;

/* The shop's state (heap slot 220, the menus' state), as The Broken Seal's
   SHOP_STATE, its lists longer here: the windows, the party row, the stock, the
   hand cursor and its icons, what the player chose, the sanctum service's
   particles and the item icon grid. */
typedef struct SHOP_STATE {
	Uint8 unused[8]; /* no view of the menu state names it */
	Sint32
		/* the menu screens' chosen party member; not named here */
		  selectedMember;
	void *moneyWin; /* the coins window */
	/* The menu screens' first window and their two sides; not named here. */
	Uint8 menuSides[20];
	void *itemWin;
	/* The menu screens' windows, icons and cursor grid; not named here. */
	Uint8  menuWins[208];
	void  *partyMemberIcons[8]; /* the party row's sprites */
	Sint16 partyMemberX[8];		/* pixels */
	Sint16 partyMemberY[8];		/* pixels */
	FIXED  partyMemberScale[8]; /* 16.16 fixed point, toFIXED(1) full size */
	/* The menu screens' choices, entries and party and the row of figures; not
	   named here. */
	Uint8		menuLists[280];
	Sint16		stockItemIds[301]; /* the items on sale, ended by 0 */
	Sint16		partyMemberIds[9]; /* the party in order */
	MENU_CURSOR cursor;			   /* the hand cursor */
	/* VRAM cache slots of the six cursor icons, loaded when the state is set up
	   and freed with it. */
	Uint16 cursorIcon; /* the hand */
	Uint16 prevPageIcon;
	Uint16 nextPageIcon;
	Uint16 gemIcon;
	Uint16 statUpIcon;
	Uint16 statDownIcon;
	Uint8  unusedBeforeKeeper[2]; /* no view of the menu state names it */
	Uint16 keeperGraphics; /* the shopkeeper's animation; picks the portrait */
	Uint16 selectedItem;   /* the item being bought, sold or repaired */
	Uint8  unusedAfterSelection[4]; /* no view of the menu state names it */
	Sint16 stockCnt;
	Sint8  partyMemberCnt;
	Uint8  cursorFrm; /* the frames the hand cursor takes to its next place */
	Sint8  shopType;  /* the stock row's type (SHOP_TYPE) */
	Sint8  action;	  /* the counter or service chosen (SHOP_ACTION or */
	/* SANCTUM_SERVICE) the sanctum's party member whose pose is randomized;
	   -1 */
	Sint8 burstMember;
	/* none ITEM_MENU_STATE's: nonzero while mnUpdatePartyFigures places the
	   party figures; cleared while a details window covers them. */
	Uint8 memberSprsShown;
	Uint8 figuresShown; /* ITEM_MENU_STATE's; not named here */
	/* The item picker's check: 0 for an item to drop, 1 for one to remove. */
	Sint8 itemMode;
	/* The sanctum service's particles. */
	FIELD_PARTICLE effs[SANCTUM_PARTICLE_COUNT];
	Uint8 menuIcons[72]; /* ITEM_MENU_STATE's five menu icons; not named here */
	/* The item icon grid (mnItemIconGridShowInventory): a sprite and a VRAM
	   cache slot (ICON_NONE when empty) for each inventory slot. */
	ICON_SPR iconSprs[INVENTORY_SLOTS];
	Uint16	 iconSlots[INVENTORY_SLOTS];
	Sint8 iconsVisible; /* nonzero while mnItemIconGridDraw queues the icons */
	void *listWin; /* the party row's window at the sell and forge counters */
} SHOP_STATE;

STATIC_ASSERT(offsetof(SHOP_STATE, itemWin) == 36);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberIcons) == 248);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberScale) == 312);
STATIC_ASSERT(offsetof(SHOP_STATE, stockItemIds) == 624);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberIds) == 1226);
STATIC_ASSERT(offsetof(SHOP_STATE, cursor) == 1244);
STATIC_ASSERT(offsetof(SHOP_STATE, keeperGraphics) == 1274);
STATIC_ASSERT(offsetof(SHOP_STATE, stockCnt) == 1282);
STATIC_ASSERT(offsetof(SHOP_STATE, action) == 1287);
STATIC_ASSERT(offsetof(SHOP_STATE, effs) == 1292);
STATIC_ASSERT(offsetof(SHOP_STATE, iconSprs) == 3092);
STATIC_ASSERT(offsetof(SHOP_STATE, iconsVisible) == 3302);
STATIC_ASSERT(offsetof(SHOP_STATE, listWin) == 3304);
STATIC_ASSERT(sizeof(SHOP_STATE) == 3308);

/* The quantity cursor's x, in pixels: buy.c and sell.c both place it, further
   left in the longer languages. */
#if LANGUAGE == LANGUAGE_GERMAN
#define SHOP_QUANTITY_CURSOR_X 120
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_QUANTITY_CURSOR_X 104
#else
#define SHOP_QUANTITY_CURSOR_X 128
#endif

/* The shop's price label, sprite tiles beside an item's price (price.4bpp). */
extern const Uint8 Shop_PriceTiles[];
/* The shop's quantity label, beside the count of an item bought
   (quantity.4bpp). */
extern const Uint8 Shop_QuantityTiles[];

extern const Uint16		  Shop_QuantityBarSegmentOffsets[30];
extern const Uint16		  Shop_PriceDigitTileOffsets[5];
extern const Sint16		  Shop_GameTicketCreditCosts[30];
extern const Sint16		  Shop_StockTable[SHOP_STOCK_ROWS][SHOP_STOCK_COLUMNS];
extern const FORGE_RECIPE Forge_Recipes[];
extern const Sint8		  Sanctum_ServiceTunes[4];
extern const Sint8		  Inn_PricesPerGuest[16];

#endif
