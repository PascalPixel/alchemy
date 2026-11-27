// The item menu: the field menu's Item screen, the command loop, the party member and item lists,
// and Use, Equip, Give, Drop, Remove and Details. Part of menu.c.

#include "menu_internal.h"
#include "shop/shop.h"

// Where the edition's text goes. The international editions have longer words, and some languages
// move a message to fit its own.
//
// The x of the small message windows "Dropped it.", "You were cursed!" and "Removed".
#define ITEM_MODAL_SHORT_X         JAPANESE_OR(15, 14)
#define ITEM_MODAL_REMOVED_X       JAPANESE_OR(17, 14)

// The x of the small message window "Can't trade it!".
#define ITEM_CANNOT_TRADE_X        FRENCH_OR(11, 15)

// The x of the small message window "Traded".
#define ITEM_TRADED_X              ITALIAN_OR(13, 15)

// The x of "Trade for what?" in its window, in pixels.
#define ITEM_TRADE_PROMPT_X        JAPANESE_OR(16, 0)

// The x of the command grid's middle column.
#define ITEM_COMMAND_TEXT_X        JAPANESE_OR(40, 32)

// The quantity chooser's window width, in tiles.
#define ITEM_QUANTITY_WINDOW_WIDTH JAPANESE_OR(13, 30)

// The hand cursor and the "How many?" question of the quantity chooser.
#define ITEM_GIVE_CURSOR_X         JAPANESE_OR(88, 128)
#define ITEM_GIVE_ASK_X            JAPANESE_OR(0, 32)
#define ITEM_GIVE_ASK_Y            JAPANESE_OR(32, 0)

// The x of the drop question ("Are you sure you...") in its window, in pixels.
#define ITEM_DROP_QUESTION_X       JAPANESE_OR(8, 0)

// The item list's icon column and name column.
#define ITEM_LIST_PAGE_ICONS_X     JAPANESE_OR(120, 116)
#define ITEM_LIST_NAME_X           JAPANESE_OR(32, 24)

// The bytes of window tiles (BG character block 1) the menu draws over, which it saves while it is
// open and puts back as it closes.
#define SAVED_WINDOW_TILE_BYTES    8192

// The field menu's Item screen (mnFieldMenuRun, FIELD_MENU_ITEM): choose a party member, one of
// their items and a command.
Sint32 mnItemMenuRun(void)
{
#if ITEM_MENU_EXTRA_TEXT_TILES
	void *backup;
#endif
	ITEM_MENU_STATE *screen;
	Sint32           item;
	Sint32           target;
	Sint32           member;
	Sint32           result;
#if ITEM_MENU_EXTRA_TEXT_TILES
	Sint32 size = SAVED_WINDOW_TILE_BYTES;
#endif

	// Open the menu over the field, which stops moving.
#if ITEM_MENU_EXTRA_TEXT_TILES
	backup = syHeapAllocateEWRAM(size);
#endif
	screen                          = syHeapSlotAllocate(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	screen->partyCount = gmPartyListMembersStub(screen->memberIds);
	mnItemMenuInit(0, 3, 0, 7);
	mnLoadStatArrows();
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	screen->questionWindow = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
#if ITEM_MENU_EXTRA_TEXT_TILES
	// Save the window tiles the menu draws over, and fill them with colour 3.
	mnSuspendFieldEvents();
	syWordsCopy(backup, BG_CHAR_BLOCK(1), size);
	syWordsFill(BG_CHAR_BLOCK(1), size, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif

	// Let the player choose.
	mnStartWatchingStart();
	result = mnItemMenuRunCommands(&member, &target, &item);
	mnStopWatchingStart();
	if (result == 1)
	{
		FIELD_STATE *selection = FieldStateWork;

		selection->item        = (member << ACT_UNIT_SHIFT) | (item & INVENTORY_ITEM_ID_MASK);
		selection->itemSlot    = screen->selectedSlots[0];
	}

	// Close the menu and give the screen back to the field.
	mnWindowSpriteClearListStub(screen->statusWindow);
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

// The resident IWRAM word copy, called through its address.
typedef Sint32 (*ITEM_WORD_COPY_FN)(void *dest, const void *src, Sint32 size);

// syWordsCopy's function reached from syIWRAMIRQMain rather than at its fixed address: the trade's
// snapshot copies call it this way.
#define ITEM_IWRAM_COPY_WORDS ((ITEM_WORD_COPY_FN)(syIWRAMIRQMain + syWordsCopyOffset))

// The steps of the item menu's command loop (mnItemMenuRunCommands).
typedef enum ITEM_MENU_STEP
{
	ITEM_STEP_MEMBER   = 0,
	ITEM_STEP_ITEM     = 1,
	ITEM_STEP_USE      = 2,
	ITEM_STEP_EQUIP    = 3,
	ITEM_STEP_GIVE     = 4,
	ITEM_STEP_DROP     = 5,
	ITEM_STEP_TARGET   = 6,  // Give: choose whom to give it to
	ITEM_STEP_TRADE    = 7,  // the receiver is full: choose what they give back
	ITEM_STEP_IDLE     = 8,  // nothing sets it
	ITEM_STEP_COMMAND  = 9,
	ITEM_STEP_DETAILS  = 10, // Details (mnItemMenuShowDetails)
	ITEM_STEP_REMOVE   = 11,
	ITEM_STEP_QUANTITY = 12  // nothing sets it
} ITEM_MENU_STEP;

// Runs the item menu: choose a party member, one of their items and a command (Use, Equip, Give,
// Drop, Remove or Inspect), then carry it out, until the player backs out or presses Start.
Sint32 mnItemMenuRunCommands(Sint32 *memberOut, Sint32 *targetOut, Sint32 *itemOut)
{
	Sint32 done;
#if !ITEM_EVENT_USED_BY_SHOWN_HOLDER
	Sint32 member;
	Sint32 chosen;
#endif
	Sint32           selection;
	Sint32           menuResult;
	UNIT            *srcUnit;
	UNIT            *targetUnit;
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	UNIT            *srcCopy;
	UNIT            *targetCopy;
	Uint32           state;
	Sint32           command;
	Sint32           result;
	Sint32           heldCount;
	Sint32           amount;
	Sint32           transferFailed;
	Sint32           removedCount;
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
	Sint32 giveTarget;
#endif
	Uint8 srcQuantity;
	Uint8 targetQuantity;
	Sint8 stackSize;

	done = 0;
#if !ITEM_EVENT_USED_BY_SHOWN_HOLDER
	member = 0;
	chosen = 0;
#endif
	selection  = 0;
	menuResult = 0;
	state      = ITEM_STEP_MEMBER;
	menu       = MenuState;

	while (done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
	{
		switch (state)
		{
			// Choose whose items to look at.
			case ITEM_STEP_MEMBER:
				menu->selectedSlots[0] = 0;
				mnItemMenuFitMessageWindowToQuestion();
				mnItemMenuPlaceItemWindowBelowQuestion();
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				mnItemMenuDrawMessage(0, MsgWhoseItem);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
				mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
				mnDrawCoins(menu->descriptionWindow);
				command = mnItemMenuSelectMember(0);
				if (command == -1)
				{
					// DEAD: selection and command cleared though the menu ends before reading
					// either; the ROM keeps the 0 and its store to selection.
					selection = command = 0;
					menuResult          = -1;
					done                = 1;
				}
#else
				mnDrawCoins(menu->descriptionWindow);
				member = mnItemMenuSelectMember(0);
				if (member == -1)
				{
					// DEAD: chosen, selection and member cleared though the menu ends before
					// reading them; the ROM keeps the 0 and its three stores.
					chosen = selection = member = 0;
					menuResult                  = -1;
					done                        = 1;
				}
#endif
				mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
				mnItemMenuHideAllIcons();
				state = ITEM_STEP_ITEM;
				break;

			// Choose one of their items.
			case ITEM_STEP_ITEM:
				if (mnItemMenuCountItems(menu->sideMember[0]) == 0)
				{
					state = ITEM_STEP_MEMBER;
					break;
				}
				mnItemMenuFitMessageWindowToQuestion();
				mnItemMenuPlaceItemWindowBelowQuestion();
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				menu->sideIcons[0]->motion     = ICON_SHOWN;
				mnItemMenuDrawMessage(0, MsgWhichItem);
				selection = mnItemMenuRunList(0);
				state     = ITEM_STEP_MEMBER;
				if (selection == -1)
				{
					break;
				}
				menu->listMode = -1;
				state          = ITEM_STEP_COMMAND;
				break;

			// Choose what to do with it.
			case ITEM_STEP_COMMAND:
				command = mnItemMenuChooseCommand();
				if (command == -1)
				{
					state                = ITEM_STEP_ITEM;
					menu->completionFlag = 1;
				}
				if (command == ITEM_COMMAND_USE)
				{
					if (fdHasItemCueStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0)
					{
						done = 1;
						// The international editions report target 0 (command is ITEM_COMMAND_USE
						// here) and the bare item id; the Japanese ones the target chosen last and
						// the list's whole inventory entry.
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
						*memberOut = menu->sideMember[0];
						*targetOut = command;
						*itemOut   = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
#else
						*memberOut = member;
						*targetOut = chosen;
						*itemOut   = selection;
#endif
						menuResult = 1;
						break;
					}
					result = mnItemClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]);
					if (result == ITEM_USE_MODE_CHOOSE_MEMBER)
					{
						state = ITEM_STEP_USE;
					}
					if (result == ITEM_USE_MODE_WHOLE_PARTY)
					{
						mnItemMenuUse();
						mnWindowSpriteClearListStub(menu->descriptionWindow);
						mnItemMenuShowMessage(menu->msgOffset + MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
						mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
#endif
						menu->sideIcons[0]->motion = ICON_HIDDEN;
						menu->itemCount            = mnItemMenuCollectItems(
                            gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
						mnItemMenuDrawIcons(menu->items, 0);
						state = ITEM_STEP_MEMBER;
					}
					if (result == ITEM_USE_MODE_UNUSABLE || result == ITEM_USE_MODE_FROM_FIELD)
					{
						done       = 1;
						*memberOut = menu->sideMember[0];
						*targetOut = menu->sideMember[1];
						*itemOut   = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
						menuResult = 1;
					}
				}
				if (command == ITEM_COMMAND_EQUIP)
				{
					state = ITEM_STEP_EQUIP;
				}
				if (command == ITEM_COMMAND_GIVE)
				{
					state = ITEM_STEP_TARGET;
				}
				if (command == ITEM_COMMAND_DROP)
				{
					state = ITEM_STEP_DROP;
				}
				if (command == ITEM_COMMAND_REMOVE)
				{
					state = ITEM_STEP_REMOVE;
				}
				if (command == ITEM_COMMAND_DETAILS)
				{
					state = ITEM_STEP_DETAILS;
				}
				break;

			// Use the item on the party member chosen.
			case ITEM_STEP_USE:
				mnItemMenuHideAllIcons();
				mnItemMenuFitMessageWindowToItemQuestion();
				mnItemMenuPlaceItemWindowBelowItemQuestion();
				mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
				mnItemMenuDrawItemHead();
				mnTextDrawMessageAsTilesStub(MsgUseOnWhom, menu->questionWindow, 16, 16);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
				if (mnItemMenuSelectTarget(ITEM_TARGET_USE) != -1)
				{
#else
				chosen = mnItemMenuSelectTarget(ITEM_TARGET_USE);
				if (chosen != -1)
				{
#endif
					command = 0;
					if (mnItemMenuIsStatRaisingItem(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK)
					    != 0)
					{
						command = STATUS_PANEL_STATS;
					}
					result = mnItemMenuUse();
					mnStatusPanelDraw(menu->statusWindow, menu->sideMember[1], 0, command);
					if (result != -1)
					{
						mnWindowSpriteClearListStub(menu->descriptionWindow);
						mnItemMenuShowMessage(menu->msgOffset + MsgItemUseResult, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
						mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
#endif
						menu->sideIcons[0]->motion = ICON_HIDDEN;
						mnItemMenuTryBreak();
						state = ITEM_STEP_ITEM;
					}
					menu->itemCount =
						mnItemMenuCollectItems(gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
					mnItemMenuDrawIcons(menu->items, 0);
					menu->completionFlag = 1;
				}
				else
				{
					state = ITEM_STEP_COMMAND;
				}
				break;

			// Give: choose to whom.
			case ITEM_STEP_TARGET:
				mnItemMenuFitMessageWindowToItemQuestion();
				mnItemMenuPlaceItemWindowBelowItemQuestion();
				mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
				mnItemMenuDrawItemHead();
				mnTextDrawMessageAsTilesStub(MsgGiveToWhom, menu->questionWindow, 16, 16);
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
				giveTarget = mnItemMenuSelectTarget(ITEM_TARGET_GIVE);
				state      = ITEM_STEP_GIVE;
				if (giveTarget == -1)
				{
#else
				chosen = mnItemMenuSelectTarget(ITEM_TARGET_GIVE);
				state  = ITEM_STEP_GIVE;
				if (chosen == -1)
				{
#endif
					mnItemMenuDrawEquipPreview(
						menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
					state = ITEM_STEP_COMMAND;
				}
				break;

			// Drop the item, or some of a stack, after confirming.
			case ITEM_STEP_DROP:
				mnItemMenuHideAllIcons();
				item   = gmItemGetDirectStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
				result = 0;
				if ((item->flags & ITEM_STACKABLE) != 0)
				{
					heldCount = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
					if (heldCount > 1)
					{
						mnItemMenuDrawItemHead();
						result = mnItemMenuSelectGiveQuantity(0, heldCount, 1);
					}
				}
				if (result == -1)
				{
					state = ITEM_STEP_COMMAND;
					break;
				}
				menu->sideMember[1] = 0;
				mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM,
				                                (menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK)
				                                    | (result << INVENTORY_QUANTITY_SHIFT),
				                                (Uint8)menu->selectedItemIcon->index,
				                                0);
				menu->selectedItemIcon->motion = ICON_SHOWN;
				menu->selectedItemIcon->x      = 120;
				menu->selectedItemIcon->y      = 28;
				mnIconShow(menu->selectedItemIcon);
				mnWindowClearInteriorTilesStub((void *)menu->listWindow, 0, 72, 120, 96);
				mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
				// Yes: take result + 1 of the stack out, one at a time.
				if (mnItemMenuConfirmDrop(selection) == 0)
				{
#if ITEM_EVENT_USED_BY_SHOWN_HOLDER
					command = menu->sideMember[0];
					gmUnitGetStub(command);
					for (removedCount = 0; removedCount < result + 1; removedCount++)
					{
						gmInventoryRemoveStub(command, menu->selectedSlots[0]);
						gmItemAdjustRareItemStockStub(
							menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK, 1);
					}
					gmUnitCalculateStub(command);
#else
					member = menu->sideMember[0];
					gmUnitGetStub(member);
					for (removedCount = 0; removedCount < result + 1; removedCount++)
					{
						gmInventoryRemoveStub(member, menu->selectedSlots[0]);
						gmItemAdjustRareItemStockStub(
							menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK, 1);
					}
					gmUnitCalculateStub(member);
#endif
					mnItemMenuPlaceItemWindowBelowQuestion();
					mnItemMenuRefreshMember(menu->sideMember[0], 0);
					menu->selectedItemIcon->motion = ICON_HIDDEN;
					menu->sideIcons[0]->motion     = ICON_HIDDEN;
					sySynch(1);
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					mnItemMenuShowMessage(MsgDroppedIt, ITEM_MODAL_SHORT_X, 13);
					menu->completionFlag = 1;
					state                = ITEM_STEP_ITEM;
				}
				else
				{
					state = ITEM_STEP_COMMAND;
				}
				gmUnitCalculateStub(menu->sideMember[0]);
				menu->selectedItemIcon->motion = ICON_HIDDEN;
				fdPartyClearUnusableShortcutsStub();
				break;

			// Give the item, or some of a stack, to the chosen party member.
			case ITEM_STEP_GIVE:
				transferFailed = 0;
				item = gmItemGetDirectStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
				if ((item->flags & ITEM_STACKABLE) != 0)
				{
					// A receiver with a full stack of it takes none; one with no free slot and none
					// of it must trade.
					heldCount = mnItemMenuGetItemQuantity(
						menu->sideMember[1], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
					if (heldCount == INVENTORY_STACK_MAX)
					{
						transferFailed = 1;
					}
					if (mnItemMenuCountItems(menu->sideMember[1]) == INVENTORY_SLOTS
					    && heldCount == 0)
					{
						state = ITEM_STEP_TRADE;
						break;
					}
					stackSize = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
					if (transferFailed == 0)
					{
						if (heldCount + stackSize > INVENTORY_STACK_MAX)
						{
							amount = INVENTORY_STACK_MAX - heldCount;
						}
						else
						{
							amount = stackSize;
						}
						if (stackSize > 1)
						{
							result = mnItemMenuSelectGiveQuantity(0, amount, 0);
						}
						else
						{
							result = 0;
						}
						if (result == -1)
						{
							state = ITEM_STEP_TARGET;
							break;
						}
						// Move result + 1 one at a time; the broken bit goes with each, the
						// equipped bit and the quantity do not.
						for (command = 0; command < result + 1; command++)
						{
							heldCount = gmInventoryAddItemStub(
								menu->sideMember[1],
								menu->selectedItems[0]
									& (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
							if (heldCount != -1)
							{
								gmInventoryRemoveStub(menu->sideMember[0], menu->selectedSlots[0]);
								menu->selectedSlots[1] = heldCount;
							}
							else
							{
								transferFailed = 1;
							}
						}
					}
				}
				else
				{
					result = gmInventoryAddItemStub(
						menu->sideMember[1],
						menu->selectedItems[0] & (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
					if (result == -1)
					{
						state = ITEM_STEP_TRADE;
						break;
					}
					menu->selectedSlots[1] = result;
					result = gmInventoryRemoveStub(menu->sideMember[0], menu->selectedSlots[0]);
					if (result == INVENTORY_REMOVE_EMPTY_SLOT)
					{
						transferFailed = 1;
					}
				}
				gmUnitCalculateStub(menu->sideMember[0]);
				gmUnitCalculateStub(menu->sideMember[1]);
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[1]);
				result = 1;
				if (transferFailed == 0)
				{
					menu->sideMember[0]     = menu->sideMember[1];
					menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
					mnItemMenuFitMessageWindowToLongItemQuestion();
#endif
					mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
					mnItemMenuDrawItemHead();
					result = mnItemMenuConfirmEquip(0);
				}
				if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
				{
					break;
				}
				mnItemMenuRefreshMember(menu->sideMember[1], 1);
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				sySynch(1);
				// Say how it went: the receiver's stack was full, the item was given, or it was
				// given and equipped (mnItemMenuConfirmEquip returned 0).
				if (transferFailed == 1)
				{
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					mnItemMenuShowMessage(MsgCannotHoldMore, 15, 14);
				}
				else
				{
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					if (result == 1)
					{
						mnItemMenuShowMessage(MsgGiven, 15, 14);
					}
					else
					{
						mnItemMenuDrawEquipPreview(
							menu->sideMember[1], menu->selectedSlots[1], 0, menu->sideMember[1]);
						mnItemMenuShowMessage(MsgEquippedIt, 15, 14);
						item = gmItemGetDirectStub(menu->selectedItems[0]);
						if ((item->flags & ITEM_CURSED) != 0)
						{
							sySoundPlayStub(SE_CURSE);
							mnWindowSpriteClearListStub(menu->descriptionWindow);
							// "You were cursed!"
							mnItemMenuShowMessage(MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 14);
						}
					}
				}
				fdPartyClearUnusableShortcutsStub();
				state = ITEM_STEP_MEMBER;
				break;

			// Trade: the receiver gives back one of their items.
			case ITEM_STEP_TRADE:
				transferFailed = 0;
				mnItemMenuFitMessageWindowToQuestion();
				mnItemMenuPlaceItemWindowBelowQuestion();
				mnItemMenuDrawMessage(0, MsgSwapForWhat);
#if TRADE_CANCEL_KEEPS_CHOSEN_ITEM
				if (mnItemMenuRunList(1) == -1)
				{
#else
				selection = mnItemMenuRunList(1);
				if (selection == -1)
				{
#endif
					state = ITEM_STEP_TARGET;
					break;
				}
				// Snapshot both inventories so any failed part of the trade can roll back.
				srcUnit    = gmUnitGetStub(menu->sideMember[0]);
				targetUnit = gmUnitGetStub(menu->sideMember[1]);
				srcCopy    = syHeapAllocate(BATTLE_UNIT_SIZE);
				targetCopy = syHeapAllocate(BATTLE_UNIT_SIZE);
				ITEM_IWRAM_COPY_WORDS(srcCopy, srcUnit, BATTLE_UNIT_SIZE);
				ITEM_IWRAM_COPY_WORDS(targetCopy, targetUnit, BATTLE_UNIT_SIZE);
				// Take both whole stacks out, one item at a time (at most INVENTORY_STACK_MAX):
				// gmInventoryRemoveStub returns INVENTORY_REMOVE_LAST for the last one and
				// INVENTORY_REMOVE_EMPTY_SLOT for an empty slot.
				for (srcQuantity = 0; srcQuantity < INVENTORY_STACK_MAX; srcQuantity++)
				{
					result = gmInventoryRemoveStub(menu->sideMember[0], menu->selectedSlots[0]);
					if (result == INVENTORY_REMOVE_LAST)
					{
						break;
					}
					if (result == INVENTORY_REMOVE_EMPTY_SLOT)
					{
						transferFailed = 1;
						break;
					}
				}
				srcQuantity++;
				for (targetQuantity = 0; targetQuantity < INVENTORY_STACK_MAX; targetQuantity++)
				{
					// The receiver cannot trade away a worn item that will not come off.
					if ((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0)
					{
						item = gmItemGetDirectStub(menu->selectedItems[1] & INVENTORY_ITEM_ID_MASK);
						if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0)
						{
							transferFailed = 1;
						}
					}
					result = gmInventoryRemoveStub(menu->sideMember[1], menu->selectedSlots[1]);
					if (result == INVENTORY_REMOVE_LAST)
					{
						break;
					}
					if (result == INVENTORY_REMOVE_EMPTY_SLOT)
					{
						transferFailed = 1;
						break;
					}
				}
				targetQuantity++;
				// Then give each the other's items.
				while (srcQuantity != 0)
				{
					result = gmInventoryAddItemStub(
						menu->sideMember[1],
						menu->selectedItems[0] & (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
					if (result == -1)
					{
						transferFailed = 1;
						break;
					}
					menu->selectedSlots[1] = result;
					srcQuantity--;
				}
				while (targetQuantity != 0)
				{
					result = gmInventoryAddItemStub(
						menu->sideMember[0],
						menu->selectedItems[1] & (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
					if (result == -1)
					{
						transferFailed = 1;
						break;
					}
					menu->selectedSlots[0] = result;
					targetQuantity--;
				}
				sySynch(1);
				if (transferFailed == 1)
				{
					syWordsCopy(srcUnit, srcCopy, BATTLE_UNIT_SIZE);
					syWordsCopy(targetUnit, targetCopy, BATTLE_UNIT_SIZE);
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					mnItemMenuShowMessage(MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if LANGUAGE == LANGUAGE_FRENCH
					mnWindowSpriteRedrawSavedRectStub(menu->statusWindow);
#endif
				}
				else
				{
					gmUnitCalculateStub(menu->sideMember[0]);
					gmUnitCalculateStub(menu->sideMember[1]);
					gmUnitRefreshPsynergyStub(menu->sideMember[0]);
					gmUnitRefreshPsynergyStub(menu->sideMember[1]);
					mnItemMenuFitMessageWindowToItemQuestion();
#if LANGUAGE != LANGUAGE_JAPANESE
					mnItemMenuFitMessageWindowToLongItemQuestion();
					mnItemMenuHidePageIcons();
#endif
					mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
					menu->sideMember[0]     = menu->sideMember[1];
					menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
					mnItemMenuDrawItemHead();
					result = mnItemMenuConfirmEquip(0);
					if (gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
					{
						mnWindowSpriteClearListStub(menu->descriptionWindow);
						mnItemMenuPlaceItemWindowBelowItemQuestion();
						mnItemMenuRefreshMember(menu->sideMember[1], 1);
						if (result == 0)
						{
							mnItemMenuDrawEquipPreview(menu->sideMember[1],
							                           menu->selectedSlots[1],
							                           0,
							                           menu->sideMember[1]);
							mnItemMenuShowMessage(MsgEquippedIt, 15, 14);
							item = gmItemGetDirectStub(menu->selectedItems[0]);
							if ((item->flags & ITEM_CURSED) != 0)
							{
								sySoundPlayStub(SE_CURSE);
								mnWindowSpriteClearListStub(menu->descriptionWindow);
								// "You were cursed!"
								mnItemMenuShowMessage(MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 14);
							}
						}
						else
						{
							mnItemMenuShowMessage(MsgTraded, ITEM_TRADED_X, 14);
						}
					}
				}
				syHeapRelease(targetCopy);
				syHeapRelease(srcCopy);
				fdPartyClearUnusableShortcutsStub();
				state = ITEM_STEP_MEMBER;
				break;

			// Equip it.
			case ITEM_STEP_EQUIP:
				result = gmInventoryEquipStub(menu->sideMember[0], menu->selectedSlots[0]);
				if (result == INVENTORY_EQUIP_REFUSED)
				{
					state = ITEM_STEP_ITEM;
					break;
				}
				// The item it would replace is cursed and will not come off.
				if (result == INVENTORY_EQUIP_STUCK)
				{
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					mnItemMenuShowMessage(MsgCannotRemoveItem, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
					mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
#endif
					state = ITEM_STEP_ITEM;
					break;
				}
				gmUnitCalculateStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				menu->itemCount =
					mnItemMenuCollectItems(gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
				mnItemMenuDrawIcons(menu->items, 0);
				sySynch(1);
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
				mnWindowSpriteClearListStub(menu->descriptionWindow);
				mnItemMenuShowMessage(MsgEquippedIt, 15, 8);
				item = gmItemGetDirectStub(menu->selectedItems[0]);
				if ((item->flags & ITEM_CURSED) != 0)
				{
					sySoundPlayStub(SE_CURSE);
					mnWindowSpriteClearListStub(menu->descriptionWindow);
					// "You were cursed!"
					mnItemMenuShowMessage(MsgEquippedIt + 7, ITEM_MODAL_SHORT_X, 8);
				}
				state = ITEM_STEP_ITEM;
				break;

			// Unequip it.
			case ITEM_STEP_REMOVE:
				gmUnitGetStub(menu->sideMember[0])->inventory[menu->selectedSlots[0]] &=
					~INVENTORY_EQUIPPED;
				gmUnitCalculateStub(menu->sideMember[0]);
				gmUnitRefreshPsynergyStub(menu->sideMember[0]);
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				menu->itemCount =
					mnItemMenuCollectItems(gmUnitGetStub(menu->sideMember[0]), menu->items, 0);
				mnItemMenuDrawIcons(menu->items, 0);
				sySynch(1);
				// Show the stats as they are without it.
				menu->equipPreview = 1;
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
				menu->equipPreview = 0;
				mnWindowSpriteClearListStub(menu->descriptionWindow);
				mnItemMenuShowMessage(MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
				fdPartyClearUnusableShortcutsStub();
				state = ITEM_STEP_ITEM;
				break;

			// Show its description.
			case ITEM_STEP_DETAILS:
				menu->sideIcons[0]->motion = ICON_HIDDEN;
				mnItemMenuShowDetails(menu->selectedItems[0]);
				mnWindowSpriteRedrawSavedRectStub(menu->statusWindow);
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
				menu->sideIcons[0]->motion = ICON_SHOWN;
				state                      = ITEM_STEP_COMMAND;
				break;

			// Unused: not reached from any command. Chooses a quantity and keeps it in heldCount,
			// which nothing reads.
			case ITEM_STEP_QUANTITY:
				result = mnItemMenuSelectGiveQuantity(0, INVENTORY_STACK_MAX, 0);
				if (result != -1)
				{
					// Unused: the quantity chosen, which nothing reads.
					heldCount = result;
				}
				state = ITEM_STEP_ITEM;
				break;

			case ITEM_STEP_IDLE: break;

			default            : done = 1; break;
		}
	}

	if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
	{
		menuResult = -1;
	}
	return menuResult;
}

// Uses the selected item from the party member in side 0 on the one in side 1.
Sint32 mnItemMenuUse(void)
{
	ITEM_MENU_STATE *menu;
	Sint32           result;

	menu   = MenuState;
	result = mnItemUse(menu->selectedSlots[0], menu->sideMember[0], menu->sideMember[1]);

	if (result == -1)
	{
		sySoundPlayStub(SE_MENU_ERROR);
		mnWindowSpriteClearListStub(menu->descriptionWindow);
		mnItemMenuShowMessage(menu->msgOffset + MsgItemUseResult, result, result);
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
#endif
		menu->completionFlag = 1;
		return result;
	}

	mnItemPlayUseSound(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	gmUnitCalculateStub(menu->sideMember[0]);
	gmUnitCalculateStub(menu->sideMember[1]);
	return 1;
}

// Sets up the item menu's first screen: the cursor window with the party's figures, the info window
// along the bottom, and an 8 by 2 cursor grid.
void mnItemMenuInit(Sint32 x, Sint32 y, Sint32 mode, Sint32 columns)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32           memberIndex;

	mnCreatePartyFigures(mnCursorCreate(menu), 2, 2, 8, 0);
	// The four figures stand at screen y 30; the screens raise the party member chosen to 26.
	for (memberIndex = 0; memberIndex < 4; memberIndex++)
		menu->memberY[memberIndex] = MENU_MEMBER_Y;

	menu->buttonHelpWindow  = NULL;
	menu->statusWindow      = NULL;
	menu->descriptionWindow = mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
	menu->itemWindow        = NULL;
	menu->selectedColumn    = 0;
	menu->selectedRow       = 0;
	menu->columnCount       = 8;
	menu->rowCount          = 2;
}

// Creates the 32 item icon sprites in window, off screen until placed: slots 0 to 7 at
// MENU_ICON_HIDDEN_X, the rest 8 pixels further, all at MENU_ICON_HIDDEN_Y, each showing item
// graphic slot of the same number.
void mnSpawnIconEntries(ITEM_MENU_STATE *menu, WINDOW *window)
{
	WINDOW_SPR **firstIcons;
	Sint32       firstSlot;
	WINDOW_SPR **secondIcons;
	Sint32       secondSlot;
	WINDOW_SPR **remainingIcons;
	Sint32       remainingSlot;

	firstSlot = 0;
	{
		Sint32 y   = MENU_ICON_HIDDEN_Y;

		firstIcons = &menu->entryIcons[0];
		for (; firstSlot < ICON_GRID_ROW; firstSlot++)
		{
			*firstIcons++ = mnWindowSpriteCreateFromResourceStub(
				ICON_KIND_ITEM, firstSlot, window, MENU_ICON_HIDDEN_X, y);
		}
	}

	secondSlot = ICON_GRID_ROW;
	{
		Sint32 y    = MENU_ICON_HIDDEN_Y;

		secondIcons = &menu->entryIcons[ICON_GRID_ROW];
		for (; secondSlot < 2 * ICON_GRID_ROW; secondSlot++)
		{
			*secondIcons++ =
				mnWindowSpriteCreateFromResourceStub(ICON_KIND_ITEM, secondSlot, window, 256, y);
		}
	}

	remainingSlot = 2 * ICON_GRID_ROW;
	{
		Sint32 y       = MENU_ICON_HIDDEN_Y;

		remainingIcons = &menu->entryIcons[2 * ICON_GRID_ROW];
		for (; remainingSlot < MENU_ENTRY_ICONS; remainingSlot++)
		{
			*remainingIcons++ =
				mnWindowSpriteCreateFromResourceStub(ICON_KIND_ITEM, remainingSlot, window, 256, y);
		}
	}
}

// Hides all the item icons.
void mnItemMenuHideAllIcons(void)
{
	WINDOW_SPR **icons = ((ITEM_MENU_STATE *)MenuState)->entryIcons;
	Sint32       slot;

	for (slot = 0; slot < MENU_ENTRY_ICONS; slot++)
	{
		WINDOW_SPR *icon = *icons++;
		if (icon != 0)
		{
			icon->motion = ICON_HIDDEN;
		}
	}
}

#if LANGUAGE != LANGUAGE_JAPANESE
// Hides the item icons in the first column of the five-wide item grid.
void mnItemMenuHidePageIcons(void)
{
	ITEM_MENU_STATE *menu     = MenuState;
	Sint32           slot     = 0;
	Sint32           hidden   = ICON_HIDDEN;
	WINDOW_SPR     **iconSlot = menu->entryIcons;

	do
	{
		WINDOW_SPR *icon = *iconSlot++;

		if (icon != NULL && slot % MENU_ITEM_GRID_COLUMNS == 0)
			icon->motion = hidden;
		slot++;
	} while (slot <= MENU_ENTRY_ICONS - 1);
}
#endif

// Closes the item menu: releases the party figures, hides the icons and the status panel's portrait
// (the state's cursor), and closes every window the menu opened.
void mnItemMenuClose(void)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR      *cursor;

	menu = MenuState;
	mnReleasePartyFigures();
	mnItemMenuHideAllIcons();
	sySynch(1);
	cursor         = menu->cursor;
	cursor->motion = ICON_HIDDEN;
	mnWindowCloseIfOpen(&menu->mainWindow, 1);
	mnWindowCloseIfOpen(&menu->itemWindow, 1);
	mnWindowCloseIfOpen(&menu->questionWindow, 1);
	mnWindowCloseIfOpen(&menu->statusWindow, 1);
	mnWindowCloseIfOpen(&menu->buttonHelpWindow, 1);
	mnWindowCloseIfOpen(&menu->descriptionWindow, 1);
	mnWindowCloseIfOpen(&menu->equipWindow, 1);
	mnWindowCloseIfOpen(&menu->listWindow, 1);
	mnWindowCloseIfOpen(&menu->unopenedWindow, 1);
	mnWindowCloseIfOpen(&menu->modalWindow, 1);
	mnWindowCloseIfOpen(&menu->secondUnopenedWindow, 1);
}

// Lets the player choose a party member in side (0 or 1), starting from the party member chosen
// last.
Sint32 mnItemMenuSelectMember(Sint32 side)
{
	ITEM_MENU_STATE *menu   = MenuState;
	Sint32           result = 0;
	Sint32           memberIndex;
	UNIT            *unit;

	memberIndex = menu->sideIndex[side];
	mnWindowSpriteClearListStub(menu->descriptionWindow);
	menu->sideCount[side] = menu->partyCount;
	if (memberIndex == -1)
	{
		menu->sideIndex[side] = result;
		memberIndex           = 0;
	}
	else
	{
		mnCursorSlide(MENU_MEMBER_CURSOR_X(memberIndex), 16);
	}
	unit            = gmUnitGetStub(menu->memberIds[memberIndex]);
	menu->itemCount = mnItemMenuCollectItems(unit, menu->items, 0);
	result          = mnItemMenuRunMemberSelection(menu->memberIds, menu->items);
	mnIconShow(menu->sideIcons[side]);
	sySynch(1);
	return result;
}

// The party member row of the item menu.
Sint32 mnItemMenuRunMemberSelection(Uint16 *memberIds, Uint16 *items)
{
	ITEM_MENU_STATE *menu;
	Sint32           selection;
	Sint32           memberCount;
	Sint32           result;
	UNIT            *unit;
	Uint8            sortMode;
	Sint32           showEquipment;
	Bool             redraw;
	WINDOW          *window;
	Sint32           memberIndex;

	menu          = MenuState;
	selection     = menu->sideIndex[0];
	memberCount   = menu->sideCount[0];
	redraw        = TRUE;
	result        = 0;
	sortMode      = 0;
	showEquipment = 0;
	unit          = gmUnitGetStub(memberIds[selection]);
	if (mnWindowUpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, WINDOW_FLAG_INSTANT))
		mnSpawnIconEntries(menu, menu->itemWindow);
	if (mnWindowUpdateOrCreate(&menu->buttonHelpWindow, 13, 13, 17, 4, WINDOW_FLAG_INSTANT))
	{
		menu->selectedItemIcon = mnWindowSpriteCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->buttonHelpWindow, 0, result);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
	}
	// The help text: L + A sorts, R shows the equipment.
#if LANGUAGE != LANGUAGE_JAPANESE
	mnTextDrawMessageAsTilesStub(MsgArrangeItemsHelp, menu->buttonHelpWindow, 0, 0);
	mnTextDrawMessageAsTilesStub(MsgArrangeItemsHelp + 1, menu->buttonHelpWindow, 0, 8);
#else
	mnTextDrawStringInWindowStub(ItemMenuArrangeKeysString, menu->buttonHelpWindow, 0, 0);
	mnTextDrawMessageAsTilesStub(MsgArrangeItemsHelp, menu->buttonHelpWindow, 32, 0);
	mnTextDrawStringInWindowStub(ItemMenuEquipmentKeyString, menu->buttonHelpWindow, 16, 8);
	mnTextDrawMessageAsTilesStub(MsgArrangeItemsHelp + 1, menu->buttonHelpWindow, 32, 8);
#endif
	menu->sideIcons[0]->motion = redraw; // ICON_SHOWN
	while (!gmFlagGetStub(FLAG_MENU_START_PRESSED))
	{
		selection = (selection + memberCount) % memberCount;
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		if (redraw)
		{
			sortMode = 0;
			redraw   = FALSE;
			window   = menu->statusWindow;
			unit     = gmUnitGetStub(memberIds[selection]);
			if (showEquipment)
			{
				menu->itemCount =
					mnItemMenuCollectItems(gmUnitGetStub(memberIds[selection]), menu->items, 0);
				mnItemMenuShowEquipment(memberIds[selection]);
				mnStatusPanelDraw(window, memberIds[selection], 0, STATUS_PANEL_STATS);
			}
			else
			{
				mnItemMenuRefreshMember(memberIds[selection], 0);
				mnStatusPanelDraw(window, memberIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			}
			// Raise the chosen party member's figure.
			for (memberIndex = 0; memberIndex < 4; memberIndex++)
				menu->memberY[memberIndex] = MENU_MEMBER_Y;
			menu->memberY[selection] = MENU_MEMBER_RAISED_Y;
		}
		sySynch(1);
		// L + A sorts, cycling through the four sort orders.
		if (PerPush & KEY_A)
		{
			if (PerData & KEY_L)
			{
				sortMode = (sortMode + LIST_SORT_COUNT) % LIST_SORT_COUNT;
				mnItemMenuSort(unit->inventory, sortMode);
				sortMode++;
				mnItemMenuRefreshMember(memberIds[selection], 0);
				sySoundPlayStub(SE_MENU_CONFIRM);
			}
			else if (mnItemMenuCountItems(memberIds[selection]))
			{
				sySoundPlayStub(SE_MENU_CONFIRM);
				result = memberIds[selection];
				break;
			}
			else
			{
				sySoundPlayStub(SE_MENU_ERROR);
			}
		}
		if (PerPush & KEY_B)
		{
			sySoundPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if (PerPush & KEY_R)
		{
			showEquipment = 1;
			redraw        = TRUE;
		}
		if (!(PerData & KEY_R) && showEquipment == 1)
		{
			showEquipment = 0;
			redraw        = TRUE;
		}
		if (PerRepeat & KEY_LEFT)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selection--;
			redraw = TRUE;
		}
		if (PerRepeat & KEY_RIGHT)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selection++;
			redraw = TRUE;
		}
	}
	menu->sideIndex[0]   = selection;
	menu->selectedMember = memberIds[selection];
	menu->sideMember[0]  = memberIds[selection];
	return result;
}

// Shows the equipment view of party member memberId's items in the item window.
void mnItemMenuShowEquipment(Sint32 memberId)
{
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	mnHideEntryIcons();
	mnWindowSpriteRedrawSavedRectStub(menu->itemWindow);
	mnItemMenuDrawEquipment(menu->itemWindow, memberId, 0);
}

// Chooses the party member to use or give the selected item (enum ITEM_TARGET_MODE) to.
Sint32 mnItemMenuSelectTarget(Sint32 mode)
{

	ITEM_MENU_STATE *menu;
	WINDOW          *window;
	Sint32           memberCount;
	Sint32           memberIndex;
	Bool             redraw;
	Uint8            result;
	Bool             descriptionShown;
	Sint32           heldCount;
	WINDOW_SPR      *targetMarker;

	menu             = MenuState;
	window           = menu->itemWindow;
	memberIndex      = menu->sideIndex[1];
	memberCount      = menu->partyCount;
	redraw           = TRUE;
	result           = 0;
	descriptionShown = FALSE;
	mnWindowSetBounds(window, 13, 5, 17, 12);
	mnWindowSpriteRedrawSavedRectStub((void *)menu->itemWindow);
	gmUnitGetStub(menu->memberIds[menu->sideIndex[0]]);
	sySetEvent(mnEquipmentMenuUpdateCompatibilityIndicators, EVENT_PASS_TICK);
	while (!gmFlagGetStub(FLAG_MENU_START_PRESSED))
	{
		if (redraw)
		{
			redraw      = FALSE;
			memberIndex = (memberIndex + memberCount) % memberCount;
			window      = menu->itemWindow;
			gmUnitGetStub(menu->memberIds[memberIndex]);
			// The marker over the chosen party member, three tiles a party member; no screen makes
			// it, so it is NULL (mnCursorCreate).
			targetMarker               = menu->sideIcons[1];
			targetMarker->sprite.oam.x = targetMarker->x =
				((menu->mainWindow->x + memberIndex * 3) << 3) - 2;
			// Giving: the party member's items and how many of this one they hold.
			if (mode == ITEM_TARGET_GIVE)
			{
				mnItemMenuRefreshMember(menu->memberIds[memberIndex], 1);
				mnWindowDrawDividerLineStub(window, 0, 9, 16, 9);
				mnWindowClearInteriorTilesStub((void *)window, 0, 72, 120, 80);
				if (memberIndex != menu->sideIndex[0])
				{
					heldCount =
						mnItemMenuGetItemQuantity(menu->memberIds[memberIndex],
					                              menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
#if GIVE_TRADE_PROMPT_REPLACES_STOCK
					if (mnItemMenuCountItems(menu->memberIds[memberIndex]) == INVENTORY_SLOTS
					    && heldCount == 0)
						mnTextDrawMessageAsTilesStub(MsgTradeForWhat, window, 0, 72);
					else if (heldCount != 0)
					{
						mnTextDrawNumberInWindowStub(heldCount, 2, window, 8, 72);
						mnTextDrawMessageAsTilesStub(MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						mnTextDrawMessageAsTilesStub(MsgNoneInStock, window, 16, 72);
					}
#else
					if (heldCount != 0)
					{
						mnTextDrawNumberInWindowStub(heldCount, 2, window, 8, 72);
						mnTextDrawMessageAsTilesStub(MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						mnTextDrawMessageAsTilesStub(MsgNoneInStock, window, 16, 72);
					}
					if (mnItemMenuCountItems(menu->memberIds[memberIndex]) == INVENTORY_SLOTS
					    && heldCount == 0)
						mnTextDrawMessageAsTilesStub(
							MsgTradeForWhat, window, ITEM_TRADE_PROMPT_X, 72);
#endif
				}
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->memberIds[memberIndex]);
			}
			// Using: the party member's status, and the item's name once.
			if (mode == ITEM_TARGET_USE)
			{
				if (mnItemMenuIsStatRaisingItem(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
					mnStatusPanelDraw(menu->statusWindow,
					                  menu->memberIds[memberIndex],
					                  menu->selectedSlots[0],
					                  STATUS_PANEL_STATS);
				else
					mnStatusPanelDraw(menu->statusWindow,
					                  menu->memberIds[memberIndex],
					                  menu->selectedSlots[0],
					                  STATUS_PANEL_EXPERIENCE);
				if (!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !descriptionShown)
				{
					MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWindow);
					MENU_DRAW_DESCRIPTION((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK)
					                          + MsgItemPlainName,
					                      menu->descriptionWindow,
					                      0,
					                      0);
					descriptionShown = TRUE;
				}
				else
				{
					gmFlagClearStub(FLAG_MENU_INFO_KEPT);
				}
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(memberIndex), 16);
		sySynch(1);
		if (PerPush & KEY_A)
		{
			if (mode == ITEM_TARGET_GIVE && memberIndex == menu->sideIndex[0])
			{
				sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}
			sySoundPlayStub(SE_MENU_CONFIRM);
			result = menu->memberIds[memberIndex];
			break;
		}
		if (PerPush & KEY_B)
		{
			sySoundPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if (PerRepeat & KEY_LEFT)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			memberIndex--;
			redraw = TRUE;
		}
		if (PerRepeat & KEY_RIGHT)
		{
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
			memberIndex++;
		}
	}
	// The marker, still NULL.
	targetMarker       = menu->sideIcons[1];
	menu->sideIndex[1] = memberIndex;
	mnIconShow(targetMarker);
	targetMarker->motion = ICON_HIDDEN;
	mnEquipmentMenuStopCompatibilityIndicators();
	sySynch(1);
	menu->sideIndex[1]   = memberIndex;
	menu->selectedMember = menu->memberIds[memberIndex];
	menu->sideMember[1]  = menu->memberIds[memberIndex];
	return (Sint8)result;
}

// Event while mnItemMenuSelectTarget chooses a party member to use or give the item to, and while
// the command grid's cursor is on Give: every 32 ticks, has each party figure nod (ANIM_NOD) if
// that party member can equip the item, or stand (ANIM_STAND) if not.
void mnEquipmentMenuUpdateCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu;
	Sint8            memberIndex;

	menu = MenuState;
	if ((TickCount & 31) == 0 && menu->partyCount != 0)
	{
		memberIndex = 0;
		do
		{
			if (gmItemCanUnitEquipDirectStub(menu->memberIds[memberIndex],
			                                 menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK)
			    != 0)
			{
				SPR *partyFigure = menu->memberSprites[memberIndex];
				fdSpriteStartAnimationStub(partyFigure, ANIM_NOD);
			}
			else
			{
				SPR *partyFigure = menu->memberSprites[memberIndex];
				fdSpriteStartAnimationStub(partyFigure, ANIM_STAND);
			}
			memberIndex++;
		} while (memberIndex < menu->partyCount);
	}
}

// Ends mnEquipmentMenuUpdateCompatibilityIndicators: puts every party figure back to ANIM_STAND and
// removes the event.
void mnEquipmentMenuStopCompatibilityIndicators(void)
{
	ITEM_MENU_STATE *menu;
	Sint8            memberIndex;

	menu = MenuState;
	if (menu->partyCount != 0)
	{
		memberIndex = 0;
		do
		{
			SPR *partyFigure = menu->memberSprites[memberIndex];
			fdSpriteStartAnimationStub(partyFigure, ANIM_STAND);
			memberIndex++;
		} while (memberIndex < menu->partyCount);
	}
	syCloseEvent(&mnEquipmentMenuUpdateCompatibilityIndicators);
}

// Returns 1 for the items that raise a stat for good (Apple, Hard Nut, Mint and Lucky Pepper),
// whose use shows the stats panel instead of HP and PP.
Bool mnItemMenuIsStatRaisingItem(Sint32 itemId)
{
	switch (itemId)
	{
		case ITEM_APPLE:
		case ITEM_HARD_NUT:
		case ITEM_MINT:
		case ITEM_LUCKY_PEPPER: return TRUE;
	}
	return FALSE;
}

// Clears the message window and writes message into it.
void mnItemMenuDrawMessage(Sint32 unused, Sint32 msg)
{
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	mnWindowSpriteRedrawSavedRectStub(menu->questionWindow);
	mnTextDrawMessageAsTilesStub(msg, menu->questionWindow, 0, 0);
}

// Hides the icon of each of the 32 list entries whose item is 0. Each of the 32 icon slots must
// hold a sprite (mnSpawnIconEntries).
void mnHideEmptyEntryIcons(const Uint16 *items)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32           slot;

	for (slot = 0; slot < MENU_ENTRY_ICONS; slot++)
	{
		if (items[slot] == 0)
		{
			mnIconShow(menu->entryIcons[slot]);
			menu->entryIcons[slot]->motion = ICON_HIDDEN;
		}
	}
}

// Returns how many of party member memberId's 15 inventory slots hold an item.
Sint32 mnItemMenuCountItems(Sint32 memberId)
{
	Sint32  count     = 0;
	Uint16 *inventory = gmUnitGetStub(memberId)->inventory;
	Sint32  slot;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if ((*inventory++ & INVENTORY_ITEM_ID_MASK) != 0)
			count++;
	}
	return count;
}

// Returns how many of itemId the unit memberId holds: the quantity of its first stack, or 0. The
// Spanish and Italian editions add up every stack of it.
Sint32 mnItemMenuGetItemQuantity(Sint32 memberId, Sint32 itemId)
{
	UNIT  *unit;
	Sint32 slotIndex;
	Sint32 quantity;

	quantity = 0;
	unit     = gmUnitGetStub(memberId);
	for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++)
	{
		if (unit->inventory[slotIndex] != 0
		    && (unit->inventory[slotIndex] & INVENTORY_ITEM_ID_MASK) == itemId)
		{
#if ITEM_QUANTITY_ADDS_ALL_STACKS
			quantity +=
				(unit->inventory[slotIndex] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
#else
			quantity =
				(unit->inventory[slotIndex] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
			break;
#endif
		}
	}
	return quantity;
}

// Copies the unit's items into the list items (32 entries), skipping empty slots and clearing the
// rest. Returns how many there are. mode is not used.
Sint32 mnItemMenuCollectItems(UNIT *unit, Uint16 *items, Sint32 mode)
{
	Sint32 itemCount;
	Sint32 slotIndex;

	for (slotIndex = 0; slotIndex < MENU_ENTRY_ICONS; slotIndex++)
	{
		items[slotIndex] = 0;
	}
	itemCount = 0;
	for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++)
	{
		items[slotIndex] = 0;
		if (unit->inventory[slotIndex] != 0)
		{
			items[itemCount] = unit->inventory[slotIndex];
			itemCount++;
		}
	}
	return itemCount;
}

// Loads each listed item's icon into its item icon sprite, the first 15 entries of
// inventoryEntries, and hides the icons of empty entries. iconStyle 0 draws the list's icons
// (ICON_KIND_ITEM); any other iconStyle the equipment view's (ICON_KIND_EQUIPMENT_ITEM).
void mnItemMenuDrawIcons(Uint16 *inventoryEntries, Sint32 iconStyle)
{
	Sint32       i;
	Uint16      *entries;
	WINDOW_SPR **icons;
	Sint32       itemId;

	icons   = ((ITEM_MENU_STATE *)MenuState)->entryIcons;
	entries = inventoryEntries;
	for (i = 0; i < INVENTORY_SLOTS; i++)
	{
		itemId = *entries++;
		if (itemId != 0)
		{
			if (iconStyle == 0)
			{
				mnSelectionIconLoadIntoSlotStub(ICON_KIND_ITEM, itemId, (Uint8)(*icons)->index, 0);
			}
			else
			{
				mnSelectionIconLoadIntoSlotStub(
					ICON_KIND_EQUIPMENT_ITEM, itemId, (Uint8)(*icons)->index, 0);
			}
		}
		icons++;
	}
	mnHideEmptyEntryIcons(inventoryEntries);
}

// Shows party member memberId's items in the item window, laid out for layout mode (see
// mnItemMenuPlaceIcons), or "no items" when there are none.
void mnItemMenuRefreshMember(Sint32 memberId, Sint32 mode)
{
	ITEM_MENU_STATE *menu;
	UNIT            *member;
	Uint16          *items;

	menu            = MenuState;
	member          = gmUnitGetStub(memberId);
	items           = menu->items;
	menu->itemCount = mnItemMenuCollectItems(member, items, 0);
	mnWindowSpriteRedrawSavedRectStub(menu->itemWindow);
	mnItemMenuPlaceIcons(mode);
	mnItemMenuDrawIcons(items, 0);
	if (mnItemMenuCountItems(memberId) == 0)
		mnTextDrawMessageAsTilesStub(MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

UNUSED void mnItemMenuNoOperation(void)
{
}

// Shows in the status panel what the item in member's inventory slot would mean to member target:
// for a usable item, target's status; for equipment, target's stats with it equipped; for a
// Psynergy item, the Psynergy it teaches.
void mnItemMenuDrawEquipPreview(Sint32 member, Sint32 slot, Sint32 mode, Sint32 target)
{
	ITEM_MENU_STATE *menu           = MenuState;
	Sint32           panelStyle     = 0;
	UNIT            *unit           = gmUnitGetStub(member);
	Sint32           inventoryEntry = unit->inventory[slot];
	Sint32           previewSlot;

	// Mode 1 keeps the panel's name and statuses and redraws only below them.
	if (mode == 1)
		panelStyle = STATUS_PANEL_LOWER_ONLY;
	switch (gmItemGetDirectStub(inventoryEntry & INVENTORY_ITEM_ID_MASK)->type)
	{
		case ITEM_TYPE_NONE: // usable items
			mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
			break;
		case ITEM_TYPE_WEAPON:
		case ITEM_TYPE_ARMOR:
		case ITEM_TYPE_SHIELD:
		case ITEM_TYPE_HELM:
		case ITEM_TYPE_BOOTS:
		case ITEM_TYPE_UNUSED:
		case ITEM_TYPE_RING:
		case ITEM_TYPE_SHIRT:
			if (member == target)
			{
				panelStyle |= STATUS_PANEL_EQUIP;
				mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
			}
			else
			{
				Sint32 size;

				unit   = gmUnitGetStub(target);
				size   = BATTLE_UNIT_SIZE;
				member = (Sint32)syHeapAllocate(size);
				syWordsCopy((void *)member, unit, size);
				// With room made, add the item unequipped and preview it in its new slot; otherwise
				// the panel shows target's status without a preview.
				previewSlot = mnInventoryRemoveFirstUnflagged(target);
				if (previewSlot != 0)
				{
					inventoryEntry &= ~INVENTORY_EQUIPPED;
					previewSlot     = gmInventoryAddItemStub(target, inventoryEntry);
					if (previewSlot != -1)
					{
						panelStyle |= STATUS_PANEL_EQUIP;
						mnStatusPanelDraw(menu->statusWindow, target, previewSlot, panelStyle);
					}
					else
					{
						mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
					}
				}
				else
				{
					mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
				}
				syWordsCopy(unit, (void *)member, size);
				syHeapRelease((void *)member);
			}
			break;
		case ITEM_TYPE_PSYNERGY:
			if (target == member)
			{
				panelStyle |= STATUS_PANEL_LEARN;
				mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
			}
			else
			{
				Sint32 size;
				void  *buf;

				unit = gmUnitGetStub(target);
				size = BATTLE_UNIT_SIZE;
				buf  = syHeapAllocate(size);
				syWordsCopy(buf, unit, size);
				previewSlot = mnInventoryRemoveFirstUnflagged(target);
				if (previewSlot != 0)
				{
					previewSlot = gmInventoryAddItemStub(target, inventoryEntry);
					if (previewSlot != -1)
					{
						panelStyle |= STATUS_PANEL_LEARN;
						mnStatusPanelDraw(menu->statusWindow, target, previewSlot, panelStyle);
					}
					else
					{
						mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
					}
				}
				else
				{
					mnStatusPanelDraw(menu->statusWindow, target, slot, panelStyle);
				}
				syWordsCopy(unit, buf, size);
				syHeapRelease(buf);
			}
			break;
	}
}

// Makes room in memberId's inventory: if no slot is free, removes the whole stack in the first slot
// that is not equipped.
Sint32 mnInventoryRemoveFirstUnflagged(Sint32 memberId)
{
	UNIT  *unit   = gmUnitGetStub(memberId);
	Sint32 result = 0;
	Sint32 slotIndex;

	for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++)
	{
		if (unit->inventory[slotIndex] == 0)
		{
			result = 1;
			break;
		}
		if ((unit->inventory[slotIndex] & INVENTORY_EQUIPPED) == 0)
		{
			Sint32 quantity = unit->inventory[slotIndex] >> INVENTORY_QUANTITY_SHIFT;
			Sint32 count    = quantity + 1;
			Sint32 removedCount;

			if (quantity == 0)
				count = 1;
			for (removedCount = 0; removedCount < count; removedCount++)
				result = gmInventoryRemoveStub(memberId, slotIndex);
			if (result != INVENTORY_REMOVE_LAST)
				return 0;
			result = 1;
			break;
		}
	}
	return result;
}

// The item command cursor on the command grid's rows, 8 pixels apart from y 30, and in Japanese on
// its columns, 40 pixels apart from x 86.
#define ITEM_COMMAND_CURSOR_Y(row) ((row) * 8 + 30)
#if LANGUAGE == LANGUAGE_JAPANESE
#	define ITEM_COMMAND_CURSOR_X(column) ((column) * 40 + 86)
#endif

#if LANGUAGE != LANGUAGE_JAPANESE

// Returns the x of the item command cursor on column and row of the command grid (three commands to
// a row), or 0 when column or row is outside 0 to 2.
Sint32 mnItemMenuCommandCursorX(Sint32 column, Sint32 row)
{
	if (column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenuCommandColumnXTable[row * 3 + column];
}

// Returns the y of the item command cursor: its top row, or the row below for any other row.
Sint32 mnItemMenuCommandCursorY(Sint32 column, Sint32 row)
{
	Sint32 y;

	(void)column;
	y = ITEM_COMMAND_CURSOR_Y(0);
	if (row != 0)
	{
		y = ITEM_COMMAND_CURSOR_Y(1);
	}
	return y;
}
#endif

// Each item command's state in the grid.
#define ITEM_COMMAND_STATE_DISABLED  (-1)
#define ITEM_COMMAND_STATE_AVAILABLE 1

// The item command grid: Use, Equip and Details on the top row, Give, Remove and Drop below (grid
// index = row * 3 + column, as ITEM_MENU_COMMAND counts them).
Sint32 mnItemMenuChooseCommand(void)
{
	ITEM_MENU_STATE *menu;
	Sint8            commandStates[6];
	Sint32           column;
	Sint32           lastCommand;
	Sint32           row;
	Sint32           commandIndex;
	Bool             redraw;
#if LANGUAGE != LANGUAGE_JAPANESE
	Sint32 cursorX;
	Sint32 cursorY;
#endif

	commandIndex = 0;
	column       = 0;
	row          = 0;
	menu         = MenuState;
	redraw       = TRUE;
	mnItemMenuBuildCommands(commandStates);
	// Draw the item's name and the command grid, unless they are up already.
	if (menu->flags != 1)
	{
		WINDOW *msgWindow;

		mnItemMenuHideAllIcons();
		mnWindowSpriteRedrawSavedRectStub((void *)menu->listWindow);
		msgWindow = menu->questionWindow;
		mnItemMenuFitMessageWindowToItemChoices();
		mnWindowSpriteRedrawSavedRectStub((void *)msgWindow);
		mnWindowDrawDividerLineStub(msgWindow, 0, 3, 16, 3);
		mnItemMenuDrawItemHead();
		mnItemMenuDrawCommands(commandStates, msgWindow);
		MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWindow);
		MENU_DRAW_DESCRIPTION((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + MsgItemPlainName,
		                      menu->descriptionWindow,
		                      0,
		                      0);
	}
	menu->flags = 0;

	// Start on the command chosen last, or on the first one available. listMode is -1 until a
	// command is chosen; the later tests win, so the order of preference is Use, Remove, Equip,
	// Give, then Details.
	lastCommand = (Sint8)menu->listMode;
	if (lastCommand == -1)
	{
		if (commandStates[ITEM_COMMAND_DETAILS] == ITEM_COMMAND_STATE_AVAILABLE)
		{
			column = 2;
			row    = 0;
		}
		if (commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_AVAILABLE)
		{
			column = 0;
			row    = 1;
		}
		if (commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_AVAILABLE)
		{
			column = 1;
			row    = 0;
		}
		if (commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_AVAILABLE)
		{
			column = 1;
			row    = 1;
		}
		if (commandStates[ITEM_COMMAND_USE] == ITEM_COMMAND_STATE_AVAILABLE)
		{
			column = 0;
			row    = 0;
		}
	}
	else
	{
		column       = (Sint8)(lastCommand % 3);
		row          = (Sint8)(lastCommand / 3);
		commandIndex = row * 3 + column;
	}
#if LANGUAGE != LANGUAGE_JAPANESE
	cursorX = mnItemMenuCommandCursorX(column, row);
	cursorY = mnItemMenuCommandCursorY(column, row);
	mnCursorSlide(cursorX, cursorY);
#else
	mnCursorSlide(ITEM_COMMAND_CURSOR_X(column), ITEM_COMMAND_CURSOR_Y(row));
#endif

	for (;;)
	{
		if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
			break;
		if (redraw != 0)
		{
			redraw       = FALSE;
			column       = (column + 3) % 3;
			row          = (row + 2) % 2;
			commandIndex = row * 3 + column;
			mnEquipmentMenuStopCompatibilityIndicators();
			// Preview the item in the status panel for the command under the cursor: the bottom row
			// (Give, Remove, Drop) without it, Equip and Details with it, Use the party member's
			// status.
			if (commandIndex > 2)
			{
				menu->equipPreview = 1;
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
				if (commandIndex == ITEM_COMMAND_GIVE)
				{
					sySetEvent(&mnEquipmentMenuUpdateCompatibilityIndicators, EVENT_PASS_TICK);
				}
			}
			else if (commandIndex != 0)
			{
				menu->equipPreview = 0;
				mnItemMenuDrawEquipPreview(
					menu->sideMember[0], menu->selectedSlots[0], 0, menu->sideMember[0]);
			}
			else
			{
				mnStatusPanelDraw(
					menu->statusWindow, menu->sideMember[0], 0, STATUS_PANEL_EXPERIENCE);
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

		if ((PerPush & KEY_A) != 0)
		{
			if (commandStates[commandIndex] == ITEM_COMMAND_STATE_DISABLED)
			{
				sySoundPlayStub(SE_MENU_ERROR);
			}
			else
			{
				switch (commandIndex)
				{
					// Use
					case ITEM_COMMAND_USE    : sySoundPlayStub(SE_NOTE_MID); break;
					// Equip
					case ITEM_COMMAND_EQUIP  : sySoundPlayStub(SE_NOTE_HIGH); break;
					case ITEM_COMMAND_DETAILS:
					case ITEM_COMMAND_GIVE   :
					case ITEM_COMMAND_DROP   : sySoundPlayStub(SE_MENU_CONFIRM); break;
					// Remove
					case ITEM_COMMAND_REMOVE : sySoundPlayStub(SE_ITEM_REMOVE); break;
					default                  : sySoundPlayStub(SE_MENU_CONFIRM); break;
				}
				menu->listMode = commandIndex;
				break;
			}
		}
		if ((PerPush & KEY_B) != 0)
		{
			sySoundPlayStub(SE_MENU_CANCEL);
			commandIndex   = -1;
			menu->listMode = commandIndex;
			break;
		}

		if ((PerRepeat & KEY_UP) != 0)
		{
			row    -= 1;
			redraw  = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		else if ((PerRepeat & KEY_DOWN) != 0)
		{
			row    += 1;
			redraw  = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		else if ((PerRepeat & KEY_RIGHT) != 0)
		{
			column += 1;
			redraw  = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		else if ((PerRepeat & KEY_LEFT) != 0)
		{
			column -= 1;
			redraw  = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
	}

	menu->equipPreview = 0;
	mnEquipmentMenuStopCompatibilityIndicators();
	return commandIndex;
}

// Fills commandStates (indexed by ITEM_MENU_COMMAND) with ITEM_COMMAND_STATE_AVAILABLE or _DISABLED
// for the selected item: Use for items mnItemClassifyUseMode accepts and are not broken, and for
// field items, Equip for equipment the party member can wear and is not wearing, Remove for worn
// equipment that is not cursed, Give with someone to give to, Drop unless the item is
// indispensable, and neither for cursed equipment being worn.
void mnItemMenuBuildCommands(Sint8 *commandStates)
{
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;

	menu = MenuState;
	item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0]);

	if (item->type == ITEM_TYPE_NONE)
	{ // usable items
		commandStates[ITEM_COMMAND_USE]   = ITEM_COMMAND_STATE_AVAILABLE;
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
	}
	else
	{
		commandStates[ITEM_COMMAND_USE]   = ITEM_COMMAND_STATE_DISABLED;
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_AVAILABLE;
	}

	if (mnItemClassifyUseMode(menu->sideMember[0], menu->selectedItems[0])
	    != ITEM_USE_MODE_UNUSABLE)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
	else
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

	if (menu->selectedItems[0] & INVENTORY_BROKEN)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

	if (gmItemCanUnitEquipDirectStub(menu->sideMember[0],
	                                 menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK)
	    == 0)
	{
		commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
	}

	commandStates[ITEM_COMMAND_GIVE]    = ITEM_COMMAND_STATE_AVAILABLE;
	commandStates[ITEM_COMMAND_DROP]    = ITEM_COMMAND_STATE_AVAILABLE;
	commandStates[ITEM_COMMAND_DETAILS] = ITEM_COMMAND_STATE_AVAILABLE;

	if (menu->selectedItems[0] & INVENTORY_EQUIPPED)
	{
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_AVAILABLE;
		commandStates[ITEM_COMMAND_EQUIP]  = ITEM_COMMAND_STATE_DISABLED;
	}
	else
	{
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
	}

	if (item->flags & ITEM_CANNOT_UNEQUIP)
	{
		commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
		if (menu->selectedItems[0] & INVENTORY_EQUIPPED)
		{
			commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;
			commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
		}
	}

	if (fdHasItemCueStub(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) != 0)
		commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

	if (menu->partyCount <= 1)
		commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

	if (item->flags & ITEM_INDISPENSABLE)
		commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
}

#define TEXT_COLOR_DISABLED TEXT_COLOR_GREY // the grey of a command that refuses A

// Writes the six command names into window, the disabled ones in grey.
void mnItemMenuDrawCommands(const Sint8 *commandStates, WINDOW *window)
{
	Sint32 useState;
	Uint32 firstCommandMsg;

	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	useState = commandStates[ITEM_COMMAND_USE];
	if (useState == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);

	firstCommandMsg = MsgItemCommandUse;
	// "Use"
	mnTextDrawMessageAsTilesStub(firstCommandMsg, window, 0, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if (commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	// "Equip"
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 1, window, ITEM_COMMAND_TEXT_X, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if (commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	// "Give"
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 2, window, 0, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if (commandStates[ITEM_COMMAND_DROP] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	// "Drop"
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 3, window, 80, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if (commandStates[ITEM_COMMAND_DETAILS] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	// "Details"
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 4, window, 80, 24);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	if (commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_DISABLED)
		mnTextSetColorStub(TEXT_COLOR_DISABLED);
	// "Remove"
	mnTextDrawMessageAsTilesStub(firstCommandMsg + 5, window, ITEM_COMMAND_TEXT_X, 32);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
}

// Says how memberId can use inventoryEntry from the item menu (enum ITEM_USE_MODE).
Sint32 mnItemClassifyUseMode(Sint32 memberId, Sint32 inventoryEntry)
{
	Sint32           itemId = inventoryEntry;
	ITEM_DEFINITION *item;
	Sint32           result  = ITEM_USE_MODE_UNUSABLE;

	itemId                  &= INVENTORY_ITEM_ID_MASK;
	item                     = gmItemGetDirectStub(itemId);

	if (fdHasItemCueStub(itemId) != 0)
	{
		return ITEM_USE_MODE_FROM_FIELD;
	}

	{
		ABILITY_DEFINITION *action =
			gmAbilityGetDirectStub(item->abilityId & PSYNERGY_ABILITY_MASK);

		if (item->abilityId != 0)
		{
			if (item->type != ITEM_TYPE_NONE)
			{
				if (item->useType != ITEM_USE_TEACHES)
				{
					if (gmItemCanUnitEquipDirectStub(memberId, itemId) != 0)
					{
						result = ITEM_USE_MODE_CHOOSE_MEMBER;
					}
				}
			}
			else
			{
				result = ITEM_USE_MODE_CHOOSE_MEMBER;
			}

			if (result == ITEM_USE_MODE_CHOOSE_MEMBER)
			{
				Sint32 mask  = ABILITY_USABLE_IN_BATTLE;
				Uint8  flags = action->flags;

				if ((flags & ABILITY_USABLE_IN_FIELD) != 0)
				{
					Uint8 range = action->range;
					result      = (range == ABILITY_RANGE_ALL) ? ITEM_USE_MODE_WHOLE_PARTY
					                                           : ITEM_USE_MODE_CHOOSE_MEMBER;
				}
				else
				{
					result = (flags & mask) ? ITEM_USE_MODE_UNUSABLE : ITEM_USE_MODE_FROM_FIELD;
				}
			}
		}
	}

	return result;
}

// After the selected item is used: an item that may break (ITEM_USE_MAY_BREAK) breaks one time in
// eight, with the break sound and "It broke!" in the info window. Called only after a successful
// use on a chosen party member.
void mnItemMenuTryBreak(void)
{
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	// One time in eight: 8192 of syRandom's 65536 values.
	if ((gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0])->useType
	     == ITEM_USE_MAY_BREAK)
	    && (syRandom() < 8192U))
	{
		gmInventoryBreakStub(menu->sideMember[0], menu->selectedSlots[0]);
		sySoundPlayStub(SE_ITEM_BREAK);
		mnItemMenuShowMessage(MsgItemBroke, 0, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
		mnWindowSpriteRedrawSavedRectStub(menu->descriptionWindow);
#endif
	}
}

// Opens the details window (top left, 13 by 10 tiles) and describes the item in side itemIndex
// there. Returns 1.
Sint32 mnItemMenuOpenDetail(Sint32 itemIndex)
{
	ITEM_MENU_STATE *menu = MenuState;

	mnWindowUpdateOrCreate(&menu->equipWindow, 0, 0, 13, 10, WINDOW_FLAG_INSTANT);
	mnPaletteLoadIconsIntoHighlight();

	if (menu->selectedItems[itemIndex] != 0)
		mnItemMenuDrawItemDetails(menu->equipWindow, menu->selectedItems[itemIndex]);

	return 1;
}

// The item menu's Details screen: describes inventoryEntry in a window over the top half (13 tiles
// wide in Japanese) and waits for A or B. Up and Down step a page from 0 to 4 that nothing draws;
// returns that page, or -1 for B.
Sint32 mnItemMenuShowDetails(Sint32 inventoryEntry)
{
#if LANGUAGE != LANGUAGE_JAPANESE
	Bool             changed  = TRUE;
	ITEM_MENU_STATE *menu     = MenuState;
	WINDOW_SPR      *itemIcon = menu->selectedItemIcon;
#else
	ITEM_MENU_STATE *menu    = MenuState;
	Sint32           changed = TRUE;
#endif
	WINDOW *window;
	Sint32  page = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
	itemIcon->motion = ICON_HIDDEN;
#endif
	window = mnWindowCreateStub(0, 0, ITEM_QUANTITY_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
	// Hide the party figures (without their placement event they are not drawn) and the status
	// panel's portrait (the state's cursor), which the window covers, meanwhile.
	syCloseEvent(mnPlacePartyFigures);

	{
		WINDOW_SPR *cursor = menu->cursor;
		cursor->motion     = ICON_HIDDEN;
	}
	mnPaletteLoadIconsIntoHighlight();
	sySynch(1);

	for (;;)
	{
		if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
			break;

		if (changed != 0)
		{
			changed = FALSE;
			page    = (page + 5) % 5;
			mnItemMenuDrawItemDetails(window, inventoryEntry);
		}

		if (PerPush & KEY_A)
			break;
		if (PerPush & KEY_B)
		{
			page = -1;
			break;
		}

		if (PerRepeat & KEY_UP)
		{
			page    -= 1;
			changed  = TRUE;
		}
		if (PerRepeat & KEY_DOWN)
		{
			page    += 1;
			changed  = TRUE;
		}
		sySynch(1);
	}

	// Close the window and bring the menu back.
	mnWindowSpriteRedrawSavedRectStub(window);
	sySynch(1);
	mnWindowCloseStub((void *)window, 1);
	mnWindowSpriteRedrawSavedRectStub(menu->mainWindow);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	// As mnCreatePartyFigures schedules it.
	{
		Sint32 priority = EVENT_PASS_TICK;

		sySetEvent(mnPlacePartyFigures, priority);
	}

	// The Japanese window, 13 tiles wide, leaves the command grid at tile 13 up, so flags 1 tells
	// mnItemMenuChooseCommand not to draw it again; the international one, 30 wide, covers it, and
	// the grid's tick is redrawn.
	{
		WINDOW_SPR *cursor = menu->cursor;
#if LANGUAGE != LANGUAGE_JAPANESE
		cursor->motion = ICON_SHOWN;
#else
		cursor->motion = ICON_SHOWN;
		menu->flags    = 1;
#endif
	}
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowDrawBorderStub(13, 0, 17, 10);
#endif

	return page;
}

// Writes the description of the inventoryEntry into window, one line a row: for equipment its
// Attack and Defense bonuses, then each of its effects (flat bonuses, rates as multipliers,
// elemental Power and Resist with the element's icon, and the effects that are only text), whether
// it is cursed or bestows Psynergy, whether it is used up or may break, and for a stack how many
// there are.
void mnItemMenuDrawItemDetails(WINDOW *window, Sint32 inventoryEntry)
{
	ITEM_DEFINITION *definition;
	Sint32           i;
	Sint32           hasEffects;
	Sint32           amount;
	Sint32           bestowsPsynergy;
	Sint8            row;

	row             = 0;
	bestowsPsynergy = 0;
	definition      = gmItemGetDirectStub(inventoryEntry & INVENTORY_ITEM_ID_MASK);
	// Equipment: a heading, then its Attack and Defense bonuses.
	if (definition->type != ITEM_TYPE_NONE)
	{
		hasEffects = 0;
		if (definition->attack == 0 && definition->defense == 0)
		{
			for (i = 0; i < EQUIP_EFFECT_SLOTS; i++)
			{
				if (definition->effects[i].kind != 0 || definition->useType == ITEM_USE_TEACHES)
				{
					hasEffects = 1;
					break;
				}
			}
		}
		else
		{
			hasEffects = 1;
		}
		if (hasEffects == 1)
		{
			mnTextDrawMessageAsTilesStub(MsgEquipEffectHeading, window, 16, 0);
			row = 1;
		}
		if (definition->attack != 0)
		{
			mnTextDrawMessageAsTilesStub(MsgStatLabel, window, 0, row * 8);
			amount = definition->attack;
			mnItemMenuDrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
		if (definition->defense != 0)
		{
			mnTextDrawMessageAsTilesStub(MsgDefenseLabel, window, 0, row * 8);
			amount = definition->defense;
			mnItemMenuDrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
	}
	for (i = 0; i < EQUIP_EFFECT_SLOTS; i++)
	{
		if (definition->effects[i].kind == EQUIP_EFFECT_NONE)
			continue;
		amount = definition->effects[i].amount;
		switch (definition->effects[i].kind)
		{
			case EQUIP_EFFECT_NONE: break;
			case EQUIP_EFFECT_ADD_HP:
			case EQUIP_EFFECT_ADD_HP_REGEN:
			case EQUIP_EFFECT_ADD_PP:
			case EQUIP_EFFECT_ADD_PP_REGEN:
			case EQUIP_EFFECT_ADD_AGILITY:
			case EQUIP_EFFECT_ADD_LUCK:
			case EQUIP_EFFECT_ADD_ACTIONS:
				mnTextDrawMessageAsTilesStub(
					definition->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
				mnItemMenuDrawStat(amount, 3, window, 64, row * 8);
				break;
			// Power and Resist in each of the four elements, after the element's icon.
			case EQUIP_EFFECT_ADD_VENUS_POWER ... EQUIP_EFFECT_ADD_JUPITER_RESIST:
				mnWindowSetTilemapEntryStub(
					window,
					(Uint8)((definition->effects[i].kind - EQUIP_EFFECT_ADD_VENUS_POWER)
				            % ELEMENT_COUNT)
						+ 1,
					0,
					row,
					TILEMAP_ENTRY_HIGHLIGHT);
				mnTextDrawMessageAsTilesStub(
					definition->effects[i].kind + MsgEquipEffectName, window, 8, row * 8);
				mnItemMenuDrawStat(amount, 3, window, 64, row * 8);
				break;
			// Rates, in tenths: shown as a multiplier from 0.0 to 1.9.
			case EQUIP_EFFECT_SCALE_HP:
			case EQUIP_EFFECT_SCALE_HP_REGEN:
			case EQUIP_EFFECT_SCALE_PP:
			case EQUIP_EFFECT_SCALE_PP_REGEN:
			case EQUIP_EFFECT_SCALE_ATTACK:
			case EQUIP_EFFECT_SCALE_DEFENSE:
			case EQUIP_EFFECT_SCALE_AGILITY:
			case EQUIP_EFFECT_SCALE_LUCK:
				mnTextDrawMessageAsTilesStub(
					definition->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
#if LANGUAGE != LANGUAGE_JAPANESE
				mnTextDrawStringInWindowStub(MenuTimesString, window, 64, row * 8);
				if (amount >= STAT_SCALE_ONE)
				{
					mnTextDrawNumberInWindowStub(1, 1, window, 72, row * 8);
					mnTextDrawStringInWindowStub(MenuPointString, window, 80, row * 8);
					mnTextDrawNumberInWindowStub(amount - STAT_SCALE_ONE, 1, window, 88, row * 8);
				}
				else
				{
					mnTextDrawNumberInWindowStub(0, 1, window, 72, row * 8);
					mnTextDrawStringInWindowStub(MenuPointString, window, 80, row * 8);
					mnTextDrawNumberInWindowStub(amount, 1, window, 88, row * 8);
				}
#else
				if (amount >= STAT_SCALE_ONE)
				{
					mnTextDrawNumberInWindowStub(1, 1, window, 48, row * 8);
					mnTextDrawStringInWindowStub(MenuPointString, window, 56, row * 8);
					mnTextDrawNumberInWindowStub(amount - STAT_SCALE_ONE, 1, window, 64, row * 8);
				}
				else
				{
					mnTextDrawNumberInWindowStub(0, 1, window, 48, row * 8);
					mnTextDrawStringInWindowStub(MenuPointString, window, 56, row * 8);
					mnTextDrawNumberInWindowStub(amount, 1, window, 64, row * 8);
				}
				mnTextDrawMessageAsTilesStub(MsgEffectRateSuffix, window, 72, row * 8);
#endif
				break;
			case EQUIP_EFFECT_ADD_UNLEASH_CHANCE: // "Critical Hits increase"
			case EQUIP_EFFECT_CURSE_REMOVABLE:
			case EQUIP_EFFECT_LURE:               // "Boosts monster encounters"
				mnTextDrawMessageAsTilesStub(
					definition->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
				break;
		}
		row++;
	}
	// Curses, bestowed Psynergy and how the item is used up.
	if (definition->flags & ITEM_CURSED)
	{
		mnTextDrawMessageAsTilesStub(MsgItemCursed, window, 0, row * 8);
		row++;
	}
	if (definition->useType == ITEM_USE_TEACHES)
	{
		mnTextDrawMessageAsTilesStub(MsgBestowsPsynergy, window, 0, row * 8);
		bestowsPsynergy = 1;
		row++;
	}
	// "Number of Uses" and what using it costs, except for items that cost nothing and Hermes'
	// Water (ITEM_USE_EMPTIES); a Psynergy item has no heading and no line.
	if (definition->useType != ITEM_USE_EMPTIES && definition->useType != ITEM_USE_FREE)
	{
		if (!bestowsPsynergy)
		{
			mnTextDrawMessageAsTilesStub(MsgUsesHeading, window, 16, row * 8);
			row++;
		}
		switch (definition->useType)
		{
			case ITEM_USE_FREE: break;
			case ITEM_USE_CONSUMED:
				mnTextDrawMessageAsTilesStub(MsgSingleUse, window, 0, row * 8);
				row++;
				break;
			case ITEM_USE_MAY_BREAK:
			{
				Sint32 m;

				// Two lines each: "It's broken, so" / "it can't be used", or "It might break" / "if
				// used in battle".
				if (inventoryEntry & INVENTORY_BROKEN)
				{
					m = MsgBrokenNotice;
					mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
					row++;
					m++;
					mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
					row++;
				}
				else
				{
					m = MsgMightBreak;
					mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
					row++;
					m++;
					mnTextDrawMessageAsTilesStub(m, window, 0, row * 8);
					row++;
				}
				break;
			}
		}
	}
	// How many there are in a stack.
	if (definition->flags & ITEM_STACKABLE)
	{
		if (row != 0)
			row++;
		i = MsgNumberHeading;
		mnTextDrawMessageAsTilesStub(i, window, 16, row * 8);
		row++;
		amount = (inventoryEntry & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
		// The quantity field holds the count less one.
#if LANGUAGE != LANGUAGE_JAPANESE
		mnMessageArgumentPushStub(amount + 1, MESSAGE_ARGUMENT_NUMBER);
		mnTextDrawMessageAsTilesStub(i + 1, window, 0, row * 8);
#else
		mnTextDrawNumberInWindowStub(amount + 1, 2, window, 0, row * 8);
		mnTextDrawMessageAsTilesStub(i + 1, window, 16, row * 8);
#endif
		row++;
	}
	// Nothing written yet: rare, important, or nothing known.
	hasEffects = 0;
	if (row == 0)
	{
		if (definition->flags & ITEM_RARE)
		{
			mnTextDrawMessageAsTilesStub(MsgRareItem, window, 0, 0);
			hasEffects = 1;
		}
		if (!hasEffects)
		{
			if (definition->flags & ITEM_INDISPENSABLE)
			{
				mnTextDrawMessageAsTilesStub(MsgImportantItem, window, 0, 0);
				hasEffects = 1;
			}
			if (!hasEffects)
				mnTextDrawMessageAsTilesStub(MsgDetailsUnknown, window, 0, 0);
		}
	}
}

// Writes a signed stat change at (x, y) of window: the number, with a plus or minus sign just
// before its first digit.
void mnItemMenuDrawStat(Sint32 statChange, Sint32 unused, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32 digits;
	Sint32 magnitude;

	mnTextDrawNumberInWindowStub(statChange, 3, window, x, y);
	// Count the digits of |statChange| to place the sign.
	digits    = 1;
	magnitude = statChange;
	if (statChange < 0)
	{
		magnitude = -statChange;
	}
	if (magnitude > 9)
	{
		digits = 2;
	}
	magnitude = statChange;
	if (statChange < 0)
	{
		magnitude = -statChange;
	}
	if (magnitude > 99)
	{
		digits = 3;
	}
	if (statChange > 0)
	{
		mnTextDrawStringInWindowStub(&MenuPlusSignString, window, x - digits * 8 + 16, y);
	}
	else
	{
		mnTextDrawStringInWindowStub(&MenuMinusSignString, window, x - digits * 8 + 16, y);
	}
}

// Moves the item window to row 5 of the right-hand side, below a question headed by the item.
void mnItemMenuPlaceItemWindowBelowItemQuestion(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->itemWindow, 13, 5, 17, 10);
}

// Moves the item window to row 3 of the right-hand side, below a one-line question.
void mnItemMenuPlaceItemWindowBelowQuestion(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->itemWindow, 13, 3, 17, 10);
}

// Makes the message window, along the top right, 5 rows tall: the item's name and a question below
// it.
void mnItemMenuFitMessageWindowToItemQuestion(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWindow, 13, 0, 17, 5);
}

// Makes the message window 6 rows tall, for the longer questions outside Japanese.
void mnItemMenuFitMessageWindowToLongItemQuestion(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWindow, 13, 0, 17, 6);
}

// Makes the message window 7 rows tall: the item's name above the command grid or the quantity
// dial.
void mnItemMenuFitMessageWindowToItemChoices(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWindow, 13, 0, 17, 7);
}

// Makes the message window 3 rows tall, for a one-line question.
void mnItemMenuFitMessageWindowToQuestion(void)
{
	mnWindowSetBounds((WINDOW *)((ITEM_MENU_STATE *)MenuState)->questionWindow, 13, 0, 17, 3);
}

// Asks how many of the selected item stack to give or drop: Left and Right choose a number from 1
// to quantityRange (shown as baseQuantity + 1 up to baseQuantity + quantityRange on a dial of digit
// sprites), with the party member's remaining count and, when giving (dropMode 0), the receiver's
// new count beside their names.
Sint32 mnItemMenuSelectGiveQuantity(Sint32 baseQuantity, Sint32 quantityRange, Sint32 dropMode)
{
	Sint32           quantity = baseQuantity;
	Sint32           block;
	Sint32           changed;
	Sint32           receiverCount;
	Sint32           memberCount;
	ITEM_MENU_STATE *menu  = MenuState;
	// 1 KB of HEAP_SLOT_TILE_SCRATCH to draw the dial's digits in, released before returning.
	void            *tiles = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	WINDOW          *window;
	WINDOW_SPR      *rightDial;

	changed       = 1;
	receiverCount = 0;
	window        = menu->questionWindow;
	mnItemMenuFitMessageWindowToItemChoices();
	mnWindowSpriteRedrawSavedRectStub(window);
	// How many each party member holds now.
	if (dropMode == 0)
		receiverCount = mnItemMenuGetItemQuantity(menu->sideMember[1],
		                                          menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	memberCount = mnItemMenuGetItemQuantity(menu->sideMember[0],
	                                        menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
	// Two dial sprites, left and right of the number.
	block       = syVRAMCacheFindFreeEntry();
	// VRAM_CACHE_ENTRY_COUNT: no free VRAM block, and no dial.
	if (block != VRAM_CACHE_ENTRY_COUNT)
	{
		syVRAMCacheLoad(block, 256, 0);
		mnWindowSpriteCreateStub(block, OUTPUT_SIZE_32X8, window, 48, 32);
		rightDial = mnWindowSpriteCreateStub(block, OUTPUT_SIZE_32X8, window, 80, 32);
		rightDial->sprite.oam.index += 4;
		mnCursorSlide(ITEM_GIVE_CURSOR_X, 40);
		while (!gmFlagGetStub(FLAG_MENU_START_PRESSED))
		{
			if (changed)
			{
				changed  = 0;
				quantity = (quantityRange + quantity) % quantityRange;
				mnWindowSpriteRedrawSavedRectStub(window);
				mnTextDrawMessageAsTilesStub(
					MsgGiveHowMany, window, ITEM_GIVE_ASK_X, ITEM_GIVE_ASK_Y);
				syDMAXCopy(MenuQuantityTiles, tiles, DMA_COPY_WORDS(64), REG_DMA3);
				mnQuantityBarFillStub(INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH, tiles);
				mnQuantityBarFillStub(quantityRange + baseQuantity, QUANTITY_BAR_CHOOSABLE, tiles);
				mnQuantityBarFillStub(baseQuantity + quantity + 1, QUANTITY_BAR_CHOSEN, tiles);
				mnQuantityBarFillStub(baseQuantity, QUANTITY_BAR_HELD, tiles);
				syVRAMCacheLoad(block, 256, tiles);
				mnTextDrawNumberInWindowStub(quantity + 1, 2, window, 32, 32);
				mnTextDrawMessageAsTilesStub(
					(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName, window, 16, 8);
				mnTextDrawNumberInWindowStub(memberCount - quantity - 1, 2, window, 16, 24);
#if LANGUAGE != LANGUAGE_JAPANESE
				if (dropMode == 0)
					mnTextDrawNumberInWindowStub(receiverCount + quantity + 1, 2, window, 80, 24);
#else
				mnTextDrawMessageAsTilesStub(MsgItemCounter, window, 32, 24);
				if (dropMode == 0)
				{
					mnTextDrawNumberInWindowStub(receiverCount + quantity + 1, 2, window, 80, 24);
					mnTextDrawMessageAsTilesStub(MsgItemCounter, window, 96, 24);
				}
#endif
				mnTextDrawStringAtOffsetStub(
					gmUnitGetStub(menu->sideMember[0])->name, window, 16, 16);
				if (dropMode == 0)
					mnTextDrawStringAtOffsetStub(
						gmUnitGetStub(menu->sideMember[1])->name, window, 80, 16);
			}
			// A confirms, B backs out, Left and Right change the number.
			if (PerPush & KEY_A)
			{
				sySoundPlayStub(SE_MENU_CONFIRM);
				break;
			}
			if (PerPush & KEY_B)
			{
				quantity = -1;
				sySoundPlayStub(SE_MENU_CANCEL);
				break;
			}
			mnCursorPos(ITEM_GIVE_CURSOR_X, 40);
			if (PerRepeat & KEY_LEFT)
			{
				quantity--;
				changed = 1;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			if (PerRepeat & KEY_RIGHT)
			{
				quantity++;
				changed = 1;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	mnWindowSpriteRedrawSavedRectStub(window);
	mnWindowSpriteClearListStub((void *)window);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	menu->selectedItemIcon->motion = ICON_HIDDEN;
	if (gmFlagGetStub(FLAG_MENU_START_PRESSED))
		quantity = -1;
	return quantity;
}

// Shows the selected item at the top of the message window: its icon, its party member's name and
// its name.
void mnItemMenuDrawItemHead(void)
{
	ITEM_MENU_STATE *menu = MenuState;

	mnSelectionIconLoadIntoSlotStub(
		ICON_KIND_ITEM, menu->selectedItems[0], (Uint8)menu->selectedItemIcon->index, 0);
	menu->selectedItemIcon->motion = ICON_SHOWN;
	menu->selectedItemIcon->x      = 112;
	menu->selectedItemIcon->y      = 8;
	mnIconShow(menu->selectedItemIcon);
	mnTextDrawStringAtOffsetStub(
		gmUnitGetStub(menu->sideMember[0])->name, menu->questionWindow, 16, 0);
	mnTextDrawMessageAsTilesStub((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
	                             menu->questionWindow,
	                             16,
	                             8);
}

// Asks whether to drop item (an inventory entry): its name, the question, and Yes or No, No first.
// Returns 0 for Yes and 1 for No, B or Start.
Sint32 mnItemMenuConfirmDrop(Sint32 item)
{
	WINDOW *window;
	Sint32  itemId;
	Sint32  question;
	Sint32  answer;
	Sint32  rows      = 2;
	Sint32  selection = 1;
	Bool    changed   = TRUE;

	window            = mnWindowCreateStub(13, 3, 17, 10, WINDOW_FLAG_INSTANT);
	itemId            = item & INVENTORY_ITEM_ID_MASK;
	gmItemGetDirectStub(itemId);
	mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window, 24, 0);
	question = MsgConfirmDrop;
	mnTextDrawMessageAsTilesStub(question, window, ITEM_DROP_QUESTION_X, 16);
	question++;
	mnTextDrawMessageAsTilesStub(question, window, ITEM_DROP_QUESTION_X, 24);
	answer = MsgYes;
	mnTextDrawMessageAsTilesStub(answer, window, 24, 40);
	answer++;
	mnTextDrawMessageAsTilesStub(answer, window, 24, 56);
	mnCursorSlide(104, 86);
	for (;;)
	{
		if (gmFlagGetStub(FLAG_MENU_START_PRESSED))
			break;
		if (changed)
		{
			changed   = FALSE;
			selection = (selection + rows) % rows;
		}
		if (PerPush & KEY_A)
		{
			sySoundPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if (PerPush & KEY_B)
		{
			sySoundPlayStub(SE_MENU_CANCEL);
			selection = 1;
			break;
		}
		mnCursorPos(104, (selection << 4) + 70);
		if (PerRepeat & KEY_UP)
		{
			selection--;
			changed = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		if (PerRepeat & KEY_DOWN)
		{
			selection++;
			changed = TRUE;
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		}
		sySynch(1);
	}
	if (gmFlagGetStub(FLAG_MENU_START_PRESSED))
	{
		selection = 1;
	}
	mnWindowCloseStub((void *)window, 1);
	return selection;
}

// After giving or trading the selected item to the party member in side 1, offers to equip it
// there, showing the stats it would give.
Sint32 mnItemMenuConfirmEquip(Sint32 unused)
{
	Sint32           rows      = 2;
	Sint32           selection = 0;
	Bool             changed   = TRUE;
	ITEM_MENU_STATE *menu      = MenuState;
	UNIT            *unit      = gmUnitGetStub(menu->sideMember[1]);
	UNIT            *saved;
	WINDOW          *window;
	Sint32           slot;
	Sint32           member;

	slot   = menu->selectedSlots[1];
	member = menu->sideMember[1];
	mnItemMenuDrawEquipPreview(member, slot, 0, member);
	saved = syHeapAllocate(BATTLE_UNIT_SIZE);
	syWordsCopy(saved, unit, BATTLE_UNIT_SIZE);
	window = menu->questionWindow;
	// Equip it now; INVENTORY_EQUIP_REFUSED or INVENTORY_EQUIP_STUCK means it cannot be equipped.
	if ((Uint32)(gmInventoryEquipStub(menu->sideMember[1], menu->selectedSlots[1])
	             - INVENTORY_EQUIP_STUCK)
	    <= INVENTORY_EQUIP_REFUSED - INVENTORY_EQUIP_STUCK)
	{
		selection = 1;
	}
	else
	{
#if LANGUAGE != LANGUAGE_JAPANESE
		mnTextDrawMessageAsTilesStub(MsgYes, window, 24, 24);
		mnTextDrawMessageAsTilesStub(MsgYes + 1, window, 72, 24);
		mnWindowClearInteriorTilesStub((void *)window, 16, 16, 96, 24);
		mnTextDrawMessageAsTilesStub(MsgEquipThisItem, window, 0, 16);
		mnCursorSlide(110, 32);
#else
		mnTextDrawMessageAsTilesStub(MsgYes, window, 96, 0);
		mnTextDrawMessageAsTilesStub(MsgYes + 1, window, 96, 16);
		mnWindowClearInteriorTilesStub((void *)window, 16, 16, 96, 24);
		mnTextDrawMessageAsTilesStub(MsgEquipThisItem, window, 8, 16);
		mnCursorSlide(184, 5);
#endif
		for (;;)
		{
			if (gmFlagGetStub(FLAG_MENU_START_PRESSED))
				break;
			if (changed)
			{
				changed   = FALSE;
				selection = (selection + rows) % rows;
			}
			if (PerPush & KEY_A)
			{
				sySoundPlayStub(SE_NOTE_HIGH); // the Equip sound
				break;
			}
			if (PerPush & KEY_B)
			{
				sySoundPlayStub(SE_MENU_CANCEL);
				selection = 1;
				break;
			}
#if LANGUAGE != LANGUAGE_JAPANESE
			mnCursorPos(selection * 48 + 110, 32);
			if (PerRepeat & KEY_LEFT)
			{
#else
			mnCursorPos(184, (selection << 4) + 5);
			if (PerRepeat & KEY_UP)
			{
#endif
				selection--;
				changed = TRUE;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
#if LANGUAGE != LANGUAGE_JAPANESE
			if (PerRepeat & KEY_RIGHT)
			{
#else
			if (PerRepeat & KEY_DOWN)
			{
#endif
				selection++;
				changed = TRUE;
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			}
			sySynch(1);
		}
	}
	if (gmFlagGetStub(FLAG_MENU_START_PRESSED))
		selection = 1;
	// No: put the party member back as they were.
	if (selection == 1)
		syWordsCopy(unit, saved, BATTLE_UNIT_SIZE);
	syHeapRelease(saved);
	gmUnitCalculateStub(menu->sideMember[1]);
	gmUnitRefreshPsynergyStub(menu->sideMember[1]);
	return selection;
}

// Loads the stat arrows (up and down) into two free VRAM blocks, kept in the menu state's resource
// slots.
void mnLoadStatArrows(void)
{
	ITEM_MENU_STATE *menu  = MenuState;
	Sint32           block = syVRAMCacheFindFreeEntry();

	menu->resourceSlots[0] = block;
	syVRAMCacheLoad(block, 128, MenuArrowTiles);
	block                  = syVRAMCacheFindFreeEntry();
	menu->resourceSlots[1] = block;
	syVRAMCacheLoad(block, 128, MenuSmallDownArrowTiles);
}

// Fills result with the item list of the party member in side index: their unit, how many items
// they hold, and the page and row of the item chosen last for them (kept within the list), five
// items to a page. Returns 1.
Sint32 mnItemMenuBuildPagedList(PAGED_LIST *result, Sint32 index)
{
	Sint32           unit;
	ITEM_MENU_STATE *menu = MenuState;
	Sint32           itemCount;
	Sint32           row;
	Sint32           page;
	Sint32           pageCount;
	Sint32           selected;

	itemCount = mnItemMenuCountItems(menu->sideMember[index]);
	unit      = (Sint32)gmUnitGetStub(menu->sideMember[index]);
	selected  = menu->selectedIndexByMember[menu->sideMember[index]];
	if ((selected + 1) > itemCount)
	{
		selected = itemCount - 1;
	}
	page      = selected / LIST_PAGE_SIZE;
	row       = selected % LIST_PAGE_SIZE;
	pageCount = itemCount / LIST_PAGE_SIZE;
	if (itemCount % LIST_PAGE_SIZE != 0)
	{
		pageCount++;
	}
	result->memberState   = unit;
	result->page          = page;
	result->pageCount     = pageCount;
	result->row           = row;
	result->entryCount    = itemCount;
	result->selectedIndex = selected;
	return 1;
}

// Updates the item list for the row under the cursor: puts the selected item's plain name in the
// info window and highlights its row of the five. window and memberState are not used.
Sint32 mnItemMenuShowSelectedRow(WINDOW *window, void *memberState, PAGED_LIST *pageState)
{
	ITEM_MENU_STATE *menu;
	Sint32           page;
	Sint32           index;
	Sint32           row;

	page                      = pageState->page;
	menu                      = MenuState;
	index                     = page * LIST_PAGE_SIZE;
	index                    += pageState->row;
	pageState->selectedIndex  = index;

	MENU_RESET_DESCRIPTION_WINDOW(menu->descriptionWindow);
	sySynch(1);

	index = pageState->selectedIndex;
	if (menu->items[index] != 0)
	{
		Sint32 name = (menu->items[index] & INVENTORY_ITEM_ID_MASK) + MsgItemPlainName;
		MENU_DRAW_DESCRIPTION(name, menu->descriptionWindow, 0, 0);
	}

	for (row = 0; row < LIST_PAGE_SIZE; row++)
	{
		if (row == pageState->row)
		{
			mnWindowSetHighlightRect(
				(void *)menu->itemWindow, 1, row * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
		}
		else
		{
			mnWindowSetHighlightRect(
				(void *)menu->itemWindow, 1, row * 2 + 1, 14, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

// Draws one page of the item list into window: up to five item names with their icons, and the page
// numbers when there is more than one page.
Sint32 mnItemMenuDrawListPage(WINDOW *window, Sint32 unused, const PAGED_LIST *pageState)
{
	ITEM_MENU_STATE *menu = MenuState;
	Uint32           page;
	Uint32           firstEntry;
	Uint32           visibleCount;
	Uint8            row;
	const Uint16    *entry;

	(void)unused;

	mnWindowSpriteRedrawSavedRectStub(window);
	mnWindowDrawDividerLineStub(window, 0, 11, 16, 11);

	page         = pageState->page;
	firstEntry   = page * LIST_PAGE_SIZE;
	visibleCount = (Uint8)(pageState->entryCount - firstEntry);
	if (visibleCount > LIST_PAGE_SIZE)
	{
		visibleCount = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_LIST_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(
		window, pageState->entryCount, LIST_PAGE_SIZE, pageState->page, PAGE_NUMBERS_RIGHT);

	row = 0;
	if (visibleCount > row)
	{
		entry = &menu->items[firstEntry];
		do
		{
			mnTextDrawMessageAsTilesStub((entry[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
			                             menu->itemWindow,
			                             ITEM_LIST_NAME_X,
			                             row * 16 + 8);
			row++;
			entry++;
		} while (visibleCount > row);
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

// The item list of the party member in sideIndex (0: the holder, 1: the receiver of a trade), five
// items a page.
Sint32 mnItemMenuRunList(Sint32 sideIndex)
{
	ITEM_MENU_STATE *menu;
	WINDOW          *window;
	Bool             changed;
	Sint32           moved;
	Sint32           member;
	Uint8            i;
	Sint32           result;
	Bool             redraw;
	Bool             done;
	UNIT            *memberUnit;
	Sint32           prev;
	WINDOW_SPR      *icon;
	Sint32           memberState[5];
	PAGED_LIST       state;

	menu               = MenuState;
	result             = 0;
	prev               = 0;
	menu->equipPreview = result;
	mnWindowUpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	window = menu->listWindow;
	done   = FALSE;

	while (done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
	{
		memberUnit      = gmUnitGetStub(menu->sideMember[sideIndex]);
		menu->itemCount = mnItemMenuCollectItems(memberUnit, menu->items, 0);
		mnItemMenuDrawIcons(menu->items, 0);
		menu->selectedItemIcon->motion = ICON_HIDDEN;
		mnItemMenuBuildPagedList(&state, sideIndex);
		mnCursorPos(98, state.selectedIndex * 16 + 36);
		changed = TRUE;
		redraw  = TRUE;

		// One party member's list, until an item is chosen or the party member changes.
		while (gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0)
		{
			mnCursorPos(98, state.row * 16 + 36);

			if (changed != 0)
			{
				changed = FALSE;
				if (memberUnit->inventory[prev] != 0)
				{
					icon = menu->entryIcons[prev];
					mnIconShow(icon);
				}
				if (redraw != 0)
				{
					sySynch(1);
					mnItemMenuDrawListPage(window, 0, &state);
					if (sideIndex == 0)
					{
						mnTextDrawMessageAsTilesStub(
							MsgChangeCharacterHelp, window, CHANGE_CHARACTER_HELP_X, 88);
					}
					redraw = FALSE;
				}
				mnItemMenuShowSelectedRow(window, memberState, &state);
				menu->selectedItems[sideIndex] = menu->items[state.selectedIndex];
				mnItemMenuDrawEquipPreview(menu->sideMember[sideIndex],
				                           state.selectedIndex,
				                           0,
				                           menu->sideMember[sideIndex]);
				if (menu->items[state.selectedIndex] != 0)
				{
					icon                = menu->entryIcons[state.selectedIndex];
					// Swell the selected icon from its first step, in depth slot 250; mnIconShow
					// stills it again when the cursor leaves.
					icon->motion        = WINDOW_SPRITE_MOTION_SWELL;
					icon->animationStep = 0;
					icon->depth         = OAM_DEPTH_FRONT - 5;
				}
				for (i = 0; i < menu->partyCount; i++)
				{
					fdSpriteStartAnimationStub(menu->memberSprites[i], ANIM_STAND);
				}
			}

			// Every 32 ticks, the party members who can equip the item nod.
			if ((TickCount & 31) == 0)
			{
				for (i = 0; i < menu->partyCount; i++)
				{
					if (gmItemCanUnitEquipDirectStub(menu->memberIds[i],
					                                 INVENTORY_ITEM_ID_MASK
					                                     & menu->items[state.selectedIndex])
					    != 0)
					{
						fdSpriteStartAnimationStub(menu->memberSprites[i], ANIM_NOD);
					}
				}
			}

			sySynch(1);
			prev  = state.selectedIndex;
			moved = mnPagedListHandleInput(
				0, state.entryCount, LIST_PAGE_SIZE, &state.row, &state.page);
			if (moved == 1)
			{
				redraw  = TRUE;
				changed = TRUE;
			}
			if (moved == 0)
			{
				changed = TRUE;
			}
			if (moved == -1)
			{
				changed = FALSE;
			}

			if ((PerPush & KEY_A) != 0 && menu->items[state.selectedIndex] != 0)
			{
				sySoundPlayStub(SE_NOTE_LOW); // the item choice sound
				result = menu->items[state.selectedIndex];
				done   = TRUE;
				break;
			}
			if ((PerPush & KEY_B) != 0)
			{
				sySoundPlayStub(SE_MENU_CANCEL);
				result = -1;
				done   = TRUE;
				break;
			}
			// L and R: the previous or next party member who has items (side 0 only).
			if ((PerRepeat & KEY_R) != 0 || (PerRepeat & KEY_L) != 0)
			{
				if (sideIndex == 1)
				{
					sySoundPlayStub(SE_MENU_ERROR);
					sySynch(1);
				}
				else
				{
					sySoundPlayStub(SE_MENU_CURSOR_MOVE);
					menu->selectedIndexByMember[menu->sideMember[sideIndex]] = state.selectedIndex;
					member = menu->sideIndex[sideIndex];
					do
					{
						if ((PerRepeat & KEY_R) != 0)
						{
							member++;
						}
						else
						{
							member--;
						}
						member               = (member + menu->partyCount) % menu->partyCount;
						menu->selectedMember = menu->memberIds[member];
						menu->sideMember[sideIndex] = menu->memberIds[member];
						menu->sideIndex[sideIndex]  = member;
						menu->itemCount             = mnItemMenuCollectItems(
                            gmUnitGetStub(menu->sideMember[sideIndex]), menu->items, 0);
					} while (menu->itemCount == 0);
					for (i = 0; i <= 3; i++)
					{
						menu->memberY[i] = MENU_MEMBER_Y;
					}
					menu->memberY[member] = MENU_MEMBER_RAISED_Y;
					break;
				}
			}
		}
	}

	mnWindowClearInteriorTilesStub((void *)window, 0, 88, 120, 96);
	mnIconShow(menu->gridCursor);
	menu->selectedSlots[sideIndex]                           = state.selectedIndex;
	menu->selectedIndexByMember[menu->sideMember[sideIndex]] = state.selectedIndex;
	menu->selectedItems[sideIndex]                           = result;
	if (gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0)
	{
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
