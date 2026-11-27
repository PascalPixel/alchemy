#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

// The counters of a town, shops, sanctums and inns: SHOP_STATE, the state they share, the shop
// screen's tiles, and every function of the shop object.

#include "types.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "field/field_particle.h"
#include "field/sprite.h"
#include "game/item.h"
#include "system/globals.h"
#include "window/cursor.h"
#include "window/window_sprite.h"

// The ticks the hand cursor takes to glide into a newly opened list.
#define SHOP_CURSOR_ENTER_TICKS 12

// The particles a sanctum service's sparkles use (SHOP_STATE.effects), and how many of them fly in
// at the party member's icon in its burst.
#define SANCTUM_PARTICLE_COUNT  24
#define SANCTUM_BURST_SPARKLES  18

// A shop's state: its windows, the party members' icons, the stock, the cursor and its icons, and
// the field effect sprites.
//
// It is the HEAP_SLOT_MENU block, the same 2,672 bytes as the field menu's state: mnShopInitState
// takes and clears it as a shop, the inn, the sanctum or the item picker opens, and
// mnShopReleaseState frees it.
typedef struct SHOP_STATE
{
	// no view of the menu state names it (see menu/README.md)
	Uint8 unused[8];
	Sint32
		// the menu screens' chosen party member; not named here
				   selectedMember;
	struct WINDOW *moneyWindow; // the coins window
	// The menu screens' first window and their two sides; not named here.
	Uint8          menuSides[16];
	struct WINDOW *itemWindow;
	// The menu screens' windows, icons and cursor grid; not named here.
	Uint8          menuWindows[240];
	SPR           *partyMemberIcons[8]; // the party row's sprites
	Sint16         partyMemberX[8];     // pixels
	Sint16         partyMemberY[8];     // pixels
	FIXED          partyMemberScale[8]; // 16.16 fixed point, toFIXED(1) full size
	// The menu screens' choices, entries and party and the row of four figures; not named here.
	Uint8          menuLists[248];
	Sint16         stockItemIds[129]; // the items on sale, ended by 0
	Sint16         partyMemberIds[9]; // the party in order (gmPartyListMembers)
	MENU_CURSOR    cursor;            // the hand cursor
	// VRAM cache slots of the six cursor icons, loaded when the state is set up and freed with it.
	Uint16         cursorIcon; // the hand
	Uint16         prevPageIcon;
	Uint16         nextPageIcon;
	Uint16         gemIcon;
	Uint16         statUpIcon;
	Uint16         statDownIcon;
	Uint16         unusedBeforeSelection;   // no view of the menu state names it
	Uint16         selectedItem;            // the item being bought, sold or repaired
	Uint8          unusedAfterSelection[4]; // no view of the menu state names it
	Uint16         keeperGraphics;          // the shopkeeper's animation; picks the portrait
	Sint8          stockCount;
	Sint8          partyMemberCount;
	// The ticks the hand cursor's next move takes (mnCursorPlace): SHOP_CURSOR_ENTER_TICKS into a
	// new list, then a few a step.
	Uint8          cursorFrame;
	// The stock row's type (SHOP_TYPE), which picks the keeper's lines; an inn takes a weapon
	// shop's, whose lines are given as they are.
	Uint8          shopType;
	// The counter chosen (SHOP_ACTION), or at a sanctum the service (enum SANCTUM_SERVICE).
	Sint8          action;
	// The party member whose icon flickers through colours in the sanctum's burst, or -1 for none.
	Sint8          burstMember;
	// 1 at Xian's shops and inn, whose keepers have lines of their own.
	Uint8          warriorShop;
	Uint8          padding[3]; // up to the word effects starts on
	// the sanctum service's particles
	FIELD_PARTICLE effects[SANCTUM_PARTICLE_COUNT];
} SHOP_STATE;

STATIC_ASSERT(sizeof(SHOP_STATE) == 2672);
STATIC_ASSERT(offsetof(SHOP_STATE, moneyWindow) == 12);
STATIC_ASSERT(offsetof(SHOP_STATE, itemWindow) == 32);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberIcons) == 276);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberScale) == 340);
STATIC_ASSERT(offsetof(SHOP_STATE, stockItemIds) == 620);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberIds) == 878);
STATIC_ASSERT(offsetof(SHOP_STATE, cursor) == 896);
STATIC_ASSERT(offsetof(SHOP_STATE, stockCount) == 934);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberCount) == 935);
STATIC_ASSERT(offsetof(SHOP_STATE, action) == 938);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberX) == 308);
STATIC_ASSERT(offsetof(SHOP_STATE, partyMemberY) == 324);
STATIC_ASSERT(offsetof(SHOP_STATE, burstMember) == 939);
STATIC_ASSERT(offsetof(SHOP_STATE, warriorShop) == 940);
STATIC_ASSERT(offsetof(SHOP_STATE, effects) == 944);

// The shop's 16-by-16 sprites (graphics/ui/menu_sprites): the hand cursor, the gem and the list
// arrows.
extern const Uint8 MenuCursorHandTiles[];
extern const Uint8 ShopGemTiles[];
extern const Uint8 ShopSmallDownArrowTiles[];
extern const Uint8 ShopSmallUpArrowTiles[];
extern const Uint8 ShopUpArrowTiles[];
extern const Uint8 ShopDownArrowTiles[];

// The eight blank tiles of a price, which the shop writes its digits over, and of the quantity bar,
// which it fills in colours.
extern const Uint8 ShopPriceTiles[];
extern const Uint8 ShopQuantityTiles[];

struct WINDOW;
struct FIELD_PARTICLE;
struct SELECTION_NODE_LINK_VIEW;
struct SELECTION_MENU_CURSOR_VIEW;
struct WINDOW_SPR;
struct MENU_CURSOR;

// The shop module's functions in the order of its stub table (src/shop/stubs.s).
Sint32 mnShopOpenDebugShop(Sint32 unused);
Sint32 mnShopRun(Sint32 stockRow, Sint32 keeperId);
Sint32 mnSanctumRun(Sint32 keeperId);
Sint32 mnInnCheckIn(Sint32 innId, Sint32 actorId);
void   mnCursorAdvance(MENU_CURSOR *cursor);
void   mnCursorMoveTowardTarget(MENU_CURSOR *cursor);
void   mnCursorSet(MENU_CURSOR *cursor, Sint32 targetX, Sint32 targetY, Sint32 ticks);
void   mnCursorSetPosImmediate(MENU_CURSOR *cursor, Sint32 x, Sint32 y);
void   mnQuantityBarFill(Sint32 itemCount, Sint32 color, Uint8 *tileBuf);
void   mnShopSaveAndTintFieldPalette(Sint32 tint);
void   mnShopRestoreFieldPalette(void);
Sint32 mnItemPickerRun(Sint32 *selectedUnit, Sint32 *selectedSlot);
void   mnSoundWaitForJingle(void);
void   mnInnPlaySleep(Sint32 roomPrice);

// A counter's state and the shop's visit, and what every counter shares: the keeper's lines, the
// Yes or No choice and waiting out a jingle (shop_run.c).
Sint32 mnShopListAvailableArtifacts(void);
void   mnCursorStep(void);
void   mnShopInitState(void);
void   mnShopReleaseState(void);
void   mnShopShowKeeperMessage(Sint32 msgId);
void   mnShopShowKeeperMessageHidingCursor(Sint32 msgId);
Sint32 mnShopAskYesNo(Sint32 choice);
Sint32 mnSanctumAskYesNo(Sint32 choice);
struct SELECTION_NODE_LINK_VIEW *
syNodeChainGetNodeAtIndex(struct SELECTION_MENU_CURSOR_VIEW *state);

// The quantity bar's colours (mnQuantityBarFill), painted over each other longest first: past what
// can be chosen, what can be chosen, the number chosen and what is held already. Each segment is a
// black pixel and a pixel of its colour, in the window sprites' palette.
typedef enum QUANTITY_BAR_COL
{
	QUANTITY_BAR_CHOOSABLE    = 0,  // clear, the window showing through
	QUANTITY_BAR_HELD         = 2,  // red
	QUANTITY_BAR_CHOSEN       = 10, // orange
	QUANTITY_BAR_OUT_OF_REACH = 14, // grey
} QUANTITY_BAR_COL;

// The quantity bar and price sprites, the field's tint behind a counter and the hand cursor's moves
// (shop_cursor.c).
void        mnShopDrawPriceDigit(Sint32 digit, Uint8 *tileBuf, Uint32 digitPos);
WINDOW_SPR *mnShopCreatePriceSprite(Sint32 value, struct WINDOW *window, Sint32 x, Sint32 y);
void        mnCursorPlace(struct WINDOW *window, Sint32 x, Sint32 y);

// What mnShopDrawItemPrice says below an item whose price is 0.
typedef enum SHOP_PRICE_KIND
{
	SHOP_PRICE_KIND_BUY    = 0, // buying: the price, even when it is 0
	SHOP_PRICE_KIND_SELL   = 1, // selling: the shop does not buy it
	SHOP_PRICE_KIND_REPAIR = 2  // repairing: it is not broken
} SHOP_PRICE_KIND;

// Buying, and the windows the other counters share: the coins, an item's price and description, the
// party and the quantity bar (shop_buy.c).
Sint32 mnShopSelectBuy(void);
void   mnShopDrawStock(struct WINDOW *window, Sint32 selected);
void   mnShopDrawMoney(void);
void   mnShopDrawItemPrice(struct WINDOW *window, Sint32 itemId, Sint32 price, Sint32 priceKind);
void   mnShopDrawMessage(struct WINDOW *window, Sint32 msg);
void   mnShopDrawParty(struct WINDOW *window, Sint32 selected, Sint32 itemId);
void   mnShopDrawEquipComparison(struct WINDOW *window, Sint32 unitId, Sint32 itemId);
void   mnShopDrawCarriedItem(struct WINDOW *window, Sint32 unitId, Sint32 itemId);
Sint32 mnShopSelectBuyCount(Sint32 unitId, Sint32 itemId);
Sint32 mnShopSelectQuantity(Sint32 minimum, Sint32 maximum, Sint32 price);
void   mnShopBuyDone(Sint32 unitId, Sint32 itemId, Sint32 quantity);
Bool   mnShopConfirmEquip(Sint32 unitId, Sint32 slot);
Bool   mnShopSellOld(Sint32 unitId, Sint32 slot);
Sint32 mnShopSalePrice(Sint32 inventoryEntry);

// Selling, and the party member's item grid (shop_sell.c).
Sint32 mnShopChooseMember(void);
Sint32 mnShopSelectSell(Sint32 unitId);
void   mnShopDrawMemberItems(struct WINDOW *window, Sint32 unitId);
Sint32 mnShopSelectSellCount(Sint32 unitId, Sint32 slot);
void   mnShopSellItem(Sint32 unitId, Sint32 slot, Sint32 count);

// Repairing (shop_repair.c).
Sint32 mnShopRepairPrice(Sint32 inventoryEntry);
Sint32 mnShopRunRepairMemberChoice(void);
Sint32 mnShopSelectRepair(Sint32 unitId);
void   mnShopRepairItem(Sint32 unitId, Sint32 slot);

// The Game Ticket and every shop's stock (shop_stock.c).
void   mnShopGiveGameTicket(struct WINDOW *window, struct WINDOW *itemWindow);
Sint32 mnShopStockGetRowCount(void);
void   mnShopStockAddArtifacts(Sint32 stockRow);
Sint32 mnShopStockCopyItems(Sint32 stockRow, Sint16 *output);
Sint32 mnShopStockGetType(Sint32 stockRow);

// The sanctum (shop_sanctum.c).
Sint32 mnSanctumServicePrice(Sint32 unitId, Sint32 serviceKind);
Sint32 mnSanctumNeedsService(Sint32 unitId, Sint32 serviceKind);
Sint32 mnSanctumCountMembersInNeed(void);
Sint32 mnSanctumGetServiceMessage(Sint32 msgId);
void   mnSanctumShowKeeperMessage(Sint32 msgId);
void   mnSanctumShowKeeperMessageHidingCursor(Sint32 msgId);
Sint32 mnSanctumRunPartyService(void);
void   mnSanctumApplyService(Sint32 unitId, Sint32 serviceKind);
void   mnSanctumHighlightMembers(struct WINDOW *window, Sint32 selected);
void   mnSanctumDrawFee(struct WINDOW *target, Sint32 unitId);
void   mnSanctumUpdateBurstSparkle(FIELD_PARTICLE *effect);
void   mnSanctumUpdateBurst(void);
void   mnSanctumRunMemberIconBurst(Sint32 member);

// The inn (shop_inn.c).
Sint32 mnInnRoomPrice(Sint32 innId);

// The item picker (shop_item_picker.c).
Sint32 mnItemPickerSelectItem(Sint32 unitId);
void   mnItemPickerDrawItem(struct WINDOW *window, Sint32 unitId, Sint32 slot);

#endif
