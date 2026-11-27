#ifndef ALCHEMY_SHOP_SHOP_H
#define ALCHEMY_SHOP_SHOP_H

/* What the shop object defines for the code outside it. */

#include "types.h"

struct WINDOW;
struct MENU_CURSOR;
struct WINDOW_SPR;

/* The colours of a quantity bar's segments. mnQuantityBarFill paints the first
   so many segments in one, and the bar is built by painting shorter runs over
   longer: the whole bar out of reach, then what could be chosen, then what is
   chosen, then what the party member already carries. */
#define QUANTITY_BAR_OUT_OF_REACH 14
#define QUANTITY_BAR_OPEN		  0
#define QUANTITY_BAR_CHOSEN		  10
#define QUANTITY_BAR_HELD		  2

/* Rows of the shop stock table, as a script passes them to mnShopRun: Daila's
   weapon, armor and item shops, and Izumo's general store, which the debug
   rooms' shopkeepers also open. A town no other room opens a shop of names its
   rows itself. */
enum {
	SHOP_DAILA_WEAPONS = 1,
	SHOP_DAILA_ARMOR = 2,
	SHOP_DAILA_ITEMS = 3,
	SHOP_IZUMO = 24
};

/* The lines of one shop, as many as the weapon shop's, and of a sanctum, as the
   debug rooms' message browsers count them (text/msg_ids.h). */
#define SHOP_MESSAGES	 (MsgArmorShopWelcome - MsgWeaponShopWelcome)
#define SANCTUM_MESSAGES (MsgRightWhenI - MsgWelcomeWearyWanderers)

/* Daila's inn, as mnInnCheckIn numbers the inns, which the debug rooms'
   innkeepers also open. A town no other room opens an inn of names its inn
   itself. */
enum { DAILA_INN = 0 };

void   mnShopInitState(void);
void   mnShopReleaseState(void);
Sint32 mnShopSelectBuy(void);
void   mnShopDrawStock(struct WINDOW *win, Sint32 selected);
void   mnShopDrawItemPrice(
	  struct WINDOW *win, Sint32 itemId, Sint32 price, Sint32 priceKind);
void mnShopDrawMessage(struct WINDOW *win, Sint32 msg);
void mnShopDrawParty(struct WINDOW *win, Sint32 selected, Sint32 itemId);
void mnShopDrawEquipComparison(
	struct WINDOW *win, Sint32 unitId, Sint32 itemId);
void   mnShopDrawCarriedItem(struct WINDOW *win, Sint32 unitId, Sint32 itemId);
Sint32 mnShopSelectBuyCount(Sint32 unitId, Sint32 itemId);
Sint32 mnShopSelectQuantity(Sint32 minimum, Sint32 maximum, Sint32 price);
void   mnShopBuyDone(Sint32 unitId, Sint32 itemId, Sint32 quantity);
Bool   mnShopConfirmEquip(Sint32 unitId, Sint32 slot);
Bool   mnShopSellOld(Sint32 unitId, Sint32 slot);
void   mnShopGiveGameTicket(struct WINDOW *win, struct WINDOW *itemWin);
Sint32 mnShopChooseMember(void);
Sint32 mnShopSelectSell(Sint32 unitId);
void   mnShopDrawMemberItems(struct WINDOW *win, Sint32 unitId);
Sint32 mnShopSelectSellCount(Sint32 unitId, Sint32 slot);
void   mnShopSellItem(Sint32 unitId, Sint32 slot, Sint32 cnt);
Sint32 mnShopRepairPrice(Sint32 inventoryEntry);
Sint32 mnShopRunRepairMemberChoice(void);
Sint32 mnShopSelectRepair(Sint32 unitId);
void   mnShopRepairItem(Sint32 unitId, Sint32 slot);
void   mnShopShowKeeperMessage(Sint32 msgId);
struct WINDOW_SPR *mnShopCreatePriceSpr(
	Sint32 value, struct WINDOW *win, Sint32 x, Sint32 y);

Sint32 mnShopStockGetRowCount(void);
Sint32 mnShopStockGetType(Sint32 stockRow);
void   mnShopStockAddArtifacts(Sint32 stockRow);
Sint32 mnShopStockCopyItems(Sint32 stockRow, Sint16 *output);

Sint32 mnInnRoomPrice(Sint32 innId);

Sint32 mnForgeChooseMember(void);
Sint32 mnForgeSelectItem(Sint32 unitId);
Sint32 mnForgeGetItemCondition(Sint32 itemId);
Sint32 mnForgeOfferItem(Sint32 unitId, Sint32 slot);

Sint32 mnSanctumRunPartyService(void);
void   mnSanctumHighlightMembers(struct WINDOW *win, Sint32 selected);
void   mnSanctumDrawFee(struct WINDOW *win, Sint32 unitId);
void   mnSanctumApplyService(Sint32 unitId, Sint32 serviceKind);
void   mnSanctumRunMemberIconBurst(Sint32 member);

Sint32 mnItemPickerRun(
	Sint32 *selectedUnit, Sint32 *selectedSlot, Sint32 itemMode);
Sint32 mnItemPickerSelectItem(Sint32 unitId);
void   mnItemPickerDrawItem(struct WINDOW *win, Sint32 unitId, Sint32 slot);
void   mnItemIconGridInit(void);
void   mnItemIconGridRelease(void);
void   mnItemIconGridShowInventory(
	  struct WINDOW *win, Sint32 x, Sint32 y, Sint32 unitId);

/* The ticks the hand cursor takes to glide into a newly opened list. */
#define SHOP_CURSOR_ENTER_TICKS 12

/* The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price
   sprite's tiles (mnShopDrawPriceDigit). */
extern Uint8 ShopGlyphBytes[];
Sint32		 mnSanctumNeedsService(Sint32 unitId, Sint32 serviceKind);

struct MENU_CURSOR;

/* The shop module's functions in the order of its stub table. */
/* Unused: opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at its
   cap, broken equipment for repairs and poisoned or haunted party members for
   the sanctum, on the empty stock row. Nothing calls it or its stub call
   export; The Broken Seal's debug menu opened its own. */
Sint32 mnShopOpenDebugShop(void);
/* The shops, inns, sanctums and forges as scripts open them. */
Sint32 mnShopRun(Sint32 stockRow, Sint32 keeperId);
/* Runs a sanctum: the keeper's welcome, the service menu until B, and the
   farewell; `actorId` is the keeper, whose portrait the lines show. */
Sint32 mnSanctumRun(Sint32 actorId);
/* Runs an inn's counter: the innkeeper names the price, and on Yes the party
   sleeps. `innId` is the row of InnPricesPerGuest and `actorId` the innkeeper,
   whose portrait the lines show. */
Sint32 mnInnCheckIn(Sint32 innId, Sint32 actorId);
/* Runs the forge: the smith's line, then a party member's materials to forge,
   until the player leaves. `actorId` is the actor whose portrait the messages
   show. Called through the stub table; returns 0. */
Sint32 mnForgeRun(Sint32 actorId);
/* Returns the night's price at inn `innId` with the shop's state opened for it,
   for a script to quote before checking in. */
Sint32 mnInnQuoteRoomPrice(Sint32 innId);
/* Moves the cursor a tick along its straight line toward its target. */
void mnCursorAdvance(struct MENU_CURSOR *cursor);
/* Moves each cursor axis one quarter of the remaining distance, with a minimum
   one-pixel step. */
void mnCursorMoveTowardTarget(struct MENU_CURSOR *cursor);
/* Starts a glide of the cursor from where it is to (tgtX, tgtY) over
   `ticks` ticks, for mnCursorAdvance to run. */
void mnCursorSet(
	struct MENU_CURSOR *cursor, Sint32 tgtX, Sint32 tgtY, Sint8 ticks);
/* Places the cursor and its sprite at (x, y) at once, with a one-tick glide
   that has already arrived. */
void mnCursorSetPosImmediate(struct MENU_CURSOR *cursor, Sint32 x, Sint32 y);
/* Paints the first `segments` segments of the quantity bar in `col` (a
   QUANTITY_BAR_ colour), over what was painted before. */
void mnQuantityBarFill(Sint32 segments, Sint32 col, Uint8 *tileBuf);
/* Saves the field's colours and fades them to `tint` (a PALETTE_TINT_ mode)
   over SHOP_TINT_TICKS ticks: the grey of the sanctum's healing, and the
   element's colour as a Djinni joins the party. */
void mnShopSaveAndTintFieldPalette(Sint32 tint);
/* Fades back to the colours mnShopSaveAndTintFieldPalette saved, over
   SHOP_TINT_TICKS ticks. */
void mnShopRestoreFieldPalette(void);
/* Runs the item picker for an item to drop, showing each item's sale price. */
Sint32 mnItemPickerRunToDrop(Sint32 *selectedUnit, Sint32 *selectedSlot);
/* Runs the item picker for an item to show, refusing one that cannot be
   unequipped: Naribwe's fortune teller reads the party's fortune from it. */
Sint32 mnItemPickerRunToShow(Sint32 *selectedUnit, Sint32 *selectedSlot);
/* Waits until the jingle playing has ended and the music it interrupted is
   back: the inn's night, the sanctum's healing and scripts that wait for a
   jingle. */
void mnSndWaitForJingle(void);
/* Charges the room and restores the HP and PP of each party member still
   standing over a night: a fade to black, the inn's music and a fade back. */
void mnInnPlaySleep(Sint32 roomPrice);
/* Decompresses resource `resourceId` and loads its first `size` bytes into VRAM
   cache entry `entryId`; does nothing for an entry past the cache. */
void mnVRAMBlockLoadResource(Sint32 entryId, Sint32 size, Sint32 resourceId);
/* Returns 1 when anyone in the party carries anything the forge takes. */
Bool mnForgePartyHasMaterial(void);
/* Records the item the forge is making, for the smith to hand over later. */
void mnForgeSetItem(Sint32 itemId);
/* Returns the item the forge is making. */
Sint32 mnForgeGetItem(void);

#endif
