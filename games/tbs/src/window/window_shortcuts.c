/* @file */
/* The area name shown on entering an area, the debug Psynergy shortcut picker
 */
/* and the unused selection state. */
/* Part of window.c. */

/* The entries of the debug shortcut picker's list, which */
/* mnFindShortcutEntries searches whole. */
#define SHORTCUT_LIST_ENTRIES 448
/* A shortcut (MainRoutineStateWork.firstShortcut and secondShortcut) is */
/* member << SHORTCUT_MEMBER_SHIFT | action, as mnDebugSelectShortcuts stores */
/* it. */
#define SHORTCUT_MEMBER_SHIFT 10
#define SHORTCUT_ABILITY_MASK 0x3ff

/* Shows the area's name in a window centred in the screen's top ten rows, for
 */
/* AREA_NAME_WINDOW_FRAMES; a per-frame task closes it. */
void mnAreaNameWindowShow(void) {
	FIELD_STATE *notice;
	s32 height;
	s32 width;
	s32 y;
	s32 x;
	s32 areaMessage;
	WINDOW *nameWindow;

	notice = FieldStateWork;
	x = 8;
	y = 8;
	areaMessage = fdAreaGetNameIndexStub(MainRoutineStateWork.next,
										 MainRoutineStateWork.entrance) +
				  MsgDebugAreaName;
	mnTextGetMessageSize(areaMessage, &x, &y, &width, &height);
	x = (DISPLAY_TILE_WIDTH - width) >> 1;
	y = (10 - height) >> 1;
	nameWindow = mnWindowCreate(x, y, width, height, WINDOW_FLAG_INSTANT);
	notice->areaNameWin = nameWindow;
	mnTextDrawMessage(areaMessage, nameWindow, 0, 0);
	notice->areaNameCountdown = AREA_NAME_WINDOW_FRAMES;
	sySetEvent(mnAreaNameWindowUpdate, TASK_PASS_FRAME);
}

/* Counts the area name down, and closes it at 0. */
void mnAreaNameWindowUpdate(void) {
	FIELD_STATE *notice;
	u16 framesLeft;

	notice = FieldStateWork;
	/* Adding (u16)-1 takes one off in 16 bits. */
	notice->areaNameCountdown =
		(framesLeft = notice->areaNameCountdown + (u16)-1);
	if(framesLeft == 0) {
		mnWindowClose(notice->areaNameWin, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

/* Closes the area name at once, if it is shown. */
void mnAreaNameWindowHide(void) {
	WINDOW *window;

	window = FieldStateWork->areaNameWin;
	if(window != NULL && window->flg != 0) {
		mnWindowClose(window, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

/* Removes itemId from the party's inventory and returns 0. */
s32 mnItemRemoveFromInventory(s32 unused, s32 itemId) {
	gmPartyInventoryRemoveStub(itemId);
	return 0;
}

/* Returns 1; the overlays reach it through the stub table. */
s32 mnItemReturnTrue(void) { return 1; }

/* Steps the window hue one step: back when keys contains LEFT, on otherwise. */
UNUSED void mnSettingsMenuStepWindowHue(s32 keys) {
	u8 hue = MainRoutineStateWork.winHue;
	if(keys & KEY_LEFT)
		hue += (u8)-1;
	else
		hue += 1;
	MainRoutineStateWork.winHue = hue;
}

/* A Psynergy the debug shortcut picker lists: its party member and the */
/* Psynergy. */
typedef struct ABILITY_LIST_ENTRY {
	u16 member;	 /* the party member's unit id */
	u16 ability; /* the action id (SHORTCUT_ABILITY_MASK in a shortcut) */
} ABILITY_LIST_ENTRY;

/* Lets the player pick two Psynergy shortcuts from all the party's Psynergy, */
/* showing each one's cost and description, and stores them in */
/* MainRoutineStateWork.firstShortcut and secondShortcut (member << */
/* SHORTCUT_MEMBER_SHIFT | Psynergy). */
void mnDebugSelectShortcuts(void) {
	ABILITY_LIST_ENTRY *list;
	WINDOW *picks;
	WINDOW *title;
	WINDOW *info;
	WINDOW_SPR *cursor;
	u32 first;
	u32 second;
	u32 count;
	s32 done = 0;
	s32 row = 0;
	s32 redraw;
	s32 slot;
	s32 y;
	s32 cost;
	s32 ability;

	cursor = NULL;
	redraw = 1;
	/* Build the debug shortcut candidates before creating their three windows.
	 */
	list = syHeapAlloc(SHORTCUT_LIST_ENTRIES * sizeof(ABILITY_LIST_ENTRY));
	gmUnitLearnPsynergyStub(ISAAC, ABILITY_MOVE);
	gmUnitLearnPsynergyStub(GARET, ABILITY_MOVE);
	gmUnitLearnPsynergyStub(IVAN, ABILITY_MOVE);
	gmUnitLearnPsynergyStub(IVAN, ABILITY_MIND_READ);
	gmUnitLearnPsynergyStub(IVAN, ABILITY_WHIRLWIND);
	gmUnitLearnPsynergyStub(MIA, ABILITY_PLY);
	gmUnitLearnPsynergyStub(JENNA, ABILITY_MOVE);
	second = 0;
	first = 0;
	count = mnPartyListPsynergy(list);
	if(count != 0) {
		mnFindShortcutEntries(&first, &second, list);
		picks = mnWindowCreate(4, 6, 20, 7, WINDOW_FLAG_INSTANT);
		title = mnWindowCreate(4, 3, 20, 3, WINDOW_FLAG_INSTANT);
		info = mnWindowCreate(4, 14, 20, 5, WINDOW_FLAG_INSTANT);
		/* syVramCacheFindFreeEntry reports no free entry as
		 * VRAM_CACHE_ENTRY_COUNT, */
		/* not 0: this test lets that through, and with entry 0 it leaves cursor
		 */
		/* NULL for the writes below. */
		slot = syVramCacheFindFreeEntry();
		if(slot != 0) {
			syVramCacheLoad(slot, 128, PointingHandTiles);
			cursor =
				mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, picks, 0, 0);
		}
		/* The picker borrows the status panel's labels: "HP/Max" over it, */
		/* "Status" over the picks, "Attack" and "Defense" (the two messages
		 * before */
		/* "Status") before them. slot, done with the cursor's VRAM entry,
		 * carries */
		/* the first. */
		slot = MsgPanelHpMaxLabel;
		mnTextDrawMessageAsTiles(slot, title, 16, 0);
		/* Redraw both shortcut rows and the selected ability's detail side. */
		while(!done) {
			if(redraw != 0) {
				redraw = 0;
				/* Wrap both picks and the row; a step below 0 wraps through the
				 */
				/* unsigned sum. */
				first = (first + count) % count;
				second = (second + count) % count;
				row = (row + 2) % 2;
				y = (row << 4) + (picks->y << 3) + 28;
				cursor->y = y;
				cursor->spr.oam.y = y;
				mnWindowSprRedrawSavedRect(picks);
				mnWindowDrawDividerLine(picks, 1, 2, 17, 2);
				mnTextDrawMessageAsTiles(MsgPanelStatusLabel, picks, 48, 0);
				mnTextDrawMessageAsTiles(list[first].ability + MsgAbilityName,
										 picks, 56, 16);
				mnTextDrawMessageAsTiles(list[second].ability + MsgAbilityName,
										 picks, 56, 32);
				mnTextDrawMessageAsTiles(MsgPanelStatusLabel - 2, picks, 16,
										 16);
				mnTextDrawMessageAsTiles(MsgPanelStatusLabel - 1, picks, 16,
										 32);
				mnTextDrawMessageAsTiles(list[first].member + MsgCharacterName,
										 picks, 104, 16);
				mnTextDrawMessageAsTiles(list[second].member + MsgCharacterName,
										 picks, 104, 32);
				mnWindowSprRedrawSavedRect(info);
				mnTextDrawMessageAsTiles(MsgPsynergyNeedMorePp, info, 0, 16);
				if(row != 0) {
					cost = mnAbilityGet(list[second].ability)->ppCost;
					ability = list[second].ability;
				}
				else {
					cost = mnAbilityGet(list[first].ability)->ppCost;
					ability = list[first].ability;
				}
				mnTextDrawNumAtOffset(cost, 2, info, 64, 16);
				mnTextDrawMessageAsTiles(ability + MsgAbilityDescription, info,
										 0, 0);
			}
			sySynch(1);
			if(KeysRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(row != 0)
					second--;
				else
					first--;
				redraw = 1;
			}
			if(KeysRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(row != 0)
					second++;
				else
					first++;
				redraw = 1;
			}
			if(KeysRepeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				row--;
				redraw = 1;
			}
			if(KeysRepeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				row++;
				redraw = 1;
			}
			if(KeysPressed & KEY_A)
				sySndPlayStub(SE_MENU_CONFIRM);
			else if(KeysPressed & KEY_B)
				sySndPlayStub(SE_MENU_CANCEL);
			else if(KeysPressed & KEY_START)
				sySndPlayStub(SE_MENU_CANCEL);
			else
				continue;
			done = 1;
			break;
		}
		/* Persist both member/ability pairs before releasing the picker
		 * resources. */
		/* A, B and START all keep the picks. */
		MainRoutineStateWork.firstShortcut =
			(list[first].member << SHORTCUT_MEMBER_SHIFT) | list[first].ability;
		MainRoutineStateWork.secondShortcut =
			(list[second].member << SHORTCUT_MEMBER_SHIFT) |
			list[second].ability;
		mnWindowClose(picks, 1);
		mnWindowClose(title, 1);
		mnWindowClose(info, 1);
		sySynch(1);
	}
	syHeapRelease(list);
}

/* Lists every Psynergy of the active party members as (party member, Psynergy)
 */
/* pairs in output and returns how many. output must hold every party member's
 */
/* nonzero action slots; there is no capacity argument. */
s32 mnPartyListPsynergy(ABILITY_LIST_ENTRY *output) {
	u16 memberIds[14];
	s32 outputCount = 0;
	s32 memberCount = gmPartyListMembersStub(memberIds);
	s32 i;

	for(i = 0; i < memberCount; i++) {
		UNIT *unit;
		u32 member;
		s32 slotIndex;
		u32 ability;

		member = memberIds[i];
		unit = gmUnitGetStub(member);
		for(slotIndex = 0; slotIndex < (s32)(sizeof(unit->psynergy) /
											 sizeof(unit->psynergy[0]));
			slotIndex++) {
			ability = unit->psynergy[slotIndex].encodedAction & ACTION_ID_MASK;
			if(ability == 0)
				break;
			mnAbilityGet(ability);
			output[outputCount].member = member;
			output[outputCount].ability = ability;
			outputCount++;
		}
	}

	return outputCount;
}

/* Finds the two Psynergy shortcuts in a list from mnPartyListPsynergy, and */
/* returns their indexes (0 for one not found). */
void mnFindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
						   const ABILITY_LIST_ENTRY *entries) {
	s32 entryIndex;

	*firstIndex = 0;
	*secondIndex = 0;

	for(entryIndex = 0; entryIndex < SHORTCUT_LIST_ENTRIES; entryIndex++) {
		if(entries[entryIndex].ability ==
			   (MainRoutineStateWork.firstShortcut & SHORTCUT_ABILITY_MASK) &&
		   entries[entryIndex].member ==
			   (MainRoutineStateWork.firstShortcut >> SHORTCUT_MEMBER_SHIFT)) {
			*firstIndex = entryIndex;
			break;
		}
	}

	for(entryIndex = 0; entryIndex < SHORTCUT_LIST_ENTRIES; entryIndex++) {
		if(entries[entryIndex].ability ==
			   (MainRoutineStateWork.secondShortcut & SHORTCUT_ABILITY_MASK) &&
		   entries[entryIndex].member ==
			   (MainRoutineStateWork.secondShortcut >> SHORTCUT_MEMBER_SHIFT)) {
			*secondIndex = entryIndex;
			break;
		}
	}
}

/* Runs the debug Psynergy shortcut picker (mnDebugSelectShortcuts). */
void mnRunSelectionHook(void) { mnDebugSelectShortcuts(); }

/* The state block of HEAP_SLOT_SELECTION_STATE (SelectionStateWork): two */
/* VRAM cache entries with their flags and a window, among fields nothing here
 */
/* uses. Only the two unused functions below touch it. */
typedef struct SELECTION_STATE {
	u8 unusedBeforeFirst[70];
	/* Nonzero when firstResource is a VRAM cache entry to free on release. */
	u16 firstActive;
	u16 firstResource; /* a VRAM cache entry */
	u8 unusedBetweenResources[850 - 74];
	/* Nonzero when secondResource is a VRAM cache entry to free on release. */
	u16 secondActive;
	u16 secondResource; /* a VRAM cache entry */
	u8 unusedBeforeWin[4084 - 854];
	WINDOW *win; /* closed, and waited for, on release */
	u8 unusedAfterWin[12];
} SELECTION_STATE;

STATIC_ASSERT(sizeof(SELECTION_STATE) == 4100);

/* Unused: allocates the state block of HEAP_SLOT_SELECTION_STATE and clears */
/* its two flags. */
UNUSED void mnAllocateSelectionState(void) {
	SELECTION_STATE *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_SELECTION_STATE,
								 sizeof(SELECTION_STATE));
	state->firstActive = 0;
	state->secondActive = 0;
}

/* Unused: closes the window of HEAP_SLOT_SELECTION_STATE's state block, */
/* frees the VRAM cache entries its flags mark and frees the block. */
UNUSED void mnReleaseSelectionState(void) {
	SELECTION_STATE *state;

	state = *(SELECTION_STATE **)SelectionStateWork;
	mnWindowClose(state->win, 0);
	while(mnWindowIsClosed(state->win) == 0) {
		sySynch(1);
	}
	if(state->firstActive != 0) {
		syVramCacheFreeEntry(state->firstResource);
	}
	if(state->secondActive != 0) {
		syVramCacheFreeEntry(state->secondResource);
	}
	syHeapSlotRelease(HEAP_SLOT_SELECTION_STATE);
}

#undef SHORTCUT_LIST_ENTRIES
#undef SHORTCUT_MEMBER_SHIFT
#undef SHORTCUT_ABILITY_MASK
