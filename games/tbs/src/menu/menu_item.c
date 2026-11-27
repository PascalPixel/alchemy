/* The item menu: the field menu's Item screen, the command loop, the party
   member and item lists, and Use, Equip, Give, Drop, Remove and Details. Part
   of menu.c. */

#include "menu_internal.h"
#include "shop/shop.h"

/* Where the edition's text goes. The international editions have longer words,
   and some languages move a message to fit its own.

   The x of the small message windows "Dropped it.", "You were cursed!" and
   "Removed". */
#define ITEM_MODAL_SHORT_X	 JAPANESE_OR(15, 14)
#define ITEM_MODAL_REMOVED_X JAPANESE_OR(17, 14)

/* The x of the small message window "Can't trade it!". */
#define ITEM_CANNOT_TRADE_X FRENCH_OR(11, 15)

/* The x of the small message window "Traded". */
#define ITEM_TRADED_X ITALIAN_OR(13, 15)

/* The x of "Trade for what?" in its window, in pixels. */
#define ITEM_TRADE_PROMPT_X JAPANESE_OR(16, 0)

/* The x of the command grid's middle column. */
#define ITEM_COMMAND_TEXT_X JAPANESE_OR(40, 32)

/* The quantity chooser's window width, in tiles. */
#define ITEM_QUANTITY_WINDOW_WIDTH JAPANESE_OR(13, 30)

/* The hand cursor and the "How many?" question of the quantity chooser. */
#define ITEM_GIVE_CURSOR_X JAPANESE_OR(88, 128)
#define ITEM_GIVE_ASK_X	   JAPANESE_OR(0, 32)
#define ITEM_GIVE_ASK_Y	   JAPANESE_OR(32, 0)

/* The x of the drop question ("Are you sure you...") in its window, in
   pixels. */
#define ITEM_DROP_QUESTION_X JAPANESE_OR(8, 0)

/* The item list's icon column and name column. */
#define ITEM_LIST_PAGE_ICONS_X JAPANESE_OR(120, 116)
#define ITEM_LIST_NAME_X	   JAPANESE_OR(32, 24)

/* The bytes of window tiles (BG character block 1) the menu draws over, which
   it saves while it is open and puts back as it closes. */
#define SAVED_WINDOW_TILE_BYTES 8192

/* The field menu's Item screen (mnFieldMenuRun, FIELD_MENU_ITEM): choose a
   party member, one of their items and a command. */
Sint32 mnItemMenuRun(void)
{
#if ITEM_MENU_EXTRA_TEXT_TILES
	void *backup;
#endif
	ITEM_MENU_STATE *screen;
	Sint32			 item;
	Sint32			 tgt;
	Sint32			 member;
	Sint32			 result;
#if ITEM_MENU_EXTRA_TEXT_TILES
	Sint32 size = SAVED_WINDOW_TILE_BYTES;
#endif

	/* Open the menu over the field, which stops moving. */
#if ITEM_MENU_EXTRA_TEXT_TILES
	backup = syHeapAllocEWRAM(size);
#endif
	screen = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	screen->partyCnt = gmPartyListMembersStub(screen->memberIds);
	mnItemMenuInit(0, 3, 0, 7);
	mnLoadStatArrows();
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	screen->questionWin = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
#if ITEM_MENU_EXTRA_TEXT_TILES
	/* Save the window tiles the menu draws over, and fill them with colour
	   3. */
	mnSuspendFieldEvents();
	syWordsCopy(backup, BG_CHAR_BLOCK(1), size);
	syWordsFill(BG_CHAR_BLOCK(1), size, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif

	/* Let the player choose. */
	mnStartWatchingStart();
	result = mnItemMenuRunCommands(&member, &tgt, &item);
	mnStopWatchingStart();
	if(result == 1) {
		FIELD_STATE *selection = FieldStateWork;

		selection->item =
			(member << ACT_UNIT_SHIFT) | (item & INVENTORY_ITEM_ID_MASK);
		selection->itemSlot = screen->selectedSlots[0];
	}

	/* Close the menu and give the screen back to the field. */
	mnWindowSprClearListStub(screen->statWin);
	WINDOW_STATE->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	mnIconMenuReleaseArrows();
	syHeapSlotRelease(HEAP_SLOT_MENU);
	ActorSystemStateWork->suspended = 0;
#if ITEM_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy(BG_CHAR_BLOCK(1), backup, size);
	WINDOW_STATE->holdTilemapUpload = 0;
	syHeapRelease(backup);
	sySynch(1);
	mnResumeFieldEvents();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	WINDOW_STATE->holdTilemapUpload = 0;
	fdPartyClearUnusableShortcutsStub();
	return result;
}

/* The resident IWRAM word copy, called through its address. */
typedef Sint32 (*ITEM_WORD_COPY_FN)(void *dst, const void *src, Sint32 size);

/* syWordsCopy's function reached from syIWRAMIRQMain rather than at its fixed
   address: the trade's snapshot copies call it this way. */
#define ITEM_IWRAM_COPY_WORDS \
	((ITEM_WORD_COPY_FN)(syIWRAMIRQMain + syWordsCopyOffset))

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
	ITEM_STEP_DETAILS = 10, /* Details (mnItemMenuShowDetails) */
	ITEM_STEP_REMOVE = 11,
	ITEM_STEP_QUANTITY = 12 /* nothing sets it */
} ITEM_MENU_STEP;

/* Runs the item menu: choose a party member, one of their items and a command
   (Use, Equip, Give, Drop, Remove or Inspect), then carry it out, until the
   player backs out or presses Start. */
Sint32 mnItemMenuRunCommands(Sint32 *memberOut, Sint32 *tgtOut, Sint32 *itemOut)
{
	Sint32 done;
#if !ITEM_EVENT_USED_BY_SHOWN_HOLDER
	Sint32 member;
	Sint32 chosen;
#endif
	Sint32			 selection;
	Sint32			 menuResult;
	UNIT			*srcUnit;
	UNIT			*tgtUnit;
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	UNIT			*srcCopy;
	UNIT			*tgtCopy;
	Uint32			 state;
	Sint32			 command;
	Sint32			 result;
	Sint32			 heldCnt;
	Sint32			 amount;
	Sint32			 transferFailed;
	Sint32			 removedCnt;
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
	Sint32 giveTgt;
#endif
	Uint8 srcQuantity;
	Uint8 tgtQuantity;
	Sint8 stackSize;

	done = 0;
#if !ITEM_EVENT_USED_BY_SHOWN_HOLDER
	member = 0;
	chosen = 0;
#endif
	selection = 0;
	menuResult = 0;
	state = ITEM_STEP_MEMBER;
	menu = MenuState;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		switch(state) {
		/* Choose whose items to look at. */
		case ITEM_STEP_MEMBER:
			menu->selectedSlots[0] = 0;
			mnItemMenuFitMessageWindowToQuestion();
			mnItemMenuPlaceItemWindowBelowQuestion();
			menu->selectedItemIcon->motion = ICON_HIDDEN;
			mnItemMenuDrawMessage(0, MsgWhoseItem);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
			mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
			mnDrawCoins(menu->descriptionWin);
			command = mnItemMenuSelectMember(0);
			if(command == -1) {
				/* DEAD: selection and command cleared though the menu ends
				   before reading either; the ROM keeps the 0 and its store to
				   selection. */
				selection = command = 0;
				menuResult = -1;
				done = 1;
			}
#else
			mnDrawCoins(menu->descriptionWin);
			member = mnItemMenuSelectMember(0);
			if(member == -1) {
				/* DEAD: chosen, selection and member cleared though the menu
				   ends before reading them; the ROM keeps the 0 and its three
				   stores. */
				chosen = selection = member = 0;
				menuResult = -1;
				done = 1;
			}
#endif
			mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
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
			mnItemMenuDrawMessage(0, MsgWhichItem);
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
					/* The international editions report target 0 (command is
					   ITEM_COMMAND_USE here) and the bare item id; the Japanese
					   ones the target chosen last and the list's whole
					   inventory entry. */
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
					*memberOut = menu->sideMember[0];
					*tgtOut = command;
					*itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
#else
					*memberOut = member;
					*tgtOut = chosen;
					*itemOut = selection;
#endif
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
					mnWindowSprClearListStub(menu->descriptionWin);
					mnItemMenuShowMessage(
						menu->msgOfs + MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					menu->itemCnt = mnItemMenuCollectItems(
						gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
					mnItemMenuDrawIcons(menu->items, 0);
					state = ITEM_STEP_MEMBER;
				}
				if(result == ITEM_USE_MODE_UNUSABLE ||
					result == ITEM_USE_MODE_FROM_FIELD) {
					done = 1;
					*memberOut = menu->sideMember[0];
					*tgtOut = menu->sideMember[1];
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
			mnWindowSprRedrawSavedRectStub(menu->questionWin);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				MsgUseOnWhom, menu->questionWin, 16, 16);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
			if(mnItemMenuSelectTarget(ITEM_TARGET_USE) != -1) {
#else
			chosen = mnItemMenuSelectTarget(ITEM_TARGET_USE);
			if(chosen != -1) {
#endif
				command = 0;
				if(mnItemMenuIsStatRaisingItem(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0) {
					command = STATUS_PANEL_STATS;
				}
				result = mnItemMenuUse();
				mnStatPanelDraw(menu->statWin, menu->sideMember[1], 0, command);
				if(result != -1) {
					mnWindowSprClearListStub(menu->descriptionWin);
					mnItemMenuShowMessage(
						menu->msgOfs + MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#endif
					menu->sideIcons[0]->motion = ICON_HIDDEN;
					mnItemMenuTryBreak();
					state = ITEM_STEP_ITEM;
				}
				menu->itemCnt = mnItemMenuCollectItems(
					gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
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
			mnWindowSprRedrawSavedRectStub(menu->questionWin);
			mnItemMenuDrawItemHead();
			mnTextDrawMessageAsTilesStub(
				MsgGiveToWhom, menu->questionWin, 16, 16);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
			giveTgt = mnItemMenuSelectTarget(ITEM_TARGET_GIVE);
			state = ITEM_STEP_GIVE;
			if(giveTgt == -1) {
#else
			chosen = mnItemMenuSelectTarget(ITEM_TARGET_GIVE);
			state = ITEM_STEP_GIVE;
			if(chosen == -1) {
#endif
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
				heldCnt =
					(menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if(heldCnt > 1) {
					mnItemMenuDrawItemHead();
					result = mnItemMenuSelectGiveQuantity(0, heldCnt, 1);
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
			mnWindowClearInteriorTilesStub(
				(void *)menu->listWin, 0, 72, 120, 96);
			mnWindowSprRedrawSavedRectStub(menu->questionWin);
			/* Yes: take result + 1 of the stack out, one at a time. */
			if(mnItemMenuConfirmDrop(selection) == 0) {
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
				command = menu->sideMember[0];
				gmUnitGetStub(command);
				for(removedCnt = 0; removedCnt < result + 1; removedCnt++) {
					gmInventoryRemoveStub(command, menu->selectedSlots[0]);
					gmItemAdjustRareItemStockStub(
						menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK, 1);
				}
				gmUnitCalcStub(command);
#else
				member = menu->sideMember[0];
				gmUnitGetStub(member);
				for(removedCnt = 0; removedCnt < result + 1; removedCnt++) {
					gmInventoryRemoveStub(member, menu->selectedSlots[0]);
					gmItemAdjustRareItemStockStub(
						menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK, 1);
				}
				gmUnitCalcStub(member);
#endif
				mnItemMenuPlaceItemWindowBelowQuestion();
				mnItemMenuRefreshMember(menu->sideMember[0], 0);
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				sySynch(1);
				mnWindowSprClearListStub(menu->descriptionWin);
				mnItemMenuShowMessage(MsgDroppedIt, ITEM_MODAL_SHORT_X, 13);
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
				heldCnt = mnItemMenuGetItemQuantity(menu->sideMember[1],
					menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
				if(heldCnt == INVENTORY_STACK_MAX) {
					transferFailed = 1;
				}
				if(mnItemMenuCountItems(menu->sideMember[1]) ==
						INVENTORY_SLOTS &&
					heldCnt == 0) {
					state = ITEM_STEP_TRADE;
					break;
				}
				stackSize =
					(menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if(transferFailed == 0) {
					if(heldCnt + stackSize > INVENTORY_STACK_MAX) {
						amount = INVENTORY_STACK_MAX - heldCnt;
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
						heldCnt = gmInventoryAddItemStub(menu->sideMember[1],
							menu->selectedItems[0] &
								(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
						if(heldCnt != -1) {
							gmInventoryRemoveStub(
								menu->sideMember[0], menu->selectedSlots[0]);
							menu->selectedSlots[1] = heldCnt;
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
			gmUnitCalcStub(menu->sideMember[0]);
			gmUnitCalcStub(menu->sideMember[1]);
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			gmUnitRefreshPsynergyStub(menu->sideMember[1]);
			result = 1;
			if(transferFailed == 0) {
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
				mnItemMenuFitMessageWindowToLongItemQuestion();
#endif
				mnWindowSprRedrawSavedRectStub(menu->questionWin);
				mnItemMenuDrawItemHead();
				result = mnItemMenuConfirmEquip(0);
			}
			if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
				break;
			}
			mnItemMenuRefreshMember(menu->sideMember[1], 1);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			sySynch(1);
			/* Say how it went: the receiver's stack was full, the item was
			   given, or it was given and equipped (mnItemMenuConfirmEquip
			   returned 0). */
			if(transferFailed == 1) {
				mnWindowSprClearListStub(menu->descriptionWin);
				mnItemMenuShowMessage(MsgCannotHoldMore, 15, 14);
			} else {
				mnWindowSprClearListStub(menu->descriptionWin);
				if(result == 1) {
					mnItemMenuShowMessage(MsgGiven, 15, 14);
				} else {
					mnItemMenuDrawEquipPreview(menu->sideMember[1],
						menu->selectedSlots[1], 0, menu->sideMember[1]);
					mnItemMenuShowMessage(MsgEquippedIt, 15, 14);
					item = gmItemGetDirectStub(menu->selectedItems[0]);
					if((item->flg & ITEM_CURSED) != 0) {
						sySndPlayStub(SE_CURSE);
						mnWindowSprClearListStub(menu->descriptionWin);
						/* "You were cursed!" */
						mnItemMenuShowMessage(
							MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 14);
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
			mnItemMenuDrawMessage(0, MsgSwapForWhat);
#if TRADE_CANCEL_KEEPS_CHOSEN_ITEM
			if(mnItemMenuRunList(1) == -1) {
#else
			selection = mnItemMenuRunList(1);
			if(selection == -1) {
#endif
				state = ITEM_STEP_TARGET;
				break;
			}
			/* Snapshot both inventories so any failed part of the trade can
			   roll back. */
			srcUnit = gmUnitGetStub(menu->sideMember[0]);
			tgtUnit = gmUnitGetStub(menu->sideMember[1]);
			srcCopy = syHeapAlloc(BATTLE_UNIT_SIZE);
			tgtCopy = syHeapAlloc(BATTLE_UNIT_SIZE);
			ITEM_IWRAM_COPY_WORDS(srcCopy, srcUnit, BATTLE_UNIT_SIZE);
			ITEM_IWRAM_COPY_WORDS(tgtCopy, tgtUnit, BATTLE_UNIT_SIZE);
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
			for(tgtQuantity = 0; tgtQuantity < INVENTORY_STACK_MAX;
				tgtQuantity++) {
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
			tgtQuantity++;
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
			while(tgtQuantity != 0) {
				result = gmInventoryAddItemStub(menu->sideMember[0],
					menu->selectedItems[1] &
						(INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
				if(result == -1) {
					transferFailed = 1;
					break;
				}
				menu->selectedSlots[0] = result;
				tgtQuantity--;
			}
			sySynch(1);
			if(transferFailed == 1) {
				syWordsCopy(srcUnit, srcCopy, BATTLE_UNIT_SIZE);
				syWordsCopy(tgtUnit, tgtCopy, BATTLE_UNIT_SIZE);
				mnWindowSprClearListStub(menu->descriptionWin);
				mnItemMenuShowMessage(MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if LANGUAGE == LANGUAGE_FRENCH
				mnWindowSprRedrawSavedRectStub(menu->statWin);
#endif
			} else {
				gmUnitCalcStub(menu->sideMember[0]);
				gmUnitCalcStub(menu->sideMember[1]);
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[1]);
				mnItemMenuFitMessageWindowToItemQuestion();
#if LANGUAGE != LANGUAGE_JAPANESE
				mnItemMenuFitMessageWindowToLongItemQuestion();
				mnItemMenuHidePageIcons();
#endif
				mnWindowSprRedrawSavedRectStub(menu->questionWin);
				menu->sideMember[0] = menu->sideMember[1];
				menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
				mnItemMenuDrawItemHead();
				result = mnItemMenuConfirmEquip(0);
				if(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
					mnWindowSprClearListStub(menu->descriptionWin);
					mnItemMenuPlaceItemWindowBelowItemQuestion();
					mnItemMenuRefreshMember(menu->sideMember[1], 1);
					if(result == 0) {
						mnItemMenuDrawEquipPreview(menu->sideMember[1],
							menu->selectedSlots[1], 0, menu->sideMember[1]);
						mnItemMenuShowMessage(MsgEquippedIt, 15, 14);
						item = gmItemGetDirectStub(menu->selectedItems[0]);
						if((item->flg & ITEM_CURSED) != 0) {
							sySndPlayStub(SE_CURSE);
							mnWindowSprClearListStub(menu->descriptionWin);
							/* "You were cursed!" */
							mnItemMenuShowMessage(
								MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 14);
						}
					} else {
						mnItemMenuShowMessage(MsgTraded, ITEM_TRADED_X, 14);
					}
				}
			}
			syHeapRelease(tgtCopy);
			syHeapRelease(srcCopy);
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_MEMBER;
			break;

		/* Equip it. */
		case ITEM_STEP_EQUIP:
			result = gmInventoryEquipStub(
				menu->sideMember[0], menu->selectedSlots[0]);
			if(result == INVENTORY_EQUIP_REFUSED) {
				state = ITEM_STEP_ITEM;
				break;
			}
			/* The item it would replace is cursed and will not come off. */
			if(result == INVENTORY_EQUIP_STUCK) {
				mnWindowSprClearListStub(menu->descriptionWin);
				mnItemMenuShowMessage(MsgCannotRemoveItem, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
				mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#endif
				state = ITEM_STEP_ITEM;
				break;
			}
			gmUnitCalcStub(menu->sideMember[0]);
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCnt = mnItemMenuCollectItems(
				gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			mnWindowSprClearListStub(menu->descriptionWin);
			mnItemMenuShowMessage(MsgEquippedIt, 15, 8);
			item = gmItemGetDirectStub(menu->selectedItems[0]);
			if((item->flg & ITEM_CURSED) != 0) {
				sySndPlayStub(SE_CURSE);
				mnWindowSprClearListStub(menu->descriptionWin);
				/* "You were cursed!" */
				mnItemMenuShowMessage(MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 8);
			}
			state = ITEM_STEP_ITEM;
			break;

		/* Unequip it. */
		case ITEM_STEP_REMOVE:
			gmUnitGetStub(menu->sideMember[0])
				->inventory[menu->selectedSlots[0]] &= ~INVENTORY_EQUIPPED;
			gmUnitCalcStub(menu->sideMember[0]);
			gmUnitRefreshPsynergyStub(menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			menu->itemCnt = mnItemMenuCollectItems(
				gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
			mnItemMenuDrawIcons(menu->items, 0);
			sySynch(1);
			/* Show the stats as they are without it. */
			menu->equipPreview = 1;
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			menu->equipPreview = 0;
			mnWindowSprClearListStub(menu->descriptionWin);
			mnItemMenuShowMessage(MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
			fdPartyClearUnusableShortcutsStub();
			state = ITEM_STEP_ITEM;
			break;

		/* Show its description. */
		case ITEM_STEP_DETAILS:
			menu->sideIcons[0]->motion = ICON_HIDDEN;
			mnItemMenuShowDetails(menu->selectedItems[0]);
			mnWindowSprRedrawSavedRectStub(menu->statWin);
			mnItemMenuDrawEquipPreview(menu->sideMember[0],
				menu->selectedSlots[0], 0, menu->sideMember[0]);
			menu->sideIcons[0]->motion = ICON_SHOWN;
			state = ITEM_STEP_COMMAND;
			break;

		/* Unused: not reached from any command. Chooses a quantity and keeps it
		   in heldCnt, which nothing reads. */
		case ITEM_STEP_QUANTITY:
			result = mnItemMenuSelectGiveQuantity(0, INVENTORY_STACK_MAX, 0);
			if(result != -1) {
				/* Unused: the quantity chosen, which nothing reads. */
				heldCnt = result;
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_IDLE:
			break;

		default:
			done = 1;
			break;
		}
	}

	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
		menuResult = -1;
	}
	return menuResult;
}

/* Uses the selected item from the party member in side 0 on the one in side
   1. */
Sint32 mnItemMenuUse(void)
{
	ITEM_MENU_STATE *menu;
	Sint32			 result;

	menu = MenuState;
	result = mnItemUse(
		menu->selectedSlots[0], menu->sideMember[0], menu->sideMember[1]);

	if(result == -1) {
		sySndPlayStub(SE_MENU_ERROR);
		mnWindowSprClearListStub(menu->descriptionWin);
		mnItemMenuShowMessage(menu->msgOfs + MsgItemUseResult, result, result);
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#endif
		menu->completionFlg = 1;
		return result;
	}

	mnItemPlayUseSnd(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	gmUnitCalcStub(menu->sideMember[0]);
	gmUnitCalcStub(menu->sideMember[1]);
	return 1;
}

/* Sets up the item menu's first screen: the cursor window with the party's
   figures, the info window along the bottom, and an 8 by 2 cursor grid. */
void mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 memberIdx;

	mnCreatePartyFigures(mnCursorCreate(menu), 2, 2, 8, 0);
	/* The four figures stand at screen y 30; the screens raise the party member
	   chosen to 26. */
	for(memberIdx = 0; memberIdx < 4; memberIdx++)
		menu->memberY[memberIdx] = MENU_MEMBER_Y;

	menu->buttonHelpWin = NULL;
	menu->statWin = NULL;
	menu->descriptionWin =
		mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
	menu->itemWin = NULL;
	menu->selectedColumn = 0;
	menu->selectedRow = 0;
	menu->columnCnt = 8;
	menu->rowCnt = 2;
}

/* Creates the 32 item icon sprites in window, off screen until placed: slots 0
   to 7 at MENU_ICON_HIDDEN_X, the rest 8 pixels further, all at
   MENU_ICON_HIDDEN_Y, each showing item graphic slot of the same number. */
void mnSpawnIconEntries(ITEM_MENU_STATE *menu, WINDOW *win)
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
				ICON_KIND_ITEM, firstSlot, win, MENU_ICON_HIDDEN_X, y);
		}
	}

	secondSlot = ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		secondIcons = &menu->entryIcons[ICON_GRID_ROW];
		for(; secondSlot < 2 * ICON_GRID_ROW; secondSlot++) {
			*secondIcons++ = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, secondSlot, win, 256, y);
		}
	}

	remainingSlot = 2 * ICON_GRID_ROW;
	{
		Sint32 y = MENU_ICON_HIDDEN_Y;

		remainingIcons = &menu->entryIcons[2 * ICON_GRID_ROW];
		for(; remainingSlot < MENU_ENTRY_ICONS; remainingSlot++) {
			*remainingIcons++ = mnWindowSprCreateFromResourceStub(
				ICON_KIND_ITEM, remainingSlot, win, 256, y);
		}
	}
}

/* Hides all the item icons. */
void mnItemMenuHideAllIcons(void)
{
	WINDOW_SPR **icons = ((ITEM_MENU_STATE *)MenuState)->entryIcons;
	Sint32		 slot;

	for(slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
		WINDOW_SPR *icon = *icons++;
		if(icon != 0) {
			icon->motion = ICON_HIDDEN;
		}
	}
}

#if LANGUAGE != LANGUAGE_JAPANESE
/* Hides the item icons in the first column of the five-wide item grid. */
void mnItemMenuHidePageIcons(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 slot = 0;
	Sint32			 hidden = ICON_HIDDEN;
	WINDOW_SPR	   **iconSlot = menu->entryIcons;

	do {
		WINDOW_SPR *icon = *iconSlot++;

		if(icon != NULL && slot % MENU_ITEM_GRID_COLUMNS == 0)
			icon->motion = hidden;
		slot++;
	} while(slot <= MENU_ENTRY_ICONS - 1);
}
#endif

/* Closes the item menu: releases the party figures, hides the icons and the
   status panel's portrait (the state's cursor), and closes every window the
   menu opened. */
void mnItemMenuClose(void)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR		*cursor;

	menu = MenuState;
	mnReleasePartyFigures();
	mnItemMenuHideAllIcons();
	sySynch(1);
	cursor = menu->cursor;
	cursor->motion = ICON_HIDDEN;
	mnWindowCloseIfOpen(&menu->mainWin, 1);
	mnWindowCloseIfOpen(&menu->itemWin, 1);
	mnWindowCloseIfOpen(&menu->questionWin, 1);
	mnWindowCloseIfOpen(&menu->statWin, 1);
	mnWindowCloseIfOpen(&menu->buttonHelpWin, 1);
	mnWindowCloseIfOpen(&menu->descriptionWin, 1);
	mnWindowCloseIfOpen(&menu->equipWin, 1);
	mnWindowCloseIfOpen(&menu->listWin, 1);
	mnWindowCloseIfOpen(&menu->unopenedWin, 1);
	mnWindowCloseIfOpen(&menu->modalWin, 1);
	mnWindowCloseIfOpen(&menu->secondUnopenedWin, 1);
}

/* Lets the player choose a party member in side (0 or 1), starting from the
   party member chosen last. */
Sint32 mnItemMenuSelectMember(Sint32 side)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 result = 0;
	Sint32			 memberIdx;
	UNIT			*unit;

	memberIdx = menu->sideIdx[side];
	mnWindowSprClearListStub(menu->descriptionWin);
	menu->sideCnt[side] = menu->partyCnt;
	if(memberIdx == -1) {
		menu->sideIdx[side] = result;
		memberIdx = 0;
	} else {
		mnCursorSlide(MENU_MEMBER_CURSOR_X(memberIdx), 16);
	}
	unit = gmUnitGetStub(menu->memberIds[memberIdx]);
	menu->itemCnt = mnItemMenuCollectItems(unit, menu->items, 0);
	result = mnItemMenuRunMemberSelection(menu->memberIds, menu->items);
	mnIconShow(menu->sideIcons[side]);
	sySynch(1);
	return result;
}

/* The party member row of the item menu. */
Sint32 mnItemMenuRunMemberSelection(Uint16 *memberIds, Uint16 *items)
{
	ITEM_MENU_STATE *menu;
	Sint32			 selection;
	Sint32			 memberCnt;
	Sint32			 result;
	UNIT			*unit;
	Uint8			 sortMode;
	Sint32			 showEquipment;
	Bool			 redraw;
	WINDOW			*win;
	Sint32			 memberIdx;

	menu = MenuState;
	selection = menu->sideIdx[0];
	memberCnt = menu->sideCnt[0];
	redraw = TRUE;
	result = 0;
	sortMode = 0;
	showEquipment = 0;
	unit = gmUnitGetStub(memberIds[selection]);
	if(mnWindowUpdateOrCreate(
		   &menu->itemWin, 13, 3, 17, 10, WINDOW_FLAG_INSTANT))
		mnSpawnIconEntries(menu, menu->itemWin);
	if(mnWindowUpdateOrCreate(
		   &menu->buttonHelpWin, 13, 13, 17, 4, WINDOW_FLAG_INSTANT)) {
		menu->selectedItemIcon = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->buttonHelpWin, 0, result);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
	}
	/* The help text: L + A sorts, R shows the equipment. */
#if LANGUAGE != LANGUAGE_JAPANESE
	mnTextDrawMessageAsTilesStub(
		MsgArrangeItemsHelp, menu->buttonHelpWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		MsgArrangeItemsHelp + 1, menu->buttonHelpWin, 0, 8);
#else
	mnTextDrawStringInWindowStub(
		ItemMenuArrangeKeysString, menu->buttonHelpWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		MsgArrangeItemsHelp, menu->buttonHelpWin, 32, 0);
	mnTextDrawStringInWindowStub(
		ItemMenuEquipmentKeyString, menu->buttonHelpWin, 16, 8);
	mnTextDrawMessageAsTilesStub(
		MsgArrangeItemsHelp + 1, menu->buttonHelpWin, 32, 8);
#endif
	menu->sideIcons[0]->motion = redraw; /* ICON_SHOWN */
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		selection = (selection + memberCnt) % memberCnt;
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		if(redraw) {
			sortMode = 0;
			redraw = FALSE;
			win = menu->statWin;
			unit = gmUnitGetStub(memberIds[selection]);
			if(showEquipment) {
				menu->itemCnt = mnItemMenuCollectItems(
					gmUnitGetStub(memberIds[selection]), menu->items, 0);
				mnItemMenuShowEquipment(memberIds[selection]);
				mnStatPanelDraw(
					win, memberIds[selection], 0, STATUS_PANEL_STATS);
			} else {
				mnItemMenuRefreshMember(memberIds[selection], 0);
				mnStatPanelDraw(
					win, memberIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			}
			/* Raise the chosen party member's figure. */
			for(memberIdx = 0; memberIdx < 4; memberIdx++)
				menu->memberY[memberIdx] = MENU_MEMBER_Y;
			menu->memberY[selection] = MENU_MEMBER_RAISED_Y;
		}
		sySynch(1);
		/* L + A sorts, cycling through the four sort orders. */
		if(PerPush & KEY_A) {
			if(PerData & KEY_L) {
				sortMode = (sortMode + LIST_SORT_COUNT) % LIST_SORT_COUNT;
				mnItemMenuSort(unit->inventory, sortMode);
				sortMode++;
				mnItemMenuRefreshMember(memberIds[selection], 0);
				sySndPlayStub(SE_MENU_CONFIRM);
			} else if(mnItemMenuCountItems(memberIds[selection])) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = memberIds[selection];
				break;
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerPush & KEY_R) {
			showEquipment = 1;
			redraw = TRUE;
		}
		if(!(PerData & KEY_R) && showEquipment == 1) {
			showEquipment = 0;
			redraw = TRUE;
		}
		if(PerRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection--;
			redraw = TRUE;
		}
		if(PerRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection++;
			redraw = TRUE;
		}
	}
	menu->sideIdx[0] = selection;
	menu->selectedMember = memberIds[selection];
	menu->sideMember[0] = memberIds[selection];
	return result;
}

/* Shows the equipment view of party member memberId's items in the item
   window. */
void mnItemMenuShowEquipment(Sint32 memberId)
{
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	mnHideEntryIcons();
	mnWindowSprRedrawSavedRectStub(menu->itemWin);
	mnItemMenuDrawEquipment(menu->itemWin, memberId, 0);
}

/* Chooses the party member to use or give the selected item (enum
   ITEM_TARGET_MODE) to. */
Sint32 mnItemMenuSelectTarget(Sint32 mode)
{

	ITEM_MENU_STATE *menu;
	WINDOW			*win;
	Sint32			 memberCnt;
	Sint32			 memberIdx;
	Bool			 redraw;
	Uint8			 result;
	Bool			 descriptionShown;
	Sint32			 heldCnt;
	WINDOW_SPR		*tgtMarker;

	menu = MenuState;
	win = menu->itemWin;
	memberIdx = menu->sideIdx[1];
	memberCnt = menu->partyCnt;
	redraw = TRUE;
	result = 0;
	descriptionShown = FALSE;
	mnWindowSetBounds(win, 13, 5, 17, 12);
	mnWindowSprRedrawSavedRectStub((void *)menu->itemWin);
	gmUnitGetStub(menu->memberIds[menu->sideIdx[0]]);
	sySetEvent(mnEquipmentMenuUpdateCompatibilityIndicators, EVENT_PASS_TICK);
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = FALSE;
			memberIdx = (memberIdx + memberCnt) % memberCnt;
			win = menu->itemWin;
			gmUnitGetStub(menu->memberIds[memberIdx]);
			/* The marker over the chosen party member, three tiles a party
			   member; no screen makes it, so it is NULL (mnCursorCreate). */
			tgtMarker = menu->sideIcons[1];
			tgtMarker->spr.oam.x = tgtMarker->x =
				((menu->mainWin->x + memberIdx * 3) << 3) - 2;
			/* Giving: the party member's items and how many of this one they
			   hold. */
			if(mode == ITEM_TARGET_GIVE) {
				mnItemMenuRefreshMember(menu->memberIds[memberIdx], 1);
				mnWindowDrawDividerLineStub(win, 0, 9, 16, 9);
				mnWindowClearInteriorTilesStub((void *)win, 0, 72, 120, 80);
				if(memberIdx != menu->sideIdx[0]) {
					heldCnt =
						mnItemMenuGetItemQuantity(menu->memberIds[memberIdx],
							menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
#if GIVE_TRADE_PROMPT_REPLACES_STOCK
					if(mnItemMenuCountItems(menu->memberIds[memberIdx]) ==
							INVENTORY_SLOTS &&
						heldCnt == 0)
						mnTextDrawMessageAsTilesStub(
							MsgTradeForWhat, win, 0, 72);
					else if(heldCnt != 0) {
						mnTextDrawNumInWindowStub(heldCnt, 2, win, 8, 72);
						mnTextDrawMessageAsTilesStub(
							MsgInStock, win, STOCK_LABEL_X, 72);
					} else {
						mnTextDrawMessageAsTilesStub(
							MsgNoneInStock, win, 16, 72);
					}
#else
					if(heldCnt != 0) {
						mnTextDrawNumInWindowStub(heldCnt, 2, win, 8, 72);
						mnTextDrawMessageAsTilesStub(
							MsgInStock, win, STOCK_LABEL_X, 72);
					} else {
						mnTextDrawMessageAsTilesStub(
							MsgNoneInStock, win, 16, 72);
					}
					if(mnItemMenuCountItems(menu->memberIds[memberIdx]) ==
							INVENTORY_SLOTS &&
						heldCnt == 0)
						mnTextDrawMessageAsTilesStub(
							MsgTradeForWhat, win, ITEM_TRADE_PROMPT_X, 72);
#endif
				}
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->memberIds[memberIdx]);
			}
			/* Using: the party member's status, and the item's name once. */
			if(mode == ITEM_TARGET_USE) {
				if(mnItemMenuIsStatRaisingItem(
					   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
					mnStatPanelDraw(menu->statWin, menu->memberIds[memberIdx],
						menu->selectedSlots[0], STATUS_PANEL_STATS);
				else
					mnStatPanelDraw(menu->statWin, menu->memberIds[memberIdx],
						menu->selectedSlots[0], STATUS_PANEL_EXPERIENCE);
				if(!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !descriptionShown) {
					MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWin);
					MENU_DRAW_DESCRIPTION(
						(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
							MsgItemPlainName,
						menu->descriptionWin, 0, 0);
					descriptionShown = TRUE;
				} else {
					gmFlagClearStub(FLAG_MENU_INFO_KEPT);
				}
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(memberIdx), 16);
		sySynch(1);
		if(PerPush & KEY_A) {
			if(mode == ITEM_TARGET_GIVE && memberIdx == menu->sideIdx[0]) {
				sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			sySndPlayStub(SE_MENU_CONFIRM);
			result = menu->memberIds[memberIdx];
			break;
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIdx--;
			redraw = TRUE;
		}
		if(PerRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
			memberIdx++;
		}
	}
	/* The marker, still NULL. */
	tgtMarker = menu->sideIcons[1];
	menu->sideIdx[1] = memberIdx;
	mnIconShow(tgtMarker);
	tgtMarker->motion = ICON_HIDDEN;
	mnEquipmentMenuStopCompatibilityIndicators();
	sySynch(1);
	menu->sideIdx[1] = memberIdx;
	menu->selectedMember = menu->memberIds[memberIdx];
	menu->sideMember[1] = menu->memberIds[memberIdx];
	return (Sint8)result;
}

/* Event while mnItemMenuSelectTarget chooses a party member to use or give the
   item to, and while the command grid's cursor is on Give: every 32 ticks, has
   each party figure nod (ANIM_NOD) if that party member can equip the item, or
   stand (ANIM_STAND) if not. */
void mnEquipmentMenuUpdateCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu;
	Sint8			 memberIdx;

	menu = MenuState;
	if((TickCount & 31) == 0 && menu->partyCnt != 0) {
		memberIdx = 0;
		do {
			if(gmItemCanUnitEquipDirectStub(menu->memberIds[memberIdx],
				   menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0) {
				SPR *partyFigure = menu->memberSprs[memberIdx];
				fdSprStartAnimationStub(partyFigure, ANIM_NOD);
			} else {
				SPR *partyFigure = menu->memberSprs[memberIdx];
				fdSprStartAnimationStub(partyFigure, ANIM_STAND);
			}
			memberIdx++;
		} while(memberIdx < menu->partyCnt);
	}
}

/* Ends mnEquipmentMenuUpdateCompatibilityIndicators: puts every party figure
   back to ANIM_STAND and removes the event. */
void mnEquipmentMenuStopCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu;
	Sint8			 memberIdx;

	menu = MenuState;
	if(menu->partyCnt != 0) {
		memberIdx = 0;
		do {
			SPR *partyFigure = menu->memberSprs[memberIdx];
			fdSprStartAnimationStub(partyFigure, ANIM_STAND);
			memberIdx++;
		} while(memberIdx < menu->partyCnt);
	}
	syCloseEvent(&mnEquipmentMenuUpdateCompatibilityIndicators);
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

	menu = MenuState;
	mnWindowSprRedrawSavedRectStub(menu->questionWin);
	mnTextDrawMessageAsTilesStub(msg, menu->questionWin, 0, 0);
}

/* Hides the icon of each of the 32 list entries whose item is 0. Each of the 32
   icon slots must hold a sprite (mnSpawnIconEntries). */
void mnHideEmptyEntryIcons(const Uint16 *items)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 slot;

	for(slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
		if(items[slot] == 0) {
			mnIconShow(menu->entryIcons[slot]);
			menu->entryIcons[slot]->motion = ICON_HIDDEN;
		}
	}
}

/* Returns how many of party member memberId's 15 inventory slots hold an
   item. */
Sint32 mnItemMenuCountItems(Sint32 memberId)
{
	Sint32	cnt = 0;
	Uint16 *inventory = gmUnitGetStub(memberId)->inventory;
	Sint32	slot;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if((*inventory++ & INVENTORY_ITEM_ID_MASK) != 0)
			cnt++;
	}
	return cnt;
}

/* Returns how many of itemId the unit memberId holds: the quantity of its first
   stack, or 0. The Spanish and Italian editions add up every stack of it. */
Sint32 mnItemMenuGetItemQuantity(Sint32 memberId, Sint32 itemId)
{
	UNIT  *unit;
	Sint32 slotIdx;
	Sint32 quantity;

	quantity = 0;
	unit = gmUnitGetStub(memberId);
	for(slotIdx = 0; slotIdx < INVENTORY_SLOTS; slotIdx++) {
		if(unit->inventory[slotIdx] != 0 &&
			(unit->inventory[slotIdx] & INVENTORY_ITEM_ID_MASK) == itemId) {
#if ITEM_QUANTITY_ADDS_ALL_STACKS
			quantity += (unit->inventory[slotIdx] & INVENTORY_QUANTITY_MASK) >>
				INVENTORY_QUANTITY_SHIFT;
			quantity++;
#else
			quantity = (unit->inventory[slotIdx] & INVENTORY_QUANTITY_MASK) >>
				INVENTORY_QUANTITY_SHIFT;
			quantity++;
			break;
#endif
		}
	}
	return quantity;
}

/* Copies the unit's items into the list items (32 entries), skipping empty
   slots and clearing the rest. Returns how many there are. mode is not used. */
Sint32 mnItemMenuCollectItems(UNIT *unit, Uint16 *items, Sint32 mode)
{
	Sint32 itemCnt;
	Sint32 slotIdx;

	for(slotIdx = 0; slotIdx < MENU_ENTRY_ICONS; slotIdx++) {
		items[slotIdx] = 0;
	}
	itemCnt = 0;
	for(slotIdx = 0; slotIdx < INVENTORY_SLOTS; slotIdx++) {
		items[slotIdx] = 0;
		if(unit->inventory[slotIdx] != 0) {
			items[itemCnt] = unit->inventory[slotIdx];
			itemCnt++;
		}
	}
	return itemCnt;
}

/* Loads each listed item's icon into its item icon sprite, the first 15 entries
   of inventoryEntries, and hides the icons of empty entries. iconStyle 0 draws
   the list's icons (ICON_KIND_ITEM); any other iconStyle the equipment view's
   (ICON_KIND_EQUIPMENT_ITEM). */
void mnItemMenuDrawIcons(Uint16 *inventoryEntries, Sint32 iconStyle)
{
	Sint32		 i;
	Uint16		*entries;
	WINDOW_SPR **icons;
	Sint32		 itemId;

	icons = ((ITEM_MENU_STATE *)MenuState)->entryIcons;
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

/* Shows party member memberId's items in the item window, laid out for layout
   mode (see mnItemMenuPlaceIcons), or "no items" when there are none. */
void mnItemMenuRefreshMember(Sint32 memberId, Sint32 mode)
{
	ITEM_MENU_STATE *menu;
	UNIT			*member;
	Uint16			*items;

	menu = MenuState;
	member = gmUnitGetStub(memberId);
	items = menu->items;
	menu->itemCnt = mnItemMenuCollectItems(member, items, 0);
	mnWindowSprRedrawSavedRectStub(menu->itemWin);
	mnItemMenuPlaceIcons(mode);
	mnItemMenuDrawIcons(items, 0);
	if(mnItemMenuCountItems(memberId) == 0)
		mnTextDrawMessageAsTilesStub(MsgItemMenuEmpty, menu->itemWin, 8, 24);
}

UNUSED void mnItemMenuNoOperation(void)
{
}

/* Shows in the status panel what the item in member's inventory slot would mean
   to member target: for a usable item, target's status; for equipment, target's
   stats with it equipped; for a Psynergy item, the Psynergy it teaches. */
void mnItemMenuDrawEquipPreview(
	Sint32 member, Sint32 slot, Sint32 mode, Sint32 tgt)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 panelStyle = 0;
	UNIT			*unit = gmUnitGetStub(member);
	Sint32			 inventoryEntry = unit->inventory[slot];
	Sint32			 previewSlot;

	/* Mode 1 keeps the panel's name and statuses and redraws only below
	   them. */
	if(mode == 1)
		panelStyle = STATUS_PANEL_LOWER_ONLY;
	switch(gmItemGetDirectStub(inventoryEntry & INVENTORY_ITEM_ID_MASK)->type) {
	case ITEM_TYPE_NONE: /* usable items */
		mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
		break;
	case ITEM_TYPE_WEAPON:
	case ITEM_TYPE_ARMOR:
	case ITEM_TYPE_SHIELD:
	case ITEM_TYPE_HELM:
	case ITEM_TYPE_BOOTS:
	case ITEM_TYPE_UNUSED:
	case ITEM_TYPE_RING:
	case ITEM_TYPE_SHIRT:
		if(member == tgt) {
			panelStyle |= STATUS_PANEL_EQUIP;
			mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
		} else {
			Sint32 size;

			unit = gmUnitGetStub(tgt);
			size = BATTLE_UNIT_SIZE;
			member = (Sint32)syHeapAlloc(size);
			syWordsCopy((void *)member, unit, size);
			/* With room made, add the item unequipped and preview it in its new
			   slot; otherwise the panel shows target's status without a
			   preview. */
			previewSlot = mnInventoryRemoveFirstUnflagged(tgt);
			if(previewSlot != 0) {
				inventoryEntry &= ~INVENTORY_EQUIPPED;
				previewSlot = gmInventoryAddItemStub(tgt, inventoryEntry);
				if(previewSlot != -1) {
					panelStyle |= STATUS_PANEL_EQUIP;
					mnStatPanelDraw(
						menu->statWin, tgt, previewSlot, panelStyle);
				} else {
					mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
				}
			} else {
				mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
			}
			syWordsCopy(unit, (void *)member, size);
			syHeapRelease((void *)member);
		}
		break;
	case ITEM_TYPE_PSYNERGY:
		if(tgt == member) {
			panelStyle |= STATUS_PANEL_LEARN;
			mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
		} else {
			Sint32 size;
			void  *buf;

			unit = gmUnitGetStub(tgt);
			size = BATTLE_UNIT_SIZE;
			buf = syHeapAlloc(size);
			syWordsCopy(buf, unit, size);
			previewSlot = mnInventoryRemoveFirstUnflagged(tgt);
			if(previewSlot != 0) {
				previewSlot = gmInventoryAddItemStub(tgt, inventoryEntry);
				if(previewSlot != -1) {
					panelStyle |= STATUS_PANEL_LEARN;
					mnStatPanelDraw(
						menu->statWin, tgt, previewSlot, panelStyle);
				} else {
					mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
				}
			} else {
				mnStatPanelDraw(menu->statWin, tgt, slot, panelStyle);
			}
			syWordsCopy(unit, buf, size);
			syHeapRelease(buf);
		}
		break;
	}
}

/* Makes room in memberId's inventory: if no slot is free, removes the whole
   stack in the first slot that is not equipped. */
Sint32 mnInventoryRemoveFirstUnflagged(Sint32 memberId)
{
	UNIT  *unit = gmUnitGetStub(memberId);
	Sint32 result = 0;
	Sint32 slotIdx;

	for(slotIdx = 0; slotIdx < INVENTORY_SLOTS; slotIdx++) {
		if(unit->inventory[slotIdx] == 0) {
			result = 1;
			break;
		}
		if((unit->inventory[slotIdx] & INVENTORY_EQUIPPED) == 0) {
			Sint32 quantity =
				unit->inventory[slotIdx] >> INVENTORY_QUANTITY_SHIFT;
			Sint32 cnt = quantity + 1;
			Sint32 removedCnt;

			if(quantity == 0)
				cnt = 1;
			for(removedCnt = 0; removedCnt < cnt; removedCnt++)
				result = gmInventoryRemoveStub(memberId, slotIdx);
			if(result != INVENTORY_REMOVE_LAST)
				return 0;
			result = 1;
			break;
		}
	}
	return result;
}

/* The item command cursor on the command grid's rows, 8 pixels apart from y 30,
   and in Japanese on its columns, 40 pixels apart from x 86. */
#define ITEM_COMMAND_CURSOR_Y(row) ((row) * 8 + 30)
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_COMMAND_CURSOR_X(column) ((column) * 40 + 86)
#endif

#if LANGUAGE != LANGUAGE_JAPANESE

/* Returns the x of the item command cursor on column and row of the command
   grid (three commands to a row), or 0 when column or row is outside 0 to 2. */
Sint32 mnItemMenuCommandCursorX(Sint32 column, Sint32 row)
{
	if(column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenuCommandColumnXTable[row * 3 + column];
}

/* Returns the y of the item command cursor: its top row, or the row below for
   any other row. */
Sint32 mnItemMenuCommandCursorY(Sint32 column, Sint32 row)
{
	Sint32 y;

	(void)column;
	y = ITEM_COMMAND_CURSOR_Y(0);
	if(row != 0) {
		y = ITEM_COMMAND_CURSOR_Y(1);
	}
	return y;
}
#endif

/* Each item command's state in the grid. */
#define ITEM_COMMAND_STATE_DISABLED	 (-1)
#define ITEM_COMMAND_STATE_AVAILABLE 1

/* The item command grid: Use, Equip and Details on the top row, Give, Remove
   and Drop below (grid index = row * 3 + column, as ITEM_MENU_COMMAND counts
   them). */
Sint32 mnItemMenuChooseCommand(void)
{
	ITEM_MENU_STATE *menu;
	Sint8			 commandStates[6];
	Sint32			 column;
	Sint32			 lastCommand;
	Sint32			 row;
	Sint32			 commandIdx;
	Bool			 redraw;
#if LANGUAGE != LANGUAGE_JAPANESE
	Sint32 cursorX;
	Sint32 cursorY;
#endif

	commandIdx = 0;
	column = 0;
	row = 0;
	menu = MenuState;
	redraw = TRUE;
	mnItemMenuBuildCommands(commandStates);
	/* Draw the item's name and the command grid, unless they are up already. */
	if(menu->flg != 1) {
		WINDOW *msgWin;

		mnItemMenuHideAllIcons();
		mnWindowSprRedrawSavedRectStub((void *)menu->listWin);
		msgWin = menu->questionWin;
		mnItemMenuFitMessageWindowToItemChoices();
		mnWindowSprRedrawSavedRectStub((void *)msgWin);
		mnWindowDrawDividerLineStub(msgWin, 0, 3, 16, 3);
		mnItemMenuDrawItemHead();
		mnItemMenuDrawCommands(commandStates, msgWin);
		MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWin);
		MENU_DRAW_DESCRIPTION(
			(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
				MsgItemPlainName,
			menu->descriptionWin, 0, 0);
	}
	menu->flg = 0;

	/* Start on the command chosen last, or on the first one available. listMode
	   is -1 until a command is chosen; the later tests win, so the order of
	   preference is Use, Remove, Equip, Give, then Details. */
	lastCommand = (Sint8)menu->listMode;
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
		commandIdx = row * 3 + column;
	}
#if LANGUAGE != LANGUAGE_JAPANESE
	cursorX = mnItemMenuCommandCursorX(column, row);
	cursorY = mnItemMenuCommandCursorY(column, row);
	mnCursorSlide(cursorX, cursorY);
#else
	mnCursorSlide(ITEM_COMMAND_CURSOR_X(column), ITEM_COMMAND_CURSOR_Y(row));
#endif

	for(;;) {
		if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
			break;
		if(redraw != 0) {
			redraw = FALSE;
			column = (column + 3) % 3;
			row = (row + 2) % 2;
			commandIdx = row * 3 + column;
			mnEquipmentMenuStopCompatibilityIndicators();
			/* Preview the item in the status panel for the command under the
			   cursor:
			   the bottom row (Give, Remove, Drop) without it, Equip and Details
			   with it, Use the party member's status. */
			if(commandIdx > 2) {
				menu->equipPreview = 1;
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
				if(commandIdx == ITEM_COMMAND_GIVE) {
					sySetEvent(&mnEquipmentMenuUpdateCompatibilityIndicators,
						EVENT_PASS_TICK);
				}
			} else if(commandIdx != 0) {
				menu->equipPreview = 0;
				mnItemMenuDrawEquipPreview(menu->sideMember[0],
					menu->selectedSlots[0], 0, menu->sideMember[0]);
			} else {
				mnStatPanelDraw(menu->statWin, menu->sideMember[0], 0,
					STATUS_PANEL_EXPERIENCE);
			}
		}
#if LANGUAGE != LANGUAGE_JAPANESE
		cursorX = mnItemMenuCommandCursorX(column, row);
		cursorY = mnItemMenuCommandCursorY(column, row);
		mnCursorPos(cursorX, cursorY);
#else
		mnCursorPos(ITEM_COMMAND_CURSOR_X(column), ITEM_COMMAND_CURSOR_Y(row));
#endif
		sySynch(1);

		if((PerPush & KEY_A) != 0) {
			if(commandStates[commandIdx] == ITEM_COMMAND_STATE_DISABLED) {
				sySndPlayStub(SE_MENU_ERROR);
			} else {
				switch(commandIdx) {
				/* Use */
				case ITEM_COMMAND_USE:
					sySndPlayStub(SE_NOTE_MID);
					break;
				/* Equip */
				case ITEM_COMMAND_EQUIP:
					sySndPlayStub(SE_NOTE_HIGH);
					break;
				case ITEM_COMMAND_DETAILS:
				case ITEM_COMMAND_GIVE:
				case ITEM_COMMAND_DROP:
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				/* Remove */
				case ITEM_COMMAND_REMOVE:
					sySndPlayStub(SE_ITEM_REMOVE);
					break;
				default:
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				}
				menu->listMode = commandIdx;
				break;
			}
		}
		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			commandIdx = -1;
			menu->listMode = commandIdx;
			break;
		}

		if((PerRepeat & KEY_UP) != 0) {
			row -= 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerRepeat & KEY_DOWN) != 0) {
			row += 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerRepeat & KEY_RIGHT) != 0) {
			column += 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		} else if((PerRepeat & KEY_LEFT) != 0) {
			column -= 1;
			redraw = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}

	menu->equipPreview = 0;
	mnEquipmentMenuStopCompatibilityIndicators();
	return commandIdx;
}

/* Fills commandStates (indexed by ITEM_MENU_COMMAND) with
   ITEM_COMMAND_STATE_AVAILABLE or _DISABLED for the selected item: Use for
   items mnItemClassifyUseMode accepts and are not broken, and for field items,
   Equip for equipment the party member can wear and is not wearing, Remove for
   worn equipment that is not cursed, Give with someone to give to, Drop unless
   the item is indispensable, and neither for cursed equipment being worn. */
void mnItemMenuBuildCommands(Sint8 *commandStates)
{
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;

	menu = MenuState;
	item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0]);

	if(item->type == ITEM_TYPE_NONE) { /* usable items */
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

#define TEXT_COLOR_DISABLED \
	TEXT_COLOR_GREY /* the grey of a command that refuses A */

/* Writes the six command names into window, the disabled ones in grey. */
void mnItemMenuDrawCommands(const Sint8 *commandStates, WINDOW *win)
{
	Sint32 useState;
	Uint32 firstCommandMsg;

	mnTextSetColStub(TEXT_COLOR_NORMAL);
	useState = commandStates[ITEM_COMMAND_USE];
	if(useState == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);

	firstCommandMsg = MsgItemCommandUse;
	/* "Use" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg, win, 0, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Equip" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMsg + 1, win, ITEM_COMMAND_TEXT_X, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Give" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 2, win, 0, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DROP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Drop" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 3, win, 80, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_DETAILS] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Details" */
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 4, win, 80, 24);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColStub(TEXT_COLOR_DISABLED);
	/* "Remove" */
	mnTextDrawMessageAsTilesStub(
		firstCommandMsg + 5, win, ITEM_COMMAND_TEXT_X, 32);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
}

/* Says how memberId can use inventoryEntry from the item menu (enum
   ITEM_USE_MODE). */
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
			gmAbilityGetDirectStub(item->abilityId & PSYNERGY_ABILITY_MASK);

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
				Uint8  flg = action->flg;

				if((flg & ABILITY_USABLE_IN_FIELD) != 0) {
					Uint8 range = action->range;
					result = (range == ABILITY_RANGE_ALL)
						? ITEM_USE_MODE_WHOLE_PARTY
						: ITEM_USE_MODE_CHOOSE_MEMBER;
				} else {
					result = (flg & mask) ? ITEM_USE_MODE_UNUSABLE
										  : ITEM_USE_MODE_FROM_FIELD;
				}
			}
		}
	}

	return result;
}

/* After the selected item is used: an item that may break (ITEM_USE_MAY_BREAK)
   breaks one time in eight, with the break sound and "It broke!" in the info
   window. Called only after a successful use on a chosen party member. */
void mnItemMenuTryBreak(void)
{
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	/* One time in eight: 8192 of syRandom's 65536 values. */
	if((gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0])
			   ->useType == ITEM_USE_MAY_BREAK) &&
		(syRandom() < 8192U)) {
		gmInventoryBreakStub(menu->sideMember[0], menu->selectedSlots[0]);
		sySndPlayStub(SE_ITEM_BREAK);
		mnItemMenuShowMessage(MsgItemBroke, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub(menu->descriptionWin);
#endif
	}
}

/* Opens the details window (top left, 13 by 10 tiles) and describes the item in
   side itemIdx there. Returns 1. */
Sint32 mnItemMenuOpenDetail(Sint32 itemIdx)
{
	ITEM_MENU_STATE *menu = MenuState;

	mnWindowUpdateOrCreate(&menu->equipWin, 0, 0, 13, 10, WINDOW_FLAG_INSTANT);
	mnPaletteLoadIconsIntoHighlight();

	if(menu->selectedItems[itemIdx] != 0)
		mnItemMenuDrawItemDetails(menu->equipWin, menu->selectedItems[itemIdx]);

	return 1;
}

/* The item menu's Details screen: describes inventoryEntry in a window over the
   top half (13 tiles wide in Japanese) and waits for A or B. Up and Down step a
   page from 0 to 4 that nothing draws; returns that page, or -1 for B. */
Sint32 mnItemMenuShowDetails(Sint32 inventoryEntry)
{
#if LANGUAGE != LANGUAGE_JAPANESE
	Bool			 changed = TRUE;
	ITEM_MENU_STATE *menu = MenuState;
	WINDOW_SPR		*itemIcon = menu->selectedItemIcon;
#else
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 changed = TRUE;
#endif
	WINDOW *win;
	Sint32	page = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
	itemIcon->motion = ICON_HIDDEN;
#endif
	win = mnWindowCreateStub(
		0, 0, ITEM_QUANTITY_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
	/* Hide the party figures (without their placement event they are not drawn)
	   and the status panel's portrait (the state's cursor), which the window
	   covers, meanwhile. */
	syCloseEvent(mnPlacePartyFigures);

	{
		WINDOW_SPR *cursor = menu->cursor;
		cursor->motion = ICON_HIDDEN;
	}
	mnPaletteLoadIconsIntoHighlight();
	sySynch(1);

	for(;;) {
		if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
			break;

		if(changed != 0) {
			changed = FALSE;
			page = (page + 5) % 5;
			mnItemMenuDrawItemDetails(win, inventoryEntry);
		}

		if(PerPush & KEY_A)
			break;
		if(PerPush & KEY_B) {
			page = -1;
			break;
		}

		if(PerRepeat & KEY_UP) {
			page -= 1;
			changed = TRUE;
		}
		if(PerRepeat & KEY_DOWN) {
			page += 1;
			changed = TRUE;
		}
		sySynch(1);
	}

	/* Close the window and bring the menu back. */
	mnWindowSprRedrawSavedRectStub(win);
	sySynch(1);
	mnWindowCloseStub((void *)win, 1);
	mnWindowSprRedrawSavedRectStub(menu->mainWin);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	/* As mnCreatePartyFigures schedules it. */
	{
		Sint32 priority = EVENT_PASS_TICK;

		sySetEvent(mnPlacePartyFigures, priority);
	}

	/* The Japanese window, 13 tiles wide, leaves the command grid at tile 13
	   up, so flags 1 tells mnItemMenuChooseCommand not to draw it again; the
	   international one, 30 wide, covers it, and the grid's tick is
	   redrawn. */
	{
		WINDOW_SPR *cursor = menu->cursor;
#if LANGUAGE != LANGUAGE_JAPANESE
		cursor->motion = ICON_SHOWN;
#else
		cursor->motion = ICON_SHOWN;
		menu->flg = 1;
#endif
	}
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowDrawBorderStub(13, 0, 17, 10);
#endif

	return page;
}

/* Writes the description of the inventoryEntry into window, one line a row: for
   equipment its Attack and Defense bonuses, then each of its effects (flat
   bonuses, rates as multipliers, elemental Power and Resist with the element's
   icon, and the effects that are only text), whether it is cursed or bestows
   Psynergy, whether it is used up or may break, and for a stack how many there
   are. */
void mnItemMenuDrawItemDetails(WINDOW *win, Sint32 inventoryEntry)
{
	ITEM_DEFINITION *def;
	Sint32			 i;
	Sint32			 hasEffects;
	Sint32			 amount;
	Sint32			 bestowsPsynergy;
	Sint8			 row;

	row = 0;
	bestowsPsynergy = 0;
	def = gmItemGetDirectStub(inventoryEntry & INVENTORY_ITEM_ID_MASK);
	/* Equipment: a heading, then its Attack and Defense bonuses. */
	if(def->type != ITEM_TYPE_NONE) {
		hasEffects = 0;
		if(def->attack == 0 && def->defense == 0) {
			for(i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
				if(def->effs[i].kind != 0 || def->useType == ITEM_USE_TEACHES) {
					hasEffects = 1;
					break;
				}
			}
		} else {
			hasEffects = 1;
		}
		if(hasEffects == 1) {
			mnTextDrawMessageAsTilesStub(MsgEquipEffectHeading, win, 16, 0);
			row = 1;
		}
		if(def->attack != 0) {
			mnTextDrawMessageAsTilesStub(MsgStatLabel, win, 0, row * 8);
			amount = def->attack;
			mnItemMenuDrawStat(amount, 3, win, 64, row * 8);
			row++;
		}
		if(def->defense != 0) {
			mnTextDrawMessageAsTilesStub(MsgDefenseLabel, win, 0, row * 8);
			amount = def->defense;
			mnItemMenuDrawStat(amount, 3, win, 64, row * 8);
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
				def->effs[i].kind + MsgEquipEffectName, win, 0, row * 8);
			mnItemMenuDrawStat(amount, 3, win, 64, row * 8);
			break;
		/* Power and Resist in each of the four elements, after the element's
		   icon. */
		case EQUIP_EFFECT_ADD_VENUS_POWER ... EQUIP_EFFECT_ADD_JUPITER_RESIST:
			mnWindowSetTilemapEntryStub(win,
				(Uint8)((def->effs[i].kind - EQUIP_EFFECT_ADD_VENUS_POWER) %
					ELEMENT_COUNT) +
					1,
				0, row, TILEMAP_ENTRY_HIGHLIGHT);
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + MsgEquipEffectName, win, 8, row * 8);
			mnItemMenuDrawStat(amount, 3, win, 64, row * 8);
			break;
		/* Rates, in tenths: shown as a multiplier from 0.0 to 1.9. */
		case EQUIP_EFFECT_SCALE_HP:
		case EQUIP_EFFECT_SCALE_HP_REGEN:
		case EQUIP_EFFECT_SCALE_PP:
		case EQUIP_EFFECT_SCALE_PP_REGEN:
		case EQUIP_EFFECT_SCALE_ATTACK:
		case EQUIP_EFFECT_SCALE_DEFENSE:
		case EQUIP_EFFECT_SCALE_AGILITY:
		case EQUIP_EFFECT_SCALE_LUCK:
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + MsgEquipEffectName, win, 0, row * 8);
#if LANGUAGE != LANGUAGE_JAPANESE
			mnTextDrawStringInWindowStub(MenuTimesString, win, 64, row * 8);
			if(amount >= STAT_SCALE_ONE) {
				mnTextDrawNumInWindowStub(1, 1, win, 72, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, win, 80, row * 8);
				mnTextDrawNumInWindowStub(
					amount - STAT_SCALE_ONE, 1, win, 88, row * 8);
			} else {
				mnTextDrawNumInWindowStub(0, 1, win, 72, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, win, 80, row * 8);
				mnTextDrawNumInWindowStub(amount, 1, win, 88, row * 8);
			}
#else
			if(amount >= STAT_SCALE_ONE) {
				mnTextDrawNumInWindowStub(1, 1, win, 48, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, win, 56, row * 8);
				mnTextDrawNumInWindowStub(
					amount - STAT_SCALE_ONE, 1, win, 64, row * 8);
			} else {
				mnTextDrawNumInWindowStub(0, 1, win, 48, row * 8);
				mnTextDrawStringInWindowStub(MenuPointString, win, 56, row * 8);
				mnTextDrawNumInWindowStub(amount, 1, win, 64, row * 8);
			}
			mnTextDrawMessageAsTilesStub(MsgEffectRateSuffix, win, 72, row * 8);
#endif
			break;
		case EQUIP_EFFECT_ADD_UNLEASH_CHANCE: /* "Critical Hits increase" */
		case EQUIP_EFFECT_CURSE_REMOVABLE:
		case EQUIP_EFFECT_LURE: /* "Boosts monster encounters" */
			mnTextDrawMessageAsTilesStub(
				def->effs[i].kind + MsgEquipEffectName, win, 0, row * 8);
			break;
		}
		row++;
	}
	/* Curses, bestowed Psynergy and how the item is used up. */
	if(def->flg & ITEM_CURSED) {
		mnTextDrawMessageAsTilesStub(MsgItemCursed, win, 0, row * 8);
		row++;
	}
	if(def->useType == ITEM_USE_TEACHES) {
		mnTextDrawMessageAsTilesStub(MsgBestowsPsynergy, win, 0, row * 8);
		bestowsPsynergy = 1;
		row++;
	}
	/* "Number of Uses" and what using it costs, except for items that cost
	   nothing and Hermes' Water (ITEM_USE_EMPTIES); a Psynergy item has no
	   heading and no line. */
	if(def->useType != ITEM_USE_EMPTIES && def->useType != ITEM_USE_FREE) {
		if(!bestowsPsynergy) {
			mnTextDrawMessageAsTilesStub(MsgUsesHeading, win, 16, row * 8);
			row++;
		}
		switch(def->useType) {
		case ITEM_USE_FREE:
			break;
		case ITEM_USE_CONSUMED:
			mnTextDrawMessageAsTilesStub(MsgSingleUse, win, 0, row * 8);
			row++;
			break;
		case ITEM_USE_MAY_BREAK: {
			Sint32 m;

			/* Two lines each: "It's broken, so" / "it can't be used", or "It
			   might break" / "if used in battle". */
			if(inventoryEntry & INVENTORY_BROKEN) {
				m = MsgBrokenNotice;
				mnTextDrawMessageAsTilesStub(m, win, 0, row * 8);
				row++;
				m++;
				mnTextDrawMessageAsTilesStub(m, win, 0, row * 8);
				row++;
			} else {
				m = MsgMightBreak;
				mnTextDrawMessageAsTilesStub(m, win, 0, row * 8);
				row++;
				m++;
				mnTextDrawMessageAsTilesStub(m, win, 0, row * 8);
				row++;
			}
			break;
		}
		}
	}
	/* How many there are in a stack. */
	if(def->flg & ITEM_STACKABLE) {
		if(row != 0)
			row++;
		i = MsgNumberHeading;
		mnTextDrawMessageAsTilesStub(i, win, 16, row * 8);
		row++;
		amount = (inventoryEntry & INVENTORY_QUANTITY_MASK) /
			INVENTORY_QUANTITY_STEP;
		/* The quantity field holds the count less one. */
#if LANGUAGE != LANGUAGE_JAPANESE
		mnMessageArgumentPushStub(amount + 1, MESSAGE_ARGUMENT_NUMBER);
		mnTextDrawMessageAsTilesStub(i + 1, win, 0, row * 8);
#else
		mnTextDrawNumInWindowStub(amount + 1, 2, win, 0, row * 8);
		mnTextDrawMessageAsTilesStub(i + 1, win, 16, row * 8);
#endif
		row++;
	}
	/* Nothing written yet: rare, important, or nothing known. */
	hasEffects = 0;
	if(row == 0) {
		if(def->flg & ITEM_RARE) {
			mnTextDrawMessageAsTilesStub(MsgRareItem, win, 0, 0);
			hasEffects = 1;
		}
		if(!hasEffects) {
			if(def->flg & ITEM_INDISPENSABLE) {
				mnTextDrawMessageAsTilesStub(MsgImportantItem, win, 0, 0);
				hasEffects = 1;
			}
			if(!hasEffects)
				mnTextDrawMessageAsTilesStub(MsgDetailsUnknown, win, 0, 0);
		}
	}
}

/* Writes a signed stat change at (x, y) of window: the number, with a plus or
   minus sign just before its first digit. */
void mnItemMenuDrawStat(
	Sint32 statChange, Sint32 unused, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32 digits;
	Sint32 magnitude;

	mnTextDrawNumInWindowStub(statChange, 3, win, x, y);
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
			&MenuPlusSignString, win, x - digits * 8 + 16, y);
	} else {
		mnTextDrawStringInWindowStub(
			&MenuMinusSignString, win, x - digits * 8 + 16, y);
	}
}

/* Moves the item window to row 5 of the right-hand side, below a question
   headed by the item. */
void mnItemMenuPlaceItemWindowBelowItemQuestion(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->itemWin, 13, 5, 17, 10);
}

/* Moves the item window to row 3 of the right-hand side, below a one-line
   question. */
void mnItemMenuPlaceItemWindowBelowQuestion(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->itemWin, 13, 3, 17, 10);
}

/* Makes the message window, along the top right, 5 rows tall: the item's name
   and a question below it. */
void mnItemMenuFitMessageWindowToItemQuestion(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWin, 13, 0, 17, 5);
}

/* Makes the message window 6 rows tall, for the longer questions outside
   Japanese. */
void mnItemMenuFitMessageWindowToLongItemQuestion(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWin, 13, 0, 17, 6);
}

/* Makes the message window 7 rows tall: the item's name above the command grid
   or the quantity dial. */
void mnItemMenuFitMessageWindowToItemChoices(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWin, 13, 0, 17, 7);
}

/* Makes the message window 3 rows tall, for a one-line question. */
void mnItemMenuFitMessageWindowToQuestion(void)
{
	mnWindowSetBounds(
		(WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWin, 13, 0, 17, 3);
}

/* Asks how many of the selected item stack to give or drop: Left and Right
   choose a number from 1 to quantityRange (shown as baseQuantity + 1 up to
   baseQuantity + quantityRange on a dial of digit sprites), with the party
   member's remaining count and, when giving (dropMode 0), the receiver's new
   count beside their names. */
Sint32 mnItemMenuSelectGiveQuantity(
	Sint32 baseQuantity, Sint32 quantityRange, Sint32 dropMode)
{
	Sint32			 quantity = baseQuantity;
	Sint32			 block;
	Sint32			 changed;
	Sint32			 receiverCnt;
	Sint32			 memberCnt;
	ITEM_MENU_STATE *menu = MenuState;
	/* 1 KB of HEAP_SLOT_TILE_SCRATCH to draw the dial's digits in, released
	   before returning. */
	void	   *tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	WINDOW	   *win;
	WINDOW_SPR *rightDial;

	changed = 1;
	receiverCnt = 0;
	win = menu->questionWin;
	mnItemMenuFitMessageWindowToItemChoices();
	mnWindowSprRedrawSavedRectStub(win);
	/* How many each party member holds now. */
	if(dropMode == 0)
		receiverCnt = mnItemMenuGetItemQuantity(menu->sideMember[1],
			menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	memberCnt = mnItemMenuGetItemQuantity(
		menu->sideMember[0], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	/* Two dial sprites, left and right of the number. */
	block = syVRAMCacheFindFreeEntry();
	/* VRAM_CACHE_ENTRY_COUNT: no free VRAM block, and no dial. */
	if(block != VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(block, 256, 0);
		mnWindowSprCreateStub(block, OUTPUT_SIZE_32X8, win, 48, 32);
		rightDial = mnWindowSprCreateStub(block, OUTPUT_SIZE_32X8, win, 80, 32);
		rightDial->spr.oam.idx += 4;
		mnCursorSlide(ITEM_GIVE_CURSOR_X, 40);
		while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
			if(changed) {
				changed = 0;
				quantity = (quantityRange + quantity) % quantityRange;
				mnWindowSprRedrawSavedRectStub(win);
				mnTextDrawMessageAsTilesStub(
					MsgGiveHowMany, win, ITEM_GIVE_ASK_X, ITEM_GIVE_ASK_Y);
				syDMAXCopy(
					Menu_QuantityTiles, tiles, DMA_COPY_WORDS(64), REG_DMA3);
				mnQuantityBarFillStub(
					INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH, tiles);
				mnQuantityBarFillStub(quantityRange + baseQuantity,
					QUANTITY_BAR_CHOOSABLE, tiles);
				mnQuantityBarFillStub(
					baseQuantity + quantity + 1, QUANTITY_BAR_CHOSEN, tiles);
				mnQuantityBarFillStub(baseQuantity, QUANTITY_BAR_HELD, tiles);
				syVRAMCacheLoad(block, 256, tiles);
				mnTextDrawNumInWindowStub(quantity + 1, 2, win, 32, 32);
				mnTextDrawMessageAsTilesStub(
					(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
						MsgItemName,
					win, 16, 8);
				mnTextDrawNumInWindowStub(
					memberCnt - quantity - 1, 2, win, 16, 24);
#if LANGUAGE != LANGUAGE_JAPANESE
				if(dropMode == 0)
					mnTextDrawNumInWindowStub(
						receiverCnt + quantity + 1, 2, win, 80, 24);
#else
				mnTextDrawMessageAsTilesStub(MsgItemCounter, win, 32, 24);
				if(dropMode == 0) {
					mnTextDrawNumInWindowStub(
						receiverCnt + quantity + 1, 2, win, 80, 24);
					mnTextDrawMessageAsTilesStub(MsgItemCounter, win, 96, 24);
				}
#endif
				mnTextDrawStringAtOffsetStub(
					gmUnitGetStub(menu->sideMember[0])->name, win, 16, 16);
				if(dropMode == 0)
					mnTextDrawStringAtOffsetStub(
						gmUnitGetStub(menu->sideMember[1])->name, win, 80, 16);
			}
			/* A confirms, B backs out, Left and Right change the number. */
			if(PerPush & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if(PerPush & KEY_B) {
				quantity = -1;
				sySndPlayStub(SE_MENU_CANCEL);
				break;
			}
			mnCursorPos(ITEM_GIVE_CURSOR_X, 40);
			if(PerRepeat & KEY_LEFT) {
				quantity--;
				changed = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if(PerRepeat & KEY_RIGHT) {
				quantity++;
				changed = 1;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	mnWindowSprRedrawSavedRectStub(win);
	mnWindowSprClearListStub((void *)win);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	menu->selectedItemIcon->motion = ICON_HIDDEN;
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
		quantity = -1;
	return quantity;
}

/* Shows the selected item at the top of the message window: its icon, its party
   member's name and its name. */
void mnItemMenuDrawItemHead(void)
{
	ITEM_MENU_STATE *menu = MenuState;

	mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM, menu->selectedItems[0],
		(Uint8)menu->selectedItemIcon->idx, 0);
	menu->selectedItemIcon->motion = ICON_SHOWN;
	menu->selectedItemIcon->x = 112;
	menu->selectedItemIcon->y = 8;
	mnIconShow(menu->selectedItemIcon);
	mnTextDrawStringAtOffsetStub(
		gmUnitGetStub(menu->sideMember[0])->name, menu->questionWin, 16, 0);
	mnTextDrawMessageAsTilesStub(
		(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
		menu->questionWin, 16, 8);
}

/* Asks whether to drop item (an inventory entry): its name, the question, and
   Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start. */
Sint32 mnItemMenuConfirmDrop(Sint32 item)
{
	WINDOW *win;
	Sint32	itemId;
	Sint32	question;
	Sint32	answer;
	Sint32	rows = 2;
	Sint32	selection = 1;
	Bool	changed = TRUE;

	win = mnWindowCreateStub(13, 3, 17, 10, WINDOW_FLAG_INSTANT);
	itemId = item & INVENTORY_ITEM_ID_MASK;
	gmItemGetDirectStub(itemId);
	mnTextDrawMessageAsTilesStub(itemId + MsgItemName, win, 24, 0);
	question = MsgConfirmDrop;
	mnTextDrawMessageAsTilesStub(question, win, ITEM_DROP_QUESTION_X, 16);
	question++;
	mnTextDrawMessageAsTilesStub(question, win, ITEM_DROP_QUESTION_X, 24);
	answer = MsgYes;
	mnTextDrawMessageAsTilesStub(answer, win, 24, 40);
	answer++;
	mnTextDrawMessageAsTilesStub(answer, win, 24, 56);
	mnCursorSlide(104, 86);
	for(;;) {
		if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
			break;
		if(changed) {
			changed = FALSE;
			selection = (selection + rows) % rows;
		}
		if(PerPush & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			selection = 1;
			break;
		}
		mnCursorPos(104, (selection << 4) + 70);
		if(PerRepeat & KEY_UP) {
			selection--;
			changed = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if(PerRepeat & KEY_DOWN) {
			selection++;
			changed = TRUE;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		}
		sySynch(1);
	}
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		selection = 1;
	}
	mnWindowCloseStub((void *)win, 1);
	return selection;
}

/* After giving or trading the selected item to the party member in side 1,
   offers to equip it there, showing the stats it would give. */
Sint32 mnItemMenuConfirmEquip(Sint32 unused)
{
	Sint32			 rows = 2;
	Sint32			 selection = 0;
	Bool			 changed = TRUE;
	ITEM_MENU_STATE *menu = MenuState;
	UNIT			*unit = gmUnitGetStub(menu->sideMember[1]);
	UNIT			*saved;
	WINDOW			*win;
	Sint32			 slot;
	Sint32			 member;

	slot = menu->selectedSlots[1];
	member = menu->sideMember[1];
	mnItemMenuDrawEquipPreview(member, slot, 0, member);
	saved = syHeapAlloc(BATTLE_UNIT_SIZE);
	syWordsCopy(saved, unit, BATTLE_UNIT_SIZE);
	win = menu->questionWin;
	/* Equip it now; INVENTORY_EQUIP_REFUSED or INVENTORY_EQUIP_STUCK means it
	   cannot be equipped. */
	if((Uint32)(gmInventoryEquipStub(
					menu->sideMember[1], menu->selectedSlots[1]) -
		   INVENTORY_EQUIP_STUCK) <=
		INVENTORY_EQUIP_REFUSED - INVENTORY_EQUIP_STUCK) {
		selection = 1;
	} else {
#if LANGUAGE != LANGUAGE_JAPANESE
		mnTextDrawMessageAsTilesStub(MsgYes, win, 24, 24);
		mnTextDrawMessageAsTilesStub(MsgYes + 1, win, 72, 24);
		mnWindowClearInteriorTilesStub((void *)win, 16, 16, 96, 24);
		mnTextDrawMessageAsTilesStub(MsgEquipThisItem, win, 0, 16);
		mnCursorSlide(110, 32);
#else
		mnTextDrawMessageAsTilesStub(MsgYes, win, 96, 0);
		mnTextDrawMessageAsTilesStub(MsgYes + 1, win, 96, 16);
		mnWindowClearInteriorTilesStub((void *)win, 16, 16, 96, 24);
		mnTextDrawMessageAsTilesStub(MsgEquipThisItem, win, 8, 16);
		mnCursorSlide(184, 5);
#endif
		for(;;) {
			if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
				break;
			if(changed) {
				changed = FALSE;
				selection = (selection + rows) % rows;
			}
			if(PerPush & KEY_A) {
				sySndPlayStub(SE_NOTE_HIGH); /* the Equip sound */
				break;
			}
			if(PerPush & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				selection = 1;
				break;
			}
#if LANGUAGE != LANGUAGE_JAPANESE
			mnCursorPos(selection * 48 + 110, 32);
			if(PerRepeat & KEY_LEFT) {
#else
			mnCursorPos(184, (selection << 4) + 5);
			if(PerRepeat & KEY_UP) {
#endif
				selection--;
				changed = TRUE;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
#if LANGUAGE != LANGUAGE_JAPANESE
			if(PerRepeat & KEY_RIGHT) {
#else
			if(PerRepeat & KEY_DOWN) {
#endif
				selection++;
				changed = TRUE;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED))
		selection = 1;
	/* No: put the party member back as they were. */
	if(selection == 1)
		syWordsCopy(unit, saved, BATTLE_UNIT_SIZE);
	syHeapRelease(saved);
	gmUnitCalcStub(menu->sideMember[1]);
	gmUnitRefreshPsynergyStub(menu->sideMember[1]);
	return selection;
}

/* Loads the stat arrows (up and down) into two free VRAM blocks, kept in the
   menu state's resource slots. */
void mnLoadStatArrows(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 block = syVRAMCacheFindFreeEntry();

	menu->resourceSlots[0] = block;
	syVRAMCacheLoad(block, 128, Menu_ArrowTiles);
	block = syVRAMCacheFindFreeEntry();
	menu->resourceSlots[1] = block;
	syVRAMCacheLoad(block, 128, Menu_SmallDownArrowTiles);
}

/* Fills result with the item list of the party member in side index: their
   unit, how many items they hold, and the page and row of the item chosen last
   for them (kept within the list), five items to a page. Returns 1. */
Sint32 mnItemMenuBuildPagedList(PAGED_LIST *result, Sint32 idx)
{
	Sint32			 unit;
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 itemCnt;
	Sint32			 row;
	Sint32			 page;
	Sint32			 pageCnt;
	Sint32			 selected;

	itemCnt = mnItemMenuCountItems(menu->sideMember[idx]);
	unit = (Sint32)gmUnitGetStub(menu->sideMember[idx]);
	selected = menu->selectedIdxByMember[menu->sideMember[idx]];
	if((selected + 1) > itemCnt) {
		selected = itemCnt - 1;
	}
	page = selected / LIST_PAGE_SIZE;
	row = selected % LIST_PAGE_SIZE;
	pageCnt = itemCnt / LIST_PAGE_SIZE;
	if(itemCnt % LIST_PAGE_SIZE != 0) {
		pageCnt++;
	}
	result->memberState = unit;
	result->page = page;
	result->pageCnt = pageCnt;
	result->row = row;
	result->entryCnt = itemCnt;
	result->selectedIdx = selected;
	return 1;
}

/* Updates the item list for the row under the cursor: puts the selected item's
   plain name in the info window and highlights its row of the five. window and
   memberState are not used. */
Sint32 mnItemMenuShowSelectedRow(
	WINDOW *win, void *memberState, PAGED_LIST *pageState)
{
	ITEM_MENU_STATE *menu;
	Sint32			 page;
	Sint32			 idx;
	Sint32			 row;

	page = pageState->page;
	menu = MenuState;
	idx = page * LIST_PAGE_SIZE;
	idx += pageState->row;
	pageState->selectedIdx = idx;

	MENU_RESET_DESCRIPTION_WINDOW(menu->descriptionWin);
	sySynch(1);

	idx = pageState->selectedIdx;
	if(menu->items[idx] != 0) {
		Sint32 name =
			(menu->items[idx] & INVENTORY_ITEM_ID_MASK) + MsgItemPlainName;
		MENU_DRAW_DESCRIPTION(name, menu->descriptionWin, 0, 0);
	}

	for(row = 0; row < LIST_PAGE_SIZE; row++) {
		if(row == pageState->row) {
			mnWindowSetHighlightRect((void *)menu->itemWin, 1, row * 2 + 1, 14,
				1, HIGHLIGHT_PALETTE);
		} else {
			mnWindowSetHighlightRect(
				(void *)menu->itemWin, 1, row * 2 + 1, 14, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

/* Draws one page of the item list into window: up to five item names with their
   icons, and the page numbers when there is more than one page. */
Sint32 mnItemMenuDrawListPage(
	WINDOW *win, Sint32 unused, const PAGED_LIST *pageState)
{
	ITEM_MENU_STATE *menu = MenuState;
	Uint32			 page;
	Uint32			 firstEntry;
	Uint32			 visibleCnt;
	Uint8			 row;
	const Uint16	*entry;

	(void)unused;

	mnWindowSprRedrawSavedRectStub(win);
	mnWindowDrawDividerLineStub(win, 0, 11, 16, 11);

	page = pageState->page;
	firstEntry = page * LIST_PAGE_SIZE;
	visibleCnt = (Uint8)(pageState->entryCnt - firstEntry);
	if(visibleCnt > LIST_PAGE_SIZE) {
		visibleCnt = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, win, ITEM_LIST_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(win, pageState->entryCnt, LIST_PAGE_SIZE,
		pageState->page, PAGE_NUMBERS_RIGHT);

	row = 0;
	if(visibleCnt > row) {
		entry = &menu->items[firstEntry];
		do {
			mnTextDrawMessageAsTilesStub(
				(entry[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
				menu->itemWin, ITEM_LIST_NAME_X, row * 16 + 8);
			row++;
			entry++;
		} while(visibleCnt > row);
	}

	return 1;
}

UNUSED Sint32 mnItemMenuItemNamePageReturnTrue(void)
{
	return 1;
}

UNUSED void mnItemMenuItemNamePageNoOperation(void)
{
}

/* The item list of the party member in sideIdx (0: the holder, 1: the
   receiver of a trade), five items a page. */
Sint32 mnItemMenuRunList(Sint32 sideIdx)
{
	ITEM_MENU_STATE *menu;
	WINDOW			*win;
	Bool			 changed;
	Sint32			 moved;
	Sint32			 member;
	Uint8			 i;
	Sint32			 result;
	Bool			 redraw;
	Bool			 done;
	UNIT			*memberUnit;
	Sint32			 prev;
	WINDOW_SPR		*icon;
	Sint32			 memberState[5];
	PAGED_LIST		 state;

	menu = MenuState;
	result = 0;
	prev = 0;
	menu->equipPreview = result;
	mnWindowUpdateOrCreate(&menu->listWin, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	win = menu->listWin;
	done = FALSE;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		memberUnit = gmUnitGetStub(menu->sideMember[sideIdx]);
		menu->itemCnt = mnItemMenuCollectItems(memberUnit, menu->items, 0);
		mnItemMenuDrawIcons(menu->items, 0);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
		mnItemMenuBuildPagedList(&state, sideIdx);
		mnCursorPos(98, state.selectedIdx * 16 + 36);
		changed = TRUE;
		redraw = TRUE;

		/* One party member's list, until an item is chosen or the party member
		   changes. */
		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
			mnCursorPos(98, state.row * 16 + 36);

			if(changed != 0) {
				changed = FALSE;
				if(memberUnit->inventory[prev] != 0) {
					icon = menu->entryIcons[prev];
					mnIconShow(icon);
				}
				if(redraw != 0) {
					sySynch(1);
					mnItemMenuDrawListPage(win, 0, &state);
					if(sideIdx == 0) {
						mnTextDrawMessageAsTilesStub(MsgChangeCharacterHelp,
							win, CHANGE_CHARACTER_HELP_X, 88);
					}
					redraw = FALSE;
				}
				mnItemMenuShowSelectedRow(win, memberState, &state);
				menu->selectedItems[sideIdx] = menu->items[state.selectedIdx];
				mnItemMenuDrawEquipPreview(menu->sideMember[sideIdx],
					state.selectedIdx, 0, menu->sideMember[sideIdx]);
				if(menu->items[state.selectedIdx] != 0) {
					icon = menu->entryIcons[state.selectedIdx];
					/* Swell the selected icon from its first step, in depth
					   slot 250; mnIconShow stills it again when the cursor
					   leaves. */
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->partyCnt; i++) {
					fdSprStartAnimationStub(menu->memberSprs[i], ANIM_STAND);
				}
			}

			/* Every 32 ticks, the party members who can equip the item nod. */
			if((TickCount & 31) == 0) {
				for(i = 0; i < menu->partyCnt; i++) {
					if(gmItemCanUnitEquipDirectStub(menu->memberIds[i],
						   INVENTORY_ITEM_ID_MASK &
							   menu->items[state.selectedIdx]) != 0) {
						fdSprStartAnimationStub(menu->memberSprs[i], ANIM_NOD);
					}
				}
			}

			sySynch(1);
			prev = state.selectedIdx;
			moved = mnPagedListHandleInput(
				0, state.entryCnt, LIST_PAGE_SIZE, &state.row, &state.page);
			if(moved == 1) {
				redraw = TRUE;
				changed = TRUE;
			}
			if(moved == 0) {
				changed = TRUE;
			}
			if(moved == -1) {
				changed = FALSE;
			}

			if((PerPush & KEY_A) != 0 && menu->items[state.selectedIdx] != 0) {
				sySndPlayStub(SE_NOTE_LOW); /* the item choice sound */
				result = menu->items[state.selectedIdx];
				done = TRUE;
				break;
			}
			if((PerPush & KEY_B) != 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}
			/* L and R: the previous or next party member who has items (side 0
			   only). */
			if((PerRepeat & KEY_R) != 0 || (PerRepeat & KEY_L) != 0) {
				if(sideIdx == 1) {
					sySndPlayStub(SE_MENU_ERROR);
					sySynch(1);
				} else {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					menu->selectedIdxByMember[menu->sideMember[sideIdx]] =
						state.selectedIdx;
					member = menu->sideIdx[sideIdx];
					do {
						if((PerRepeat & KEY_R) != 0) {
							member++;
						} else {
							member--;
						}
						member = (member + menu->partyCnt) % menu->partyCnt;
						menu->selectedMember = menu->memberIds[member];
						menu->sideMember[sideIdx] = menu->memberIds[member];
						menu->sideIdx[sideIdx] = member;
						menu->itemCnt = mnItemMenuCollectItems(
							gmUnitGetStub(menu->sideMember[sideIdx]),
							menu->items, 0);
					} while(menu->itemCnt == 0);
					for(i = 0; i <= 3; i++) {
						menu->memberY[i] = MENU_MEMBER_Y;
					}
					menu->memberY[member] = MENU_MEMBER_RAISED_Y;
					break;
				}
			}
		}
	}

	mnWindowClearInteriorTilesStub((void *)win, 0, 88, 120, 96);
	mnIconShow(menu->gridCursor);
	menu->selectedSlots[sideIdx] = state.selectedIdx;
	menu->selectedIdxByMember[menu->sideMember[sideIdx]] = state.selectedIdx;
	menu->selectedItems[sideIdx] = result;
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
		result = -1;
	}
	sySynch(1);
	return result;
}

#undef ITEM_MODAL_SHORT_X
#undef ITEM_MODAL_REMOVED_X
#undef ITEM_CANNOT_TRADE_X
#undef ITEM_TRADED_X
#undef ITEM_TRADE_PROMPT_X
#undef ITEM_COMMAND_TEXT_X
#undef ITEM_QUANTITY_WINDOW_WIDTH
#undef ITEM_GIVE_CURSOR_X
#undef ITEM_GIVE_ASK_X
#undef ITEM_GIVE_ASK_Y
#undef ITEM_DROP_QUESTION_X
#undef ITEM_LIST_PAGE_ICONS_X
#undef ITEM_LIST_NAME_X
#undef SAVED_WINDOW_TILE_BYTES
#undef ITEM_IWRAM_COPY_WORDS
#undef ITEM_COMMAND_STATE_DISABLED
#undef ITEM_COMMAND_STATE_AVAILABLE
#undef TEXT_COLOR_DISABLED
