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
void   mnItemMenuDrawMessage(Sint32 unused, Sint32 msg);
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

	screen = syHeapSlotAllocate(HEAP_SLOT_MENU, MENU_STATE_SIZE);
#if ITEM_MENU_EXTRA_TEXT_TILES
	size = MENU_SAVED_TILE_BYTES;
	backup = syHeapAllocateEWRAM(size);
#endif
	RamHeapSlots->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	screen->partyCount = gmPartyListMembersStub(screen->memberIds);
	mnItemMenuInit(0, 3, 0, 7);
	mnLoadStatArrows();
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	screen->questionWindow =
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
		FIELD_STATE *state = RamHeapSlots->fieldState;

		/* The item goes to the field engine to be used, as fdUseItem reads
		   it. */
		state->item =
			(owner << ACT_UNIT_SHIFT) | (item & INVENTORY_ITEM_ID_MASK);
		state->itemSlot = screen->selectedSlots[0];
	}

	mnWindowSpriteClearListStub(screen->statusWindow);
	((WIN_STATE *)RamHeapSlots->windowState)->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	mnFreeArrows();
	syHeapSlotRelease(HEAP_SLOT_MENU);
	RamHeapSlots->actorSystem->suspended = 0;
#if ITEM_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy((void *)BG_CHAR_BLOCK(1), backup, size);
	((WIN_STATE *)RamHeapSlots->windowState)->holdTilemapUpload = 0;
	syHeapRelease(backup);
	sySynch(1);
	mnResumeFieldEvents();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	((WIN_STATE *)RamHeapSlots->windowState)->holdTilemapUpload = 0;
	fdPartyClearUnusableShortcutsStub();
	return result;
}

/* The resident IWRAM word copy, called through its address. */
typedef Sint32 (*ITEM_WORD_COPY_FN)(void *dest, const void *src, Sint32 size);

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
	UNIT			*srcUnit;
	UNIT			*targetUnit;
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	void			*srcCopy;
	void			*targetCopy;
	Uint32			 state;
	Sint32			 command;
	Sint32			 result;
	Sint32			 heldCount;
	Sint32			 amount;
	Sint32			 transferFailed;
	Sint32			 removedCount;
	Sint32			 giveTarget;
	Uint8			 srcQuantity;
	Uint8			 targetQuantity;
	Sint8			 stackSize;
	Sint32			 size;

	done = 0;
	selection = 0;
	menuResult = 0;
	state = ITEM_STEP_MEMBER;
	menu = RamHeapSlots->menuState;

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
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->infoWindow);
#endif
			mnDrawCoins((Sint32)menu->infoWindow);
			command = mnItemMenuSelectMember(0);
			if(command == -1) {
				/* DEAD: selection and command cleared though the menu ends
				   before reading either; the ROM keeps the 0 and its store to
				   selection. */
				selection = command = 0;
				menuResult = -1;
				done = 1;
			}
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->infoWindow);
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
				menu->completionFlag = 1;
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
					mnWindowSpriteClearListStub(menu->infoWindow);
					mnItemMenuShowMessage(
						menu->msgOffset + (Sint32)MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSpriteRedrawSavedRectStub(
						(WINDOW *)menu->infoWindow);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					menu->itemCount = mnItemMenuCollectItems(
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
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->questionWindow);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUseOnWhom, menu->questionWindow, 16, 16);
			if(mnItemMenuSelectTarget(ITEM_TARGET_USE) != -1) {
				command = 0;
				if(mnItemMenuIsStatRaisingItem(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0) {
					command = STATUS_PANEL_STATS;
				}
				result = mnItemMenuUse();
				mnStatusPanelDraw(
					menu->statusWindow, menu->sideMember[1], 0, command);
				if(result != -1) {
					mnWindowSpriteClearListStub(menu->infoWindow);
					mnItemMenuShowMessage(
						menu->msgOffset + (Sint32)MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSpriteRedrawSavedRectStub(
						(WINDOW *)menu->infoWindow);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					mnItemMenuTryBreak();
					state = ITEM_STEP_ITEM;
				}
				menu->itemCount = mnItemMenuCollectItems(
					syUnitGet(menu->sideMember[0]), menu->items, 0);
				mnItemMenuDrawIcons(menu->items, 0);
				menu->completionFlag = 1;
			} else {
				state = ITEM_STEP_COMMAND;
			}
			break;

		/* Give: choose to whom. */
		case ITEM_STEP_TARGET:
			mnItemMenuFitMessageWindowToItemQuestion();
			mnItemMenuPlaceItemWindowBelowItemQuestion();
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->questionWindow);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgGiveToWhom, menu->questionWindow, 16, 16);
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
			if((item->flags & ITEM_STACKABLE) != 0) {
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
				(Uint8)menu->selectedItemIcon->index, 0);
			menu->selectedItemIcon->motion = ICON_SHOWN;
			menu->selectedItemIcon->x = 120;
			menu->selectedItemIcon->y = 28;
			mnIconShow(menu->selectedItemIcon);
			mnWindowClearInteriorTilesStub(menu->listWindow, 0, 72, 120, 96);
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->questionWindow);
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
				gmUnitCalculateStub(command);
				mnItemMenuPlaceItemWindowBelowQuestion();
				mnItemMenuRefreshMember(menu->sideMember[0], 0);
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				sySynch(1);
				mnWindowSpriteClearListStub(menu->infoWindow);
				mnItemMenuShowMessage((Sint32)MsgDroppedIt, 15, 13);
				menu->completionFlag = 1;
				state = ITEM_STEP_ITEM;
			} else {
				state = ITEM_STEP_COMMAND;
			}
			gmUnitCalculateStub(menu->sideMember[0]);
			menu->selectedItemIcon->motion = ICON_HIDDEN;
			fdPartyClearUnusableShortcutsStub();
			break;

		/* Give the item, or some of a stack, to the chosen party member. */
		case ITEM_STEP_GIVE:
			transferFailed = 0;
			item = gmItemGetDirectStub(
				menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
			if((item->flags & ITEM_STACKABLE) != 0) {
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
			gmUnitCalculateStub(menu->sideMember[0]);
			gmUnitCalculateStub(menu->sideMember[1]);
			result = 1;
			if(transferFailed == 0) {
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
				mnItemMenuFitMessageWindowToLongItemQuestion();
#endif
				mnWindowSpriteRedrawSavedRectStub(
					(WINDOW *)menu->questionWindow);
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
				mnWindowSpriteClearListStub(menu->infoWindow);
				mnItemMenuShowMessage((Sint32)MsgCannotHoldMore, 15, 14);
			} else {
				mnWindowSpriteClearListStub(menu->infoWindow);
				if(result == 1) {
					mnItemMenuShowMessage((Sint32)MsgGiven, 15, 14);
				} else {
					mnItemMenuDrawEquipPreview(menu->sideMember[1],
						menu->selectedSlots[1], 0, menu->sideMember[1]);
					mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 14);
					item = gmItemGetDirectStub(menu->selectedItems[0]);
					if((item->flags & ITEM_CURSED) != 0) {
						sySoundPlayStub(SE_CURSE);
						mnWindowSpriteClearListStub(menu->infoWindow);
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
			srcUnit = syUnitGet(menu->sideMember[0]);
			targetUnit = syUnitGet(menu->sideMember[1]);
			size = BATTLE_UNIT_SIZE;
			srcCopy = syHeapAllocate(size);
			targetCopy = syHeapAllocate(size);
			ITEM_IWRAM_COPY_WORDS(srcCopy, srcUnit, size);
			ITEM_IWRAM_COPY_WORDS(targetCopy, targetUnit, size);
			/* Take both whole stacks out, one item at a time (at most
			   INVENTORY_STACK_MAX): gmInventoryRemoveStub returns
			   INVENTORY_REMOVE_LAST for the last one and
			   INVENTORY_REMOVE_EMPTY_SLOT for an empty slot. */
			for(srcQuantity = 0; srcQuantity < INVENTORY_STACK_MAX;
				srcQuantity++) {
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
			srcQuantity++;
			for(targetQuantity = 0; targetQuantity < INVENTORY_STACK_MAX;
				targetQuantity++) {
				/* The receiver cannot trade away a worn item that will not come
				   off. */
				if((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0) {
					item = gmItemGetDirectStub(
						menu->selectedItems[1] & INVENTORY_ITEM_ID_MASK);
					if((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
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
			while(srcQuantity != 0) {
				result = gmInventoryAddItemStub(menu->sideMember[1],
					menu->selectedItems[0] &
						(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
				if(result == -1) {
					transferFailed = 1;
					break;
				}
				menu->selectedSlots[1] = result;
				srcQuantity--;
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
				syWordsCopyEntry(srcUnit, srcCopy, size);
				syWordsCopyEntry(targetUnit, targetCopy, size);
				mnWindowSpriteClearListStub(menu->infoWindow);
				mnItemMenuShowMessage(
					(Sint32)MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if LANGUAGE == LANGUAGE_FRENCH
				mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->statusWindow);
#endif
			} else {
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[1]);
				gmUnitCalculateStub(menu->sideMember[0]);
				gmUnitCalculateStub(menu->sideMember[1]);
				mnItemMenuFitMessageWindowToItemQuestion();
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
				mnItemMenuFitMessageWindowToLongItemQuestion();
				mnItemMenuHideTradedIcons();
#endif
				mnWindowSpriteRedrawSavedRectStub(
					(WINDOW *)menu->questionWindow);
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
				mnItemMenuDrawItemHead();
				result = mnItemMenuConfirmEquip(0);
				if(syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
					mnWindowSpriteClearListStub(menu->infoWindow);
					mnItemMenuPlaceItemWindowBelowItemQuestion();
					mnItemMenuRefreshMember(menu->sideMember[1], 1);
					if(result == 0) {
						mnItemMenuDrawEquipPreview(menu->sideMember[1],
							menu->selectedSlots[1], 0, menu->sideMember[1]);
						mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 14);
						item = gmItemGetDirectStub(menu->selectedItems[0]);
						if((item->flags & ITEM_CURSED) != 0) {
							sySoundPlayStub(SE_CURSE);
							mnWindowSpriteClearListStub(menu->infoWindow);
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
			syHeapRelease(srcCopy);
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
				mnWindowSpriteClearListStub(menu->infoWindow);
				mnItemMenuShowMessage((Sint32)MsgCannotRemoveItem, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
				mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->infoWindow);
#endif
				state = ITEM_STEP_ITEM;
				break;
			}
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			gmUnitCalculateStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCount = mnItemMenuCollectItems(
				syUnitGet(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			mnWindowSpriteClearListStub(menu->infoWindow);
			mnItemMenuShowMessage((Sint32)MsgEquippedIt, 15, 8);
			item = gmItemGetDirectStub(menu->selectedItems[0]);
			if((item->flags & ITEM_CURSED) != 0) {
				sySoundPlayStub(SE_CURSE);
				mnWindowSpriteClearListStub(menu->infoWindow);
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
			gmUnitCalculateStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCount = mnItemMenuCollectItems(
				syUnitGet(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			menu->equipPreview = 1;
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			menu->equipPreview = 0;
			mnWindowSpriteClearListStub(menu->infoWindow);
			mnItemMenuShowMessage((Sint32)MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_DETAILS:
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			mnItemMenuShowDetails(menu->selectedItems[0]);
			mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->statusWindow);
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
void   mnItemPlayUseSound(Sint32 item);

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

	menu = RamHeapSlots->menuState;
	result = mnItemUse(
		menu->selectedSlots[0], menu->sideMember[0], menu->sideMember[1]);

	if(result == -1) {
		sySoundPlayStub(SE_MENU_ERROR);
		mnWindowSpriteClearListStub(menu->infoWindow);
		mnItemMenuShowMessage(
			menu->msgOffset + (Sint32)MsgItemUseResult, result, result);
		/* Until every edition but the Japanese, which closes the message's
		   window instead. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSpriteRedrawSavedRectStub((WINDOW *)menu->infoWindow);
#endif
		menu->completionFlag = 1;
		return result;
	}

	item = menu->selectedItems[0];
	mask = INVENTORY_ITEM_ID_MASK;
	mask &= item;
	mnItemPlayUseSound(mask);
	gmUnitCalculateStub(menu->sideMember[0]);
	gmUnitCalculateStub(menu->sideMember[1]);
	return 1;
}

/* Sets up the item menu's first screen: the cursor window with the party's
   figures (mnCursorCreate, mnShowPartyFigures), the info window along the
   bottom and an 8 by 2 cursor grid. The parameters are not used. */
void mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
	Sint32			 ownerIndex;

	mnShowPartyFigures(mnCursorCreate(menu), 2, 2);
	for(ownerIndex = 0; ownerIndex < 4; ownerIndex++)
		menu->memberY[ownerIndex] = MENU_MEMBER_Y;

	{
		Sint32 style;
		menu->buttonHelpWindow = NULL;
		menu->statusWindow = NULL;
		style = WINDOW_FLAG_INSTANT;
		menu->infoWindow = (WINDOW *)mnWindowCreateStub(0, 17, 30, 3, style);
		menu->itemWindow = NULL;
		menu->selectedColumn = 0;
		menu->selectedRow = 0;
		menu->columnCount = 8;
		menu->rowCount = style; /* the grid's two rows */
	}
}

/* mnSpawnIconEntries makes the icons a row of the eight-wide icon grid at a
   time (columnCount), as the Psynergy screen places its own. */
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
			*firstIcons++ = mnWindowSpriteCreateFromResourceStub(
				ICON_KIND_ITEM, firstSlot, window, MENU_ICON_HIDDEN_X, y);
		}
	}

	secondSlot = ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		secondIcons = &menu->entryIcons[ICON_GRID_ROW];
		for(; secondSlot < 2 * ICON_GRID_ROW; secondSlot++) {
			*secondIcons++ = mnWindowSpriteCreateFromResourceStub(
				ICON_KIND_ITEM, secondSlot, window, 256, y);
		}
	}

	remainingSlot = 2 * ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		remainingIcons = &menu->entryIcons[2 * ICON_GRID_ROW];
		for(; remainingSlot < MENU_ENTRY_ICONS; remainingSlot++) {
			*remainingIcons++ = mnWindowSpriteCreateFromResourceStub(
				ICON_KIND_ITEM, remainingSlot, window, 256, y);
		}
	}
}

/* Hides all 32 item icons. */
void mnItemMenuHideAllIcons(void)
{
	WINDOW_SPR **icons =
		((ITEM_MENU_STATE *)RamHeapSlots->menuState)->entryIcons;
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
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
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

	menu = RamHeapSlots->menuState;
	mnReleasePartyFigures();
	mnItemMenuHideAllIcons();
	sySynch(1);
	cursor = menu->cursor;
	cursor->motion = ICON_HIDDEN;
	mnWindowCloseIfOpen((void **)&menu->mainWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->itemWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->questionWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->statusWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->buttonHelpWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->infoWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->equipWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->listWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->unopenedWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->modalWindow, 1);
	mnWindowCloseIfOpen((void **)&menu->secondUnopenedWindow, 1);
}

Sint32 mnItemMenuCollectItems(UNIT *owner, Uint16 *items, Sint32 mode);
Sint32 mnItemMenuRunMemberSelection(Uint16 *ownerIds, Uint16 *items);

/* Lets the player choose a party member in side (0 or 1), starting from the
   party member chosen last. */
Sint32 mnItemMenuSelectMember(Sint32 pane)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 result = 0;
	Sint32			 index;
	void			*owner;

	index = menu->sideIndex[pane];
	mnWindowSpriteClearListStub(menu->infoWindow);
	menu->sideCount[pane] = menu->partyCount;
	if(index == -1) {
		menu->sideIndex[pane] = result;
		index = 0;
	} else {
		mnCursorSlide(index * 24 - 10, 16);
	}
	owner = syUnitGet((Uint16)menu->memberIds[index]);
	menu->itemCount = mnItemMenuCollectItems(owner, menu->items, 0);
	result = mnItemMenuRunMemberSelection(menu->memberIds, menu->items);
	mnIconShow(menu->sideIcons[pane]);
	sySynch(1);
	return result;
}

#if LANGUAGE == LANGUAGE_JAPANESE
/* `L+A:`, the key name Japanese draws before the arrange help. */
extern const Uint8 ItemMenuArrangeKeysString[];
/* `R:`, the key name Japanese draws before the equipment help. */
extern const Uint8 ItemMenuEquipmentKeyString[];
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 selection = menu->sideIndex[0];
	Sint32			 count;
	Sint32			 redraw;
	Sint32			 result;
	Uint8			 sortMode;
	Sint32			 showEquipment;
	UNIT			*owner;
	void			*window;

	count = menu->sideCount[0];
	redraw = 2;
	result = 0;
	sortMode = 0;
	showEquipment = 0;
	owner = syUnitGet(ownerIds[selection]);

	if(mnWindowUpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, redraw))
		mnSpawnIconEntries(menu, menu->itemWindow);
	if(mnWindowUpdateOrCreate(&menu->buttonHelpWindow, 13, 13, 17, 4, redraw)) {
		menu->selectedItemIcon = mnWindowSpriteCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->buttonHelpWindow, 0, result);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
	}
#if LANGUAGE == LANGUAGE_JAPANESE
	/* Japanese draws the key names as their own strings, the help beside
	   them. */
	mnTextDrawStringInWindowStub(
		ItemMenuArrangeKeysString, menu->buttonHelpWindow, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp, menu->buttonHelpWindow, 32, 0);
	mnTextDrawStringInWindowStub(
		ItemMenuEquipmentKeyString, menu->buttonHelpWindow, 16, 8);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp + 1, menu->buttonHelpWindow, 32, 8);
#else
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp, menu->buttonHelpWindow, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgArrangeItemsHelp + 1, menu->buttonHelpWindow, 0, 8);
#endif
	menu->sideIcons[0]->motion = ICON_SHOWN;
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			sortMode = 0;
			window = menu->statusWindow;
			owner = syUnitGet(ownerIds[selection]);
			if(showEquipment) {
				menu->itemCount = mnItemMenuCollectItems(
					syUnitGet(ownerIds[selection]), menu->items, 0);
				mnItemMenuShowEquipment(ownerIds[selection]);
				mnStatusPanelDraw(
					window, ownerIds[selection], 0, STATUS_PANEL_STATS);
			} else {
				mnItemMenuRefreshMember(ownerIds[selection], 0);
				mnStatusPanelDraw(
					window, ownerIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			}
			if(redraw == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->mainWindow, selection, count);
		}
		mnCursorPos(selection % 4 * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			if(PerWork.data & KEY_L) {
				sortMode = (sortMode + 4) % 4;
				mnItemMenuSort(owner->inventory, sortMode);
				sortMode++;
				mnItemMenuRefreshMember(ownerIds[selection], 0);
				sySoundPlayStub(SE_MENU_CONFIRM);
			} else if(mnItemMenuCountItems(ownerIds[selection])) {
				sySoundPlayStub(SE_MENU_CONFIRM);
				result = ownerIds[selection];
				break;
			} else {
				sySoundPlayStub(SE_MENU_ERROR);
			}
		}
		if(PerWork.push & KEY_B) {
			sySoundPlayStub(SE_MENU_CANCEL);
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
	menu->sideIndex[0] = selection;
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;

	mnHideEntryIcons();
	mnWindowSpriteRedrawSavedRectStub(menu->itemWindow);
	mnItemMenuDrawEquipment(menu->itemWindow, ownerId, 0);
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
#define mnItemMenuClearInfo mnWindowSpriteRedrawSavedRectStub
#else
#define mnItemMenuClearInfo mnWindowSpriteClearListStub
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 selection = menu->sideIndex[1];
	void			*window = menu->itemWindow;
	Sint32			 count = menu->partyCount;
	Uint8			 result = 0;
	Bool			 descriptionShown = FALSE;
	Sint8			 redraw;
	Sint32			 heldCount;
	WINDOW_SPR		*marker;

	mnWindowSetBounds(window, 13, 5, 17, 12);
	mnWindowSpriteRedrawSavedRectStub(menu->itemWindow);
	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(menu->memberIds[menu->sideIndex[0]]);
	sySetEvent(mnEquipmentMenuUpdateCompatibilityIndicators, EVENT_PASS_TICK);
	redraw = 2;
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			window = menu->itemWindow;
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberIds[selection]);
			marker = menu->sideIcons[1];
			marker->sprite.oam.x = marker->x =
				((selection * 3 + menu->mainWindow->x) << 3) - 2;
			if(redraw == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->mainWindow, selection, count);
			if(mode == ITEM_TARGET_GIVE) {
				mnItemMenuRefreshMember(menu->memberIds[selection], 1);
				mnWindowDrawDividerLineStub(window, 0, 9, 16, 9);
				mnWindowClearInteriorTilesStub(window, 0, 72, 120, 80);
				if(selection != menu->sideIndex[0]) {
					heldCount =
						mnItemMenuGetItemQuantity(menu->memberIds[selection],
							menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
					if(mnItemMenuCountItems(menu->memberIds[selection]) ==
							INVENTORY_SLOTS &&
						heldCount == 0) {
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgTradeForWhat, window, TRADE_LABEL_X, 72);
					} else if(heldCount != 0) {
						mnTextDrawNumberInWindowStub(
							heldCount, 2, window, 8, 72);
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
					mnStatusPanelDraw(menu->statusWindow,
						menu->memberIds[selection], menu->selectedSlots[0],
						STATUS_PANEL_STATS);
				else
					mnStatusPanelDraw(menu->statusWindow,
						menu->memberIds[selection], menu->selectedSlots[0],
						STATUS_PANEL_EXPERIENCE);
				if(!syGameFlagGet(FLAG_INFO_WINDOW_HOLDS_MESSAGE) &&
					!descriptionShown) {
					mnItemMenuClearInfo(menu->infoWindow);
					mnItemMenuDrawInfo(
						(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
							(Sint32)MsgItemDescription,
						menu->infoWindow, 0, 0);
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
			if(mode == ITEM_TARGET_GIVE && selection == menu->sideIndex[0]) {
				sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}
			sySoundPlayStub(SE_MENU_CONFIRM);
			result = menu->memberIds[selection];
			break;
		}
		if(PerWork.push & KEY_B) {
			sySoundPlayStub(SE_MENU_CANCEL);
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
	menu->sideIndex[1] = selection;
	menu->selectedMember = menu->memberIds[selection];
	menu->sideMember[1] = menu->memberIds[selection];
	selection = menu->sideIndex[0];
	count = menu->sideCount[0];
	mnCreatePartyFigures(selection / 4 * 4);
	mnMarkChosenMember(menu->mainWindow, selection, count);
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return (Sint8)result;
}

#undef IN_STOCK_LABEL_X

/* Event while mnItemMenuSelectTarget chooses a party member to use or give the
   item to, and while the command grid's cursor is on Give: every 32 ticks, has
   each party figure nod (ANIM_NOD) if that party member can equip the item, or
   stand (ANIM_STAND) if not. */
void mnEquipmentMenuUpdateCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint8			 i;

	if((TickCount & 31) == 0) {
		for(i = 0; i <= 3; i++) {
			Sint32 owner = menu->memberSpriteIds[i];

			if(owner != -1) {
				if(gmItemCanUnitEquipDirectStub(
					   owner, menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
					fdSpriteStartAnimationStub(
						menu->memberSprites[i], ANIM_NOD);
				else
					fdSpriteStartAnimationStub(
						menu->memberSprites[i], ANIM_STAND);
			}
		}
	}
}

/* Ends mnEquipmentMenuUpdateCompatibilityIndicators: puts every party figure
   back to ANIM_STAND and removes the event. */
void mnEquipmentMenuStopCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint8			 i;

	for(i = 0; i < menu->partyCount; i++)
		fdSpriteStartAnimationStub(menu->memberSprites[i], ANIM_STAND);
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
void mnItemMenuDrawMessage(Sint32 unused, Sint32 msg)
{
	ITEM_MENU_STATE *menu;

	menu = RamHeapSlots->menuState;
	mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
	mnTextDrawMessageAsTilesStub(msg, (Sint32)menu->questionWindow, 0, 0);
}

/* Hides the icon of each of the 32 list entries whose item is 0. Each of the 32
   icon slots must hold a sprite (mnSpawnIconEntries). */
void mnHideEmptyEntryIcons(const Uint16 *items)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
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

	icons = ((ITEM_MENU_STATE *)RamHeapSlots->menuState)->entryIcons;
	entries = inventoryEntries;
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		itemId = *entries++;
		if(itemId != 0) {
			if(iconStyle == 0) {
				mnSelectionIconLoadIntoSlotStub(
					ICON_KIND_ITEM, itemId, (Uint8)(*icons)->index, 0);
			} else {
				mnSelectionIconLoadIntoSlotStub(ICON_KIND_EQUIPMENT_ITEM,
					itemId, (Uint8)(*icons)->index, 0);
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

	menu = RamHeapSlots->menuState;
	owner = syUnitGet(ownerId);
	items = menu->items;
	menu->itemCount = mnItemMenuCollectItems(owner, items, 0);
	mnWindowSpriteRedrawSavedRectStub(menu->itemWindow);
	mnItemMenuPlaceIcons(mode);
	mnItemMenuDrawIcons(items, 0);
	if(mnItemMenuCountItems(ownerId) == 0)
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

/* Shows in the status panel what the item in owner's inventory slot would mean
   to party member target: for a usable item, target's status; for equipment,
   target's stats with it equipped; for a Psynergy item, the Psynergy it
   teaches. */
void mnItemMenuDrawEquipPreview(
	Sint32 owner, Sint32 slot, Sint32 mode, Sint32 target)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 style = 0;
	UNIT			*unit = syUnitGet(owner);
	Sint32			 entry = unit->inventory[slot];
	Sint32			 previewSlot;
	void			*buf;
	ITEM_DEFINITION *item = gmItemGetDirectStub(entry & INVENTORY_ITEM_ID_MASK);

	if(mode == 1)
		style = STATUS_PANEL_LOWER_ONLY;
	switch(item->type) {
	case ITEM_TYPE_NONE: /* usable items */
		mnStatusPanelDraw(menu->statusWindow, target, slot, style);
		break;
	case ITEM_TYPE_ELEMENTAL_STAR:
		mnStatusPanelDraw(menu->statusWindow, target, slot, style);
		break;
	case ITEM_TYPE_CLASS_CHANGE:
		if(owner == target) {
			style |= STATUS_PANEL_EQUIP_CLASS;
			mnStatusPanelDraw(menu->statusWindow, target, slot, style);
		} else {
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buf = syHeapAllocate(size);
			syWordsCopyEntry(buf, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				entry &= ~INVENTORY_EQUIPPED;
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_EQUIP_CLASS;
					mnStatusPanelDraw(
						menu->statusWindow, target, previewSlot, style);
				} else {
					mnStatusPanelDraw(menu->statusWindow, target, slot, style);
				}
			} else {
				mnStatusPanelDraw(menu->statusWindow, target, slot, style);
			}
			syWordsCopyEntry(unit, buf, size);
			syHeapRelease(buf);
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
			mnStatusPanelDraw(menu->statusWindow, target, slot, style);
		} else {
			/* The class item's preview again, as the Psynergy item's below: one
			   function shared by the three does not keep the ROM's bytes. */
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buf = syHeapAllocate(size);
			syWordsCopyEntry(buf, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				entry &= ~INVENTORY_EQUIPPED;
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_EQUIP;
					mnStatusPanelDraw(
						menu->statusWindow, target, previewSlot, style);
				} else {
					mnStatusPanelDraw(menu->statusWindow, target, slot, style);
				}
			} else {
				mnStatusPanelDraw(menu->statusWindow, target, slot, style);
			}
			syWordsCopyEntry(unit, buf, size);
			syHeapRelease(buf);
		}
		break;
	case ITEM_TYPE_PSYNERGY:
		if(target == owner) {
			style |= STATUS_PANEL_LEARN;
			mnStatusPanelDraw(menu->statusWindow, target, slot, style);
		} else {
			Sint32 size;

			unit = syUnitGet(target);
			size = BATTLE_UNIT_SIZE;
			buf = syHeapAllocate(size);
			syWordsCopyEntry(buf, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(target);
			if(previewSlot != 0) {
				previewSlot = gmInventoryAddItemStub(target, entry);
				if(previewSlot != -1) {
					style |= STATUS_PANEL_LEARN;
					mnStatusPanelDraw(
						menu->statusWindow, target, previewSlot, style);
				} else {
					mnStatusPanelDraw(menu->statusWindow, target, slot, style);
				}
			} else {
				mnStatusPanelDraw(menu->statusWindow, target, slot, style);
			}
			syWordsCopyEntry(unit, buf, size);
			syHeapRelease(buf);
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

	menu = RamHeapSlots->menuState;
	commandIndex = 0;
	column = 0;
	row = 0;
	redraw = TRUE;
	mnItemMenuBuildCommands(commandStates);
	redrawFlag = &menu->flags;
	if(*redrawFlag != 1) {
		void *msgWindow;

		mnItemMenuHideAllIcons();
		mnWindowSpriteRedrawSavedRectStub(menu->listWindow);
		msgWindow = menu->questionWindow;
		mnItemMenuFitMessageWindowToItemChoices();
		mnWindowSpriteRedrawSavedRectStub(msgWindow);
		mnWindowDrawDividerLineStub(msgWindow, 0, 3, 16, 3);
		mnItemMenuDrawItemHead();
		mnItemMenuDrawCommands(commandStates, msgWindow);
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
						EVENT_PASS_TICK);
			} else if(commandIndex != 0) {
				menu->equipPreview = 0;
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
			} else {
				mnStatusPanelDraw(menu->statusWindow, menu->sideMember[0], 0,
					STATUS_PANEL_EXPERIENCE);
			}
		}
		mnCursorPos(
			COMMAND_CURSOR_X(column, row), COMMAND_CURSOR_Y(column, row));
		sySynch(1);

		if((PerWork.push & KEY_A) != 0) {
			if(commandStates[commandIndex] == ITEM_COMMAND_STATE_DISABLED) {
				sySoundPlayStub(SE_MENU_ERROR);
			} else {
				switch(commandIndex) {
				case ITEM_COMMAND_USE:
					sySoundPlayStub(SE_NOTE_MID);
					break;
				case ITEM_COMMAND_EQUIP:
					sySoundPlayStub(SE_NOTE_HIGH);
					break;
				case ITEM_COMMAND_DETAILS:
				case ITEM_COMMAND_GIVE:
				case ITEM_COMMAND_DROP:
					sySoundPlayStub(SE_MENU_CONFIRM);
					break;
				case ITEM_COMMAND_REMOVE:
					sySoundPlayStub(SE_ITEM_REMOVE);
					break;
				default:
					sySoundPlayStub(SE_MENU_CONFIRM);
					break;
				}
				menu->listMode = commandIndex;
				break;
			}
		}
		if((PerWork.push & KEY_B) != 0) {
			sySoundPlayStub(SE_MENU_CANCEL);
			commandIndex = -1;
			menu->listMode = commandIndex;
			break;
		}

		if((PerWork.repeat & KEY_UP) != 0) {
			row -= 1;
			redraw = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_DOWN) != 0) {
			row += 1;
			redraw = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_RIGHT) != 0) {
			column += 1;
			redraw = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerWork.repeat & KEY_LEFT) != 0) {
			column -= 1;
			redraw = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
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

	menu = RamHeapSlots->menuState;
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

	if(item->flags & ITEM_CANNOT_UNEQUIP) {
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
		if(menu->selectedItems[0] & INVENTORY_EQUIPPED) {
			commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;
			commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
		}
	}

	if(fdHasItemCueStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

	if(menu->partyCount <= 1)
		commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

	if(item->flags & ITEM_INDISPENSABLE)
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
	Uint32 firstCommandMsg;

	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	useState = commandStates[ITEM_COMMAND_USE];
	if(useState == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);

	firstCommandMsg = MsgItemCommandUse;
	/* "Use" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg, window, 0, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	/* "Equip" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMsg + 1, window, ITEM_COMMAND_TEXT_X, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	/* "Give" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 2, window, 0, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DROP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	/* "Drop" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 3, window, 80, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DETAILS] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	/* "Details" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 4, window, 80, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	/* "Remove" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMsg + 5, window, ITEM_COMMAND_TEXT_X, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
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
				Uint8  flags = action->flags;

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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Uint32			 item = menu->selectedItems[0];
	Uint32			 mask;

	mask = INVENTORY_ITEM_ID_MASK;
	mask &= item;
	/* One time in eight: 8192 of syRandom's 65536 values. */
	if(gmItemGetDirectStub(mask)->useType == ITEM_USE_MAY_BREAK &&
		syRandom() < 8192) {
		gmInventoryBreakStub(menu->sideMember[0], menu->selectedSlots[0]);
		sySoundPlayStub(SE_ITEM_BREAK);
		mnItemMenuShowMessage((Sint32)MsgItBroke, 0, -1);
		mnWindowSpriteRedrawSavedRectStub(menu->infoWindow);
	}
}

#undef ITEM_COMMAND_TEXT_X

/* Opens the details window (top left, 13 by 10 tiles) and describes the item in
   side itemIndex there. Returns 1. */
Sint32 mnItemMenuOpenDetail(Sint32 itemIndex)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;

	mnWindowUpdateOrCreate(
		&menu->equipWindow, 0, 0, 13, 10, WINDOW_FLAG_INSTANT);
	mnPaletteLoadIconsIntoHighlight();

	if(menu->selectedItems[itemIndex] != 0)
		mnItemMenuDrawItemDetails(
			menu->equipWindow, menu->selectedItems[itemIndex]);

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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
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

	mnWindowSpriteRedrawSavedRectStub(window);
	sySynch(1);
	mnWindowCloseStub(window, 1);
	mnWindowSpriteRedrawSavedRectStub(menu->mainWindow);
	mnWindowDrawDividerLineStub(menu->mainWindow, 0, 3, 12, 3);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	{
		Sint32 priority = EVENT_PASS_TICK;

		sySetEvent(mnUpdatePartyFigures, priority);
	}

	{
		WINDOW_SPR *cursor = menu->cursor;
		cursor->motion = ICON_SHOWN;
	}
#if LANGUAGE == LANGUAGE_JAPANESE
	menu->flags = 1;
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
extern Uint8 MenuPointString[];

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
	ITEM_DEFINITION *definition;
	Sint32			 i;
	Sint32			 found;
	Sint32			 amount;
	Sint32			 consumable;
	Sint8			 row;

	row = 0;
	consumable = 0;
	definition = gmItemGetDirectStub(item & INVENTORY_ITEM_ID_MASK);
	if(definition->type != ITEM_TYPE_NONE) {
		found = 0;
		if(definition->attack == 0 && definition->defense == 0) {
			for(i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
				if(definition->effects[i].kind != 0 ||
					definition->useType == ITEM_USE_TEACHES) {
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
		if(definition->attack != 0) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgStatLabel, window, 0, row * 8);
			amount = definition->attack;
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			row++;
		}
		if(definition->defense != 0) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgDefenseLabel, window, 0, row * 8);
			amount = definition->defense;
			mnItemMenuDrawStat(
				amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
			row++;
		}
	}
	for(i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
		if(definition->effects[i].kind == EQUIP_EFFECT_NONE)
			continue;
		amount = definition->effects[i].amount;
		switch(definition->effects[i].kind) {
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
				definition->effects[i].kind + (Sint32)MsgEquipEffectName,
				window, 0, row * 8);
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
				(Uint8)((definition->effects[i].kind -
							EQUIP_EFFECT_ADD_VENUS_POWER) %
					ELEMENT_COUNT) +
					1,
				0, row, TILEMAP_ENTRY_HIGHLIGHT);
			mnTextDrawMessageAsTilesStub(
				definition->effects[i].kind + (Sint32)MsgEquipEffectName,
				window, 8, row * 8);
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
				definition->effects[i].kind + (Sint32)MsgEquipEffectName,
				window, 0, row * 8);
			/* The scale is in tenths: "1.2" for 12. */
			if(amount >= STAT_SCALE_ONE) {
				mnTextDrawNumberInWindowStub(
					1, 1, window, ITEM_DETAILS_MULTIPLIER_X, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, window,
					ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
				mnTextDrawNumberInWindowStub(amount - STAT_SCALE_ONE, 1, window,
					ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
			} else {
				mnTextDrawNumberInWindowStub(
					0, 1, window, ITEM_DETAILS_MULTIPLIER_X, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, window,
					ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
				mnTextDrawNumberInWindowStub(
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
				definition->effects[i].kind + (Sint32)MsgEquipEffectName,
				window, 0, row * 8);
#if LANGUAGE == LANGUAGE_GERMAN
			/* German continues these three names on a second line, the messages
			   after the last effect's name in the order of their effects:
			   "steigen" and "aufgehoben werden.". */
			row++;
			if(definition->effects[i].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 1, window,
					0, row * 8);
			else if(definition->effects[i].kind == EQUIP_EFFECT_CURSE_REMOVABLE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 2, window,
					0, row * 8);
#if !FIX_LURE_CAP_STRAY_SECOND_LINE
			/* BUG: German has only those two second lines, so the Lure Cap's
			   "Erhöht gegn." goes on with the message after them, "Items". */
			else if(definition->effects[i].kind == EQUIP_EFFECT_LURE)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgEquipEffectName + EQUIP_EFFECT_LURE + 3, window,
					0, row * 8);
#endif
#endif
			break;
		}
		row++;
	}
	if(definition->flags & ITEM_CURSED) {
		mnTextDrawMessageAsTilesStub((Sint32)MsgItemCursed, window, 0, row * 8);
		row++;
	}
	if(definition->useType == ITEM_USE_TEACHES) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgBestowsPsynergy, window, 0, row * 8);
		consumable = 1;
		row++;
	}
	if(definition->useType != ITEM_USE_EMPTIES &&
		definition->useType != ITEM_USE_FREE) {
		if(!consumable) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUsesHeading, window, ITEM_DETAILS_USES_X, row * 8);
			row++;
		}
		switch(definition->useType) {
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
	if(definition->flags & ITEM_STACKABLE) {
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
			mnTextDrawNumberInWindowStub(amount + 1, 2, window, 0, row * 8);
			mnTextDrawMessageAsTilesStub(i + 1, window, 16, row * 8);
			row++;
		} else if(count == 0) {
			mnTextDrawMessageAsTilesStub(i + 8, window, 0, row * 8);
			row++;
		} else {
			amount = count;
			mnTextDrawNumberInWindowStub(amount, 3, window, 0, row * 8);
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
		if(definition->flags & ITEM_RARE) {
			mnTextDrawMessageAsTilesStub((Sint32)MsgRareItem, window, 0, 0);
			found = 1;
		}
		if(!found) {
			if(definition->flags & ITEM_INDISPENSABLE) {
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

	mnTextDrawNumberInWindowStub(statChange, 3, window, x, y);
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
			&MenuPlusSignString, window, x - digits * 8 + 16, y);
	} else {
		mnTextDrawStringInWindowStub(
			&MenuMinusSignString, window, x - digits * 8 + 16, y);
	}
}

#define ITEM_MENU ((ITEM_MENU_STATE *)RamHeapSlots->menuState)

/* Moves the item window to row 5 of the right-hand side. */
void mnItemMenuPlaceItemWindowBelowItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->itemWindow, 13, 5, 17, 10);
}

/* Moves the item window to row 3 of the right-hand side, below a one-line
   question. */
void mnItemMenuPlaceItemWindowBelowQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->itemWindow, 13, 3, 17, 10);
}

/* Makes the message window, along the top right, 5 rows tall: the item's name
   and a question below it. */
void mnItemMenuFitMessageWindowToItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 5);
}

/* Makes the item menu's message window six tiles high. */
void mnItemMenuFitMessageWindowToLongItemQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 6);
}

/* Makes the item menu's message window seven tiles high. */
void mnItemMenuFitMessageWindowToItemChoices(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 7);
}

/* Makes the item menu's message window three tiles high. */
void mnItemMenuFitMessageWindowToQuestion(void)
{
	mnWindowSetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 3);
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
extern const Uint8 MenuQuantityTiles[];

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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Uint8	   *tiles = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	Sint32		entry;
	Sint32		changed = 1;
	Sint32		targetCount = 0;
	void	   *window = menu->questionWindow;
	Sint32		ownerCount;
	WINDOW_SPR *arrow;

	mnItemMenuFitMessageWindowToItemChoices();
	mnWindowSpriteRedrawSavedRectStub(window);
	if(dropMode == 0)
		targetCount = mnItemMenuGetItemQuantity(menu->sideMember[1],
			menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	ownerCount = mnItemMenuGetItemQuantity(
		menu->sideMember[0], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	entry = syVRAMCacheFindFreeEntry();
	if(entry != VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(entry, 256, 0);
		mnWindowSpriteCreateStub(
			entry, WINDOW_SPRITE_SIZE_32X8, window, 48, 32);
		arrow = mnWindowSpriteCreateStub(
			entry, WINDOW_SPRITE_SIZE_32X8, window, 80, 32);
		arrow->sprite.oam.index = arrow->sprite.oam.index + 4;
		mnCursorSlide(HOW_MANY_CURSOR_X, 40);
		while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
			if(changed) {
				changed = 0;
				value = (value + quantityRange) % quantityRange;
				mnWindowSpriteRedrawSavedRectStub(window);
				mnTextDrawMessageAsTilesStub(
					HOW_MANY_MESSAGE, window, HOW_MANY_X, HOW_MANY_Y);
				syDMAXCopy(
					MenuQuantityTiles, tiles, DMA_COPY_WORDS(64), REG_DMA3);
				mnQuantityBarFillStub(
					INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH, tiles);
				mnQuantityBarFillStub(
					baseQuantity + quantityRange, QUANTITY_BAR_OPEN, tiles);
				mnQuantityBarFillStub(
					baseQuantity + value + 1, QUANTITY_BAR_CHOSEN, tiles);
				mnQuantityBarFillStub(baseQuantity, QUANTITY_BAR_HELD, tiles);
				syVRAMCacheLoad(entry, 256, tiles);
				mnTextDrawNumberInWindowStub(value + 1, 2, window, 32, 32);
				mnTextDrawMessageAsTilesStub(
					(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
						(Sint32)MsgItemName,
					window, 16, 8);
				mnTextDrawNumberInWindowStub(
					ownerCount - value - 1, 2, window, 16, 24);
#if LANGUAGE == LANGUAGE_JAPANESE
				mnTextDrawMessageAsTilesStub(PIECE_MESSAGE, window, 32, 24);
#endif
				if(dropMode == 0) {
					mnTextDrawNumberInWindowStub(
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
				sySoundPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if(PerWork.push & KEY_B) {
				value = -1;
				sySoundPlayStub(SE_MENU_CANCEL);
				break;
			}
			mnCursorPos(HOW_MANY_CURSOR_X, 40);
			if(PerWork.repeat & KEY_LEFT) {
				value--;
				changed = 1;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if(PerWork.repeat & KEY_RIGHT) {
				value++;
				changed = 1;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	mnWindowSpriteRedrawSavedRectStub(window);
	mnWindowSpriteClearListStub(window);
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;

	mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM, menu->selectedItems[0],
		(Uint8)menu->selectedItemIcon->index, 0);
	menu->selectedItemIcon->motion = ICON_SHOWN;
	menu->selectedItemIcon->x = 112;
	menu->selectedItemIcon->y = 8;
	mnIconShow(menu->selectedItemIcon);
	mnTextDrawStringAtOffsetStub(
		syUnitGet(menu->sideMember[0]), menu->questionWindow, 16, 0);
	mnTextDrawMessageAsTilesStub(
		(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + (Sint32)MsgItemName,
		menu->questionWindow, 16, 8);
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
			sySoundPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerWork.push & KEY_B) {
			sySoundPlayStub(SE_MENU_CANCEL);
			selection = 1;
			break;
		}
		mnCursorPos(104, (selection << 4) + 70);
		if(PerWork.repeat & KEY_UP) {
			selection--;
			changed = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerWork.repeat & KEY_DOWN) {
			selection++;
			changed = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
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
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
	void			*unit = syUnitGet(menu->sideMember[1]);
	void			*saved;
	Sint32			 window;
	Sint32			 slot;
	Sint32			 member;
	Sint32			 result;

	slot = menu->selectedSlots[1];
	member = menu->sideMember[1];
	mnItemMenuDrawEquipPreview(member, slot, 0, member);
	saved = syHeapAllocate(BATTLE_UNIT_SIZE);
	syWordsCopy(saved, unit, BATTLE_UNIT_SIZE);
	window = menu->questionWindow;
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
				sySoundPlayStub(SE_NOTE_HIGH);
				break;
			}
			if(PerWork.push & KEY_B) {
				sySoundPlayStub(SE_MENU_CANCEL);
				selection = 1;
				break;
			}
			mnCursorPos(EQUIP_CURSOR_X(selection), EQUIP_CURSOR_Y(selection));
			if(PerWork.repeat & EQUIP_PREVIOUS_KEY) {
				selection--;
				changed = TRUE;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if(PerWork.repeat & EQUIP_NEXT_KEY) {
				selection++;
				changed = TRUE;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
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
	gmUnitCalculateStub(menu->sideMember[1]);
	return selection;
}

#undef EQUIP_QUESTION

/* Loads the small up and down arrows of the stat changes
   (mnIconCreateStatChangeArrow) into free VRAM cache slots. */
void mnLoadStatArrows(void)
{
	ITEM_MENU_STATE *state = RamHeapSlots->menuState;
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 count;
	Sint32			 row;
	Sint32			 page;
	Sint32			 pages;
	Sint32			 selected;

	count = mnItemMenuCountItems(menu->sideMember[pane]);
	unit = syUnitGet(menu->sideMember[pane]);
	selected = menu->selectedIndexByMember[menu->sideMember[pane]];
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
	list->pageCount = pages;
	list->row = row;
	list->entryCount = count;
	list->selectedIndex = selected;
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
	menu = RamHeapSlots->menuState;
	window = menu->infoWindow;
	combined = page * LIST_PAGE_SIZE;
	combined += state->row;
	state->selectedIndex = combined;

	/* The Japanese release clears the info window; in every edition but the
	   Japanese it redraws the window's saved rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowSpriteRedrawSavedRectStub(window);
#else
	mnWindowSpriteClearListStub(window);
#endif
	sySynch(1);

	combined = state->selectedIndex;
	if(menu->items[combined] != 0) {
		Sint32 masked = (menu->items[combined] & INVENTORY_ITEM_ID_MASK) +
			(Sint32)MsgItemDescription;

		/* Japanese draws the name as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnTextDrawMessageStub(masked, menu->infoWindow, 0, 0);
#else
		mnTextDrawMessageAsTilesStub(masked, menu->infoWindow, 0, 0);
#endif
	}

	for(row = 0; row <= LIST_PAGE_SIZE - 1; row++) {
		if(row == state->row) {
			mnWindowSetHighlightRect(
				menu->itemWindow, 1, row * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
		} else {
			mnWindowSetHighlightRect(
				menu->itemWindow, 1, row * 2 + 1, 14, 1, WINDOW_PALETTE);
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
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Uint32			 page;
	Uint32			 firstEntry;
	Uint32			 visibleCount;
	Uint8			 row;
	const Uint16	*itemId;

	mnWindowSpriteRedrawSavedRectStub(window);
	mnWindowDrawDividerLineStub(window, 0, 11, 16, 11);

	page = state->page;
	firstEntry = page * LIST_PAGE_SIZE;
	visibleCount = (Uint8)(state->entryCount - firstEntry);
	if(visibleCount > LIST_PAGE_SIZE) {
		visibleCount = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(window, state->entryCount, LIST_PAGE_SIZE,
		state->page, PAGE_NUMBERS_RIGHT);

	row = 0;
	if(visibleCount > row) {
		itemId = &menu->items[firstEntry];
		do {
			mnTextDrawMessageAsTilesStub(
				(itemId[0] & INVENTORY_ITEM_ID_MASK) + (Sint32)MsgItemName,
				menu->itemWindow, ENTRY_X, row * 16 + 8);
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
	Sint32			 prev;
	WINDOW_SPR		*icon;
	Sint32			 unused[5];
	PAGED_LIST		 state;

	menu = RamHeapSlots->menuState;
	result = 0;
	prev = 0;
	menu->equipPreview = result;
	mnWindowUpdateOrCreate(
		&menu->listWindow, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	window = menu->listWindow;
	done = FALSE;

	while(done == 0 && syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
		owner = syUnitGet(menu->sideMember[pane]);
		menu->itemCount = mnItemMenuCollectItems(owner, menu->items, 0);
		mnItemMenuDrawIcons(menu->items, 0);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
		mnItemMenuBuildPagedList(&state, pane);
		mnCursorPos(98, state.selectedIndex * 16 + 36);
		changed = TRUE;
		redraw = TRUE;

		while(syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
			mnCursorPos(98, state.row * 16 + 36);

			if(changed != 0) {
				changed = FALSE;
				if(owner->inventory[prev] != 0) {
					icon = menu->entryIcons[prev];
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
				menu->selectedItems[pane] = menu->items[state.selectedIndex];
				mnItemMenuDrawEquipPreview(menu->sideMember[pane],
					state.selectedIndex, 0, menu->sideMember[pane]);
				if(menu->items[state.selectedIndex] != 0) {
					icon = menu->entryIcons[state.selectedIndex];
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->animationStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->partyCount; i++)
					fdSpriteStartAnimationStub(
						menu->memberSprites[i], ANIM_STAND);
			}

			if((TickCount & 31) == 0) {
				for(i = 0; i <= 3; i++) {
					Sint32 slot = menu->memberSpriteIds[i];
					if(slot != -1 &&
						gmItemCanUnitEquipDirectStub(slot,
							INVENTORY_ITEM_ID_MASK &
								menu->items[state.selectedIndex]) != 0)
						fdSpriteStartAnimationStub(
							menu->memberSprites[i], ANIM_NOD);
				}
			}

			sySynch(1);
			prev = state.selectedIndex;
			moved = mnPagedListHandleInput(
				0, state.entryCount, LIST_PAGE_SIZE, &state.row, &state.page);
			if(moved == 1) {
				redraw = TRUE;
				changed = TRUE;
			}
			if(moved == 0)
				changed = TRUE;
			if(moved == -1)
				changed = FALSE;

			if((PerWork.push & KEY_A) != 0 &&
				menu->items[state.selectedIndex] != 0) {
				sySoundPlayStub(SE_NOTE_LOW);
				result = menu->items[state.selectedIndex];
				done = TRUE;
				break;
			}
			if((PerWork.push & KEY_B) != 0) {
				sySoundPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}
			if((PerWork.repeat & KEY_R) != 0 || (PerWork.repeat & KEY_L) != 0) {
				if(pane == 1) {
					sySoundPlayStub(SE_MENU_ERROR);
					sySynch(1);
				} else {
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
					menu->selectedIndexByMember[menu->sideMember[pane]] =
						state.selectedIndex;
					member = menu->sideIndex[pane];
					count = menu->partyCount;
					do {
						if((PerWork.repeat & KEY_R) != 0)
							member++;
						else
							member--;
						member = (member + count) % count;
						menu->selectedMember = menu->memberIds[member];
						menu->sideMember[pane] = menu->memberIds[member];
						menu->sideIndex[pane] = member;
						menu->itemCount = mnItemMenuCollectItems(
							syUnitGet(menu->sideMember[pane]), menu->items, 0);
					} while(menu->itemCount == 0);
					mnCreatePartyFigures(member / 4 * 4);
					mnMarkChosenMember(menu->mainWindow, member, count);
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
	menu->selectedSlots[pane] = state.selectedIndex;
	menu->selectedIndexByMember[menu->sideMember[pane]] = state.selectedIndex;
	menu->selectedItems[pane] = result;
	if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
		result = -1;
	sySynch(1);
	return result;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_SPANISH)
extern Sint32 ItemMenuCommandColumnXTable[6];

/* The cursor's x at the item command in column and row, from the table; 0
   outside three columns and three rows. */
Sint32 mnItemMenuGetCommandCursorX(Sint32 column, Sint32 row)
{
	if(column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenuCommandColumnXTable[row * 3 + column];
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
