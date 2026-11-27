/* Running a shop: the artifacts it lists, opening its state, the counter's loop
   and the message helpers every counter uses. Part of shop.c. */

#include "item_ids.h"
#include "sounds.h"
#include "field/mode_stubs.h"

/* Lists the current shop's artifacts: every item of the shop's type whose
   shared stock counter is above zero, or of any type at a general store. Writes
   them to the shop state's stock list and returns how many. Called by mnShopRun
   when the player picks Artifacts, and by mnShopSelectBuy to refresh the list
   after a purchase. */
Sint32 mnShopListAvailableArtifacts(void)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		cnt = 0;
	Sint32		itemId;

	/* Every 9-bit id is tried. */
	for(itemId = 0; itemId <= INVENTORY_ITEM_ID_MASK; itemId++) {
		Sint32 shopType = shop->shopType;

		if(shopType != SHOP_TYPE_GENERAL_STORE) {
			if(shopType != SHOP_TYPE_EVERY_GROUP) {
				if(shopType != gmItemGetEquipmentGroupStub(itemId))
					continue;
			}
		}
		if(gmItemAdjustRareItemStockStub(itemId, 0) != 0)
			shop->stockItemIds[cnt++] = itemId;
	}
	shop->stockItemIds[cnt] = 0;
	shop->stockCnt = cnt;
	return cnt;
}

/* Advances the hand cursor one tick. Runs every tick as an event while a
   counter is open. */
void mnCursorStep(void)
{
	mnCursorAdvance(&((SHOP_STATE *)Ram_HeapSlots->menuState)->cursor);
}

/* Allocates and clears the shop's state in the menu's heap slot, lists the
   party, loads the six cursor icons into the VRAM cache, starts the cursor's
   event and readies the item icon grid. Every counter opens with it: mnShopRun,
   the inn, the sanctum, the forge and the item picker. The block, the icons'
   cache entries and the event last until mnShopReleaseState. */
void mnShopInitState(void)
{
	SHOP_STATE *shop;
	Sint32		resourceSlot;

	shop = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(SHOP_STATE));
	fdScriptEndAllLastingPsynergyStub();
	DMA_FILL32(0, shop, sizeof(SHOP_STATE));
	/* The hand cursor's glides take SHOP_CURSOR_ENTER_TICKS until a screen
	   changes it. */
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	shop->partyMemberCnt = gmPartyListMembersStub(shop->partyMemberIds);
	mnPaletteLoadIconsIntoHighlightStub();
	/* Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite */
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->cursorIcon = resourceSlot;
	mnVRAMBlockLoadResource(resourceSlot, 128, RESOURCE_UI_ICON_TILES);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->prevPageIcon = resourceSlot;
	mnVRAMBlockLoadResource(resourceSlot, 128, RESOURCE_SHOP_UP_ARROW_TILES);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->nextPageIcon = resourceSlot;
	mnVRAMBlockLoadResource(resourceSlot, 128, RESOURCE_SHOP_DOWN_ARROW_TILES);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->gemIcon = resourceSlot;
	mnVRAMBlockLoadResource(resourceSlot, 128, RESOURCE_SHOP_GEM_TILES);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->statDownIcon = resourceSlot;
	mnVRAMBlockLoadResource(
		resourceSlot, 128, RESOURCE_MENU_SMALL_DOWN_ARROW_TILES);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->statUpIcon = resourceSlot;
	mnVRAMBlockLoadResource(resourceSlot, 128, RESOURCE_MENU_ARROW_TILES);
	sySetEvent(mnCursorStep, EVENT_PASS_TICK);
	mnItemIconGridInit();
}

/* Releases the item icon grid, stops the cursor event, frees the six cursor
   icons and the shop's state. Every counter closes with it. */
void mnShopReleaseState(void)
{
	SHOP_STATE *shop;

	shop = Ram_HeapSlots->menuState;
	mnItemIconGridRelease();
	syCloseEvent(mnCursorStep);
	mnMessageCloseFinishedWindowsStub();
	syVRAMCacheFreeEntry(shop->cursorIcon);
	syVRAMCacheFreeEntry(shop->prevPageIcon);
	syVRAMCacheFreeEntry(shop->nextPageIcon);
	syVRAMCacheFreeEntry(shop->gemIcon);
	syVRAMCacheFreeEntry(shop->statUpIcon);
	syVRAMCacheFreeEntry(shop->statDownIcon);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
   until the player leaves. `stockRow` picks the row of the stock table (the
   empty row 0 for one out of range) and `keeperId` the actor whose portrait the
   messages show. Seeds the row's artifacts on the first visit and holds the
   shop's state while it runs. Called by the town scenes' shopkeepers through
   the stub table, and by the debug shop. Returns 0. */
Sint32 mnShopRun(Sint32 stockRow, Sint32 keeperId)
{
	SHOP_STATE *shop;
	WINDOW_SPR *anchor;
	WINDOW	   *win;
	Sint32		action = 0;

	if(stockRow >= mnShopStockGetRowCount() || stockRow < 0)
		stockRow = SHOP_STOCK_ROW_EMPTY;
	mnShopStockAddArtifacts(stockRow);
	mnShopInitState();
	shop = Ram_HeapSlots->menuState;
	shop->shopType = mnShopStockGetType(stockRow);
	shop->keeperGraphics =
		((SPR *)fdActorGetByIdStub(keeperId)->spr)->entries[0]->anmId;
	/* The shopkeeper's portrait window, or an empty one off the right edge when
	   there is no portrait. */
	win = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
	if(win == 0)
		win = mnWindowCreateStub(30, 0, 2, 2, WINDOW_FLAG_INSTANT);
	anchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
	anchor->depth = OAM_DEPTH_FRONT;
	anchor->motion = WINDOW_SPRITE_MOTION_STILL;
	anchor->kind = WINDOW_SPRITE_PLAIN;
	/* The hand waits off the left edge until a screen places it */
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = anchor;
	mnShopShowKeeperMessage((Sint32)MsgWeaponShopWelcome);
	while(1) {
		action = mnSelectShopCommandStub(action);
		shop->action = action;
		if(action == SHOP_ACTION_BUY) {
			shop->stockCnt = mnShopStockCopyItems(stockRow, shop->stockItemIds);
			mnShopShowKeeperMessage((Sint32)MsgWhatWouldYouLike);
			mnShopSelectBuy();
		} else if(action == SHOP_ACTION_SELL) {
			mnShopShowKeeperMessage((Sint32)MsgWhatToSell);
			mnShopChooseMember();
		} else if(action == SHOP_ACTION_ARTIFACTS) {
			if(mnShopListAvailableArtifacts() != 0) {
				mnShopShowKeeperMessage((Sint32)MsgConnoisseur);
				mnShopSelectBuy();
			} else {
				mnShopShowKeeperMessage((Sint32)MsgOutOfStock);
				sySynch(1);
			}
		} else if(action == SHOP_ACTION_REPAIR) {
			mnShopShowKeeperMessage((Sint32)MsgFixDamaged);
			mnShopRunRepairMemberChoice();
		} else {
			break;
		}
		mnCursorSetPosImmediate(&shop->cursor, -32, 112);
		mnShopShowKeeperMessage((Sint32)MsgAnythingElse);
	}
	mnShopShowKeeperMessage((Sint32)MsgShopFarewell);
	mnWindowCloseStub(win, 2);
	mnShopReleaseState();
	return 0;
}

/* The coins the debug shop gives. */
#define DEBUG_SHOP_COINS  200000
/* The actor whose sprite picks the debug shop keeper's portrait, in whatever
   room it opens. */
#define DEBUG_SHOP_KEEPER 30

/* Unused: opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at its
   cap, broken equipment for repairs and poisoned or haunted party members for
   the sanctum, on the empty stock row. Nothing calls it or its stub call
   export; The Broken Seal's debug menu opened its own. */
Sint32 mnShopOpenDebugShop(void)
{
	MainRoutineStateWork.coins = DEBUG_SHOP_COINS;
	MainRoutineStateWork.shopGifts = SHOP_GIFTS_MAX;
	gmInventoryEquipStub(GARET,
		gmInventoryAddItemStub(GARET, INVENTORY_BROKEN | ITEM_VIRTUOUS_ARMLET));
	gmInventoryEquipStub(
		ISAAC, gmInventoryAddItemStub(ISAAC, INVENTORY_BROKEN | ITEM_MURAMASA));
	gmInventoryAddItemStub(IVAN, ITEM_BONE);
	syUnitGet(MIA)->poison = POISON_NORMAL;
	syUnitGet(JENNA)->poison = POISON_NORMAL;
	syUnitGet(IVAN)->haunt = 1;
	mnShopRun(SHOP_STOCK_ROW_EMPTY, DEBUG_SHOP_KEEPER);
	return 0;
}

/* Waits until the jingle playing has ended and the music it interrupted is
   back: the inn's night, the sanctum's healing and scripts that wait for a
   jingle. */
void mnSndWaitForJingle(void)
{
	while(sySndGetBGMRestoreDelayStub() != 0) {
		sySynch(1);
	}
}

/* Turns the weapon shop's line `message` into the one the shop of `shopType`
   says: the armor shop, the item shop and the general store each have their own
   version, as many messages on. */
#define SHOP_KEEPER_LINE(message, shopType) \
	((shopType) == SHOP_TYPE_GENERAL_STORE && \
			((message) += MsgGeneralStoreWelcome - MsgWeaponShopWelcome), \
		(shopType) == SHOP_TYPE_ARMOR && \
			((message) += MsgArmorShopWelcome - MsgWeaponShopWelcome), \
		(shopType) == SHOP_TYPE_ITEMS && \
			((message) += MsgItemShopWelcome - MsgWeaponShopWelcome))

/* Shows the shopkeeper's line `msgId`, given as the weapon shop's
   (SHOP_KEEPER_LINE), and waits until it is typed out. */
void mnShopShowKeeperMessage(Sint32 msgId)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		resolvedMsg;
	Sint32		voice;
	Sint8		shopType;

	resolvedMsg = msgId;
	voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);
	mnMessageCloseFinishedWindowsStub();
	shopType = shop->shopType;
	SHOP_KEEPER_LINE(resolvedMsg, shopType);
	mnMessageOpenWindowStub(resolvedMsg, 5, 0,
		MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
			MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0)
		sySynch(1);
	sySynch(1);
}

/* Shows a shopkeeper's line as mnShopShowKeeperMessage does, hiding the hand
   cursor while it shows and putting back its motion after. */
void mnShopShowKeeperMessageHidingCursor(Sint32 msgId)
{
	Sint32		 voice;
	Sint32		 resolvedMsg;
	Sint8		 shopType;
	Uint8		 savedCursorMode;
	SHOP_STATE	*shop;
	WINDOW_SPR **cursorSlot;

	shop = Ram_HeapSlots->menuState;
	cursorSlot = &shop->cursor.anchor;
	savedCursorMode = (*cursorSlot)->motion;
	resolvedMsg = msgId;
	voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);
	shopType = shop->shopType;
	SHOP_KEEPER_LINE(resolvedMsg, shopType);
	(*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	mnMessageCloseFinishedWindowsStub();
	mnMessageOpenWindowStub(resolvedMsg, 5, 0,
		MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
			MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0)
		sySynch(1);
	sySynch(1);
	shop->cursor.anchor->motion = savedCursorMode;
}

/* The shop's Yes/No menu: one column further left in German, Spanish and
   Italian. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_ITALIAN)
#define SHOP_CONFIRM_MENU_X 6
#else
#define SHOP_CONFIRM_MENU_X 7
#endif

/* Runs the Yes or No choice beside the shopkeeper's line and returns it: 0 for
   Yes, 1 for No or B. `choice` is where the cursor starts. Keeps the hand
   cursor's motion. Called by the counters before they buy, sell, equip or
   repair, and by the inn. */
Sint32 mnShopAskYesNo(Sint32 choice)
{
	WINDOW_SPR **cursorSlot =
		&((SHOP_STATE *)Ram_HeapSlots->menuState)->cursor.anchor;
	Uint8 savedCursorMode = (*cursorSlot)->motion;
	mnIconShowStub(*cursorSlot);
	choice = mnYesNoMenuRunAtStub(SHOP_CONFIRM_MENU_X, 5, choice);
	(*cursorSlot)->motion = savedCursorMode;
	return choice;
}

#undef SHOP_CONFIRM_MENU_X

/* Runs the Yes or No choice at the sanctum's place on the screen and returns
   it, keeping the hand cursor's motion. Called by mnSanctumRunPartyService. */
Sint32 mnSanctumAskYesNo(Sint32 choice)
{
	WINDOW_SPR **cursorSlot =
		&((SHOP_STATE *)Ram_HeapSlots->menuState)->cursor.anchor;
	Uint8 savedCursorMode = (*cursorSlot)->motion;
	mnIconShowStub(*cursorSlot);
	choice = mnYesNoMenuRunAtStub(7, 7, choice);
	(*cursorSlot)->motion = savedCursorMode;
	return choice;
}

/* Moves a cursor over `cnt` entries laid out `columns` to a row by the d-pad:
   left and right wrap round, up and down stop at the ends. Plays the cursor
   sound when it moves. Returns how it moved (MENU_GRID_MOVE), a page being one
   row of `columns`. Called by every counter's lists. */
Sint32 mnGridMoveCursor(Sint32 *cursor, Sint32 cnt, Sint32 columns)
{
	Sint32 changed = MENU_GRID_STAYED;
	Sint32 prev = *cursor;
	Sint32 pos = prev;

	if(PerWork.repeat & KEY_LEFT) {
		pos--;
		pos = (pos + cnt) % cnt;
	}
	if(PerWork.repeat & KEY_RIGHT) {
		pos++;
		pos = (pos + cnt) % cnt;
	}
	if(PerWork.repeat & KEY_UP) {
		if(pos - columns >= 0)
			pos -= columns;
	}
	if(PerWork.repeat & KEY_DOWN) {
		if(pos + columns < (columns + cnt - 1) / columns * columns)
			pos += columns;
		if(pos > cnt - 1)
			pos = cnt - 1;
	}
	if(prev != pos) {
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		changed = MENU_GRID_MOVED;
	}
	if(prev / columns * columns != pos / columns * columns)
		changed = MENU_GRID_CHANGED_PAGE;
	*cursor = pos;
	return changed;
}
