/* The item picker: which party member's item to drop when every pack is full.
   Part of shop.c. */

#include "shop_internal.h"

/* Lets the player choose a party member, then one of their items that may be
   dropped, in the selling screen without its shopkeeper. Returns 0 with the
   choice in selectedUnit and selectedSlot, or -1 for B with both -1. */
Sint32 mnItemPickerRun(Sint32 *selectedUnit, Sint32 *selectedSlot)
{
	SHOP_STATE *shop;
	WINDOW_SPR *cursorAnchor;
	WINDOW	   *partyWin;
	Sint32		memberIdx = 0;
	Bool		redraw = TRUE;
	Sint32		unitId = 0;
	Sint32		itemSlot;
	Sint32		result = 0;

	mnShopInitState();
	shop = MenuState;
	shop->itemWin = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
	partyWin = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	cursorAnchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, partyWin, 0, 0);
	cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	cursorAnchor->kind = WINDOW_SPRITE_PLAIN;
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	mnCreatePartyFiguresStub(partyWin, 2, 0, 8, 0);

	while(1) {
		if(redraw != 0) {
			redraw = FALSE;
			memberIdx =
				(memberIdx + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[memberIdx];
			mnCursorPlace(partyWin, memberIdx * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnShopDrawParty(partyWin, memberIdx, 0);
			mnShopDrawMemberItems(shop->itemWin, unitId);
		}

		sySynch(1);
		if((PerPush & KEY_A) != 0) {
			if(gmInventoryCountStub(unitId) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}

			sySndPlayStub(SE_MENU_CONFIRM);
			itemSlot = mnItemPickerSelectItem(unitId);
			if(itemSlot == -1) {
				shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
				redraw = TRUE;
				continue;
			}
			*selectedUnit = unitId;
			*selectedSlot = itemSlot;
			result = 0;
			break;
		}

		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			*selectedUnit = -1;
			*selectedSlot = -1;
			result = -1;
			break;
		}

		if((PerRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIdx--;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIdx++;
			redraw = TRUE;
		}
	}

	mnReleasePartyFiguresStub();
	mnWindowCloseStub(partyWin, 2);
	mnWindowCloseStub(shop->itemWin, 2);
	sySynch(1);
	mnShopReleaseState();
	return result;
}

/* Where the line under the item draws "Price", the stack's sale value and the
   coins word, in pixels, and how far back from MsgPriceHeading the coins word
   is: MsgCoins, or in Japanese the message just before. */
#define ITEM_PICKER_PRICE_LABEL_X		 GERMAN_OR(0, 8)
#define ITEM_PICKER_PRICE_NUMBER_X		 BY_LANGUAGE(40, 40, 32, 40, 32, 40)
#define ITEM_PICKER_COINS_LABEL_X		 BY_LANGUAGE(80, 80, 72, 88, 72, 80)
#define ITEM_PICKER_COINS_MESSAGE_OFFSET JAPANESE_OR(1, 5)

/* Lets the player choose one of the party member's items to drop, refusing an
   indispensable one with a message and an equipped cursed one with only the
   cancel sound (the game's bug; BUGFIX shows its message, see
   SHOP_CANNOT_REMOVE_STATUS). Returns the slot, or -1 when cancelled. */
Sint32 mnItemPickerSelectItem(Sint32 unitId)
{
	SHOP_STATE *shop = MenuState;
	WINDOW	   *descriptionWin;
	WINDOW	   *priceWin;
	UNIT	   *unit;
	Sint32		slot;
	Bool		redraw;
	Sint32		inventoryCnt;
	Sint32		selectionResult;
	Sint32		discardStat;
	Sint32		itemId;
	WINDOW	   *win;
	Sint32		x;
	Sint32		y;

	unit = gmUnitGetStub(unitId);
	inventoryCnt = 1;
	selectionResult = 0;
	redraw = TRUE;
	priceWin = mnWindowCreateStub(ITEM_PICKER_WINDOW_X, 8,
		ITEM_PICKER_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	descriptionWin = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	slot = 0;

	while(1) {
		if(redraw != 0) {
			redraw = FALSE;
			inventoryCnt = gmInventoryCountStub(unitId);
			if(slot > inventoryCnt - 1)
				slot = inventoryCnt - 1;

			itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
			win = shop->itemWin;
			x = slot % SHOP_INVENTORY_COLUMNS << 4;
			y = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
			mnCursorPlace(win, x, y);
			shop->cursorFrm = 3;
			mnItemPickerDrawItem(priceWin, unitId, slot);
			mnShopDrawMessage(descriptionWin, itemId + MsgItemPlainName);
		}

		sySynch(1);

		if((PerPush & KEY_A) != 0) {
			discardStat = gmInventoryCheckDiscardStub(unitId, slot);
			if(discardStat == INVENTORY_DISCARD_ALLOWED) {
				sySndPlayStub(SE_MENU_CONFIRM);
				selectionResult = slot;
				break;
			}
			/* BUG: default code selects the error using selectionResult, still
			   zero. */
			if(discardStat == INVENTORY_DISCARD_INDISPENSABLE) {
				/* "You can't drop it!" */
				mnMessageOpenWindowStub(
					MsgCannotDropIt, 8, 1, MESSAGE_WINDOW_UNREAD);
			} else if(SHOP_CANNOT_REMOVE_STATUS(discardStat, selectionResult) ==
				INVENTORY_DISCARD_STUCK) {
				/* "You can't remove it!" */
				mnMessageOpenWindowStub(
					MsgCannotRemoveIt, 8, 1, MESSAGE_WINDOW_UNREAD);
			}
			sySndPlayStub(SE_MENU_CANCEL);
			while(mnMessageAllTypedStub() == 0)
				sySynch(1);
			mnMessageCloseFinishedWindowsStub();
			continue;
		}

		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}

		if((PerRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot--;
			slot = (slot + inventoryCnt) % inventoryCnt;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot++;
			slot = (slot + inventoryCnt) % inventoryCnt;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_UP) != 0) {
			slot -= SHOP_INVENTORY_COLUMNS;
			if(slot < 0)
				slot += INVENTORY_SLOTS;
			while(slot >= inventoryCnt)
				slot -= SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
		}
		if((PerRepeat & KEY_DOWN) != 0) {
			slot += SHOP_INVENTORY_COLUMNS;
			if(slot >= inventoryCnt)
				slot -= INVENTORY_SLOTS;
			while(slot < 0)
				slot += SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
		}
	}

	mnWindowCloseStub(descriptionWin, 2);
	mnWindowCloseStub(priceWin, 2);
	sySynch(1);
	return selectionResult;
}

/* Draws the slot's item name and, below it, why it cannot be dropped or what
   the whole stack would sell for. */
void mnItemPickerDrawItem(WINDOW *win, Sint32 unitId, Sint32 slot)
{
	UNIT  *unit = gmUnitGetStub(unitId);
	Sint32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	Uint32 inventoryEntry = unit->inventory[slot];
	Sint32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

	if(win != 0) {
		Sint32 discardStat;

		mnWindowSprRedrawSavedRectStub(win);
		mnTextDrawMessageAsTilesStub(itemId + MsgItemName, win, 0, 0);

		discardStat = gmInventoryCheckDiscardStub(unitId, slot);
		if(discardStat == INVENTORY_DISCARD_INDISPENSABLE) {
			mnTextDrawMessageAsTilesStub(
				MsgCannotDrop, win, SHOP_ITEM_NOTE_X, 8);
		} else if(discardStat == INVENTORY_DISCARD_STUCK) {
			mnTextDrawMessageAsTilesStub(
				MsgCannotRemove, win, SHOP_ITEM_NOTE_X, 8);
		} else {
			Sint32 unitSalePrice;
			Sint32 stackSalePrice;

			unitSalePrice = mnShopSalePrice(unit->inventory[slot]);
			stackSalePrice = quantity * unitSalePrice;

			mnTextDrawMessageAsTilesStub(
				MsgPriceHeading, win, ITEM_PICKER_PRICE_LABEL_X, 8);
			mnTextDrawNumInWindowStub(
				stackSalePrice, 5, win, ITEM_PICKER_PRICE_NUMBER_X, 8);
			mnTextDrawMessageAsTilesStub(
				MsgPriceHeading - ITEM_PICKER_COINS_MESSAGE_OFFSET, win,
				ITEM_PICKER_COINS_LABEL_X, 8);
		}
	}
}

#undef ITEM_PICKER_PRICE_LABEL_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef ITEM_PICKER_COINS_LABEL_X
#undef ITEM_PICKER_COINS_MESSAGE_OFFSET
