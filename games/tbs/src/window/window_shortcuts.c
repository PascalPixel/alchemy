/* The area name shown on entering an area, the debug Psynergy shortcut picker
   and the unused selection state. Part of window.c. */

/* The entries of the debug shortcut picker's list, which mnFindShortcutEntries
   searches whole. */
#define SHORTCUT_LIST_ENTRIES 448
/* A shortcut (MainRoutineStateWork.firstShortcut and secondShortcut) is member
   << SHORTCUT_MEMBER_SHIFT | action, as mnDebugSelectShortcuts stores it. */
#define SHORTCUT_MEMBER_SHIFT 10
#define SHORTCUT_ABILITY_MASK 0x3ff

/* Shows the area's name in a window centred in the screen's top ten rows, for
   AREA_NAME_WINDOW_TICKS; a per-tick event closes it. */
void mnAreaNameWindowShow(void)
{
	FIELD_STATE *notice;
	Sint32		 height;
	Sint32		 width;
	Sint32		 y;
	Sint32		 x;
	Sint32		 areaMsg;
	WINDOW		*nameWin;

	notice = FieldStateWork;
	x = 8;
	y = 8;
	areaMsg = fdAreaGetNameIndexStub(
				  MainRoutineStateWork.next, MainRoutineStateWork.entrance) +
		MsgDebugAreaName;
	mnTextGetMessageSize(areaMsg, &x, &y, &width, &height);
	x = (DISPLAY_TILE_WIDTH - width) >> 1;
	y = (10 - height) >> 1;
	nameWin = mnWindowCreate(x, y, width, height, WINDOW_FLAG_INSTANT);
	notice->areaNameWin = nameWin;
	mnTextDrawMessage(areaMsg, nameWin, 0, 0);
	notice->areaNameCountdown = AREA_NAME_WINDOW_TICKS;
	sySetEvent(mnAreaNameWindowUpdate, EVENT_PASS_TICK);
}

/* Counts the area name down, and closes it at 0. */
void mnAreaNameWindowUpdate(void)
{
	FIELD_STATE *notice;
	Uint16		 ticksLeft;

	notice = FieldStateWork;
	/* Adding (Uint16)-1 takes one off in 16 bits. */
	notice->areaNameCountdown =
		(ticksLeft = notice->areaNameCountdown + (Uint16)-1);
	if(ticksLeft == 0) {
		mnWindowClose(notice->areaNameWin, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

/* Closes the area name at once, if it is shown. */
void mnAreaNameWindowHide(void)
{
	WINDOW *win;

	win = FieldStateWork->areaNameWin;
	if(win != NULL && win->flg != 0) {
		mnWindowClose(win, WINDOW_FLAG_INSTANT);
		syCloseEvent(mnAreaNameWindowUpdate);
	}
}

/* Removes itemId from the party's inventory and returns 0. */
Sint32 mnItemRemoveFromInventory(Sint32 unused, Sint32 itemId)
{
	gmPartyInventoryRemoveStub(itemId);
	return 0;
}

/* Returns 1; the overlays reach it through the stub table. */
Sint32 mnItemReturnTrue(void)
{
	return 1;
}

/* Steps the window hue one step: back when keys contains LEFT, on otherwise. */
UNUSED void mnSettingsMenuStepWindowHue(Sint32 keys)
{
	Uint8 hue = MainRoutineStateWork.winHue;
	if(keys & KEY_LEFT)
		hue += (Uint8)-1;
	else
		hue += 1;
	MainRoutineStateWork.winHue = hue;
}

/* A Psynergy the debug shortcut picker lists: its party member and the
   Psynergy. */
typedef struct ABILITY_LIST_ENTRY {
	Uint16 member;	/* the party member's unit id */
	Uint16 ability; /* the action id (SHORTCUT_ABILITY_MASK in a shortcut) */
} ABILITY_LIST_ENTRY;

/* Lets the player pick two Psynergy shortcuts from all the party's Psynergy,
   showing each one's cost and description, and stores them in
   MainRoutineStateWork.firstShortcut and secondShortcut (member <<
   SHORTCUT_MEMBER_SHIFT | Psynergy). */
void mnDebugSelectShortcuts(void)
{
	ABILITY_LIST_ENTRY *list;
	WINDOW			   *picks;
	WINDOW			   *title;
	WINDOW			   *info;
	WINDOW_SPR		   *cursor;
	Uint32				first;
	Uint32				second;
	Uint32				cnt;
	Sint32				done = 0;
	Sint32				row = 0;
	Sint32				redraw;
	Sint32				slot;
	Sint32				y;
	Sint32				cost;
	Sint32				ability;

	cursor = NULL;
	redraw = 1;
	/* Build the debug shortcut candidates before creating their three
	   windows. */
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
	cnt = mnPartyListPsynergy(list);
	if(cnt != 0) {
		mnFindShortcutEntries(&first, &second, list);
		picks = mnWindowCreate(4, 6, 20, 7, WINDOW_FLAG_INSTANT);
		title = mnWindowCreate(4, 3, 20, 3, WINDOW_FLAG_INSTANT);
		info = mnWindowCreate(4, 14, 20, 5, WINDOW_FLAG_INSTANT);
		/* syVRAMCacheFindFreeEntry reports no free entry as
		   VRAM_CACHE_ENTRY_COUNT, not 0: this test lets that through, and with
		   entry 0 it leaves cursor NULL for the writes below. */
		slot = syVRAMCacheFindFreeEntry();
		if(slot != 0) {
			syVRAMCacheLoad(slot, 128, PointingHandTiles);
			cursor =
				mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, picks, 0, 0);
		}
		/* The picker borrows the status panel's labels: "HP/Max" over it,
		   "Status" over the picks, "Attack" and "Defense" (the two messages
		   before "Status") before them. slot, done with the cursor's VRAM
		   entry, carries the first. */
		slot = MsgPanelHpMaxLabel;
		mnTextDrawMessageAsTiles(slot, title, 16, 0);
		/* Redraw both shortcut rows and the selected ability's detail side. */
		while(!done) {
			if(redraw != 0) {
				redraw = 0;
				/* Wrap both picks and the row; a step below 0 wraps through the
				   unsigned sum. */
				first = (first + cnt) % cnt;
				second = (second + cnt) % cnt;
				row = (row + 2) % 2;
				y = (row << 4) + (picks->y << 3) + 28;
				cursor->y = y;
				cursor->spr.oam.y = y;
				mnWindowSprRedrawSavedRect(picks);
				mnWindowDrawDividerLine(picks, 1, 2, 17, 2);
				mnTextDrawMessageAsTiles(MsgPanelStatusLabel, picks, 48, 0);
				mnTextDrawMessageAsTiles(
					list[first].ability + MsgAbilityName, picks, 56, 16);
				mnTextDrawMessageAsTiles(
					list[second].ability + MsgAbilityName, picks, 56, 32);
				mnTextDrawMessageAsTiles(
					MsgPanelStatusLabel - 2, picks, 16, 16);
				mnTextDrawMessageAsTiles(
					MsgPanelStatusLabel - 1, picks, 16, 32);
				mnTextDrawMessageAsTiles(
					list[first].member + MsgCharacterName, picks, 104, 16);
				mnTextDrawMessageAsTiles(
					list[second].member + MsgCharacterName, picks, 104, 32);
				mnWindowSprRedrawSavedRect(info);
				mnTextDrawMessageAsTiles(MsgPsynergyNeedMorePp, info, 0, 16);
				if(row != 0) {
					cost = gmAbilityGetDirectStub(list[second].ability)->ppCost;
					ability = list[second].ability;
				} else {
					cost = gmAbilityGetDirectStub(list[first].ability)->ppCost;
					ability = list[first].ability;
				}
				mnTextDrawNumAtOffset(cost, 2, info, 64, 16);
				mnTextDrawMessageAsTiles(
					ability + MsgAbilityDescription, info, 0, 0);
			}
			sySynch(1);
			if(PerRepeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(row != 0)
					second--;
				else
					first--;
				redraw = 1;
			}
			if(PerRepeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				if(row != 0)
					second++;
				else
					first++;
				redraw = 1;
			}
			if(PerRepeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				row--;
				redraw = 1;
			}
			if(PerRepeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				row++;
				redraw = 1;
			}
			if(PerPush & KEY_A)
				sySndPlayStub(SE_MENU_CONFIRM);
			else if(PerPush & KEY_B)
				sySndPlayStub(SE_MENU_CANCEL);
			else if(PerPush & KEY_START)
				sySndPlayStub(SE_MENU_CANCEL);
			else
				continue;
			done = 1;
			break;
		}
		/* Persist both member/ability pairs before releasing the picker
		   resources. A, B and START all keep the picks. */
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
   pairs in output and returns how many. output must hold every party member's
   nonzero action slots; there is no capacity argument. */
Sint32 mnPartyListPsynergy(ABILITY_LIST_ENTRY *output)
{
	Uint16 memberIds[14];
	Sint32 outputCnt = 0;
	Sint32 memberCnt = gmPartyListMembersStub(memberIds);
	Sint32 i;

	for(i = 0; i < memberCnt; i++) {
		UNIT  *unit;
		Uint32 member;
		Sint32 slotIdx;
		Uint32 ability;

		member = memberIds[i];
		unit = gmUnitGetStub(member);
		for(slotIdx = 0; slotIdx <
			(Sint32)(sizeof(unit->psynergy) / sizeof(unit->psynergy[0]));
			slotIdx++) {
			ability = unit->psynergy[slotIdx].encodedAction & ACTION_ID_MASK;
			if(ability == 0)
				break;
			gmAbilityGetDirectStub(ability);
			output[outputCnt].member = member;
			output[outputCnt].ability = ability;
			outputCnt++;
		}
	}

	return outputCnt;
}

/* Finds the two Psynergy shortcuts in a list from mnPartyListPsynergy, and
   returns their indexes (0 for one not found). */
void mnFindShortcutEntries(
	Uint32 *firstIdx, Uint32 *secondIdx, const ABILITY_LIST_ENTRY *entries)
{
	Sint32 entryIdx;

	*firstIdx = 0;
	*secondIdx = 0;

	for(entryIdx = 0; entryIdx < SHORTCUT_LIST_ENTRIES; entryIdx++) {
		if(entries[entryIdx].ability ==
				(MainRoutineStateWork.firstShortcut & SHORTCUT_ABILITY_MASK) &&
			entries[entryIdx].member ==
				(MainRoutineStateWork.firstShortcut >> SHORTCUT_MEMBER_SHIFT)) {
			*firstIdx = entryIdx;
			break;
		}
	}

	for(entryIdx = 0; entryIdx < SHORTCUT_LIST_ENTRIES; entryIdx++) {
		if(entries[entryIdx].ability ==
				(MainRoutineStateWork.secondShortcut & SHORTCUT_ABILITY_MASK) &&
			entries[entryIdx].member ==
				(MainRoutineStateWork.secondShortcut >>
					SHORTCUT_MEMBER_SHIFT)) {
			*secondIdx = entryIdx;
			break;
		}
	}
}

/* Runs the debug Psynergy shortcut picker (mnDebugSelectShortcuts). */
void mnRunSelectionHook(void)
{
	mnDebugSelectShortcuts();
}

/* The state block of HEAP_SLOT_SELECTION_STATE (SelectionStateWork): two VRAM
   cache entries with their flags and a window, among fields nothing here uses.
   Only the two unused functions below touch it. */
typedef struct SELECTION_STATE {
	Uint8 unusedBeforeFirst[70];
	/* Nonzero when firstResource is a VRAM cache entry to free on release. */
	Uint16 firstActive;
	Uint16 firstResource; /* a VRAM cache entry */
	Uint8  unusedBetweenResources[850 - 74];
	/* Nonzero when secondResource is a VRAM cache entry to free on release. */
	Uint16	secondActive;
	Uint16	secondResource; /* a VRAM cache entry */
	Uint8	unusedBeforeWin[4084 - 854];
	WINDOW *win; /* closed, and waited for, on release */
	Uint8	unusedAfterWin[12];
} SELECTION_STATE;

STATIC_ASSERT(sizeof(SELECTION_STATE) == 4100);

/* Unused: allocates the state block of HEAP_SLOT_SELECTION_STATE and clears its
   two flags. */
UNUSED void mnAllocateSelectionState(void)
{
	SELECTION_STATE *state;

	state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_SELECTION_STATE, sizeof(SELECTION_STATE));
	state->firstActive = 0;
	state->secondActive = 0;
}

/* Unused: closes the window of HEAP_SLOT_SELECTION_STATE's state block, frees
   the VRAM cache entries its flags mark and frees the block. */
UNUSED void mnReleaseSelectionState(void)
{
	SELECTION_STATE *state;

	state = *(SELECTION_STATE **)SelectionStateWork;
	mnWindowClose(state->win, 0);
	while(mnWindowIsClosed(state->win) == 0) {
		sySynch(1);
	}
	if(state->firstActive != 0) {
		syVRAMCacheFreeEntry(state->firstResource);
	}
	if(state->secondActive != 0) {
		syVRAMCacheFreeEntry(state->secondResource);
	}
	syHeapSlotRelease(HEAP_SLOT_SELECTION_STATE);
}

#undef SHORTCUT_LIST_ENTRIES
#undef SHORTCUT_MEMBER_SHIFT
#undef SHORTCUT_ABILITY_MASK
