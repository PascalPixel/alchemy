#ifndef ALCHEMY_SHOP_SHOP_H
#define ALCHEMY_SHOP_SHOP_H

/* @file */
/* What the shop object defines for the code outside it. */

#include "types.h"

struct WINDOW;
struct MENU_CURSOR;
struct WINDOW_SPR;

/* The colours of a quantity bar's segments. mnQuantityBarFill paints the first
 */
/* so many segments in one, and the bar is built by painting shorter runs over
 */
/* longer: the whole bar out of reach, then what could be chosen, then what is
 */
/* chosen, then what the party member already carries. */
#define QUANTITY_BAR_OUT_OF_REACH 14
#define QUANTITY_BAR_OPEN		  0
#define QUANTITY_BAR_CHOSEN		  10
#define QUANTITY_BAR_HELD		  2

/* Rows of the shop stock table, as an event passes them to mnShopRun: Daila's
 */
/* weapon, armor and item shops, and Izumo's general store, which the debug */
/* rooms' shopkeepers also open. A town no other room opens a shop of names */
/* its rows itself. */
enum {
	SHOP_DAILA_WEAPONS = 1,
	SHOP_DAILA_ARMOR = 2,
	SHOP_DAILA_ITEMS = 3,
	SHOP_IZUMO = 24
};

/* The lines of one shop, as many as the weapon shop's, and of a sanctum, as */
/* the debug rooms' message browsers count them (text/msg_ids.h). */
#define SHOP_MESSAGES	 (MsgArmorShopWelcome - MsgWeaponShopWelcome)
#define SANCTUM_MESSAGES (MsgRightWhenI - MsgWelcomeWearyWanderers)

/* Daila's inn, as mnInnCheckIn numbers the inns, which the debug rooms' */
/* innkeepers also open. A town no other room opens an inn of names its inn */
/* itself. */
enum { DAILA_INN = 0 };

void mnShopInitState(void);
void mnShopReleaseState(void);
s32 mnShopSelectBuy(void);
void mnShopDrawStock(struct WINDOW *window, s32 selected);
void mnShopDrawItemPrice(struct WINDOW *window, s32 itemId, s32 price,
						 s32 priceKind);
void mnShopDrawMessage(struct WINDOW *window, s32 message);
void mnShopDrawParty(struct WINDOW *window, s32 selected, s32 itemId);
void mnShopDrawEquipComparison(struct WINDOW *window, s32 unitId, s32 itemId);
void mnShopDrawCarriedItem(struct WINDOW *window, s32 unitId, s32 itemId);
s32 mnShopSelectBuyCount(s32 unitId, s32 itemId);
s32 mnShopSelectQuantity(s32 minimum, s32 maximum, s32 price);
void mnShopBuyDone(s32 unitId, s32 itemId, s32 quantity);
s32 mnShopConfirmEquip(s32 unitId, s32 slot);
s32 mnShopSellOld(s32 unitId, s32 slot);
void mnShopGiveGameTicket(struct WINDOW *window, struct WINDOW *itemWindow);
s32 mnShopChooseMember(void);
s32 mnShopSelectSell(s32 unitId);
void mnShopDrawMemberItems(struct WINDOW *window, s32 unitId);
s32 mnShopSelectSellCount(s32 unitId, s32 slot);
void mnShopSellItem(s32 unitId, s32 slot, s32 count);
s32 mnShopRepairPrice(s32 inventoryEntry);
s32 mnShopRunRepairMemberChoice(void);
s32 mnShopSelectRepair(s32 unitId);
void mnShopRepairItem(s32 unitId, s32 slot);
void mnShopShowKeeperMessage(s32 messageId);
struct WINDOW_SPR *mnShopCreatePriceSpr(s32 value, struct WINDOW *window, s32 x,
										s32 y);

s32 mnShopStockGetRowCount(void);
s32 mnShopStockGetType(s32 stockRow);
void mnShopStockAddArtifacts(s32 stockRow);
s32 mnShopStockCopyItems(s32 stockRow, s16 *output);

s32 mnInnRoomPrice(s32 innId);

s32 mnForgeChooseMember(void);
s32 mnForgeSelectItem(s32 unitId);
s32 mnForgeGetItemCondition(s32 itemId);
s32 mnForgeOfferItem(s32 unitId, s32 slot);

s32 mnSanctumRunPartyService(void);
void mnSanctumHighlightMembers(struct WINDOW *window, s32 selected);
void mnSanctumDrawFee(struct WINDOW *window, s32 unitId);
void mnSanctumApplyService(s32 unitId, s32 serviceKind);
void mnSanctumRunMemberIconBurst(s32 member);

s32 mnItemPickerRun(s32 *selectedUnit, s32 *selectedSlot, s32 itemMode);
s32 mnItemPickerSelectItem(s32 unitId);
void mnItemPickerDrawItem(struct WINDOW *window, s32 unitId, s32 slot);
void mnItemIconGridInit(void);
void mnItemIconGridRelease(void);
void mnItemIconGridShowInventory(struct WINDOW *window, s32 x, s32 y,
								 s32 unitId);

/* The frames the hand cursor takes to glide into a newly opened list. */
#define SHOP_CURSOR_ENTER_FRAMES 12

/* The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price */
/* sprite's tiles (mnShopDrawPriceDigit). */
extern u8 Shop_GlyphBytes[];
s32 mnSanctumNeedsService(s32 unitId, s32 serviceKind);

struct MENU_CURSOR;

/* The shop module's functions in the order of its stub table. */
/* Unused: opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at */
/* its cap, broken equipment for repairs and poisoned or haunted party members
 */
/* for the sanctum, on the empty stock row. Nothing calls it or its stub call */
/* export; The Broken Seal's debug menu opened its own. */
s32 mnShopOpenDebugShop(void);
/* The shops, inns, sanctums and forges as events open them. */
s32 mnShopRun(s32 stockRow, s32 keeperId);
/* Runs a sanctum: the keeper's welcome, the service menu until B, and the */
/* farewell; `actorId` is the keeper, whose portrait the lines show. */
s32 mnSanctumRun(s32 actorId);
/* Runs an inn's counter: the innkeeper names the price, and on Yes the party */
/* sleeps. `innId` is the row of Inn_PricesPerGuest and `actorId` the */
/* innkeeper, whose portrait the lines show. */
s32 mnInnCheckIn(s32 innId, s32 actorId);
/* Runs the forge: the smith's line, then a party member's materials to */
/* forge, until the player leaves. `actorId` is the actor whose portrait the */
/* messages show. Called through the stub table; returns 0. */
s32 mnForgeRun(s32 actorId);
/* Returns the night's price at inn `innId` with the shop's state opened for */
/* it, for an event to quote before checking in. */
s32 mnInnQuoteRoomPrice(s32 innId);
/* Moves the cursor a frame along its straight line toward its target. */
void mnCursorAdvance(struct MENU_CURSOR *cursor);
/* Moves each cursor axis one quarter of the remaining distance, with a minimum
 */
/* one-pixel step. */
void mnCursorMoveTowardTarget(struct MENU_CURSOR *cursor);
/* Starts a glide of the cursor from where it is to (targetX, targetY) over */
/* `frames` frames, for mnCursorAdvance to run. */
void mnCursorSet(struct MENU_CURSOR *cursor, s32 targetX, s32 targetY,
				 s8 frames);
/* Places the cursor and its sprite at (x, y) at once, with a one-frame glide */
/* that has already arrived. */
void mnCursorSetPosImmediate(struct MENU_CURSOR *cursor, s32 x, s32 y);
/* Paints the first `segments` segments of the quantity bar in `color` (a */
/* QUANTITY_BAR_ colour), over what was painted before. */
void mnQuantityBarFill(s32 segments, s32 color, u8 *tileBuffer);
/* Saves the field's colours and fades them to `tint` (a PALETTE_TINT_ mode) */
/* over SHOP_TINT_FRAMES frames: the grey of the sanctum's healing, and the */
/* element's colour as a Djinni joins the party. */
void mnShopSaveAndTintFieldPalette(s32 tint);
/* Fades back to the colours mnShopSaveAndTintFieldPalette saved, over */
/* SHOP_TINT_FRAMES frames. */
void mnShopRestoreFieldPalette(void);
/* Runs the item picker for an item to drop, showing each item's sale price. */
s32 mnItemPickerRunToDrop(s32 *selectedUnit, s32 *selectedSlot);
/* Runs the item picker for an item to show, refusing one that cannot be */
/* unequipped: Naribwe's fortune teller reads the party's fortune from it. */
s32 mnItemPickerRunToShow(s32 *selectedUnit, s32 *selectedSlot);
/* Waits until the jingle playing has ended and the music it interrupted is */
/* back: the inn's night, the sanctum's healing and events that wait for a */
/* jingle. */
void mnSndWaitForJingle(void);
/* Charges the room and restores the HP and PP of each party member still */
/* standing over a night: a fade to black, the inn's music and a fade back. */
void mnInnPlaySleep(s32 roomPrice);
/* Decompresses resource `resourceId` and loads its first `size` bytes into */
/* VRAM cache entry `entryId`; does nothing for an entry past the cache. */
void mnVramBlockLoadResource(s32 entryId, s32 size, s32 resourceId);
/* Returns 1 when anyone in the party carries anything the forge takes. */
s32 mnForgePartyHasMaterial(void);
/* Records the item the forge is making, for the smith to hand over later. */
void mnForgeSetItem(s32 itemId);
/* Returns the item the forge is making. */
s32 mnForgeGetItem(void);

#endif
