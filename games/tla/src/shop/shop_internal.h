#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes, its work, the
/// counters and services it runs, the stock table's columns and the tables
/// shop_tables.c holds. shop.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "window/cursor.h"
#include "stddef.h"
#include "static_assert.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "edition.h"
#include "graphics/render_input.h"
#include "system/main_routine_work.h"
#include "battle/battle.h"
#include "window/window.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/scene.h"
#include "system/io_reg.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "version.h"
#include "menu/shop.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "flags.h"
#include "battle/battle_system.h"
#include "game/game_flags.h"
#include "game/owner_state.h"
#include "fixed_math.h"
#include "field/field_event.h"
#include "field/field_sequence.h"
#include "window/item_icon.h"
#include "game/character_ids.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/window.h"
#include "shop/shop.h"
#include "field/field_particle.h"

void WindowSprite_RedrawSavedRectFar(void *window);

/// What the player chose at a shop's counter, in the order of its menu's
/// Buy, Sell, Artifacts and Repair, held in shop->action while it runs. The
/// forge, which The Lost Age adds, runs the sell counter's screens as a
/// counter of its own.
enum ShopAction {
  SHOP_ACTION_BUY,
  SHOP_ACTION_SELL,
  SHOP_ACTION_ARTIFACTS,
  SHOP_ACTION_REPAIR,
  SHOP_ACTION_FORGE
};

/// A shop's type, the last column of its stock row: the equipment group it
/// sells, as Item_GetEquipmentGroup sorts items, or a general store, which
/// The Lost Age adds and which lists artifacts of every group. It picks the
/// artifacts the shop lists and which of the keepers' lines it says: the inn
/// and the forge take the weapon shop's type, whose lines are the messages as
/// given.
enum ShopType {
  SHOP_TYPE_ITEMS = EQUIPMENT_GROUP_NONE,
  SHOP_TYPE_WEAPONS = EQUIPMENT_GROUP_WEAPON,
  SHOP_TYPE_ARMOR = EQUIPMENT_GROUP_ARMOR,
  SHOP_TYPE_GENERAL_STORE = 3
};

/// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts
/// (each list ending with 0 when shorter), then the shop's type.
enum {
  SHOP_STOCK_ITEMS = 0,          ///< the first ordinary item's column
  SHOP_STOCK_ITEM_COUNT = 24,    ///< ordinary items a row holds
  SHOP_STOCK_ARTIFACTS = 24,     ///< the first artifact's column
  SHOP_STOCK_ARTIFACT_COUNT = 8, ///< artifacts a row holds
  SHOP_STOCK_TYPE = 32,          ///< the shop's type (enum ShopType)
  SHOP_STOCK_COLUMNS = 33,
  /// Rows, by the stock row an event passes to Shop_Run; row 0 sells nothing
  /// and stands in for a row out of range.
  SHOP_STOCK_ROWS = 32
};

/// The stock row the shop code itself names: it sells nothing and stands in for
/// a bad row.
#define SHOP_STOCK_ROW_EMPTY 0

enum {
  /// Icons a row in a party member's 15-slot inventory grid, 16 pixels apart.
  SHOP_INVENTORY_COLUMNS = 5,
  /// Stock items a page of the buy screen, 32 pixels apart.
  SHOP_STOCK_PAGE_ITEMS = 7,
  /// Party members a page of the party row, 24 pixels apart.
  SHOP_PARTY_PAGE_MEMBERS = 4,
  /// The most of one item a party member can carry, and so buy at once.
  SHOP_STACK_LIMIT = 30
};

/// What Shop_DrawItemPrice says below an item: its price, or when the price is
/// 0 that the shop does not buy it or that it is not broken; at the forge, the
/// item's condition (Forge_GetItemCondition) in place of a price.
enum ShopPriceKind {
  SHOP_PRICE_KIND_BUY = 0,    ///< buying: the price, even when it is 0
  SHOP_PRICE_KIND_SELL = 1,   ///< selling: the shop does not buy it
  SHOP_PRICE_KIND_REPAIR = 2, ///< repairing: it is not broken
  SHOP_PRICE_KIND_FORGE = 3   ///< forging: "Processed" or "Rusted"
};

/// What the sanctum does for one party member; each kind picks the party
/// member's status it needs, its donation and its cure (shop_sanctum.c).
enum SanctumService {
  SANCTUM_SERVICE_REVIVE,      ///< for a downed party member (HP 0)
  SANCTUM_SERVICE_CURE_POISON, ///< for poison or deadly poison
  SANCTUM_SERVICE_REPEL_EVIL,  ///< for a haunted party member
  SANCTUM_SERVICE_REMOVE_CURSE ///< for any curse bit set
};

/// What a sparkle of the sanctum's burst is doing (FieldParticle.state,
/// Sanctum_UpdateBurstSparkle).
enum SanctumSparkleState {
  /// Placed on a ring around the burst's centre and aimed inside it.
  SANCTUM_SPARKLE_START = 0,
  SANCTUM_SPARKLE_FLYING = 1, ///< flying in; starts again on arrival
  /// Set as the burst ends: freed on arrival.
  SANCTUM_SPARKLE_LAST_FLIGHT = 2
};

/// The sanctum's particle slots (ShopWork.effects), and the sparkles a burst
/// sends of them.
#define SANCTUM_PARTICLE_COUNT 24
#define SANCTUM_BURST_SPARKLES 18

/// What the item picker chooses an item for (ShopWork.itemMode).
enum ItemPickerMode {
  ITEM_PICKER_DROP = 0, ///< an item to drop, by Inventory_CheckDiscard
  ITEM_PICKER_SHOW = 1  ///< an item to show, by Inventory_CheckUnequip
};

/// What the forge does with a material, its recipe's second column; the item
/// list shows it from MsgItemCondition on, after "----------" for an item the
/// forge has no recipe for.
enum ForgeCondition {
  FORGE_CONDITION_PROCESSED = 0, ///< "Processed": a rare material
  FORGE_CONDITION_RUSTED = 1     ///< "Rusted": a rusty weapon
};

/// What the forge makes of one material: one of its results, by weight out of
/// 100. A rusted weapon has one result.
struct ForgeRecipe {
  s16 item;       ///< the material
  s16 condition;  ///< enum ForgeCondition
  s16 results[8]; ///< the items it may become, ended by 0 when fewer
  s16 weights[8]; ///< each result's chance, in percent
};

/// The shop's work (heap slot 220, the menus' work), as The Broken Seal's
/// struct ShopWork, its lists longer here: the windows, the party row, the
/// stock, the hand cursor and its icons, what the player chose, the
/// sanctum service's particles and the item icon grid.
struct ShopWork {
  u8 unused[8];       ///< no view of the menu work names it
  s32 selectedMember; ///< the menu screens' chosen party member; not named here
  void *moneyWindow;  ///< the coins window
  /// The menu screens' first window and their two sides; not named here.
  u8 menuSides[20];
  void *itemWindow;
  /// The menu screens' windows, icons and cursor grid; not named here.
  u8 menuWindows[208];
  void *partyMemberIcons[8]; ///< the party row's sprites
  s16 partyMemberX[8];       ///< pixels
  s16 partyMemberY[8];       ///< pixels
  FIXED partyMemberScale[8]; ///< 16.16 fixed point, FIXED_ONE full size
  /// The menu screens' choices, entries and party and the row of figures; not
  /// named here.
  u8 menuLists[280];
  s16 stockItemIds[301];    ///< the items on sale, ended by 0
  s16 partyMemberIds[9];    ///< the party in order
  struct MenuCursor cursor; ///< the hand cursor
  /// VRAM cache slots of the six cursor icons, loaded when the work is set up
  /// and freed with it.
  ///@{
  u16 cursorIcon; ///< the hand
  u16 previousPageIcon;
  u16 nextPageIcon;
  u16 gemIcon;
  u16 statUpIcon;
  u16 statDownIcon;
  ///@}
  u8 unusedBeforeKeeper[2]; ///< no view of the menu work names it
  u16 keeperGraphics;       ///< the shopkeeper's animation; picks the portrait
  u16 selectedItem;         ///< the item being bought, sold or repaired
  u8 unusedAfterSelection[4]; ///< no view of the menu work names it
  s16 stockCount;
  s8 partyMemberCount;
  u8 cursorFrames; ///< the frames the hand cursor takes to its next place
  s8 shopType;     ///< the stock row's type (enum ShopType)
  s8 action;       ///< the counter or service chosen (enum ShopAction or
                   ///< enum SanctumService)
  s8 burstMember;  ///< the sanctum's party member whose pose is randomized; -1
                   ///< none
  /// ItemMenuWork's: nonzero while Menu_UpdateEntryObjectTransforms places
  /// the party figures; cleared while a details window covers them.
  u8 memberSpritesShown;
  u8 figuresShown; ///< ItemMenuWork's; not named here
  /// The item picker's check: 0 for an item to drop, 1 for one to remove.
  s8 itemMode;
  /// The sanctum service's particles.
  struct FieldParticle effects[SANCTUM_PARTICLE_COUNT];
  u8 menuIcons[72]; ///< ItemMenuWork's five menu icons; not named here
  /// The item icon grid (ItemIconGrid_ShowInventory): a sprite and a VRAM
  /// cache slot (ICON_NONE when empty) for each inventory slot.
  struct IconSprite iconSprites[INVENTORY_SLOTS];
  u16 iconSlots[INVENTORY_SLOTS];
  s8 iconsVisible;  ///< nonzero while ItemIconGrid_Draw queues the icons
  void *listWindow; ///< the party row's window at the sell and forge counters
};

STATIC_ASSERT(offsetof(struct ShopWork, itemWindow) == 36);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIcons) == 248);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberScale) == 312);
STATIC_ASSERT(offsetof(struct ShopWork, stockItemIds) == 624);
STATIC_ASSERT(offsetof(struct ShopWork, partyMemberIds) == 1226);
STATIC_ASSERT(offsetof(struct ShopWork, cursor) == 1244);
STATIC_ASSERT(offsetof(struct ShopWork, keeperGraphics) == 1274);
STATIC_ASSERT(offsetof(struct ShopWork, stockCount) == 1282);
STATIC_ASSERT(offsetof(struct ShopWork, action) == 1287);
STATIC_ASSERT(offsetof(struct ShopWork, effects) == 1292);
STATIC_ASSERT(offsetof(struct ShopWork, iconSprites) == 3092);
STATIC_ASSERT(offsetof(struct ShopWork, iconsVisible) == 3302);
STATIC_ASSERT(offsetof(struct ShopWork, listWindow) == 3304);
STATIC_ASSERT(sizeof(struct ShopWork) == 3308);

/// The quantity cursor's x, in pixels: buy.c and sell.c both place it, further
/// left in the longer languages.
#if LANGUAGE == LANGUAGE_GERMAN
#define SHOP_QUANTITY_CURSOR_X 120
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||           \
    LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_QUANTITY_CURSOR_X 104
#else
#define SHOP_QUANTITY_CURSOR_X 128
#endif

/// The shop's price label, sprite tiles beside an item's price (price.4bpp).
extern const u8 gShop_PriceTiles[];
/// The shop's quantity label, beside the count of an item bought
/// (quantity.4bpp).
extern const u8 gShop_QuantityTiles[];

extern const u16 gShop_SelectorTileOffsets[30];
extern const u16 gShop_PriceDigitTileOffsets[5];
extern const s16 gShop_GameTicketCreditCosts[30];
extern const s16 gShop_StockTable[SHOP_STOCK_ROWS][SHOP_STOCK_COLUMNS];
extern const struct ForgeRecipe gForge_Recipes[];
extern const s8 gSanctum_ServiceTunes[4];
extern const s8 gInn_PricesPerGuest[16];

#endif
