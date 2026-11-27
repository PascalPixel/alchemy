/* @file */
/* The item picker: which party member's item to drop when every pack is full.
 */
/* Part of shop.c. */

#include "shop_internal.h"

/* Lets the player choose a party member, then one of their items that may be */
/* dropped, in the selling screen without its shopkeeper. Returns 0 with the */
/* choice in selectedUnit and selectedSlot, or -1 for B with both -1. */
s32 mnItemPickerRun(s32 *selectedUnit, s32 *selectedSlot) {
	SHOP_STATE *shop;
	WINDOW_SPR *cursorAnchor;
	WINDOW *partyWindow;
	s32 memberIndex = 0;
	s32 redraw = 1;
	s32 unitId = 0;
	s32 itemSlot;
	s32 result = 0;

	mnShopInitState();
	shop = MenuState;
	shop->itemWin = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
	partyWindow = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	cursorAnchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, partyWindow, 0, 0);
	cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	cursorAnchor->kind = WINDOW_SPRITE_PLAIN;
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	mnCreatePartyFiguresStub(partyWindow, 2, 0, 8, 0);

	while(1) {
		if(redraw != 0) {
			redraw = 0;
			memberIndex =
				(memberIndex + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[memberIndex];
			mnCursorPlace(partyWindow, memberIndex * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnShopDrawParty(partyWindow, memberIndex, 0);
			mnShopDrawMemberItems(shop->itemWin, unitId);
		}

		sySynch(1);
		if((KeysPressed & KEY_A) != 0) {
			if(gmInventoryCountStub(unitId) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}

			sySndPlayStub(SE_MENU_CONFIRM);
			itemSlot = mnItemPickerSelectItem(unitId);
			if(itemSlot == -1) {
				shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
				redraw = 1;
				continue;
			}
			*selectedUnit = unitId;
			*selectedSlot = itemSlot;
			result = 0;
			break;
		}

		if((KeysPressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			*selectedUnit = -1;
			*selectedSlot = -1;
			result = -1;
			break;
		}

		if((KeysRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIndex--;
			redraw = 1;
		}
		if((KeysRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIndex++;
			redraw = 1;
		}
	}

	mnReleasePartyFiguresStub();
	mnWindowCloseStub(partyWindow, 2);
	mnWindowCloseStub(shop->itemWin, 2);
	sySynch(1);
	mnShopReleaseState();
	return result;
}

/* Where the line under the item draws "Price", the stack's sale value and the
 */
/* coins word, in pixels, and how far back from MsgPriceHeading the coins word
 */
/* is: MsgCoins, or in Japanese the message just before. */
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_PICKER_PRICE_LABEL_X		 8
#define ITEM_PICKER_PRICE_NUMBER_X		 40
#define ITEM_PICKER_COINS_LABEL_X		 80
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_GERMAN
#define ITEM_PICKER_PRICE_LABEL_X		 0
#define ITEM_PICKER_PRICE_NUMBER_X		 32
#define ITEM_PICKER_COINS_LABEL_X		 72
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_SPANISH
#define ITEM_PICKER_PRICE_LABEL_X		 8
#define ITEM_PICKER_PRICE_NUMBER_X		 40
#define ITEM_PICKER_COINS_LABEL_X		 88
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#elif LANGUAGE == LANGUAGE_FRENCH
#define ITEM_PICKER_PRICE_LABEL_X		 8
#define ITEM_PICKER_PRICE_NUMBER_X		 32
#define ITEM_PICKER_COINS_LABEL_X		 72
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 5
#else
#define ITEM_PICKER_PRICE_LABEL_X		 8
#define ITEM_PICKER_PRICE_NUMBER_X		 40
#define ITEM_PICKER_COINS_LABEL_X		 80
#define ITEM_PICKER_COINS_MESSAGE_OFFSET 1
#endif

/* Lets the player choose one of the party member's items to drop, refusing an
 */
/* indispensable one with a message and an equipped cursed one with only the */
/* cancel sound (the game's bug; BUGFIX shows its message, see */
/* SHOP_CANNOT_REMOVE_STATUS). Returns the slot, or -1 when cancelled. */
s32 mnItemPickerSelectItem(s32 unitId) {
	SHOP_STATE *shop = MenuState;
	WINDOW *descriptionWindow;
	WINDOW *priceWindow;
	UNIT *unit;
	s32 slot;
	s32 redraw;
	s32 inventoryCount;
	s32 selectionResult;
	s32 discardStatus;
	s32 itemId;
	WINDOW *window;
	s32 x;
	s32 y;

	unit = gmUnitGetStub(unitId);
	inventoryCount = 1;
	selectionResult = 0;
	redraw = 1;
	priceWindow =
		mnWindowCreateStub(ITEM_PICKER_WINDOW_X, 8, ITEM_PICKER_WINDOW_WIDTH, 4,
						   WINDOW_FLAG_INSTANT);
	descriptionWindow = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	slot = 0;

	while(1) {
		if(redraw != 0) {
			redraw = 0;
			inventoryCount = gmInventoryCountStub(unitId);
			if(slot > inventoryCount - 1)
				slot = inventoryCount - 1;

			itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
			window = shop->itemWin;
			x = slot % SHOP_INVENTORY_COLUMNS << 4;
			y = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
			mnCursorPlace(window, x, y);
			shop->cursorFrm = 3;
			mnItemPickerDrawItem(priceWindow, unitId, slot);
			mnShopDrawMessage(descriptionWindow, itemId + MsgItemPlainName);
		}

		sySynch(1);

		if((KeysPressed & KEY_A) != 0) {
			discardStatus = gmInventoryCheckDiscardStub(unitId, slot);
			if(discardStatus == INVENTORY_DISCARD_ALLOWED) {
				sySndPlayStub(SE_MENU_CONFIRM);
				selectionResult = slot;
				break;
			}
			/* BUG: default code selects the error using selectionResult, still
			 * zero. */
			if(discardStatus == INVENTORY_DISCARD_INDISPENSABLE) {
				/* "You can't drop it!" */
				mnMessageOpenWindowStub(MsgCannotDropIt, 8, 1,
										MESSAGE_WINDOW_UNREAD);
			}
			else if(SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult) ==
					INVENTORY_DISCARD_STUCK) {
				/* "You can't remove it!" */
				mnMessageOpenWindowStub(MsgCannotRemoveIt, 8, 1,
										MESSAGE_WINDOW_UNREAD);
			}
			sySndPlayStub(SE_MENU_CANCEL);
			while(mnMessageAllTypedStub() == 0)
				sySynch(1);
			mnMessageCloseFinishedWindowsStub();
			continue;
		}

		if((KeysPressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}

		if((KeysRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot--;
			slot = (slot + inventoryCount) % inventoryCount;
			redraw = 1;
		}
		if((KeysRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot++;
			slot = (slot + inventoryCount) % inventoryCount;
			redraw = 1;
		}
		if((KeysRepeat & KEY_UP) != 0) {
			slot -= SHOP_INVENTORY_COLUMNS;
			if(slot < 0)
				slot += INVENTORY_SLOTS;
			while(slot >= inventoryCount)
				slot -= SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = 1;
		}
		if((KeysRepeat & KEY_DOWN) != 0) {
			slot += SHOP_INVENTORY_COLUMNS;
			if(slot >= inventoryCount)
				slot -= INVENTORY_SLOTS;
			while(slot < 0)
				slot += SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = 1;
		}
	}

	mnWindowCloseStub(descriptionWindow, 2);
	mnWindowCloseStub(priceWindow, 2);
	sySynch(1);
	return selectionResult;
}

/* Draws the slot's item name and, below it, why it cannot be dropped or what */
/* the whole stack would sell for. */
void mnItemPickerDrawItem(WINDOW *window, s32 unitId, s32 slot) {
	UNIT *unit = gmUnitGetStub(unitId);
	s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	u32 inventoryEntry = unit->inventory[slot];
	s32 quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

	if(window != 0) {
		s32 discardStatus;

		mnWindowSprRedrawSavedRectStub(window);
		mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window, 0, 0);

		discardStatus = gmInventoryCheckDiscardStub(unitId, slot);
		if(discardStatus == INVENTORY_DISCARD_INDISPENSABLE) {
			mnTextDrawMessageAsTilesStub(MsgCannotDrop, window,
										 SHOP_ITEM_NOTE_X, 8);
		}
		else if(discardStatus == INVENTORY_DISCARD_STUCK) {
			mnTextDrawMessageAsTilesStub(MsgCannotRemove, window,
										 SHOP_ITEM_NOTE_X, 8);
		}
		else {
			s32 unitSalePrice;
			s32 stackSalePrice;

			unitSalePrice = mnShopSalePrice(unit->inventory[slot]);
			stackSalePrice = quantity * unitSalePrice;

			mnTextDrawMessageAsTilesStub(MsgPriceHeading, window,
										 ITEM_PICKER_PRICE_LABEL_X, 8);
			mnTextDrawNumInWindowStub(stackSalePrice, 5, window,
									  ITEM_PICKER_PRICE_NUMBER_X, 8);
			mnTextDrawMessageAsTilesStub(MsgPriceHeading -
											 ITEM_PICKER_COINS_MESSAGE_OFFSET,
										 window, ITEM_PICKER_COINS_LABEL_X, 8);
		}
	}
}

#undef ITEM_PICKER_PRICE_LABEL_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef ITEM_PICKER_COINS_LABEL_X
#undef ITEM_PICKER_COINS_MESSAGE_OFFSET
