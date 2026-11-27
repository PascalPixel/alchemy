/* Running a shop, its cursor state, and the message helpers every counter uses.
   Part of shop.c. */

#include "shop_internal.h"

/* Lists the current shop's artifacts: every item of the shop's type whose
   shared stock counter is above zero. */
Sint32 mnShopListAvailableArtifacts(void)
{
	SHOP_STATE *shop;
	Sint16	   *stockItems;
	Sint32		itemId;
	Sint32		count;
	Sint8		equipmentGroup;

	shop = MenuState;
	count = 0;
	itemId = 0;
	stockItems = shop->stockItemIds;
	/* The shop's type is the equipment group it sells (SHOP_TYPE). Every 9-bit
	   id is tried, past the 324 rows of item_table.c too. */
	for(; itemId <= INVENTORY_ITEM_ID_MASK; itemId++) {
		equipmentGroup = shop->shopType;
		if(equipmentGroup == gmItemGetEquipmentGroupStub(itemId) &&
			gmItemAdjustRareItemStockStub(itemId, 0) != 0) {
			*stockItems = itemId;
			count++;
			stockItems++;
		}
	}
	shop->stockItemIds[count] = 0;
	shop->stockCnt = count;
	return count;
}

/* Advances the hand cursor one frame. Runs every frame as an event while a
   counter is open. */
void mnCursorStep(void)
{
	mnCursorAdvance(&((SHOP_STATE *)MenuState)->cursor);
}

/* Opens a counter's state: the party list, the cursor icons and the cursor's
   event, which last until mnShopReleaseState. Every counter opens with it. */
void mnShopInitState(void)
{
	SHOP_STATE *shop;
	Sint32		resourceSlot;

	shop = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(SHOP_STATE));
	fdEndBusyAndRevealStub();
	DMA_FILL32(0, shop, sizeof(SHOP_STATE));
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	shop->partyMemberCnt = gmPartyListMembersStub(shop->partyMemberIds);
	/* Each icon is 128 bytes: four tiles of 4 bits a pixel, a 16x16 sprite */
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->cursorIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, MenuCursor_HandTiles);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->prevPageIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, Shop_UpArrowTiles);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->nextPageIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, Shop_DownArrowTiles);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->gemIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, Shop_GemTiles);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->statDownIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, Shop_SmallDownArrowTiles);
	resourceSlot = syVRAMCacheFindFreeEntry();
	shop->statUpIcon = resourceSlot;
	syVRAMCacheLoad(resourceSlot, 128, Shop_SmallUpArrowTiles);
	sySetEvent(mnCursorStep, EVENT_PASS_FRAME);
}

/* Stops the cursor event, frees the six cursor icons and the state block. Every
   counter closes with it. */
void mnShopReleaseState(void)
{
	SHOP_STATE *state;

	state = MenuState;
	syCloseEvent(mnCursorStep);
	mnMessageCloseFinishedWindowsStub();
	syVRAMCacheFreeEntry(state->cursorIcon);
	syVRAMCacheFreeEntry(state->prevPageIcon);
	syVRAMCacheFreeEntry(state->nextPageIcon);
	syVRAMCacheFreeEntry(state->gemIcon);
	syVRAMCacheFreeEntry(state->statUpIcon);
	syVRAMCacheFreeEntry(state->statDownIcon);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Runs a shop: the shopkeeper's welcome, then buy, sell, artifacts and repair
   until the player leaves. keeperId is the actor whose portrait the lines
   show. */
Sint32 mnShopRun(Sint32 stockRow, Sint32 keeperId)
{
	SHOP_STATE *shop;
	WINDOW_SPR *anchor;
	WINDOW	   *window;
	Sint32		action = 0;

	if(stockRow >= mnShopStockGetRowCount() || stockRow < 0)
		stockRow = SHOP_STOCK_ROW_EMPTY;
	/* A shop's artifacts join the world's stock the first time it opens. */
	mnShopStockAddArtifacts(stockRow);
	mnShopInitState();
	shop = MenuState;
	shop->shopType = mnShopStockGetType(stockRow);
	/* Xian's shops are the warriors' shops, which greet differently. XXX (no
	   evidence): why by row number rather than a column of the row; the stock
	   row's Sint16 type column could have carried it. */
	if(stockRow == SHOP_STOCK_ROW_XIAN_WEAPONS)
		shop->warriorShop = 1;
	if(stockRow == SHOP_STOCK_ROW_XIAN_ARMOR)
		shop->warriorShop = 1;
	if(stockRow == SHOP_STOCK_ROW_XIAN_ITEMS)
		shop->warriorShop = 1;
	shop->keeperGraphics =
		(fdActorGetByIdStub(keeperId))->spr->entries[0]->anmId;
	/* The shopkeeper's portrait window, or an empty one off the left edge when
	   there is no portrait. */
	window = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
	if(window == 0)
		window = mnWindowCreateStub(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
	anchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
	anchor->motion = WINDOW_SPRITE_MOTION_STILL;
	/* The hand's icon is the shop's, freed by mnShopReleaseState, not the
	   sprite's. */
	anchor->kind = WINDOW_SPRITE_PLAIN;
	/* The hand waits off the left edge until a screen places it */
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = anchor;
	/* The lines are quoted as a weapon shop words them; "Come in! Welcome! Take
	   a look at our fine weapons." */
	mnShopShowKeeperMessage(MsgWeaponShopWelcome);
	while(1) {
		action = mnSelectShopCommandStub(action);
		shop->action = action;
		if(action == SHOP_ACTION_BUY) {
			shop->stockCnt = mnShopStockCopyItems(stockRow, shop->stockItemIds);
			/* "What would you like?" */
			mnShopShowKeeperMessage(MsgWhatWouldYouLike);
			mnShopSelectBuy();
		} else if(action == SHOP_ACTION_SELL) {
			/* "What do you want to sell?" */
			mnShopShowKeeperMessage(MsgWhatToSell);
			mnShopChooseMember();
		} else if(action == SHOP_ACTION_ARTIFACTS) {
			if(mnShopListAvailableArtifacts() != 0) {
				/* "You must be a connoisseur if those caught your eye." */
				mnShopShowKeeperMessage(MsgConnoisseur);
				mnShopSelectBuy();
			} else {
				/* "Sorry, but we're out of stock. Come back later, OK?" */
				mnShopShowKeeperMessage(MsgOutOfStock);
				sySynch(1);
			}
		} else if(action == SHOP_ACTION_REPAIR) {
			/* "You should fix damaged weapons before they break." */
			mnShopShowKeeperMessage(MsgFixDamaged);
			mnShopRunRepairMemberChoice();
		} else {
			break;
		}
		mnCursorSetPosImmediate(&shop->cursor, -32, 112);
		/* "Anything else today?" */
		mnShopShowKeeperMessage(MsgAnythingElse);
	}
	/* "Feel free to drop in anytime." */
	mnShopShowKeeperMessage(MsgShopFarewell);
	mnWindowCloseStub(window, 2);
	mnShopReleaseState();
	return 0;
}

/* The debug shop sells Vale's armor in Japanese and its weapons in every
   edition but the Japanese. */
#define SHOP_DEBUG_STOCK_ROW \
	WHEN(DEBUG_SHOP_SELLS_WEAPONS, SHOP_STOCK_ROW_VALE_WEAPONS, \
		SHOP_STOCK_ROW_VALE_ARMOR)

/* The coins the debug shop gives. */
#define DEBUG_SHOP_COINS  200000
/* The actor whose sprite picks the debug shop keeper's portrait, in whatever
   room it opens. */
#define DEBUG_SHOP_KEEPER 30

/* Opens the debug shop with DEBUG_SHOP_COINS, the ticket counter at its cap,
   broken equipment for repairs and poisoned or haunted party members for the
   sanctum. */
Sint32 mnShopOpenDebugShop(Sint32 unused)
{
	MainRoutineStateWork.holdings.coins = DEBUG_SHOP_COINS;
	MainRoutineStateWork.holdings.shopGifts = SHOP_GIFTS_MAX;
	gmInventoryEquipStub(GARET,
		gmInventoryAddItemStub(GARET, INVENTORY_BROKEN | ITEM_VIRTUOUS_ARMLET));
	gmInventoryEquipStub(
		ISAAC, gmInventoryAddItemStub(ISAAC, INVENTORY_BROKEN | ITEM_MURAMASA));
	gmInventoryAddItemStub(IVAN, ITEM_BONE);
	gmUnitGetStub(MIA)->poison = POISON_NORMAL;
	gmUnitGetStub(JENNA)->poison = POISON_NORMAL;
	gmUnitGetStub(IVAN)->haunt = 1;
	mnShopRun(SHOP_DEBUG_STOCK_ROW, DEBUG_SHOP_KEEPER);
	return 0;
}

/* Waits a frame at a time while a jingle holds back the music, as the inn's
   does through the night. */
void mnSndWaitForJingle(void)
{
	while(sySndGetBGMRestoreDelayStub() != 0) {
		sySynch(1);
	}
}

/* Shows a shopkeeper's line and waits until it is typed out. The line is given
   as a weapon shop's: an armor or item shop says its own, the same distance on
   in the text, and a warriors' shop its own beyond that. */
void mnShopShowKeeperMessage(Sint32 messageId)
{
	SHOP_STATE *shop = MenuState;
	Sint32		voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);
	Sint32		resolvedMessage = messageId;
	Sint8		shopType;

	mnMessageCloseFinishedWindowsStub();
	shopType = shop->shopType;
	if(shopType == SHOP_TYPE_ARMOR)
		resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	if(shopType == SHOP_TYPE_ITEMS)
		resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
	/* A warriors' shop adds its distance on top of its type's */
	if((Sint8)shop->warriorShop != 0)
		resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	mnMessageOpenWindowStub(resolvedMessage, 5, 0,
		MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
			MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0)
		sySynch(1);
	sySynch(1);
}

/* Shows a shopkeeper's line as mnShopShowKeeperMessage does, hiding the hand
   cursor while it shows and putting back its motion after. */
void mnShopShowKeeperMessageHidingCursor(Sint32 messageId)
{
	Sint32		 voice;
	Sint32		 resolvedMessage;
	Sint8		 shopType;
	Sint8		 warriorShop;
	Uint8		 savedCursorMode;
	SHOP_STATE	*shop;
	WINDOW_SPR **cursorSlot;

	shop = MenuState;
	cursorSlot = &shop->cursor.anchor;
	savedCursorMode = (*cursorSlot)->motion;
	resolvedMessage = messageId;
	voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);
	shopType = shop->shopType;
	if(shopType == SHOP_TYPE_ARMOR) {
		resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	}
	if(shopType == SHOP_TYPE_ITEMS) {
		resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
	}
	warriorShop = shop->warriorShop;
	if(warriorShop != 0) {
		resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	}
	(*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	mnMessageCloseFinishedWindowsStub();
	mnMessageOpenWindowStub(resolvedMessage, 5, 0,
		MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
			MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0) {
		sySynch(1);
	}
	sySynch(1);
	shop->cursor.anchor->motion = savedCursorMode;
}

/* Runs the Yes or No choice beside the shopkeeper's line: 0 for Yes, 1 for No
   or B. */
Sint32 mnShopAskYesNo(Sint32 choice)
{
	WINDOW_SPR **cursorSlot = &((SHOP_STATE *)MenuState)->cursor.anchor;
	Uint8		 savedCursorMode = (*cursorSlot)->motion;
	mnIconShowStub(*cursorSlot);
	choice = mnYesNoMenuRunAtStub(SHOP_CONFIRM_MENU_X, 5, choice);
	(*cursorSlot)->motion = savedCursorMode;
	return choice;
}

/* Runs the Yes or No choice at the sanctum's place on the screen and returns
   it, keeping the hand cursor's motion. */
Sint32 mnSanctumAskYesNo(Sint32 choice)
{
	WINDOW_SPR **cursorSlot = &((SHOP_STATE *)MenuState)->cursor.anchor;
	Uint8		 savedCursorMode = (*cursorSlot)->motion;
	mnIconShowStub(*cursorSlot);
	choice = mnYesNoMenuRunAtStub(7, 7, choice);
	(*cursorSlot)->motion = savedCursorMode;
	return choice;
}

/* Returns the node `state->cnt` next links on from `state->node`. Nothing calls
   it. */
UNUSED SELECTION_NODE_LINK_VIEW *syNodeChainGetNodeAtIndex(
	SELECTION_MENU_CURSOR_VIEW *state)
{
	SELECTION_NODE_LINK_VIEW *node = state->node;
	Sint32					  linkIndex;

	for(linkIndex = 0; linkIndex != state->cnt; ++linkIndex) {
		node = node->next;
	}
	return node;
}

#undef SHOP_DEBUG_STOCK_ROW
#undef DEBUG_SHOP_COINS
#undef DEBUG_SHOP_KEEPER
