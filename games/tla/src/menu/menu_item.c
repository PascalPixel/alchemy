/* The item menu, as far as this object holds it: the field menu's Item screen,
   its command loop, Use, the first screen's setup and the item icons. Part of
   menu.c. */

#include "field/field_mode.h"
#include "shop/shop.h"

/* Each item command's state in the grid. */
#define ITEM_COMMAND_STATE_DISABLED	 (-1)
#define ITEM_COMMAND_STATE_AVAILABLE 1

/* What gmInventoryEquipStub returns when it does not equip the item. */
typedef enum EQUIP_REFUSAL {
	EQUIP_REFUSED_CANNOT_WEAR = -1, /* the party member cannot wear it */
	EQUIP_REFUSED_CURSED = -2		/* what it would replace is cursed on */
} EQUIP_REFUSAL;

/* Defined below or in later parts of the menu object. */
Sint32 mnItemMenuChooseCommand(void);
/* Copies the unit's items into the list items (32 entries), skipping empty
   slots and clearing the rest. Returns how many there are. mode is not used. */
Sint32 mnItemMenuCollectItems(UNIT *owner, Uint16 *items, Sint32 mode);
Sint32 mnItemMenuConfirmDrop(Sint32 item);
Sint32 mnItemMenuConfirmEquip(Sint32 unused);
Sint32 mnItemMenuCountItems(Sint32 ownerId);
void   mnItemMenuDrawIcons(Uint16 *items, Sint32 style);
void   mnItemMenuDrawItemHead(void);
void   mnItemMenuDrawMessage(Sint32 unused, Sint32 message);
Sint32 mnItemMenuGetItemQuantity(Sint32 owner, Sint32 item);
Bool   mnItemMenuIsStatRaisingItem(Sint32 itemId);
void   mnItemMenuRefreshMember(Sint32 ownerId, Sint32 mode);
Sint32 mnItemMenuRunList(Sint32 pane);
Sint32 mnItemMenuSelectGiveQuantity(
	Sint32 baseQuantity, Sint32 quantityRange, Sint32 dropMode);
Sint32 mnItemMenuSelectMember(Sint32 pane);

/* Whom mnItemMenuSelectTarget chooses. */
typedef enum ITEM_TARGET_MODE {
	ITEM_TARGET_USE = 0, /* the party member to use the item on */
	ITEM_TARGET_GIVE = 1 /* the party member to give it to */
} ITEM_TARGET_MODE;

Sint32 mnItemMenuSelectTarget(Sint32 mode);
void   mnItemMenuPlaceItemWindowBelowQuestion(void);
void   mnItemMenuPlaceItemWindowBelowItemQuestion(void);
void   mnItemMenuFitMessageWindowToQuestion(void);
void   mnItemMenuFitMessageWindowToItemQuestion(void);
Sint32 mnItemMenuShowDetails(Sint32 value);
void   mnItemMenuTryBreak(void);
Sint32 mnItemMenuUse(void);
Sint32 mnItemClassifyUseMode(Sint32 owner, Sint32 itemId);

/* Where the edition's text goes. The international editions have longer words,
   and some languages move a message to fit its own.

   The x of the small message windows "You were cursed!" and "Removed". */
#define ITEM_MODAL_SHORT_X	 JAPANESE_OR(15, 14)
#define ITEM_MODAL_REMOVED_X JAPANESE_OR(17, 14)

/* "Can't trade it!" starts four tiles further left in French, over the status
   window, which is redrawn after it. */
#define ITEM_CANNOT_TRADE_X FRENCH_OR(11, 15)

/* The column "Traded" opens at, two tiles further left in Italian. */
#define ITEM_TRADED_X ITALIAN_OR(13, 15)

void   mnLoadStatArrows(void);
void   mnItemMenuClose(void);
Sint32 mnItemMenuRunCommands(
	Sint32 *memberOut, Sint32 *targetOut, Sint32 *itemOut);
void mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns);
void mnItemMenuHideAllIcons(void);

/* The field menu's Item screen: choose a party member, one of their items and a
   command. Owns the menu state while it runs and frees it before returning.
   Returns mnItemMenuRunCommands' result: 1 when an item is to be used on the
   field, -1 when the player backed out or pressed Start. */
Sint32 mnItemMenuRun(void)
{
#if ITEM_MENU_EXTRA_TEXT_TILES
	void *backup;
#endif
	ITEM_MENU_STATE *screen;
	Sint32			 item;
	Sint32			 target;
	Sint32			 owner;
	Sint32			 result;
#if ITEM_MENU_EXTRA_TEXT_TILES
	Sint32 size;
#endif

	screen = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
#if ITEM_MENU_EXTRA_TEXT_TILES
	size = MENU_SAVED_TILE_BYTES;
	backup = syHeapAllocEWRAM(size);
#endif
	Ram_HeapSlots->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	screen->partyCnt = gmPartyListMembersStub(screen->memberIds);
	mnItemMenuInit(0, 3, 0, 7);
	mnLoadStatArrows();
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	screen->questionWin =
		(WINDOW *)mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
#if ITEM_MENU_EXTRA_TEXT_TILES
	mnSuspendFieldEvents();
	syWordsCopy(backup, (void *)BG_CHAR_BLOCK(1), size);
	syWordsFill((void *)BG_CHAR_BLOCK(1), size, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif

	mnStartWatchingStart();
	result = mnItemMenuRunCommands(&owner, &target, &item);
	mnStopWatchingStart();
	if(result == 1) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		/* The item goes to the field engine to be used, as fdUseItem reads
		   it. */
		state->item =
			(owner << ACT_UNIT_SHIFT) | (item & INVENTORY_ITEM_ID_MASK);
		state->itemSlot = screen->selectedSlots[0];
	}

	mnWindowSprClearListStub(screen->statWin);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	mnFreeArrows();
	syHeapSlotRelease(HEAP_SLOT_MENU);
	Ram_HeapSlots->actorSystem->suspended = 0;
#if ITEM_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy((void *)BG_CHAR_BLOCK(1), backup, size);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	syHeapRelease(backup);
	sySynch(1);
	mnResumeFieldEvents();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	fdPartyClearUnusableShortcutsStub();
	return result;
}

/* The resident IWRAM word copy, called through its address. */
typedef Sint32 (*ITEM_WORD_COPY_FN)(void *dst, const void *src, Sint32 size);

/* syWordsCopy's function reached from syIWRAMIRQMain rather than at its fixed
   address: the trade's snapshot copies call it this way. */
#define ITEM_IWRAM_COPY_WORDS \
	((ITEM_WORD_COPY_FN)(syIWRAMIRQMain + syWordsCopyOffset))

/* The item command grid, by row * 3 + column: Use, Equip and Details on top,
   Give, Remove and Drop below. */
typedef enum ITEM_MENU_COMMAND {
	ITEM_COMMAND_USE,
	ITEM_COMMAND_EQUIP,
	ITEM_COMMAND_DETAILS,
	ITEM_COMMAND_GIVE,
	ITEM_COMMAND_REMOVE,
	ITEM_COMMAND_DROP
} ITEM_MENU_COMMAND;

/* The steps of the item menu's command loop (mnItemMenuRunCommands). */
typedef enum ITEM_MENU_STEP {
	ITEM_STEP_MEMBER = 0,
	ITEM_STEP_ITEM = 1,
	ITEM_STEP_USE = 2,
	ITEM_STEP_EQUIP = 3,
	ITEM_STEP_GIVE = 4,
	ITEM_STEP_DROP = 5,
	ITEM_STEP_TARGET = 6, /* Give: choose whom to give it to */
	ITEM_STEP_TRADE = 7,  /* the receiver is full: choose what they give back */
	ITEM_STEP_IDLE = 8,	  /* nothing sets it */
	ITEM_STEP_COMMAND = 9,
	ITEM_STEP_DETAILS = 10, /* mnItemMenuShowDetails */
	ITEM_STEP_REMOVE = 11,
	ITEM_STEP_QUANTITY = 12 /* nothing sets it */
} ITEM_MENU_STEP;

#ifdef MODERN
/* The item menu calls this with no prototype in scope, so its argument takes
   the old promotions; a current GCC, which refuses implicit declarations, gets
   it here. */
void gmUnitRefreshPsynergyStub(Sint32 unit);
#endif
void mnItemMenuDrawEquipPreview(
	Sint32 member, Sint32 slot, Sint32 mode, Sint32 target);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
void mnItemMenuHideTradedIcons(void);
#endif

/* Runs the item menu: choose a party member, one of their items and a command
   (Use, Equip, Give, Drop, Remove or Details), then carry it out, until the
   player backs out or presses Start. Returns 1 when an item is to be used
   outside the menu, with its party member, target and item id in the out
   parameters; -1 when the player backed out or pressed Start; 0 otherwise. */
Sint32 mnItemMenuRunCommands(
	Sint32 *ownerOut, Sint32 *targetOut, Sint32 *itemOut)
{
	Sint32			 done;
	Sint32			 selection;
	Sint32			 menuResult;
	UNIT			*sourceUnit;
	UNIT			*targetUnit;
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	void			*sourceCopy;
	void			*targetCopy;
	Uint32			 state;
	Sint32			 command;
	Sint32			 result;
	Sint32			 heldCount;
	Sint32			 amount;
	Sint32			 transferFailed;
	Sint32			 removedCount;
	Sint32			 giveTarget;
	Uint8			 sourceQuantity;
	Uint8			 targetQuantity;
	Sint8			 stackSize;
	Sint32			 size;

	done = 0;
	selection = 0;
	menuResult = 0;
	state = ITEM_STEP_MEMBER;
	menu = Ram_HeapSlots->menuState;

	while(done == 0 && syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
		switch(state) {
		/* Choose whose items to look at. */
		case ITEM_STEP_MEMBER:
			menu->selectedSlots[0] = 0;
			mnItemMenuFitMessageWindowToQuestion();
			mnItemMenuPlaceItemWindowBelowQuestion();
			menu->selectedItemIcon->motion = ICON_HIDDEN;
			mnItemMenuDrawMessage(0, (Sint32)&MsgWhoseItem);
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Japanese redraws the information window after each modal message
			   instead. */
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
#endif
			mnDrawCoins((Sint32)menu->infoWin);
			command = mnItemMenuSelectMember(0);
			if(command == -1) {
				/* DEAD: selection and command cleared though the menu ends
				   before reading either; the ROM keeps the 0 and its store to
				   selection. */
				selection = command = 0;
				menuResult = -1;
				done = 1;
			}
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
			mnItemMenuHideAllIcons();
			state = ITEM_STEP_ITEM;
			break;

		/* Choose one of their items. */
		case ITEM_STEP_ITEM:
			if(mnItemMenuCountItems(menu->sideMember[0]) == 0) {
				state = ITEM_STEP_MEMBER;
				break;
			}
			mnItemMenuFitMessageWindowToQuestion();
			mnItemMenuPlaceItemWindowBelowQuestion();
			menu->selectedItemIcon->motion = ICON_HIDDEN;
			menu->sideIcons[0]->motion = ICON_SHOWN;
			mnItemMenuDrawMessage(0, (Sint32)&MsgWhichItem);
			selection = mnItemMenuRunList(0);
			state = ITEM_STEP_MEMBER;
			if(selection == -1) {
				break;
			}
			menu->listMode = -1;
			state = ITEM_STEP_COMMAND;
			break;

		/* Choose what to do with it. */
		case ITEM_STEP_COMMAND:
			command = mnItemMenuChooseCommand();
			if(command == -1) {
				state = ITEM_STEP_ITEM;
				menu->completionFlg = 1;
			}
			if(command == ITEM_COMMAND_USE) {
				if(fdHasItemCueStub(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0) {
					done = 1;
					*ownerOut = menu->sideMember[0];
					*targetOut = command;
					*itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
					menuResult = 1;
					break;
				}
				result = mnItemClassifyUseMode(
					menu->sideMember[0], menu->selectedItems[0]);
				if(result == ITEM_USE_MODE_CHOOSE_MEMBER) {
					state = ITEM_STEP_USE;
				}
				if(result == ITEM_USE_MODE_WHOLE_PARTY) {
					mnItemMenuUse();
					mnWindowSprClearListStub(menu->infoWin);
					mnItemMenuShowMessage(
						menu->msgOfs + (Sint32)MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					menu->itemCnt = mnItemMenuCollectItems(
						syUnitGet(menu->sideMember[0]), menu->items, 0);
					mnItemMenuDrawIcons(menu->items, 0);
					state = ITEM_STEP_MEMBER;
				}
				if(result == ITEM_USE_MODE_UNUSABLE ||
					result == ITEM_USE_MODE_FROM_FIELD) {
					done = 1;
					*ownerOut = menu->sideMember[0];
					*targetOut = menu->sideMember[1];
					*itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
					menuResult = 1;
				}
			}
			if(command == ITEM_COMMAND_EQUIP) {
				state = ITEM_STEP_EQUIP;
			}
			if(command == ITEM_COMMAND_GIVE) {
				state = ITEM_STEP_TARGET;
			}
			if(command == ITEM_COMMAND_DROP) {
				state = ITEM_STEP_DROP;
			}
			if(command == ITEM_COMMAND_REMOVE) {
				state = ITEM_STEP_REMOVE;
			}
			if(command == ITEM_COMMAND_DETAILS) {
				state = ITEM_STEP_DETAILS;
			}
			break;

		/* Use the item on the party member chosen. */
		case ITEM_STEP_USE:
			mnItemMenuHideAllIcons();
			mnItemMenuFitMessageWindowToItemQuestion();
			mnItemMenuPlaceItemWindowBelowItemQuestion();
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->questionWin);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUseOnWhom, menu->questionWin, 16, 16);
			if(mnItemMenuSelectTarget(ITEM_TARGET_USE) != -1) {
				command = 0;
				if(mnItemMenuIsStatRaisingItem(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0) {
					command = STATUS_PANEL_STATS;
				}
				result = mnItemMenuUse();
				mnStatPanelDraw(menu->statWin, menu->sideMember[1], 0, command);
				if(result != -1) {
					mnWindowSprClearListStub(menu->infoWin);
					mnItemMenuShowMessage(
						menu->msgOfs + (Sint32)MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					mnItemMenuTryBreak();
					state = ITEM_STEP_ITEM;
				}
				menu->itemCnt = mnItemMenuCollectItems(
					syUnitGet(menu->sideMember[0]), menu->items, 0);
				mnItemMenuDrawIcons(menu->items, 0);
				menu->completionFlg = 1;
			} else {
				state = ITEM_STEP_COMMAND;
			}
			break;

		/* Give: choose to whom. */
		case ITEM_STEP_TARGET:
			mnItemMenuFitMessageWindowToItemQuestion();
			mnItemMenuPlaceItemWindowBelowItemQuestion();
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->questionWin);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgGiveToWhom, menu->questionWin, 16, 16);
			giveTarget = mnItemMenuSelectTarget(ITEM_TARGET_GIVE);
			state = ITEM_STEP_GIVE;
			if(giveTarget == -1) {
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
				state = ITEM_STEP_COMMAND;
			}
			break;

		/* Drop the item, or some of a stack, after confirming. */
		case ITEM_STEP_DROP:
			mnItemMenuHideAllIcons();
			item = gmItemGetDirectStub(
				menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
			result = 0;
			if((item->flg & ITEM_STACKABLE) != 0) {
				heldCount =
					(menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if(heldCount > 1) {
					mnItemMenuDrawItemHead();
					result = mnItemMenuSelectGiveQuantity(0, heldCount, 1);
				}
			}
			if(result == -1) {
				state = ITEM_STEP_COMMAND;
				break;
			}
			menu->sideMember[1] = 0;
			mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM,
				(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) |
					(result << INVENTORY_QUANTITY_SHIFT),
				(Uint8)menu->selectedItemIcon->idx, 0);
			menu->selectedItemIcon->motion = ICON_SHOWN;
			menu->selectedItemIcon->x = 120;
			menu->selectedItemIcon->y = 28;
			mnIconShow(menu->selectedItemIcon);
			mnWindowClearInteriorTilesStub(menu->listWin, 0, 72, 120, 96);
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->questionWin);
			/* Yes: take result + 1 of the stack out, one at a time. */
			if(mnItemMenuConfirmDrop(selection) == 0) {
				command = menu->sideMember[0];
				/* DEAD: syUnitGet's result is not used; the ROM keeps the
				   call. */
				syUnitGet(command);
				for(removedCount = 0; removedCount < result + 1;
					removedCount++) {
					gmInventoryRemoveStub(command, menu->selectedSlots[0]);
					gmItemAdjustRareItemStockStub(
						menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK, 1);
				}
				gmUnitRefreshPsynergyStub(command);
				gmUnitCalcStub(command);
				mnItemMenuPlaceItemWindowBelowQuestion();
				mnItemMenuRefreshMember(menu->sideMember[0], 0);
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				sySynch(1);
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage((Sint32)MsgDroppedIt, 15, 13);
				menu->completionFlg = 1;
				state = ITEM_STEP_ITEM;
			} else {
				state = ITEM_STEP_COMMAND;
			}
			gmUnitCalcStub(menu->sideMember[0]);
			menu->selectedItemIcon->motion = ICON_HIDDEN;
			fdPartyClearUnusableShortcutsStub();
			break;

		/* Give the item, or some of a stack, to the chosen party member. */
		case ITEM_STEP_GIVE:
			transferFailed = 0;
			item = gmItemGetDirectStub(
				menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
			if((item->flg & ITEM_STACKABLE) != 0) {
				/* A receiver with a full stack of it takes none; one with no
				   free slot and none of it must trade. */
				heldCount = mnItemMenuGetItemQuantity(menu->sideMember[1],
					menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
				if(heldCount == INVENTORY_STACK_MAX) {
					transferFailed = 1;
				}
				if(mnItemMenuCountItems(menu->sideMember[1]) ==
						INVENTORY_SLOTS &&
					heldCount == 0) {
					state = ITEM_STEP_TRADE;
					break;
				}
				stackSize =
					(menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if(transferFailed == 0) {
					if(heldCount + stackSize > INVENTORY_STACK_MAX) {
						amount = INVENTORY_STACK_MAX - heldCount;
					} else {
						amount = stackSize;
					}
					if(stackSize > 1) {
						result = mnItemMenuSelectGiveQuantity(0, amount, 0);
					} else {
						result = 0;
					}
					if(result == -1) {
						state = ITEM_STEP_TARGET;
						break;
					}
					/* Move result + 1 one at a time; the broken bit goes with
					   each, the equipped bit and the quantity do not. */
					for(command = 0; command < result + 1; command++) {
						heldCount = gmInventoryAddItemStub(menu->sideMember[1],
							menu->selectedItems[0] &
								(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
						if(heldCount != -1) {
							gmInventoryRemoveStub(
								menu->sideMember[0], menu->selectedSlots[0]);
							menu->selectedSlots[1] = heldCount;
						} else {
							transferFailed = 1;
						}
					}
				}
			} else {
				result = gmInventoryAddItemStub(menu->sideMember[1],
					menu->selectedItems[0] &
						(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
				if(result == -1) {
					state = ITEM_STEP_TRADE;
					break;
				}
				menu->selectedSlots[1] = result;
				result = gmInventoryRemoveStub(
					menu->sideMember[0], menu->selectedSlots[0]);
				if(result == INVENTORY_REMOVE_EMPTY_SLOT) {
					transferFailed = 1;
				}
			}
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			gmUnitRefreshPsynergyStub(menu->sideMember[1]);
			gmUnitCalcStub(menu->sideMember[0]);
			gmUnitCalcStub(menu->sideMember[1]);
			result = 1;
			if(transferFailed == 0) {
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
				mnItemMenuFitMessageWindowToLongItemQuestion();
#endif
				mnWindowSprRedrawSavedRectStub((WINDOW *)menu->questionWin);
				mnItemMenuDrawItemHead();
				result = mnItemMenuConfirmEquip(0);
			}
			if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0) {
				break;
			}
			mnItemMenuRefreshMember(menu->sideMember[1], 1);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			sySynch(1);
			/* Say how it went: the receiver's stack was full, the item was
			   given, or it was given and equipped (mnItemMenuConfirmEquip
			   returned 0). */
			if(transferFailed == 1) {
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage((Sint32)MsgCannotHoldMore, 15, 14);
			} else {
				mnWindowSprClearListStub(menu->infoWin);
				if(result == 1) {
					mnItemMenuShowMessage((Sint32)MsgGiven, 15, 14);
				} else {
					mnItemMenuDrawEquipPreview(menu->sideMember[1],
						menu->selectedSlots[1], 0, menu->sideMember[1]);
					mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 14);
					item = gmItemGetDirectStub(menu->selectedItems[0]);
					if((item->flg & ITEM_CURSED) != 0) {
						sySndPlayStub(SE_CURSE);
						mnWindowSprClearListStub(menu->infoWin);
						/* "You were cursed!" */
						mnItemMenuShowMessage(
							(Sint32)MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 14);
					}
				}
			}
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_MEMBER;
			break;

		/* Trade: the receiver gives back one of their items. */
		case ITEM_STEP_TRADE:
			transferFailed = 0;
			mnItemMenuFitMessageWindowToQuestion();
			mnItemMenuPlaceItemWindowBelowQuestion();
			mnItemMenuDrawMessage(0, (Sint32)&MsgSwapForWhat);
			if(mnItemMenuRunList(1) == -1) {
				state = ITEM_STEP_TARGET;
				break;
			}
			/* Snapshot both inventories so any failed part of the trade can
			   roll back. */
			sourceUnit = syUnitGet(menu->sideMember[0]);
			targetUnit = syUnitGet(menu->sideMember[1]);
			size = BATTLE_UNIT_SIZE;
			sourceCopy = syHeapAlloc(size);
			targetCopy = syHeapAlloc(size);
			ITEM_IWRAM_COPY_WORDS(sourceCopy, sourceUnit, size);
			ITEM_IWRAM_COPY_WORDS(targetCopy, targetUnit, size);
			/* Take both whole stacks out, one item at a time (at most
			   INVENTORY_STACK_MAX): gmInventoryRemoveStub returns
			   INVENTORY_REMOVE_LAST for the last one and
			   INVENTORY_REMOVE_EMPTY_SLOT for an empty slot. */
			for(sourceQuantity = 0; sourceQuantity < INVENTORY_STACK_MAX;
				sourceQuantity++) {
				result = gmInventoryRemoveStub(
					menu->sideMember[0], menu->selectedSlots[0]);
				if(result == INVENTORY_REMOVE_LAST) {
					break;
				}
				if(result == INVENTORY_REMOVE_EMPTY_SLOT) {
					transferFailed = 1;
					break;
				}
			}
			sourceQuantity++;
			for(targetQuantity = 0; targetQuantity < INVENTORY_STACK_MAX;
				targetQuantity++) {
				/* The receiver cannot trade away a worn item that will not come
				   off. */
				if((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0) {
					item = gmItemGetDirectStub(
						menu->selectedItems[1] & INVENTORY_ITEM_ID_MASK);
					if((item->flg & ITEM_CANNOT_UNEQUIP) != 0) {
						transferFailed = 1;
					}
				}
				result = gmInventoryRemoveStub(
					menu->sideMember[1], menu->selectedSlots[1]);
				if(result == INVENTORY_REMOVE_LAST) {
					break;
				}
				if(result == INVENTORY_REMOVE_EMPTY_SLOT) {
					transferFailed = 1;
					break;
				}
			}
			targetQuantity++;
			/* Then give each the other's items. */
			while(sourceQuantity != 0) {
				result = gmInventoryAddItemStub(menu->sideMember[1],
					menu->selectedItems[0] &
						(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
				if(result == -1) {
					transferFailed = 1;
					break;
				}
				menu->selectedSlots[1] = result;
				sourceQuantity--;
			}
			while(targetQuantity != 0) {
				result = gmInventoryAddItemStub(menu->sideMember[0],
					menu->selectedItems[1] &
						(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
				if(result == -1) {
					transferFailed = 1;
					break;
				}
				menu->selectedSlots[0] = result;
				targetQuantity--;
			}
			sySynch(1);
			if(transferFailed == 1) {
				syWordsCopyEntry(sourceUnit, sourceCopy, size);
				syWordsCopyEntry(targetUnit, targetCopy, size);
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage(
					(Sint32)MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if LANGUAGE == LANGUAGE_FRENCH
				mnWindowSprRedrawSavedRectStub((WINDOW *)menu->statWin);
#endif
			} else {
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[1]);
				gmUnitCalcStub(menu->sideMember[0]);
				gmUnitCalcStub(menu->sideMember[1]);
				mnItemMenuFitMessageWindowToItemQuestion();
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
				mnItemMenuFitMessageWindowToLongItemQuestion();
				mnItemMenuHideTradedIcons();
#endif
				mnWindowSprRedrawSavedRectStub((WINDOW *)menu->questionWin);
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
				mnItemMenuDrawItemHead();
				result = mnItemMenuConfirmEquip(0);
				if(syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
					mnWindowSprClearListStub(menu->infoWin);
					mnItemMenuPlaceItemWindowBelowItemQuestion();
					mnItemMenuRefreshMember(menu->sideMember[1], 1);
					if(result == 0) {
						mnItemMenuDrawEquipPreview(menu->sideMember[1],
							menu->selectedSlots[1], 0, menu->sideMember[1]);
						mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 14);
						item = gmItemGetDirectStub(menu->selectedItems[0]);
						if((item->flg & ITEM_CURSED) != 0) {
							sySndPlayStub(SE_CURSE);
							mnWindowSprClearListStub(menu->infoWin);
							/* "You were cursed!" */
							mnItemMenuShowMessage((Sint32)MsgEquippedIt + 7,
								ITEM_MODAL_SHORT_X, 14);
						}
					} else {
						mnItemMenuShowMessage(
							(Sint32)MsgTraded, ITEM_TRADED_X, 14);
					}
				}
			}
			syHeapRelease(targetCopy);
			syHeapRelease(sourceCopy);
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_MEMBER;
			break;

		case ITEM_STEP_EQUIP:
			result = gmInventoryEquipStub(
				menu->sideMember[0], menu->selectedSlots[0]);
			if(result == EQUIP_REFUSED_CANNOT_WEAR) {
				state = ITEM_STEP_ITEM;
				break;
			}
			if(result == EQUIP_REFUSED_CURSED) {
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage((Sint32)MsgCannotRemoveItem, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
				mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
#endif
				state = ITEM_STEP_ITEM;
				break;
			}
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			gmUnitCalcStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCnt = mnItemMenuCollectItems(
				syUnitGet(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			mnWindowSprClearListStub(menu->infoWin);
			mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 8);
			item = gmItemGetDirectStub(menu->selectedItems[0]);
			if((item->flg & ITEM_CURSED) != 0) {
				sySndPlayStub(SE_CURSE);
				mnWindowSprClearListStub(menu->infoWin);
				/* "You were cursed!" */
				mnItemMenuShowMessage(
					(Sint32)MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 8);
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_REMOVE:
			(syUnitGet(menu->sideMember[0]))
				->inventory[menu->selectedSlots[0]] &= ~INVENTORY_EQUIPPED;
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			gmUnitCalcStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCnt = mnItemMenuCollectItems(
				syUnitGet(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			menu->equipPreview = 1;
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			menu->equipPreview = 0;
			mnWindowSprClearListStub(menu->infoWin);
			mnItemMenuShowMessage((Sint32)MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_DETAILS:
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			mnItemMenuShowDetails(menu->selectedItems[0]);
			mnWindowSprRedrawSavedRectStub((WINDOW *)menu->statWin);
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_SHOWN;
			state = ITEM_STEP_COMMAND;
			break;

		case ITEM_STEP_QUANTITY:
			mnItemMenuSelectGiveQuantity(0, INVENTORY_STACK_MAX, 0);
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_IDLE:
			break;

		default:
			done = 1;
			break;
		}
	}

	if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0) {
		menuResult = -1;
	}
	return menuResult;
}

Sint32 mnItemUse(Sint32 slot, Sint32 owner, Sint32 target);
void   mnItemPlayUseSnd(Sint32 item);

/* Uses the selected item from the party member in side 0 on the one in side 1.
   On failure plays the error sound, shows why in the info window and returns
   -1; otherwise plays the item's animation, recalculates both party members and
   returns 1. */
Sint32 mnItemMenuUse(void)
{
	ITEM_MENU_STATE *menu;
	Sint32			 result;
	Uint32			 item;
	Uint32			 mask;

	menu = Ram_HeapSlots->menuState;
	result = mnItemUse(
		menu->selectedSlots[0], menu->sideMember[0], menu->sideMember[1]);

	if(result == -1) {
		sySndPlayStub(SE_MENU_ERROR);
		mnWindowSprClearListStub(menu->infoWin);
		mnItemMenuShowMessage(
			menu->msgOfs + (Sint32)MsgItemUseResult, result, result);
		/* Until every edition but the Japanese, which closes the message's
		   window instead. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub((WINDOW *)menu->infoWin);
#endif
		menu->completionFlg = 1;
		return result;
	}

	item = menu->selectedItems[0];
	mask = INVENTORY_ITEM_ID_MASK;
	mask &= item;
	mnItemPlayUseSnd(mask);
	gmUnitCalcStub(menu->sideMember[0]);
	gmUnitCalcStub(menu->sideMember[1]);
	return 1;
}

/* Sets up the item menu's first screen: the cursor window with the party's
   figures (mnCursorCreate, mnShowPartyFigures), the info window along the
   bottom and an 8 by 2 cursor grid. The parameters are not used. */
void mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	Sint32			 ownerIndex;

	mnShowPartyFigures(mnCursorCreate(menu), 2, 2);
	for(ownerIndex = 0; ownerIndex < 4; ownerIndex++)
		menu->memberY[ownerIndex] = MENU_MEMBER_Y;

	{
		Sint32 style;
		menu->buttonHelpWin = NULL;
		menu->statWin = NULL;
		style = WINDOW_FLAG_INSTANT;
		menu->infoWin = (WINDOW *)mnWindowCreateStub(0, 17, 30, 3, style);
		menu->itemWin = NULL;
		menu->selectedColumn = 0;
		menu->selectedRow = 0;
		menu->columnCnt = 8;
		menu->rowCnt = style; /* the grid's two rows */
	}
}

/* mnSpawnIconEntries makes the icons a row of the eight-wide icon grid at a
   time (columnCnt), as the Psynergy screen places its own. */
#define ICON_GRID_ROW 8

/* Creates the 32 item icon sprites in window, off screen until placed: slots 0
   to 7 at MENU_ICON_HIDDEN_X, the rest 8 pixels further, all at
   MENU_ICON_HIDDEN_Y, each showing item graphic slot of the same number. */
void mnSpawnIconEntries(ITEM_MENU_STATE *menu, WINDOW *window)
{
	WINDOW_SPR **firstIcons;
	Sint32		 firstSlot;
	WINDOW_SPR **secondIcons;
	Sint32		 secondSlot;
	WINDOW_SPR **remainingIcons;
	Sint32		 remainingSlot;

	firstSlot = 0;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		firstIcons = &menu->entryIcons[0];
		for(; firstSlot < ICON_GRID_ROW; firstSlot++) {
			*firstIcons++ = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, firstSlot, window, MENU_ICON_HIDDEN_X, y);
		}
	}

	secondSlot = ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		secondIcons = &menu->entryIcons[ICON_GRID_ROW];
		for(; secondSlot < 2 * ICON_GRID_ROW; secondSlot++) {
			*secondIcons++ = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, secondSlot, window, 256, y);
		}
	}

	remainingSlot = 2 * ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		remainingIcons = &menu->entryIcons[2 * ICON_GRID_ROW];
		for(; remainingSlot < MENU_ENTRY_ICONS; remainingSlot++) {
			*remainingIcons++ = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, remainingSlot, window, 256, y);
		}
	}
}

/* Hides all 32 item icons. */
void mnItemMenuHideAllIcons(void)
{
	WINDOW_SPR **icons =
		((ITEM_MENU_STATE *)Ram_HeapSlots->menuState)->entryIcons;
	Sint32 slot;

	for(slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
		WINDOW_SPR *icon = *icons++;
		if(icon != 0) {
			icon->motion = ICON_HIDDEN;
		}
	}
}

#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
/* Hides the item icons in every fifth slot, 0 to 30, after a trade. */
void mnItemMenuHideTradedIcons(void)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	Sint32			 slot;

	for(slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
		WINDOW_SPR *icon = menu->entryIcons[slot];
		if(icon != 0 && slot % 5 == 0)
			icon->motion = ICON_HIDDEN;
	}
}
#endif

#undef ITEM_IWRAM_COPY_WORDS

/* Closes the item menu: releases the party figures, hides the icons and the
   status panel's portrait (the state's cursor), and closes every window the
   menu opened. */
void mnItemMenuClose(void)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR		*cursor;

	menu = Ram_HeapSlots->menuState;
	mnReleasePartyFigures();
	mnItemMenuHideAllIcons();
	sySynch(1);
	cursor = menu->cursor;
	cursor->motion = ICON_HIDDEN;
	mnWindowCloseIfOpen((void **)&menu->mainWin, 1);
	mnWindowCloseIfOpen((void **)&menu->itemWin, 1);
	mnWindowCloseIfOpen((void **)&menu->questionWin, 1);
	mnWindowCloseIfOpen((void **)&menu->statWin, 1);
	mnWindowCloseIfOpen((void **)&menu->buttonHelpWin, 1);
	mnWindowCloseIfOpen((void **)&menu->infoWin, 1);
	mnWindowCloseIfOpen((void **)&menu->equipWin, 1);
	mnWindowCloseIfOpen((void **)&menu->listWin, 1);
	mnWindowCloseIfOpen((void **)&menu->unopenedWin, 1);
	mnWindowCloseIfOpen((void **)&menu->modalWin, 1);
	mnWindowCloseIfOpen((void **)&menu->secondUnopenedWin, 1);
}

Sint32 mnItemMenuCollectItems(UNIT *owner, Uint16 *items, Sint32 mode);
Sint32 mnItemMenuRunMemberSelection(Uint16 *ownerIds, Uint16 *items);

/* Lets the player choose a party member in side (0 or 1), starting from the
   party member chosen last. */
Sint32 mnItemMenuSelectMember(Sint32 pane)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 result = 0;
	Sint32			 index;
	void			*owner;

	index = menu->sideIdx[pane];
	mnWindowSprClearListStub(menu->infoWin);
	menu->sideCnt[pane] = menu->partyCnt;
	if(index == -1) {
		menu->sideIdx[pane] = result;
		index = 0;
	} else {
		mnCursorSlide(index * 24 - 10, 16);
	}
	owner = syUnitGet((Uint16)menu->memberIds[index]);
	menu->itemCnt = mnItemMenuCollectItems(owner, menu->items, 0);
	result = mnItemMenuRunMemberSelection(menu->memberIds, menu->items);
	mnIconShow(menu->sideIcons[pane]);
	sySynch(1);
	return result;
}

#if LANGUAGE == LANGUAGE_JAPANESE
/* `L+A:`, the key name Japanese draws before the arrange help. */
extern const Uint8 ItemMenu_ArrangeKeysString[];
/* `R:`, the key name Japanese draws before the equipment help. */
extern const Uint8 ItemMenu_EquipmentKeyString[];
#endif

void mnItemMenuShowEquipment(Sint32 ownerId);

void mnItemMenuRefreshMember(Sint32 owner, Sint32 mode);

Sint32 mnItemMenuCountItems(Sint32 owner);

/* The party member row of the item menu: moving shows each member's items and
   status, holding R their equipment; L + A sorts the member's items, a new
   order each press; A chooses a member who has items and B backs out. The
   members are paged four at a time by mnMoveGridCursor, which moves the
   selection and says what to redraw (2 for a new page). Returns the chosen
   member's id, or -1. */
Sint32 mnItemMenuRunMemberSelection(Uint16 *ownerIds, Uint16 *items)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 selection = menu->sideIdx[0];
	Sint32			 count;
	Sint32			 redraw;
	Sint32			 result;
	Uint8			 sortMode;
	Sint32			 showEquipment;
	UNIT			*owner;
	void			*window;

	count = menu->sideCnt[0];
	redraw = 2;
	result = 0;
	sortMode = 0;
	showEquipment = 0;
	owner = syUnitGet(ownerIds[selection]);

	if(mnWindowUpdateOrCreate(&menu->itemWin, 13, 3, 17, 10, redraw))
		mnSpawnIconEntries(menu, menu->itemWin);
	if(mnWindowUpdateOrCreate(&menu->buttonHelpWin, 13, 13, 17, 4, redraw)) {
		menu->selectedItemIcon = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->buttonHelpWin, 0, result);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
	}
#if LANGUAGE == LANGUAGE_JAPANESE
	/* Japanese draws the key names as their own strings, the help beside
	   them. */
	mnTextDrawStringInWindowStub(
		ItemMenu_ArrangeKeysString, menu->buttonHelpWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp, menu->buttonHelpWin, 32, 0);
	mnTextDrawStringInWindowStub(
		ItemMenu_EquipmentKeyString, menu->buttonHelpWin, 16, 8);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp + 1, menu->buttonHelpWin, 32, 8);
#else
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp, menu->buttonHelpWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp + 1, menu->buttonHelpWin, 0, 8);
#endif
	menu->sideIcons[0]->motion = ICON_SHOWN;
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			sortMode = 0;
			window = menu->statWin;
			owner = syUnitGet(ownerIds[selection]);
			if(showEquipment) {
				menu->itemCnt = mnItemMenuCollectItems(
					syUnitGet(ownerIds[selection]), menu->items, 0);
				mnItemMenuShowEquipment(ownerIds[selection]);
				mnStatPanelDraw(
					window, ownerIds[selection], 0, STATUS_PANEL_STATS);
			} else {
				mnItemMenuRefreshMember(ownerIds[selection], 0);
				mnStatPanelDraw(
					window, ownerIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			}
			if(redraw == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->mainWin, selection, count);
		}
		mnCursorPos(selection % 4 * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			if(PerWork.data & KEY_L) {
				sortMode = (sortMode + 4) % 4;
				mnItemMenuSort(owner->inventory, sortMode);
				sortMode++;
				mnItemMenuRefreshMember(ownerIds[selection], 0);
				sySndPlayStub(SE_MENU_CONFIRM);
			} else if(mnItemMenuCountItems(ownerIds[selection])) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = ownerIds[selection];
				break;
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerWork.push & KEY_R) {
			showEquipment = 1;
			redraw = 1;
		} else if(!(PerWork.data & KEY_R) && showEquipment == 1) {
			showEquipment = 0;
			redraw = 1;
		} else {
			redraw = mnMoveGridCursor(&selection, count, 4);
		}
	}
	menu->sideIdx[0] = selection;
	menu->selectedMember = ownerIds[selection];
	menu->sideMember[0] = ownerIds[selection];
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return result;
}

/* Shows the equipment view of party member ownerId's items in the item
   window. */
void mnItemMenuShowEquipment(Sint32 ownerId)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	mnHideEntryIcons();
	mnWindowSprRedrawSavedRectStub(menu->itemWin);
	mnItemMenuDrawEquipment(menu->itemWin, ownerId, 0);
}

/* The Japanese release writes the item's description into the info window as
   text, the others as tiles. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define TRADE_LABEL_X	   16
#define IN_STOCK_LABEL_X   24
#define mnItemMenuDrawInfo mnTextDrawMessageStub
#else
#define TRADE_LABEL_X	   0
#define mnItemMenuDrawInfo mnTextDrawMessageAsTilesStub
#endif

/* The Japanese release clears the info window; in every edition but the
   Japanese it redraws the window's saved rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define mnItemMenuClearInfo mnWindowSprRedrawSavedRectStub
#else
#define mnItemMenuClearInfo mnWindowSprClearListStub
#endif

#if (LANGUAGE == LANGUAGE_SPANISH)
#define IN_STOCK_LABEL_X 30
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define IN_STOCK_LABEL_X 29
#elif (LANGUAGE != LANGUAGE_JAPANESE)
#define IN_STOCK_LABEL_X 32
#endif

void mnEquipmentMenuUpdateCompatibilityIndicators(void);

Sint32 mnItemMenuGetItemQuantity(Sint32 owner, Sint32 item);

Bool mnItemMenuIsStatRaisingItem(Sint32 item);

void mnEquipmentMenuStopCompatibilityIndicators(void);

/* Chooses the party member to use or give the selected item (enum
   ITEM_TARGET_MODE) to. */
Sint32 mnItemMenuSelectTarget(Sint32 mode)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 selection = menu->sideIdx[1];
	void			*window = menu->itemWin;
	Sint32			 count = menu->partyCnt;
	Uint8			 result = 0;
	Bool			 descriptionShown = FALSE;
	Sint8			 redraw;
	Sint32			 heldCount;
	WINDOW_SPR		*marker;

	mnWindowSetBounds(window, 13, 5, 17, 12);
	mnWindowSprRedrawSavedRectStub(menu->itemWin);
	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(menu->memberIds[menu->sideIdx[0]]);
	sySetEvent(mnEquipmentMenuUpdateCompatibilityIndicators, EVENT_PASS_FRAME);
	redraw = 2;
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			window = menu->itemWin;
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberIds[selection]);
			marker = menu->sideIcons[1];
			marker->spr.oam.x = marker->x =
				((selection * 3 + menu->mainWin->x) << 3) - 2;
			if(redraw == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->mainWin, selection, count);
			if(mode == ITEM_TARGET_GIVE) {
				mnItemMenuRefreshMember(menu->memberIds[selection], 1);
				mnWindowDrawDividerLineStub(window, 0, 9, 16, 9);
				mnWindowClearInteriorTilesStub(window, 0, 72, 120, 80);
				if(selection != menu->sideIdx[0]) {
					heldCount =
						mnItemMenuGetItemQuantity(menu->memberIds[selection],
							menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
					if(mnItemMenuCountItems(menu->memberIds[selection]) ==
							INVENTORY_SLOTS &&
						heldCount == 0) {
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgTradeForWhat, window, TRADE_LABEL_X, 72);
					} else if(heldCount != 0) {
						mnTextDrawNumInWindowStub(heldCount, 2, window, 8, 72);
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgInStock, window, IN_STOCK_LABEL_X, 72);
					} else {
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgNoneInStock, window, 16, 72);
					}
				}
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->memberIds[selection]);
			}
			if(mode == ITEM_TARGET_USE) {
				if(mnItemMenuIsStatRaisingItem(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
					mnStatPanelDraw(menu->statWin, menu->memberIds[selection],
						menu->selectedSlots[0], STATUS_PANEL_STATS);
				else
					mnStatPanelDraw(menu->statWin, menu->memberIds[selection],
						menu->selectedSlots[0], STATUS_PANEL_EXPERIENCE);
				if(!syGameFlagGet(FLAG_INFO_WINDOW_HOLDS_MESSAGE) &&
					!descriptionShown) {
					mnItemMenuClearInfo(menu->infoWin);
					mnItemMenuDrawInfo(
						(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
							(Sint32)MsgItemDescription,
						menu->infoWin, 0, 0);
					descriptionShown = TRUE;
				} else {
					syGameFlagClear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
				}
			}
			redraw = 0;
		}
		mnCursorPos(selection % 4 * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			if(mode == ITEM_TARGET_GIVE && selection == menu->sideIdx[0]) {
				sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			sySndPlayStub(SE_MENU_CONFIRM);
			result = menu->memberIds[selection];
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		redraw = mnMoveGridCursor(&selection, count, 4);
	}
	marker = menu->sideIcons[1];
	mnIconShow(marker);
	marker->motion = ICON_HIDDEN;
	mnEquipmentMenuStopCompatibilityIndicators();
	sySynch(1);
	menu->sideIdx[1] = selection;
	menu->selectedMember = menu->memberIds[selection];
	menu->sideMember[1] = menu->memberIds[selection];
	selection = menu->sideIdx[0];
	count = menu->sideCnt[0];
	mnCreatePartyFigures(selection / 4 * 4);
	mnMarkChosenMember(menu->mainWin, selection, count);
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return (Sint8)result;
}

#undef IN_STOCK_LABEL_X

/* Event while mnItemMenuSelectTarget chooses a party member to use or give the
   item to, and while the command grid's cursor is on Give: every 32 frames, has
   each party figure nod (ANIM_NOD) if that party member can equip the item, or
   stand (ANIM_STAND) if not. */
void mnEquipmentMenuUpdateCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint8			 i;

	if((FrameCount & 31) == 0) {
		for(i = 0; i <= 3; i++) {
			Sint32 owner = menu->memberSprIds[i];

			if(owner != -1) {
				if(gmItemCanUnitEquipDirectStub(
					   owner, menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
					fdSprStartAnimationStub(menu->memberSprs[i], ANIM_NOD);
				else
					fdSprStartAnimationStub(menu->memberSprs[i], ANIM_STAND);
			}
		}
	}
}

/* Ends mnEquipmentMenuUpdateCompatibilityIndicators: puts every party figure
   back to ANIM_STAND and removes the event. */
void mnEquipmentMenuStopCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint8			 i;

	for(i = 0; i < menu->partyCnt; i++)
		fdSprStartAnimationStub(menu->memberSprs[i], ANIM_STAND);
	syCloseEvent(mnEquipmentMenuUpdateCompatibilityIndicators);
}

/* Returns 1 for the items that raise a stat for good (Apple, Hard Nut, Mint and
   Lucky Pepper), whose use shows the stats panel instead of HP and PP. */
Bool mnItemMenuIsStatRaisingItem(Sint32 itemId)
{
	switch(itemId) {
	case ITEM_APPLE:
	case ITEM_HARD_NUT:
	case ITEM_MINT:
	case ITEM_LUCKY_PEPPER:
		return TRUE;
	}
	return FALSE;
}

/* Clears the message window and writes message into it. */
void mnItemMenuDrawMessage(Sint32 unused, Sint32 message)
{
	ITEM_MENU_STATE *menu;

	menu = Ram_HeapSlots->menuState;
	mnWindowSprRedrawSavedRectStub(menu->questionWin);
	mnTextDrawMessageAsTilesStub(message, (Sint32)menu->questionWin, 0, 0);
}

/* Hides the icon of each of the 32 list entries whose item is 0. Each of the 32
   icon slots must hold a sprite (mnSpawnIconEntries). */
void mnHideEmptyEntryIcons(const Uint16 *items)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 slot;

	for(slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
		if(items[slot] == 0) {
			mnIconShow(menu->entryIcons[slot]);
			menu->entryIcons[slot]->motion = ICON_HIDDEN;
		}
	}
}

/* Counts the nonempty slots in the owner's inventory. */
Sint32 mnItemMenuCountItems(Sint32 ownerId)
{
	Sint32 count = 0;
	UNIT  *state = syUnitGet(ownerId);
	Sint32 i;

	for(i = 0; i < INVENTORY_SLOTS; i++) {
		Sint32 itemId = state->inventory[i] & INVENTORY_ITEM_ID_MASK;

		if(itemId != 0)
			count++;
	}
	return count;
}

/* Returns how many of item party member owner holds, adding up every stack of
   it in the inventory. */
Sint32 mnItemMenuGetItemQuantity(Sint32 owner, Sint32 item)
{
	UNIT  *state;
	Sint32 i;
	Sint32 quantity;

	quantity = 0;
	state = syUnitGet(owner);
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		if(state->inventory[i] != 0 &&
			(state->inventory[i] & INVENTORY_ITEM_ID_MASK) == item) {
			quantity += (state->inventory[i] & INVENTORY_QUANTITY_MASK) >>
				INVENTORY_QUANTITY_SHIFT;
			quantity++;
		}
	}
	return quantity;
}

Sint32 mnItemMenuCollectItems(UNIT *owner, Uint16 *items, Sint32 mode)
{
	Sint32 count;
	Sint32 i;

	for(i = 0; i < MENU_ENTRY_ICONS; i++) {
		items[i] = 0;
	}
	count = 0;
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		items[i] = 0;
		if(owner->inventory[i] != 0) {
			items[count] = owner->inventory[i];
			count++;
		}
	}
	return count;
}

/* Loads each listed item's icon into its item icon sprite, the first 15 entries
   of inventoryEntries, and hides the icons of empty entries. iconStyle 0 draws
   the list's icons (ICON_KIND_ITEM); any other iconStyle the equipment
   view's. */
void mnItemMenuDrawIcons(Uint16 *inventoryEntries, Sint32 iconStyle)
{
	Sint32		 i;
	Uint16		*entries;
	WINDOW_SPR **icons;
	Sint32		 itemId;

	icons = ((ITEM_MENU_STATE *)Ram_HeapSlots->menuState)->entryIcons;
	entries = inventoryEntries;
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		itemId = *entries++;
		if(itemId != 0) {
			if(iconStyle == 0) {
				mnSelectionIconLoadIntoSlotStub(
					ICON_KIND_ITEM, itemId, (Uint8)(*icons)->idx, 0);
			} else {
				mnSelectionIconLoadIntoSlotStub(
					ICON_KIND_EQUIPMENT_ITEM, itemId, (Uint8)(*icons)->idx, 0);
			}
		}
		icons++;
	}
	mnHideEmptyEntryIcons(inventoryEntries);
}

/* Shows party member ownerId's items in the item window, laid out for layout
   mode (see mnItemMenuPlaceIcons), or "no items" when there are none. */
void mnItemMenuRefreshMember(Sint32 ownerId, Sint32 mode)
{
	ITEM_MENU_STATE *menu;
	UNIT			*owner;
	Uint16			*items;

	menu = Ram_HeapSlots->menuState;
	owner = syUnitGet(ownerId);
	items = menu->items;
	menu->itemCnt = mnItemMenuCollectItems(owner, items, 0);
	mnWindowSprRedrawSavedRectStub(menu->itemWin);
	mnItemMenuPlaceIcons(mode);
	mnItemMenuDrawIcons(items, 0);
	if(mnItemMenuCountItems(ownerId) == 0)
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgItemMenuEmpty, menu->itemWin, 8, 24);
}

/* Shows in the status panel what the item in owner's inventory slot would mean
   to party member target: for a usable item, target's status; for equipment,
   target's stats with it equipped; for a Psynergy item, the Psynergy it
   teaches. */
void mnItemMenuDrawEquipPreview(
	Sint32 owner, Sint32 slot, Sint32 mode, Sint32 target)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 style = 0;
	UNIT			*unit = syUnitGet(owner);
	Sint32			 entry = unit->inventory[slot];
	Sint32			 previewSlot;
	void			*buffer;
	ITEM_DEFINITION *item = gmItemGetDirectStub(entry & INVENTORY_ITEM_ID_MASK);

	if(mode == 1)
		style = STATUS_PANEL_LOWER_ONLY;
	switch(item->type) {
	case ITEM_TYPE_NONE: /* usable items */
		mnStatPanelDraw(menu->statWin, target, slot, style);
		break;
	case ITEM_TYPE_ELEMENTAL_STAR:
		mnStatPanelDraw(menu->statWin, target, slot, style);
		break;
	case ITEM_TYPE_CLASS_CHANGE:
		if(owner == target) {
			style |= STATUS_PANEL_EQUIP_CLASS;
			mnStatPanelDraw(menu->statWin, target, slot, style);
		} else {
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buffer = syHeapAlloc(size);
			syWordsCopyEntry(buffer, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				entry &= ~INVENTORY_EQUIPPED;
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_EQUIP_CLASS;
					mnStatPanelDraw(menu->statWin, target, previewSlot, style);
				} else {
					mnStatPanelDraw(menu->statWin, target, slot, style);
				}
			} else {
				mnStatPanelDraw(menu->statWin, target, slot, style);
			}
			syWordsCopyEntry(unit, buffer, size);
			syHeapRelease(buffer);
		}
		break;
	case ITEM_TYPE_WEAPON:
	case ITEM_TYPE_ARMOR:
	case ITEM_TYPE_SHIELD:
	case ITEM_TYPE_HELM:
	case ITEM_TYPE_BOOTS:
	case ITEM_TYPE_TRIDENT:
	case ITEM_TYPE_RING:
	case ITEM_TYPE_SHIRT:
		if(owner == target) {
			style |= STATUS_PANEL_EQUIP;
			mnStatPanelDraw(menu->statWin, target, slot, style);
		} else {
			/* The class item's preview again, as the Psynergy item's below: one
			   function shared by the three does not keep the ROM's bytes. */
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buffer = syHeapAlloc(size);
			syWordsCopyEntry(buffer, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				entry &= ~INVENTORY_EQUIPPED;
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_EQUIP;
					mnStatPanelDraw(menu->statWin, target, previewSlot, style);
				} else {
					mnStatPanelDraw(menu->statWin, target, slot, style);
				}
			} else {
				mnStatPanelDraw(menu->statWin, target, slot, style);
			}
			syWordsCopyEntry(unit, buffer, size);
			syHeapRelease(buffer);
		}
		break;
	case ITEM_TYPE_PSYNERGY:
		if(target == owner) {
			style |= STATUS_PANEL_LEARN;
			mnStatPanelDraw(menu->statWin, target, slot, style);
		} else {
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buffer = syHeapAlloc(size);
			syWordsCopyEntry(buffer, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_LEARN;
					mnStatPanelDraw(menu->statWin, target, previewSlot, style);
				} else {
					mnStatPanelDraw(menu->statWin, target, slot, style);
				}
			} else {
				mnStatPanelDraw(menu->statWin, target, slot, style);
			}
			syWordsCopyEntry(unit, buffer, size);
			syHeapRelease(buffer);
		}
		break;
	}
}

/* Makes room in memberId's inventory: if no slot is free, removes the whole
   stack in the first slot that is not equipped. */
Sint32 mnInventoryRemoveFirstUnflagged(Sint32 memberId)
{
	UNIT  *unit = syUnitGet(memberId);
	Sint32 result = 0;
	Sint32 slotIndex;

	for(slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
		if(unit->inventory[slotIndex] == 0) {
			result = 1;
			break;
		}
		if((unit->inventory[slotIndex] & INVENTORY_EQUIPPED) == 0) {
			Sint32 quantity =
				unit->inventory[slotIndex] >> INVENTORY_QUANTITY_SHIFT;
			Sint32 count = quantity + 1;
			Sint32 removedCount;

			if(quantity == 0)
				count = 1;
			for(removedCount = 0; removedCount < count; removedCount++)
				result = gmInventoryRemoveStub(memberId, slotIndex);
			if(result != INVENTORY_REMOVE_LAST)
				return 0;
			result = 1;
			break;
		}
	}
	return result;
}

/* Where the cursor points at a command. The German, French and Spanish releases
   ask two helpers of their own: the first reads x from a table that sets the
   middle column 9 pixels left of the even 40-pixel spacing the other releases
   compute, the second gives y. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_SPANISH)
Sint32 mnItemMenuGetCommandCursorX(Sint32 column, Sint32 row);
Sint32 mnItemMenuGetCommandCursorY(Sint32 column, Sint32 row);
#define COMMAND_CURSOR_X(column, row) mnItemMenuGetCommandCursorX(column, row)
#define COMMAND_CURSOR_Y(column, row) mnItemMenuGetCommandCursorY(column, row)
#else
#define COMMAND_CURSOR_X(column, row) ((column) * 40 + 86)
#define COMMAND_CURSOR_Y(column, row) ((row) * 8 + 30)
#endif

void mnItemMenuBuildCommands(Sint8 *commandStates);

void mnItemMenuFitMessageWindowToItemChoices(void);

void mnItemMenuDrawItemHead(void);
void mnItemMenuDrawCommands(const Sint8 *commandStates, Sint32 window);

/* The item command grid: Use, Equip and Details on the top row, Give, Remove
   and Drop below (grid index = row * 3 + column, as ITEM_MENU_COMMAND counts
   them). */
Sint32 mnItemMenuChooseCommand(void)
{
	ITEM_MENU_STATE *menu;
	Sint8			 commandStates[6];
	Uint16			*redrawFlag;
	Sint32			 column;
	Sint32			 lastCommand;
	Sint32			 row;
	Sint32			 commandIndex;
	Bool			 redraw;

	menu = Ram_HeapSlots->menuState;
	commandIndex = 0;
	column = 0;
	row = 0;
	redraw = TRUE;
	mnItemMenuBuildCommands(commandStates);
	redrawFlag = &menu->flg;
	if(*redrawFlag != 1) {
		void *messageWindow;

		mnItemMenuHideAllIcons();
		mnWindowSprRedrawSavedRectStub(menu->listWin);
		messageWindow = menu->questionWin;
		mnItemMenuFitMessageWindowToItemChoices();
		mnWindowSprRedrawSavedRectStub(messageWindow);
		mnWindowDrawDividerLineStub(messageWindow, 0, 3, 16, 3);
		mnItemMenuDrawItemHead();
		mnItemMenuDrawCommands(commandStates, messageWindow);
	}
	*redrawFlag = 0;

	lastCommand = menu->listMode;
	if(lastCommand == -1) {
		if(commandStates[ITEM_COMMAND_DETAILS] ==
			ITEM_COMMAND_STATE_AVAILABLE) {
			column = 2;
			row = 0;
		}
		if(commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_AVAILABLE) {
			column = 0;
			row = 1;
		}
		if(commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_AVAILABLE) {
			column = 1;
			row = 0;
		}
		if(commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_AVAILABLE) {
			column = 1;
			row = 1;
		}
		if(commandStates[ITEM_COMMAND_USE] == ITEM_COMMAND_STATE_AVAILABLE) {
			column = 0;
			row = 0;
		}
	} else {
		column = (Sint8)(lastCommand % 3);
		row = (Sint8)(lastCommand / 3);
		commandIndex = row * 3 + column;
	}
	mnCursorSlide(COMMAND_CURSOR_X(column, row), COMMAND_CURSOR_Y(column, row));

	for(;;) {
		if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
			break;
		if(redraw != 0) {
			redraw = FALSE;
			column = (column + 3) % 3;
			row = (row + 2) % 2;
			commandIndex = row * 3 + column;
			mnEquipmentMenuStopCompatibilityIndicators();
			if(commandIndex > 2) {
				menu->equipPreview = 1;
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
				if(commandIndex == ITEM_COMMAND_GIVE)
					sySetEvent(mnEquipmentMenuUpdateCompatibilityIndicators,
						EVENT_PASS_FRAME);
			} else if(commandIndex != 0) {
				menu->equipPreview = 0;
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
			} else {
				mnStatPanelDraw(menu->statWin, menu->sideMember[0], 0,
					STATUS_PANEL_EXPERIENCE);
			}
		}
		mnCursorPos(
			COMMAND_CURSOR_X(column, row), COMMAND_CURSOR_Y(column, row));
		sySynch(1);

		if((PerWork.push & KEY_A) != 0) {
			if(commandStates[commandIndex] == ITEM_COMMAND_STATE_DISABLED) {
				sySndPlayStub(SE_MENU_ERROR);
			} else {
				switch(commandIndex) {
				case ITEM_COMMAND_USE:
					sySndPlayStub(SE_NOTE_MID);
					break;
				case ITEM_COMMAND_EQUIP:
					sySndPlayStub(SE_NOTE_HIGH);
					break;
				case ITEM_COMMAND_DETAILS:
				case ITEM_COMMAND_GIVE:
				case ITEM_COMMAND_DROP:
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				case ITEM_COMMAND_REMOVE:
					sySndPlayStub(SE_ITEM_REMOVE);
					break;
				default:
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				}
				menu->listMode = commandIndex;
				break;
			}
		}
		if((PerWork.push & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			commandIndex = -1;
			menu->listMode = commandIndex;
			break;
		}

		if((PerWork.repeat & KEY_UP) != 0) {
			row -= 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_DOWN) != 0) {
			row += 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_RIGHT) != 0) {
			column += 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_LEFT) != 0) {
			column -= 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}

	menu->equipPreview = 0;
	mnEquipmentMenuStopCompatibilityIndicators();
	return commandIndex;
}

#undef COMMAND_CURSOR_Y

Sint32 mnItemClassifyUseMode(Sint32 owner, Sint32 item);
/* Returns whether the scene has a script for item on what the leader faces. */
Sint32 fdHasItemCueStub(Sint32 item);

/* Fills commandStates (indexed by ITEM_MENU_COMMAND) with
   ITEM_COMMAND_STATE_AVAILABLE or _DISABLED for the selected item: Use for
   items mnItemClassifyUseMode accepts and are not broken, and for field items,
   Equip for equipment the party member can wear and is not wearing, Remove for
   worn equipment that is not cursed, Give with someone to give to, Drop unless
   the item is indispensable, and neither for cursed equipment being worn.
   Details is always available. Each later test overrides the earlier ones. */
void mnItemMenuBuildCommands(Sint8 *commandStates)
{
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;

	menu = Ram_HeapSlots->menuState;
	item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0]);

	if(item->type == ITEM_TYPE_NONE) {
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
	} else {
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_AVAILABLE;
	}

	if(mnItemClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]) !=
		ITEM_USE_MODE_UNUSABLE)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
	else
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

	if(menu->selectedItems[0] & INVENTORY_BROKEN)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

	if(gmItemCanUnitEquipDirectStub(menu->sideMember[0],
		   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) == 0) {
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
	}

	commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_AVAILABLE;
	commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_AVAILABLE;
	commandStates[ITEM_COMMAND_DETAILS] = ITEM_COMMAND_STATE_AVAILABLE;

	if(menu->selectedItems[0] & INVENTORY_EQUIPPED) {
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_AVAILABLE;
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
	} else {
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
	}

	if(item->flg & ITEM_CANNOT_UNEQUIP) {
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
		if(menu->selectedItems[0] & INVENTORY_EQUIPPED) {
			commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;
			commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
		}
	}

	if(fdHasItemCueStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

	if(menu->partyCnt <= 1)
		commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

	if(item->flg & ITEM_INDISPENSABLE)
		commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
}

/* The x of the command grid's middle column: German, Spanish and French draw
   Equip and Remove 8 pixels further left. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH)
#define ITEM_COMMAND_TEXT_X 32
#else
#define ITEM_COMMAND_TEXT_X 40
#endif

/* The grey of a command that refuses A. */
#define TEXT_COLOR_DISABLED TEXT_COLOR_GREY

/* Writes the six command names into window, the disabled ones in grey. */
void mnItemMenuDrawCommands(const Sint8 *commandStates, Sint32 window)
{
	Sint32 useState;
	Uint32 firstCommandMessage;

	mnTextSetColStub(TEXT_COLOR_NORMAL);
	useState = commandStates[ITEM_COMMAND_USE];
	if(useState == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);

	firstCommandMessage = MsgItemCommandUse;
	/* "Use" */
	mnTextDrawMessageAsTilesStub(firstCommandMessage, window, 0, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Equip" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMessage + 1, window, ITEM_COMMAND_TEXT_X, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Give" */
	mnTextDrawMessageAsTilesStub(firstCommandMessage + 2, window, 0, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DROP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Drop" */
	mnTextDrawMessageAsTilesStub(firstCommandMessage + 3, window, 80, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DETAILS] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Details" */
	mnTextDrawMessageAsTilesStub(firstCommandMessage + 4, window, 80, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Remove" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMessage + 5, window, ITEM_COMMAND_TEXT_X, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
}

/* Says how memberId can use inventoryEntry from the item menu. */
Sint32 mnItemClassifyUseMode(Sint32 memberId, Sint32 inventoryEntry)
{
	Sint32			 itemId = inventoryEntry;
	ITEM_DEFINITION *item;
	Sint32			 result = ITEM_USE_MODE_UNUSABLE;

	itemId &= INVENTORY_ITEM_ID_MASK;
	item = gmItemGetDirectStub(itemId);

	if(fdHasItemCueStub(itemId) != 0) {
		return ITEM_USE_MODE_FROM_FIELD;
	}

	{
		ABILITY_DEFINITION *action =
			(ABILITY_DEFINITION *)gmAbilityGetDirectStub(
				item->abilityId & PSYNERGY_ABILITY_MASK);

		if(item->abilityId != 0) {
			if(item->type != ITEM_TYPE_NONE) {
				if(item->useType != ITEM_USE_TEACHES) {
					if(gmItemCanUnitEquipDirectStub(memberId, itemId) != 0) {
						result = ITEM_USE_MODE_CHOOSE_MEMBER;
					}
				}
			} else {
				result = ITEM_USE_MODE_CHOOSE_MEMBER;
			}

			if(result == ITEM_USE_MODE_CHOOSE_MEMBER) {
				Sint32 mask = ABILITY_USABLE_IN_BATTLE;
				Uint8  flags = action->flg;

				if((flags & ABILITY_USABLE_IN_FIELD) != 0) {
					Uint8 range = action->range;
					result = (range == ABILITY_RANGE_ALL)
						? ITEM_USE_MODE_WHOLE_PARTY
						: ITEM_USE_MODE_CHOOSE_MEMBER;
				} else {
					result = (flags & mask) ? ITEM_USE_MODE_UNUSABLE
											: ITEM_USE_MODE_FROM_FIELD;
				}
			}
		}
	}

	return result;
}

void gmInventoryBreakStub(Sint32 owner, Sint32 slot);

/* After the selected item is used: an item that may break (ITEM_USE_MAY_BREAK)
   breaks one time in eight, with the break sound and "It broke!" in the info
   window. Called only after a successful use on a chosen party member. */
void mnItemMenuTryBreak(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Uint32			 item = menu->selectedItems[0];
	Uint32			 mask;

	mask = INVENTORY_ITEM_ID_MASK;
	mask &= item;
	/* One time in eight: 8192 of syRandom's 65536 values. */
	if(gmItemGetDirectStub(mask)->useType == ITEM_USE_MAY_BREAK &&
		syRandom() < 8192) {
		gmInventoryBreakStub(menu->sideMember[0], menu->selectedSlots[0]);
		sySndPlayStub(SE_ITEM_BREAK);
		mnItemMenuShowMessage((Sint32)MsgItBroke, 0, -1);
		mnWindowSprRedrawSavedRectStub(menu->infoWin);
	}
}

#undef ITEM_COMMAND_TEXT_X

/* Opens the details window (top left, 13 by 10 tiles) and describes the item in
   side itemIndex there. Returns 1. */
Sint32 mnItemMenuOpenDetail(Sint32 itemIndex)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	mnWindowUpdateOrCreate(&menu->equipWin, 0, 0, 13, 10, WINDOW_FLAG_INSTANT);
	mnPaletteLoadIconsIntoHighlight();

	if(menu->selectedItems[itemIndex] != 0)
		mnItemMenuDrawItemDetails(
			menu->equipWin, menu->selectedItems[itemIndex]);

	return 1;
}

/* The Details window's width in tiles: Japanese opens it over the left part
   only, beside the item window, the other editions over the whole top half. */
#define ITEM_DETAILS_WINDOW_WIDTH JAPANESE_OR(13, 30)

void mnWindowCloseStub(Sint32, Sint32);

/* The item menu's Details screen: describes the item entry value in a window
   over the top half (ITEM_DETAILS_WINDOW_WIDTH) and waits for A or B. Up and
   Down move an unused counter from 0 to 4. Returns the counter after A or
   Start, or -1 after B. */
Sint32 mnItemMenuShowDetails(Sint32 value)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Bool			 changed = TRUE;
#if LANGUAGE != LANGUAGE_JAPANESE
	WINDOW_SPR *itemIcon = menu->selectedItemIcon;
#endif
	Sint32 window;
	Sint32 counter = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
	itemIcon->motion = ICON_HIDDEN;
#endif
	window = mnWindowCreateStub(
		0, 0, ITEM_DETAILS_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
	syCloseEvent(mnUpdatePartyFigures);

	{
		WINDOW_SPR *cursor = menu->cursor;
		cursor->motion = ICON_HIDDEN;
	}
	mnPaletteLoadIconsIntoHighlight();
	sySynch(1);

	for(;;) {
		if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
			break;

		if(changed != 0) {
			changed = FALSE;
			counter = (counter + 5) % 5;
			mnItemMenuDrawItemDetails(window, value);
		}

		if(PerWork.push & KEY_A)
			break;
		if(PerWork.push & KEY_B) {
			counter = -1;
			break;
		}

		if(PerWork.repeat & KEY_UP) {
			counter -= 1;
			changed = TRUE;
		}
		if(PerWork.repeat & KEY_DOWN) {
			counter += 1;
			changed = TRUE;
		}
		sySynch(1);
	}

	mnWindowSprRedrawSavedRectStub(window);
	sySynch(1);
	mnWindowCloseStub(window, 1);
	mnWindowSprRedrawSavedRectStub(menu->mainWin);
	mnWindowDrawDividerLineStub(menu->mainWin, 0, 3, 12, 3);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	{
		Sint32 priority = EVENT_PASS_FRAME;

		sySetEvent(mnUpdatePartyFigures, priority);
	}

	{
		WINDOW_SPR *cursor = menu->cursor;
		cursor->motion = ICON_SHOWN;
	}
#if LANGUAGE == LANGUAGE_JAPANESE
	menu->flg = 1;
#else
	mnWindowDrawBorderStub(13, 0, 17, 10);
#endif

	return counter;
}

#undef ITEM_DETAILS_WINDOW_WIDTH

/* Describes item in window with the entry's own count, through
   mnItemMenuDrawItemDetailsWithCount. */
void mnItemMenuDrawItemDetails(Sint32 window, Sint32 item)
{
	mnItemMenuDrawItemDetailsWithCount(window, item, -1);
}

/* The headings are indented in English and Japanese; German, Spanish and
   Italian start all three at the window's left edge, French the uses heading
   only. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_DETAILS_HEADING_X 0
#define ITEM_DETAILS_USES_X	   0
#define ITEM_DETAILS_NUMBER_X  0
#elif LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X	   0
#define ITEM_DETAILS_NUMBER_X  16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_HEADING_X 16
#define ITEM_DETAILS_USES_X	   16
#define ITEM_DETAILS_NUMBER_X  16
#else
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X	   16
#define ITEM_DETAILS_NUMBER_X  16
#endif

/* Japanese draws the values nearer the names, and a multiplier further left to
   leave room for its suffix. */
#define ITEM_DETAILS_VALUE_X	  JAPANESE_OR(64, 72)
#define ITEM_DETAILS_MULTIPLIER_X JAPANESE_OR(48, 72)

/* `.`, the decimal point of a multiplier. */
extern Uint8 Menu_PointString[];

void mnItemMenuDrawStat(
	Sint32 delta, Sint32 unused, Sint32 window, Sint32 x, Sint32 y);

/* Describes an item (an inventory entry) in window, one 8-pixel row a line: its
   equip bonuses and effects, a curse, the Psynergy it bestows, its uses and its
   count, or failing all of these whether it is rare or important. count is the
   count to show, which only Japanese reads; mnItemMenuDrawItemDetails passes
   -1, the entry's own. */
void mnItemMenuDrawItemDetailsWithCount(
	Sint32 window, Sint32 item, Sint32 count)
{
	ITEM_DEFINITION *def;
	Sint32			 i;
	Sint32			 found;
	Sint32			 amount;
	Sint32			 consumable;
	Sint8			 row;

	row = 0;
	consumable = 0;
	def = gmItemGetDirectStub(item & INVENTORY_ITEM_ID_MASK);
	if(def->type != ITEM_TYPE_NONE) {
		found = 0;
		if(def->attack == 0 && def->defense == 0) {
			for(i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
				if(def->effs[i].kind != 0 || def->useType == ITEM_USE_TEACHES) {
					found = 1;
					break;
				}
			}
		} else {
			found = 1;
		}
		if(found == 1) {
			mnTextDrawMessageAsTilesStub((Sint32)MsgEquipEffectHeading, window,
				ITEM_DETAILS_HEADING_X, 0);
			row = 1;
		}
		if(def->attack != 0) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgStatLabel, window, 0, row * 8);
			amount = def->attack;
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			row++;
		}
		if(def->defense != 0) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgDefenseLabel, window, 0, row * 8);
			amount = def->defense;
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			row++;
		}
	}
	for(i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
		if(def->effs[i].kind == EQUIP_EFFECT_NONE)
			continue;
		amount = def->effs[i].amount;
		switch(def->effs[i].kind) {
		case EQUIP_EFFECT_NONE:
			break;
		case EQUIP_EFFECT_ADD_HP:
		case EQUIP_EFFECT_ADD_HP_REGEN:
		case EQUIP_EFFECT_ADD_PP:
		case EQUIP_EFFECT_ADD_PP_REGEN:
		case EQUIP_EFFECT_ADD_AGILITY:
		case EQUIP_EFFECT_ADD_LUCK:
		case EQUIP_EFFECT_ADD_ACTIONS:
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + (Sint32)MsgEquipEffectName, window, 0,
				row * 8);
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			break;
		case EQUIP_EFFECT_ADD_VENUS_POWER:
		case EQUIP_EFFECT_ADD_MERCURY_POWER:
		case EQUIP_EFFECT_ADD_MARS_POWER:
		case EQUIP_EFFECT_ADD_JUPITER_POWER:
		case EQUIP_EFFECT_ADD_VENUS_RESIST:
		case EQUIP_EFFECT_ADD_MERCURY_RESIST:
		case EQUIP_EFFECT_ADD_MARS_RESIST:
		case EQUIP_EFFECT_ADD_JUPITER_RESIST:
			mnWindowSetTilemapEntryStub(window,
				(Uint8)((def->effs[i].kind - EQUIP_EFFECT_ADD_VENUS_POWER) %
					ELEMENT_COUNT) +
					1,
				0, row, TILEMAP_ENTRY_HIGHLIGHT);
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + (Sint32)MsgEquipEffectName, window, 8,
				row * 8);
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			break;
		case EQUIP_EFFECT_SCALE_HP:
		case EQUIP_EFFECT_SCALE_HP_REGEN:
		case EQUIP_EFFECT_SCALE_PP:
		case EQUIP_EFFECT_SCALE_PP_REGEN:
		case EQUIP_EFFECT_SCALE_ATTACK:
		case EQUIP_EFFECT_SCALE_DEFENSE:
		case EQUIP_EFFECT_SCALE_AGILITY:
		case EQUIP_EFFECT_SCALE_LUCK:
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + (Sint32)MsgEquipEffectName, window, 0,
				row * 8);
			/* The scale is in tenths: "1.2" for 12. */
			if(amount >= STAT_SCALE_ONE) {
				mnTextDrawNumInWindowStub(
					1, 1, window, ITEM_DETAILS_MULTIPLIER_X, row * 8);
				mnTextDrawStringInWindowStub(Menu_PointString, window,
					ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
				mnTextDrawNumInWindowStub(amount - STAT_SCALE_ONE, 1, window,
					ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
			} else {
				mnTextDrawNumInWindowStub(
					0, 1, window, ITEM_DETAILS_MULTIPLIER_X, row * 8);
				mnTextDrawStringInWindowStub(Menu_PointString, window,
					ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
				mnTextDrawNumInWindowStub(
					amount, 1, window, ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
			}
#if LANGUAGE == LANGUAGE_JAPANESE
			mnTextDrawMessageAsTilesStub((Sint32)MsgTimesSuffix, window,
				ITEM_DETAILS_MULTIPLIER_X + 24, row * 8);
#endif
			break;
		case EQUIP_EFFECT_ADD_UNLEASH_CHANCE:
		case EQUIP_EFFECT_CURSE_REMOVABLE:
		case EQUIP_EFFECT_LURE:
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + (Sint32)MsgEquipEffectName, window, 0,
				row * 8);
#if LANGUAGE == LANGUAGE_GERMAN
			/* German continues these three names on a second line, the messages
			   after the last effect's name in the order of their effects:
			   "steigen" and "aufgehoben werden.". */
			row++;
			if(def->effs[i].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 1, window,
					0, row * 8);
			else if(def->effs[i].kind == EQUIP_EFFECT_CURSE_REMOVABLE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 2, window,
					0, row * 8);
#if !FIX_LURE_CAP_STRAY_SECOND_LINE
			/* BUG: German has only those two second lines, so the Lure Cap's
			   "Erhöht gegn." goes on with the message after them, "Items". */
			else if(def->effs[i].kind == EQUIP_EFFECT_LURE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 3, window,
					0, row * 8);
#endif
#endif
			break;
		}
		row++;
	}
	if(def->flg & ITEM_CURSED) {
		mnTextDrawMessageAsTilesStub((Sint32)MsgItemCursed, window, 0, row * 8);
		row++;
	}
	if(def->useType == ITEM_USE_TEACHES) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgBestowsPsynergy, window, 0, row * 8);
		consumable = 1;
		row++;
	}
	if(def->useType != ITEM_USE_EMPTIES && def->useType != ITEM_USE_FREE) {
		if(!consumable) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUsesHeading, window, ITEM_DETAILS_USES_X, row * 8);
			row++;
		}
		switch(def->useType) {
		case ITEM_USE_FREE:
			break;
		case ITEM_USE_CONSUMED:
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgSingleUse, window, 0, row * 8);
			row++;
			break;
		case ITEM_USE_MAY_BREAK: {
			Sint32 m;

			if(item & INVENTORY_BROKEN) {
				m = (Sint32)MsgBrokenNotice;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#if LANGUAGE == LANGUAGE_GERMAN
				/* German ends the sentence on a third line, six messages on. */
				mnTextDrawMessageAsTilesStub(m + 1, window, 0, row * 8);
				row++;
				m += 6;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#else
				m++;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#endif
			} else {
				m = (Sint32)MsgMightBreak;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#if LANGUAGE == LANGUAGE_GERMAN
				mnTextDrawMessageAsTilesStub(m + 1, window, 0, row * 8);
				row++;
				m += 7;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#else
				m++;
				mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
				row++;
#endif
			}
			break;
		}
		}
	}
	if(def->flg & ITEM_STACKABLE) {
		if(row != 0)
			row++;
		i = (Sint32)MsgNumberHeading;
		mnTextDrawMessageAsTilesStub(i, window, ITEM_DETAILS_NUMBER_X, row * 8);
		row++;
#if LANGUAGE == LANGUAGE_JAPANESE
		/* Japanese draws the count itself: the entry's own for -1, "none" for
		   0. */
		if(count == -1) {
			amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
			mnTextDrawNumInWindowStub(amount + 1, 2, window, 0, row * 8);
			mnTextDrawMessageAsTilesStub(i + 1, window, 16, row * 8);
			row++;
		} else if(count == 0) {
			mnTextDrawMessageAsTilesStub(i + 8, window, 0, row * 8);
			row++;
		} else {
			amount = count;
			mnTextDrawNumInWindowStub(amount, 3, window, 0, row * 8);
			mnTextDrawMessageAsTilesStub(i + 1, window, 24, row * 8);
			row++;
		}
#else
		amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
		mnMessageArgumentPushStub(amount + 1, MESSAGE_ARGUMENT_NUMBER);
		mnTextDrawMessageAsTilesStub(i + 1, window, 0, row * 8);
		row++;
#endif
	}
	found = 0;
	if(row == 0) {
		if(def->flg & ITEM_RARE) {
			mnTextDrawMessageAsTilesStub((Sint32)MsgRareItem, window, 0, 0);
			found = 1;
		}
		if(!found) {
			if(def->flg & ITEM_INDISPENSABLE) {
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgImportantItem, window, 0, 0);
				found = 1;
			}
			if(!found)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDetailsUnknown, window, 0, 0);
		}
	}
}

#undef ITEM_DETAILS_MULTIPLIER_X

#define mnTextDrawStringInWindowStub \
	((void (*)(Uint8 *, Sint32, Sint32, Sint32))mnTextDrawStringInWindowStub)

/* Writes a signed stat change at (x, y) of window: the number, with a plus or
   minus sign just before its first digit. */
void mnItemMenuDrawStat(
	Sint32 statChange, Sint32 unused, Sint32 window, Sint32 x, Sint32 y)
{
	Sint32 digits;
	Sint32 magnitude;

	mnTextDrawNumInWindowStub(statChange, 3, window, x, y);
	/* Count the digits of |statChange| to place the sign. */
	digits = 1;
	magnitude = statChange;
	if(statChange < 0) {
		magnitude = -statChange;
	}
	if(magnitude > 9) {
		digits = 2;
	}
	magnitude = statChange;
	if(statChange < 0) {
		magnitude = -statChange;
	}
	if(magnitude > 99) {
		digits = 3;
	}
	if(statChange > 0) {
		mnTextDrawStringInWindowStub(
			&Menu_PlusSignString, window, x - digits * 8 + 16, y);
	} else {
		mnTextDrawStringInWindowStub(
			&Menu_MinusSignString, window, x - digits * 8 + 16, y);
	}
}

#define ITEM_MENU ((ITEM_MENU_STATE *)Ram_HeapSlots->menuState)

/* Moves the item window to row 5 of the right-hand side. */
void mnItemMenuPlaceItemWindowBelowItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->itemWin, 13, 5, 17, 10);
}

/* Moves the item window to row 3 of the right-hand side, below a one-line
   question. */
void mnItemMenuPlaceItemWindowBelowQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->itemWin, 13, 3, 17, 10);
}

/* Makes the message window, along the top right, 5 rows tall: the item's name
   and a question below it. */
void mnItemMenuFitMessageWindowToItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWin, 13, 0, 17, 5);
}

/* Makes the item menu's message window six tiles high. */
void mnItemMenuFitMessageWindowToLongItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWin, 13, 0, 17, 6);
}

/* Makes the item menu's message window seven tiles high. */
void mnItemMenuFitMessageWindowToItemChoices(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWin, 13, 0, 17, 7);
}

/* Makes the item menu's message window three tiles high. */
void mnItemMenuFitMessageWindowToQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWin, 13, 0, 17, 3);
}

#undef mnTextDrawStringInWindowStub
#undef ITEM_MENU

/* The Japanese window asks いくつ? ("How many?") on the count's row instead
   of the top row, draws the counter こ after each count held, and keeps its
   cursor further left. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* いくつ?, the message three before MsgShortcutHelp. */
#define HOW_MANY_MESSAGE  ((Sint32)MsgShortcutHelp - 3)
#define HOW_MANY_X		  0
#define HOW_MANY_Y		  32
/* こ, the counter after a count: the message before MsgItemCommandUse. */
#define PIECE_MESSAGE	  ((Sint32)MsgItemCommandUse - 1)
#define HOW_MANY_CURSOR_X 88
#else
#define HOW_MANY_MESSAGE  ((Sint32)MsgHowMany)
#define HOW_MANY_X		  32
#define HOW_MANY_Y		  0
#define HOW_MANY_CURSOR_X 128
#endif

/* The item menu's quantity bar picture, which mnQuantityBarFillStub fills in
   under the "How many?" question. */
extern const Uint8 Menu_QuantityTiles[];

/* Colours the first count segments of the quantity bar in tiles, each two
   pixels wide: the left pixel in colour 1, the right in colour color. */
void mnQuantityBarFillStub(Sint32 count, Sint32 color, void *tiles);

/* Asks how many of the selected item stack to give or drop: Left and Right
   choose a number from 1 to quantityRange (shown as baseQuantity + 1 up to
   baseQuantity + quantityRange on a dial of digit sprites), with the party
   member's remaining count and, when giving (dropMode 0), the receiver's new
   count beside their names. */
Sint32 mnItemMenuSelectGiveQuantity(
	Sint32 baseQuantity, Sint32 quantityRange, Sint32 dropMode)
{
	Sint32			 value = baseQuantity;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Uint8			*tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	Sint32			 entry;
	Sint32			 changed = 1;
	Sint32			 targetCount = 0;
	void			*window = menu->questionWin;
	Sint32			 ownerCount;
	WINDOW_SPR		*arrow;

	mnItemMenuFitMessageWindowToItemChoices();
	mnWindowSprRedrawSavedRectStub(window);
	if(dropMode == 0)
		targetCount = mnItemMenuGetItemQuantity(menu->sideMember[1],
			menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	ownerCount = mnItemMenuGetItemQuantity(
		menu->sideMember[0], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	entry = syVRAMCacheFindFreeEntry();
	if(entry != VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(entry, 256, 0);
		mnWindowSprCreateStub(entry, WINDOW_SPRITE_SIZE_32X8, window, 48, 32);
		arrow = mnWindowSprCreateStub(
			entry, WINDOW_SPRITE_SIZE_32X8, window, 80, 32);
		arrow->spr.oam.idx = arrow->spr.oam.idx + 4;
		mnCursorSlide(HOW_MANY_CURSOR_X, 40);
		while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
			if(changed) {
				changed = 0;
				value = (value + quantityRange) % quantityRange;
				mnWindowSprRedrawSavedRectStub(window);
				mnTextDrawMessageAsTilesStub(
					HOW_MANY_MESSAGE, window, HOW_MANY_X, HOW_MANY_Y);
				syDMAXCopy(
					Menu_QuantityTiles, tiles, DMA_COPY_WORDS(64), REG_DMA3);
				mnQuantityBarFillStub(
					INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH, tiles);
				mnQuantityBarFillStub(
					baseQuantity + quantityRange, QUANTITY_BAR_OPEN, tiles);
				mnQuantityBarFillStub(
					baseQuantity + value + 1, QUANTITY_BAR_CHOSEN, tiles);
				mnQuantityBarFillStub(baseQuantity, QUANTITY_BAR_HELD, tiles);
				syVRAMCacheLoad(entry, 256, tiles);
				mnTextDrawNumInWindowStub(value + 1, 2, window, 32, 32);
				mnTextDrawMessageAsTilesStub(
					(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
						(Sint32)MsgItemName,
					window, 16, 8);
				mnTextDrawNumInWindowStub(
					ownerCount - value - 1, 2, window, 16, 24);
#if LANGUAGE == LANGUAGE_JAPANESE
				mnTextDrawMessageAsTilesStub(PIECE_MESSAGE, window, 32, 24);
#endif
				if(dropMode == 0) {
					mnTextDrawNumInWindowStub(
						targetCount + value + 1, 2, window, 80, 24);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnTextDrawMessageAsTilesStub(PIECE_MESSAGE, window, 96, 24);
#endif
				}
				mnTextDrawStringAtOffsetStub(
					syUnitGet(menu->sideMember[0]), window, 16, 16);
				if(dropMode == 0)
					mnTextDrawStringAtOffsetStub(
						syUnitGet(menu->sideMember[1]), window, 80, 16);
			}
			if(PerWork.push & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if(PerWork.push & KEY_B) {
				value = -1;
				sySndPlayStub(SE_MENU_CANCEL);
				break;
			}
			mnCursorPos(HOW_MANY_CURSOR_X, 40);
			if(PerWork.repeat & KEY_LEFT) {
				value--;
				changed = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if(PerWork.repeat & KEY_RIGHT) {
				value++;
				changed = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	mnWindowSprRedrawSavedRectStub(window);
	mnWindowSprClearListStub(window);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	menu->selectedItemIcon->motion = ICON_HIDDEN;
	if(syGameFlagGet(FLAG_MENU_START_PRESSED))
		value = -1;
	return value;
}

#undef HOW_MANY_Y
#undef HOW_MANY_CURSOR_X

/* Shows the selected item at the top of the message window: its icon, its party
   member's name and its name. */
void mnItemMenuDrawItemHead(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM, menu->selectedItems[0],
		(Uint8)menu->selectedItemIcon->idx, 0);
	menu->selectedItemIcon->motion = ICON_SHOWN;
	menu->selectedItemIcon->x = 112;
	menu->selectedItemIcon->y = 8;
	mnIconShow(menu->selectedItemIcon);
	mnTextDrawStringAtOffsetStub(
		syUnitGet(menu->sideMember[0]), menu->questionWin, 16, 0);
	mnTextDrawMessageAsTilesStub(
		(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + (Sint32)MsgItemName,
		menu->questionWin, 16, 8);
}

/* The French question starts at the window's left edge. */
#define ITEM_DROP_QUESTION_X FRENCH_OR(0, 8)

/* Asks whether to drop item (an inventory entry): its name, the question, and
   Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start. */
Sint32 mnItemMenuConfirmDrop(Sint32 item)
{
	Sint32 window;
	Sint32 itemId;
	Sint32 question;
	Sint32 answer;
	Sint32 rows = 2;
	Sint32 selection = 1;
	Bool   changed = TRUE;

	window = mnWindowCreateStub(13, 3, 17, 10, WINDOW_FLAG_INSTANT);
	itemId = item & INVENTORY_ITEM_ID_MASK;
	gmItemGetDirectStub(itemId);
	mnTextDrawMessageAsTilesStub(itemId + (Sint32)MsgItemName, window, 24, 0);
	question = (Sint32)MsgConfirmDrop;
	mnTextDrawMessageAsTilesStub(question, window, ITEM_DROP_QUESTION_X, 16);
	question++;
	mnTextDrawMessageAsTilesStub(question, window, ITEM_DROP_QUESTION_X, 24);
	answer = (Sint32)MsgYes;
	mnTextDrawMessageAsTilesStub(answer, window, 24, 40);
	answer++;
	mnTextDrawMessageAsTilesStub(answer, window, 24, 56);
	mnCursorSlide(104, 86);
	for(;;) {
		if(syGameFlagGet(FLAG_MENU_START_PRESSED))
			break;
		if(changed) {
			changed = FALSE;
			selection = (selection + rows) % rows;
		}
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			selection = 1;
			break;
		}
		mnCursorPos(104, (selection << 4) + 70);
		if(PerWork.repeat & KEY_UP) {
			selection--;
			changed = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_DOWN) {
			selection++;
			changed = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		sySynch(1);
	}
	if(syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		selection = 1;
	}
	mnWindowCloseStub(window, 1);
	return selection;
}

#undef ITEM_DROP_QUESTION_X

/* The question and its answers, as each translation lays them out. English and
   Japanese stack Yes over No to the right of the question, and Up and Down move
   the cursor; German, Spanish, French and Italian set them side by side below
   it, and Left and Right move the cursor. Japanese indents the question. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define EQUIP_YES_X			   24
#define EQUIP_YES_Y			   24
#define EQUIP_NO_X			   72
#define EQUIP_NO_Y			   24
#define EQUIP_CURSOR_X(answer) ((answer) * 48 + 110)
#define EQUIP_CURSOR_Y(answer) 32
#define EQUIP_PREVIOUS_KEY	   KEY_LEFT
#define EQUIP_NEXT_KEY		   KEY_RIGHT
#else
#define EQUIP_YES_X			   96
#define EQUIP_YES_Y			   0
#define EQUIP_NO_X			   96
#define EQUIP_NO_Y			   16
#define EQUIP_CURSOR_X(answer) 184
#define EQUIP_CURSOR_Y(answer) (((answer) << 4) + 5)
#define EQUIP_PREVIOUS_KEY	   KEY_UP
#define EQUIP_NEXT_KEY		   KEY_DOWN
#endif
#define EQUIP_QUESTION_X JAPANESE_OR(8, 0)

/* The question is named in the English catalog alone; in every edition it is
   the second message after MsgConfirmDrop. */
#define EQUIP_QUESTION \
	ENGLISH_OR(((Sint32)MsgEquipThisItem), ((Sint32)MsgConfirmDrop + 2))

/* After giving or trading the selected item to the party member in side 1,
   offers to equip it there, showing the stats it would give. */
Sint32 mnItemMenuConfirmEquip(Sint32 unused)
{
	Sint32			 rows = 2;
	Sint32			 selection = 0;
	Bool			 changed = TRUE;
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	void			*unit = syUnitGet(menu->sideMember[1]);
	void			*saved;
	Sint32			 window;
	Sint32			 slot;
	Sint32			 member;
	Sint32			 result;

	slot = menu->selectedSlots[1];
	member = menu->sideMember[1];
	mnItemMenuDrawEquipPreview(member, slot, 0, member);
	saved = syHeapAlloc(BATTLE_UNIT_SIZE);
	syWordsCopy(saved, unit, BATTLE_UNIT_SIZE);
	window = menu->questionWin;
	/* Nothing to ask when it cannot be worn, or what it would replace is cursed
	   on. */
	result = gmInventoryEquipStub(menu->sideMember[1], menu->selectedSlots[1]);
	if(result == EQUIP_REFUSED_CANNOT_WEAR || result == EQUIP_REFUSED_CURSED) {
		selection = 1;
	} else {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgYes, window, EQUIP_YES_X, EQUIP_YES_Y);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgYes + 1, window, EQUIP_NO_X, EQUIP_NO_Y);
		mnWindowClearInteriorTilesStub(window, 16, 16, 96, 24);
		mnTextDrawMessageAsTilesStub(
			EQUIP_QUESTION, window, EQUIP_QUESTION_X, 16);
		mnCursorSlide(EQUIP_CURSOR_X(0), EQUIP_CURSOR_Y(0));
		for(;;) {
			if(syGameFlagGet(FLAG_MENU_START_PRESSED))
				break;
			if(changed) {
				changed = FALSE;
				selection = (selection + rows) % rows;
			}
			if(PerWork.push & KEY_A) {
				sySndPlayStub(SE_NOTE_HIGH);
				break;
			}
			if(PerWork.push & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				selection = 1;
				break;
			}
			mnCursorPos(EQUIP_CURSOR_X(selection), EQUIP_CURSOR_Y(selection));
			if(PerWork.repeat & EQUIP_PREVIOUS_KEY) {
				selection--;
				changed = TRUE;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if(PerWork.repeat & EQUIP_NEXT_KEY) {
				selection++;
				changed = TRUE;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	if(syGameFlagGet(FLAG_MENU_START_PRESSED))
		selection = 1;
	if(selection == 1)
		syWordsCopy(unit, saved, BATTLE_UNIT_SIZE);
	syHeapRelease(saved);
	gmUnitRefreshPsynergyStub(menu->sideMember[1]);
	gmUnitCalcStub(menu->sideMember[1]);
	return selection;
}

#undef EQUIP_QUESTION

/* Loads the small up and down arrows of the stat changes
   (mnIconCreateStatChangeArrow) into free VRAM cache slots. */
void mnLoadStatArrows(void)
{
	ITEM_MENU_STATE *state = Ram_HeapSlots->menuState;
	Sint32			 value = syVRAMCacheFindFreeEntry();

	state->resourceSlots[0] = value;
	mnVRAMBlockLoadResourceStub(
		value, ICON_TILE_BYTES, RESOURCE_MENU_ARROW_TILES);
	value = syVRAMCacheFindFreeEntry();
	state->resourceSlots[1] = value;
	mnVRAMBlockLoadResourceStub(
		value, ICON_TILE_BYTES, RESOURCE_MENU_SMALL_DOWN_ARROW_TILES);
}

/* Sets list up over the items of the party member in side pane, five a page,
   its cursor on the item chosen last there (or the last item). */
Sint32 mnItemMenuBuildPagedList(PAGED_LIST *list, Sint32 pane)
{
	Sint32			 unit;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 count;
	Sint32			 row;
	Sint32			 page;
	Sint32			 pages;
	Sint32			 selected;

	count = mnItemMenuCountItems(menu->sideMember[pane]);
	unit = syUnitGet(menu->sideMember[pane]);
	selected = menu->selectedIdxByMember[menu->sideMember[pane]];
	if((selected + 1) > count) {
		selected = count - 1;
	}
	page = selected / LIST_PAGE_SIZE;
	row = selected % LIST_PAGE_SIZE;
	pages = count / LIST_PAGE_SIZE;
	if(count % LIST_PAGE_SIZE != 0) {
		pages++;
	}
	list->memberState = unit;
	list->page = page;
	list->pageCnt = pages;
	list->row = row;
	list->entryCnt = count;
	list->selectedIdx = selected;
	return 1;
}

/* Names the item under the list's cursor in the info window and highlights its
   row alone. */
Sint32 mnItemMenuShowSelectedRow(
	Sint32 listWindow, void *unused, PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu;
	Sint32			 page;
	Sint32			 combined;
	Sint32			 row;
	Sint32			 window;

	page = state->page;
	menu = Ram_HeapSlots->menuState;
	window = menu->infoWin;
	combined = page * LIST_PAGE_SIZE;
	combined += state->row;
	state->selectedIdx = combined;

	/* The Japanese release clears the info window; in every edition but the
	   Japanese it redraws the window's saved rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowSprRedrawSavedRectStub(window);
#else
	mnWindowSprClearListStub(window);
#endif
	sySynch(1);

	combined = state->selectedIdx;
	if(menu->items[combined] != 0) {
		Sint32 masked = (menu->items[combined] & INVENTORY_ITEM_ID_MASK) +
			(Sint32)MsgItemDescription;

		/* Japanese draws the name as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnTextDrawMessageStub(masked, menu->infoWin, 0, 0);
#else
		mnTextDrawMessageAsTilesStub(masked, menu->infoWin, 0, 0);
#endif
	}

	for(row = 0; row <= LIST_PAGE_SIZE - 1; row++) {
		if(row == state->row) {
			mnWindowSetHighlightRect(
				menu->itemWin, 1, row * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
		} else {
			mnWindowSetHighlightRect(
				menu->itemWin, 1, row * 2 + 1, 14, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

/* The Japanese page draws its page icons and names further right. */
#define ITEM_PAGE_ICONS_X JAPANESE_OR(120, 116)
#define ENTRY_X			  JAPANESE_OR(32, 24)

/* Draws one page of the item list into window: a divider, the page's icons and
   page numbers, and up to five item names. */
Sint32 mnItemMenuDrawListPage(
	Sint32 window, Sint32 unused, const PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Uint32			 page;
	Uint32			 firstEntry;
	Uint32			 visibleCount;
	Uint8			 row;
	const Uint16	*itemId;

	mnWindowSprRedrawSavedRectStub(window);
	mnWindowDrawDividerLineStub(window, 0, 11, 16, 11);

	page = state->page;
	firstEntry = page * LIST_PAGE_SIZE;
	visibleCount = (Uint8)(state->entryCnt - firstEntry);
	if(visibleCount > LIST_PAGE_SIZE) {
		visibleCount = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(window, state->entryCnt, LIST_PAGE_SIZE,
		state->page, PAGE_NUMBERS_RIGHT);

	row = 0;
	if(visibleCount > row) {
		itemId = &menu->items[firstEntry];
		do {
			mnTextDrawMessageAsTilesStub(
				(itemId[0] & INVENTORY_ITEM_ID_MASK) + (Sint32)MsgItemName,
				menu->itemWin, ENTRY_X, row * 16 + 8);
			row++;
			itemId++;
		} while(visibleCount > row);
	}

	return 1;
}

#undef ITEM_PAGE_ICONS_X
#undef ENTRY_X

/* The item list of the party member in side pane (0: the holder, 1: the
   receiver of a trade), five items a page. The cursor's item is named in the
   info window and previewed in the status panel, its icon animates, and the
   party members who can equip it move. Up and Down move the cursor, Left and
   Right turn pages, and in side 0 L and R switch to the previous or next party
   member who has items. Returns the chosen inventory entry, or -1 when the
   player backs out. */
Sint32 mnItemMenuRunList(Sint32 pane)
{
	ITEM_MENU_STATE *menu;
	void			*window;
	Bool			 changed;
	Sint32			 moved;
	Sint32			 member;
	Sint32			 count;
	Uint8			 i;
	Sint32			 result;
	Bool			 redraw;
	Bool			 done;
	UNIT			*owner;
	Sint32			 previous;
	WINDOW_SPR		*icon;
	Sint32			 unused[5];
	PAGED_LIST		 state;

	menu = Ram_HeapSlots->menuState;
	result = 0;
	previous = 0;
	menu->equipPreview = result;
	mnWindowUpdateOrCreate(&menu->listWin, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	window = menu->listWin;
	done = FALSE;

	while(done == 0 && syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
		owner = syUnitGet(menu->sideMember[pane]);
		menu->itemCnt = mnItemMenuCollectItems(owner, menu->items, 0);
		mnItemMenuDrawIcons(menu->items, 0);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
		mnItemMenuBuildPagedList(&state, pane);
		mnCursorPos(98, state.selectedIdx * 16 + 36);
		changed = TRUE;
		redraw = TRUE;

		while(syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
			mnCursorPos(98, state.row * 16 + 36);

			if(changed != 0) {
				changed = FALSE;
				if(owner->inventory[previous] != 0) {
					icon = menu->entryIcons[previous];
					mnIconShow(icon);
				}
				if(redraw != 0) {
					sySynch(1);
					mnItemMenuDrawListPage(window, 0, &state);
					if(pane == 0)
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgChangeCharacterHelp, window,
							CHANGE_HELP_X, 88);
					redraw = FALSE;
				}
				mnItemMenuShowSelectedRow(window, unused, &state);
				menu->selectedItems[pane] = menu->items[state.selectedIdx];
				mnItemMenuDrawEquipPreview(menu->sideMember[pane],
					state.selectedIdx, 0, menu->sideMember[pane]);
				if(menu->items[state.selectedIdx] != 0) {
					icon = menu->entryIcons[state.selectedIdx];
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->partyCnt; i++)
					fdSprStartAnimationStub(menu->memberSprs[i], ANIM_STAND);
			}

			if((FrameCount & 31) == 0) {
				for(i = 0; i <= 3; i++) {
					Sint32 slot = menu->memberSprIds[i];
					if(slot != -1 &&
						gmItemCanUnitEquipDirectStub(slot,
							INVENTORY_ITEM_ID_MASK &
								menu->items[state.selectedIdx]) != 0)
						fdSprStartAnimationStub(menu->memberSprs[i], ANIM_NOD);
				}
			}

			sySynch(1);
			previous = state.selectedIdx;
			moved = mnPagedListHandleInput(
				0, state.entryCnt, LIST_PAGE_SIZE, &state.row, &state.page);
			if(moved == 1) {
				redraw = TRUE;
				changed = TRUE;
			}
			if(moved == 0)
				changed = TRUE;
			if(moved == -1)
				changed = FALSE;

			if((PerWork.push & KEY_A) != 0 &&
				menu->items[state.selectedIdx] != 0) {
				sySndPlayStub(SE_NOTE_LOW);
				result = menu->items[state.selectedIdx];
				done = TRUE;
				break;
			}
			if((PerWork.push & KEY_B) != 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}
			if((PerWork.repeat & KEY_R) != 0 || (PerWork.repeat & KEY_L) != 0) {
				if(pane == 1) {
					sySndPlayStub(SE_MENU_ERROR);
					sySynch(1);
				} else {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					menu->selectedIdxByMember[menu->sideMember[pane]] =
						state.selectedIdx;
					member = menu->sideIdx[pane];
					count = menu->partyCnt;
					do {
						if((PerWork.repeat & KEY_R) != 0)
							member++;
						else
							member--;
						member = (member + count) % count;
						menu->selectedMember = menu->memberIds[member];
						menu->sideMember[pane] = menu->memberIds[member];
						menu->sideIdx[pane] = member;
						menu->itemCnt = mnItemMenuCollectItems(
							syUnitGet(menu->sideMember[pane]), menu->items, 0);
					} while(menu->itemCnt == 0);
					mnCreatePartyFigures(member / 4 * 4);
					mnMarkChosenMember(menu->mainWin, member, count);
					menu->pageArrows[0]->motion = ICON_HIDDEN;
					menu->pageArrows[1]->motion = ICON_HIDDEN;
					sySynch(1);
					break;
				}
			}
		}
	}

	mnWindowClearInteriorTilesStub(window, 0, 88, 120, 96);
	mnIconShow(menu->gridCursor);
	menu->selectedSlots[pane] = state.selectedIdx;
	menu->selectedIdxByMember[menu->sideMember[pane]] = state.selectedIdx;
	menu->selectedItems[pane] = result;
	if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
		result = -1;
	sySynch(1);
	return result;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_SPANISH)
extern Sint32 ItemMenu_CommandColumnXTable[6];

/* The cursor's x at the item command in column and row, from the table; 0
   outside three columns and three rows. */
Sint32 mnItemMenuGetCommandCursorX(Sint32 column, Sint32 row)
{
	if(column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenu_CommandColumnXTable[row * 3 + column];
}

/* The cursor's y at the item command in row: 30 in the first row, 38 below. */
Sint32 mnItemMenuGetCommandCursorY(Sint32 column, Sint32 row)
{
	Sint32 y = 30;

	if(row != 0)
		y = 38;
	return y;
}
#endif
