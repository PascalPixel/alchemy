/* The Psynergy menu: the field menu's Psynergy screen, the party member and
   Psynergy lists, targets, and the L and R shortcuts. Part of menu.c. */

/* The bits of a listed Psynergy the info window's description is found by:
   nine, where the list's other reads take PSYNERGY_ABILITY_MASK's fourteen. */
#define ABILITY_DESCRIPTION_MASK 0x1ff

Bool mnPsynergyMenuIsActionRestricted(Sint32 actionEntry);

Sint32 mnPsynergyMenuRunSteps(
	Sint32 *outMember, Sint32 *unused, Sint32 *outAction);

/* The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over
   the field, which stops meanwhile. A Psynergy chosen to use on the field goes
   to FIELD_STATE.psynergy with the party member who uses it. Returns
   mnPsynergyMenuRunSteps' result. */
Sint32 mnPsynergyMenuRun(void)
{
	/* German, Spanish, French and Italian also suspend the field events and
	   keep character block 1's tiles aside, filled with colour 3, while the
	   menu is open. */
	ITEM_MENU_STATE *menu;
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	void *savedTiles;
#endif
	Sint32 psynergy;
	Sint32 unused;
	Sint32 member;
	Sint32 result;

	menu = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	savedTiles = syHeapAllocEWRAM(MENU_SAVED_TILE_BYTES);
#endif
	Ram_HeapSlots->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	mnSuspendFieldEvents();
	syWordsCopy(savedTiles, (void *)BG_CHAR_BLOCK(1), MENU_SAVED_TILE_BYTES);
	syWordsFill(
		(void *)BG_CHAR_BLOCK(1), MENU_SAVED_TILE_BYTES, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif
	menu->partyCnt = gmPartyListMembersStub(menu->memberIds);
	mnItemMenuInit(0, 3, 0, 7);
	menu->questionWin = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);

	mnStartWatchingStart();
	result = mnPsynergyMenuRunSteps(&member, &unused, &psynergy);
	mnStopWatchingStart();
	if(result == 1) {
		FIELD_STATE *tgt = Ram_HeapSlots->fieldState;
		Uint16		 request;

		/* DEAD: gmAbilityGetDirectStub's result is not used; the ROM keeps the
		   call. */
		gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK & menu->selectedItems[0]);
		request = psynergy | (member << ACT_UNIT_SHIFT);
		tgt->psynergy = request;
	}

	mnWindowSprClearListStub(menu->statWin);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	Ram_HeapSlots->actorSystem->suspended = 0;
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy((void *)BG_CHAR_BLOCK(1), savedTiles, MENU_SAVED_TILE_BYTES);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	syHeapRelease(savedTiles);
	sySynch(1);
	mnResumeFieldEvents();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	fdPartyClearUnusableShortcutsStub();
	return result;
}

/* Returns whether the scene has a cue for field Psynergy type on what it would
   act on. */
Sint32 fdHasPsynergyCueStub(Sint32 type);

/* PSYNERGY_MENU_STATE.flg: the Psynergy list shows the shortcut help (Select
   held). */
#define PSYNERGY_MENU_SHORTCUT_HELP 2

#define MENU_TARGET_WHOLE_PARTY 9 /* memberIds[1] for the whole party */

/* PSYNERGY_MENU_STATE.flg: set after a use or a cancelled target. The Item
   screen's command grid reads the same bit; nothing in the Psynergy screen
   does. */
#define PSYNERGY_MENU_USED 1

/* The steps of the Psynergy menu (mnPsynergyMenuRunSteps). */
typedef enum PSYNERGY_MENU_STEP {
	PSYNERGY_STEP_MEMBER,
	PSYNERGY_STEP_LIST,
	/* how the Psynergy is used (mnPsynergyMenuClassifySelectedPsynergy) */
	PSYNERGY_STEP_CLASSIFY,
	PSYNERGY_STEP_TARGET,
	PSYNERGY_STEP_APPLY,
} PSYNERGY_MENU_STEP;

/* The Psynergy shortcut a choice sets (mnPsynergyMenuSetShortcut). */
typedef enum PSYNERGY_SHORTCUT {
	PSYNERGY_SHORTCUT_L,
	PSYNERGY_SHORTCUT_R,
} PSYNERGY_SHORTCUT;

/* What the Psynergy menu is open for (PSYNERGY_MENU_STATE.mode). */
typedef enum PSYNERGY_MENU_MODE {
	PSYNERGY_MODE_USE,
	PSYNERGY_MODE_SET_L,
	PSYNERGY_MODE_SET_R,
} PSYNERGY_MENU_MODE;

Sint16 gmUnitAddPpStub(Sint32 unit, Sint32 amount);

Sint32 mnPsynergyMenuSetShortcut(Sint32 memberId, Sint32 id, Sint32 second);

Sint32 mnPsynergyMenuClassifySelectedPsynergy(void);
Sint32 mnPsynergyMenuSelectPartySlot(Sint32 paneIdx);
Sint32 mnPsynergyMenuSelectTarget(Sint32 browseOnly);
Sint32 mnPsynergyMenuRunList(Sint32 paneIdx);

/* Runs the Psynergy menu's steps: choose a party member and one of their
   Psynergy, then use it on a member (or the whole party), or, in the shortcut
   modes, set it as the L or R shortcut. Returns 1 with the member and Psynergy
   in the out parameters when the chosen Psynergy works on the field, to be used
   once the menu closes; -1 when the player backs out or presses Start;
   otherwise the result of the last use. */
Sint32 mnPsynergyMenuRunSteps(
	Sint32 *outMember, Sint32 *unused, Sint32 *outAction)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 useResult;
	Sint32				 step;
	Sint32				 done;
	Sint32				 chosenAction;
	Sint32				 tgtMode;
	Sint32				 actionEntry;
	Sint32				 chosenTgt;
	ABILITY_DEFINITION	*action;

	step = PSYNERGY_STEP_MEMBER;
	done = 0;
	menu = Ram_HeapSlots->menuState;
	useResult = 0;

	while(done == 0 && syGameFlagGet(FLAG_MENU_START_PRESSED) == 0) {
		switch(step) {
		/* Choose whose Psynergy to look at. */
		case PSYNERGY_STEP_MEMBER: {
			Sint32 chosen;

			menu->sideRow[0] = 0;
			mnItemMenuDrawMessage(0, MsgPsynergyChooseOwner);
			chosen = mnPsynergyMenuSelectPartySlot(0);
			if(chosen == -1) {
				/* FAKEMATCH(dead): chosen cleared though the menu ends before
				 * reading it, as the Item menu's member step clears its own
				 * (there the ROM keeps the store; here it leaves none); as the
				 * arm without it. */
				chosen = 0;
				useResult = -1;
				done = 1;
			}
			mnWindowSprRedrawSavedRectStub(menu->infoWin);
			step = PSYNERGY_STEP_LIST;
			break;
		}

		/* Choose one of their Psynergy; in the shortcut modes, set it as the L
		   or R shortcut and start again. */
		case PSYNERGY_STEP_LIST:
			sySynch(1);
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberIds[0]);
			if(menu->psynergyCnt == 0) {
				step = PSYNERGY_STEP_MEMBER;
			} else {
				switch(menu->mode) {
				case PSYNERGY_MODE_USE:
					mnItemMenuDrawMessage(0, (Sint32)MsgPsynergyChooseAbility);
					break;
				case PSYNERGY_MODE_SET_L:
					mnItemMenuDrawMessage(0, (Sint32)MsgPsynergyChooseForL);
					break;
				case PSYNERGY_MODE_SET_R:
					mnItemMenuDrawMessage(0, (Sint32)MsgPsynergyChooseForR);
					break;
				}
				mnHideEntryIcons();
				mnStatPanelDraw(menu->statWin, menu->memberIds[0], 0,
					STATUS_PANEL_EXPERIENCE);
				chosenAction = mnPsynergyMenuRunList(0);
				step = PSYNERGY_STEP_MEMBER;
				if(chosenAction != -1) {
					step = PSYNERGY_STEP_CLASSIFY;
					if(menu->mode != PSYNERGY_MODE_USE) {
						if(menu->mode == PSYNERGY_MODE_SET_L) {
							mnPsynergyMenuSetShortcut(menu->memberIds[0],
								chosenAction, PSYNERGY_SHORTCUT_L);
							mnWindowSprClearListStub(menu->infoWin);
							mnItemMenuShowMessage(
								(Sint32)MsgShortcutSetL, -1, -1);
							mnWindowSprRedrawSavedRectStub(menu->infoWin);
						} else {
							mnPsynergyMenuSetShortcut(menu->memberIds[0],
								chosenAction, PSYNERGY_SHORTCUT_R);
							mnWindowSprClearListStub(menu->infoWin);
							mnItemMenuShowMessage(
								(Sint32)MsgShortcutSetR, -1, -1);
							mnWindowSprRedrawSavedRectStub(menu->infoWin);
						}
						step = PSYNERGY_STEP_MEMBER;
					}
				}
			}
			break;

		/* Choose the member to use it on. */
		case PSYNERGY_STEP_TARGET:
			mnItemMenuDrawMessage(0, (Sint32)MsgPsynergyChooseTarget);
			chosenTgt = mnPsynergyMenuSelectTarget(0);
			step = PSYNERGY_STEP_APPLY;
			if(chosenTgt == -1) {
				menu->flg |= PSYNERGY_MENU_USED;
				step = PSYNERGY_STEP_LIST;
			}
			break;

		/* How is it used? On one member, on the whole party, or on the field
		   once the menu closes. */
		case PSYNERGY_STEP_CLASSIFY:
			tgtMode = mnPsynergyMenuClassifySelectedPsynergy();
			if(tgtMode == ITEM_USE_MODE_CHOOSE_MEMBER) {
				step = PSYNERGY_STEP_TARGET;
				break;
			}
			if(tgtMode == ITEM_USE_MODE_WHOLE_PARTY) {
				menu->memberIds[1] = MENU_TARGET_WHOLE_PARTY;
				step = PSYNERGY_STEP_APPLY;
				break;
			}
			done = 1;
			useResult = 1;
			*outMember = menu->memberIds[0];
			*outAction = menu->sideAction[0] & PSYNERGY_ABILITY_MASK;
			break;

		/* Use it: pay its PP, show the result and go back to the list, or to
		   the target when it failed on one member. */
		case PSYNERGY_STEP_APPLY:
			chosenTgt = 0;
			actionEntry = menu->sideAction[0];
			useResult = mnAbilityApplyToTargets(
				actionEntry, menu->memberIds[0], menu->memberIds[1], 0);
			if(menu->memberIds[1] == MENU_TARGET_WHOLE_PARTY) {
				menu->memberIds[1] = menu->memberIds[0];
				chosenTgt = MENU_TARGET_WHOLE_PARTY;
			}
			if(useResult != -1) {
				action = gmAbilityGetDirectStub(
					menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
				gmUnitAddPpStub(menu->memberIds[0], -action->ppCost);
			}
			gmUnitCalcStub(menu->memberIds[0]);
			if(useResult != -1) {
				mnStatPanelDraw(menu->statWin, menu->memberIds[1], 0,
					STATUS_PANEL_EXPERIENCE);
				mnAbilityPlayUseSnd(
					menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage(
					menu->useResult + (Sint32)MsgItemUseResult, 0, -1);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub(menu->infoWin);
				mnItemMenuShowMessage(
					menu->useResult + (Sint32)MsgItemUseResult, useResult,
					useResult);
			}
			if(useResult == -1) {
				menu->skipSlide = 1;
				if(chosenTgt == MENU_TARGET_WHOLE_PARTY) {
					menu->flg |= PSYNERGY_MENU_USED;
					step = PSYNERGY_STEP_LIST;
				} else {
					step = PSYNERGY_STEP_TARGET;
				}
			} else {
				useResult = 1;
				menu->flg |= PSYNERGY_MENU_USED;
				step = PSYNERGY_STEP_LIST;
			}
			break;

		default:
			done = 1;
			break;
		}
	}

	if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
		useResult = -1;
	return useResult;
}

/* Says how the selected Psynergy is used from the menu (ITEM_USE_MODE): on the
   field once the menu closes when the scene has a script for its field Psynergy
   on what the leader faces, on the whole party for ABILITY_RANGE_ALL, on one
   chosen party member when it targets ABILITY_TARGET_ALLIES, and otherwise on
   the field. */
Sint32 mnPsynergyMenuClassifySelectedPsynergy(void)
{
	ABILITY_DEFINITION *action;
	Sint32				classification;

	action = gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK &
		((PSYNERGY_MENU_STATE *)Ram_HeapSlots->menuState)->sideAction[0]);
	if(fdHasPsynergyCueStub(action->fieldPsynergy) != 0)
		return ITEM_USE_MODE_FROM_FIELD;
	if(action->range == ABILITY_RANGE_ALL)
		return ITEM_USE_MODE_WHOLE_PARTY;
	if(action->tgtMode != ABILITY_TARGET_ALLIES)
		classification = ITEM_USE_MODE_FROM_FIELD;
	else
		classification = ITEM_USE_MODE_CHOOSE_MEMBER;
	return classification;
}

Sint32 mnPsynergyMenuCollectActions(UNIT *unit, Uint16 *actions, Sint32 mode);
Sint32 mnPsynergyMenuSelectMember(Uint16 *ownerIds, Uint16 *unused);

/* Shows side partySlot's icon, collects the actions of the member chosen there
   last (the first when none was) and runs mnPsynergyMenuSelectMember on them.
   Returns its result. */
Sint32 mnPsynergyMenuSelectPartySlot(Sint32 partySlot)
{
	PSYNERGY_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32				 memberIdx = menu->tabIdx[partySlot];
	WINDOW_SPR			*icon;
	UNIT				*unit;
	Uint16				*actions;
	Sint32				 result;

	result = 0;
	icon = menu->sideIcon[partySlot];
	icon->motion = ICON_SHOWN;
	icon->anmStep = result;
	menu->cursorIcon->motion = ICON_HIDDEN;
	menu->tabCnts[partySlot] = menu->memberCnt;

	if(memberIdx == -1) {
		menu->tabIdx[partySlot] = 0;
		memberIdx = 0;
	} else {
		mnCursorSlide(memberIdx * 24 - 10, 16);
	}

	unit = syUnitGet(menu->memberTbl[memberIdx]);
	actions = menu->psynergies;
	menu->psynergyCnt =
		mnPsynergyMenuCollectActions(unit, actions, PSYNERGY_LIST_FIELD);
	result = mnPsynergyMenuSelectMember(menu->memberTbl, actions);
	icon = menu->sideIcon[partySlot];
	mnIconShow(icon);
	sySynch(1);
	return result;
}

void mnPsynergyMenuRefreshMemberPsynergy(Sint32 ownerId);

Sint32 mnPsynergyMenuDrawShortcuts(Sint32 win, Sint32 owner);

/* The party member row of the Psynergy menu: moving shows each member's
   Psynergy, status and shortcuts; A chooses a member who has Psynergy, L or R
   one to set that shortcut from, and B backs out. Returns the chosen member's
   id, or -1. */
Sint32 mnPsynergyMenuSelectMember(Uint16 *ownerIds, Uint16 *unused)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 selection;
	Sint32				 cnt;
	Sint32				 redraw;
	Sint32				 result;
	void				*unit;
	Sint32				 win;
	Bool				 shown;
	Uint16				*actions;
	Sint8				 fieldCnt;
	Sint32				 shortcutCnt;

	menu = Ram_HeapSlots->menuState;
	selection = menu->tabIdx[0];
	cnt = menu->tabCnts[0];
	redraw = 2;
	result = 0;
	shown = FALSE;
	menu->mode = result;
	unit = syUnitGet(ownerIds[selection]);
	if(mnWindowUpdateOrCreate(
		   &menu->psynergyWin, 13, 3, 17, 10, WINDOW_FLAG_INSTANT))
		mnSpawnIconEntries(menu, menu->psynergyWin);
	if(mnWindowUpdateOrCreate(
		   &menu->shortcutWin, 13, 13, 17, 4, WINDOW_FLAG_INSTANT)) {
		menu->cursorIcon = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->shortcutWin, 0, result);
		menu->cursorIcon->motion = ICON_HIDDEN;
	}
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			win = menu->statWin;
			unit = syUnitGet(ownerIds[selection]);
			mnPsynergyMenuRefreshMemberPsynergy(ownerIds[selection]);
			mnStatPanelDraw(
				win, ownerIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			mnPsynergyMenuDrawShortcuts(menu->shortcutWin, ownerIds[selection]);
			mnReloadMemberPortrait(menu, ownerIds[selection]);
			if(redraw == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->auxiliaryWin, selection, cnt);
			if(!syGameFlagGet(FLAG_INFO_WINDOW_HOLDS_MESSAGE) && !shown) {
				mnWindowSprClearListStub(menu->infoWin);
#if LANGUAGE != LANGUAGE_JAPANESE
				/* Japanese does not redraw the info window here. */
				mnWindowSprRedrawSavedRectStub(menu->infoWin);
#endif
				mnDrawCoins(menu->infoWin);
				shown = TRUE;
			} else {
				syGameFlagClear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
			}
		}
		mnCursorPos((selection % 4) * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			if(menu->psynergyCnt) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = ownerIds[selection];
				break;
			}
			sySndPlayStub(SE_MENU_ERROR);
		}
		if((PerWork.push & KEY_L) || (PerWork.push & KEY_R)) {
			result = ownerIds[selection];
			if(PerWork.push & KEY_L)
				menu->mode = PSYNERGY_MODE_SET_L;
			else
				menu->mode = PSYNERGY_MODE_SET_R;
			actions = syHeapAlloc(32 * sizeof(Uint16));
			fieldCnt = mnPsynergyMenuCollectActions(
				unit, actions, PSYNERGY_LIST_SHORTCUTS);
			syHeapRelease(actions);
			shortcutCnt = fieldCnt;
			if(shortcutCnt == 0) {
				menu->mode = shortcutCnt;
				sySndPlayStub(SE_MENU_ERROR);
			} else {
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		redraw = mnMoveGridCursor(&selection, cnt, 4);
	}
	menu->tabIdx[0] = selection;
	menu->selectedMember = ownerIds[selection];
	menu->memberIds[0] = ownerIds[selection];
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return result;
}

void mnPsynergyMenuDrawPsynergyIcons(Uint16 *psynergies);

/* Collects the owner's Psynergy and redraws the list, saying when it is
   empty. */
void mnPsynergyMenuRefreshMemberPsynergy(Sint32 ownerId)
{
	Uint16				*psynergies;
	PSYNERGY_MENU_STATE *menu;
	void				*owner;

	menu = Ram_HeapSlots->menuState;
	owner = syUnitGet(ownerId);
	psynergies = menu->psynergies;
	menu->psynergyCnt =
		mnPsynergyMenuCollectActions(owner, psynergies, PSYNERGY_LIST_FIELD);
	mnWindowSprRedrawSavedRectStub(menu->psynergyWin);
	mnPsynergyMenuPlaceEntryIcons(108, 32, 8);
	mnPsynergyMenuDrawPsynergyIcons(psynergies);
	if(menu->psynergyCnt == 0) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgPsynergyMenuEmpty, menu->psynergyWin, 0, 24);
	}
}

/* Chooses the party member to use the selected Psynergy on, showing each one's
   status and the Psynergy's description unless browseOnly is set. */
Sint32 mnPsynergyMenuSelectTarget(Sint32 browseOnly)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 selection;
	Sint32				 cnt;
	Sint32				 redraw;
	Sint32				 result;
	Bool				 shown;
	WINDOW_SPR			*icon;
	Sint32				 column;

	menu = Ram_HeapSlots->menuState;
	selection = menu->tabIdx[1];
	cnt = menu->memberCnt;
	result = 0;
	shown = FALSE;
	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(menu->memberTbl[menu->tabIdx[0]]);
	redraw = 2;
	mnCursorSlide(selection * 24 - 10, 16);
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberTbl[selection]);
			icon = menu->sideIcon[1];
			column = (selection % 4) * 3;
			icon->spr.oam.x = icon->x =
				((Uint16)menu->auxiliaryWin->x + column) * 8 - 2;
			if(browseOnly == 0) {
				mnStatPanelDraw(menu->statWin, menu->memberTbl[selection], 0,
					STATUS_PANEL_EXPERIENCE);
				mnReloadMemberPortrait(menu, menu->memberTbl[selection]);
				if(redraw == 2) {
					mnCreatePartyFigures(selection / 4 * 4);
					sySynch(1);
				}
				mnMarkChosenMember(menu->auxiliaryWin, selection, cnt);
				if(!syGameFlagGet(FLAG_INFO_WINDOW_HOLDS_MESSAGE) && !shown) {
					/* The Japanese release clears the info window; in every
					   edition but the Japanese it redraws the window's saved
					   rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
					mnWindowSprRedrawSavedRectStub(menu->infoWin);
#else
					mnWindowSprClearListStub(menu->infoWin);
#endif
					/* Japanese draws the description as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
					mnTextDrawMessageStub(
						(menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
							MsgAbilityDescription,
						menu->infoWin, 0, 0);
#else
					mnTextDrawMessageAsTilesStub(
						(menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
							MsgAbilityDescription,
						menu->infoWin, 0, 0);
#endif
					shown = TRUE;
				} else {
					syGameFlagClear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
				}
			}
		}
		mnCursorPos((selection % 4) * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = menu->memberTbl[selection];
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		redraw = mnMoveGridCursor(&selection, cnt, 4);
	}
	icon = menu->sideIcon[1];
	mnIconShow(icon);
	icon->motion = ICON_HIDDEN;
	sySynch(1);
	menu->tabIdx[1] = selection;
	menu->selectedMember = menu->memberTbl[selection];
	menu->memberIds[1] = menu->memberTbl[selection];
	selection = menu->tabIdx[0];
	cnt = menu->tabCnts[0];
	mnCreatePartyFigures(selection / 4 * 4);
	mnMarkChosenMember(menu->auxiliaryWin, selection, cnt);
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return result;
}

/* Sets the L Psynergy shortcut, or the R one when second is set, to the
   Psynergy id used by party member memberId. Returns 1. */
Sint32 mnPsynergyMenuSetShortcut(Sint32 memberId, Sint32 id, Sint32 second)
{
	Uint32 packed = (memberId << ACT_UNIT_SHIFT) | (id & PSYNERGY_ABILITY_MASK);

	if(second == 0)
		MainRoutineStateWork.firstShortcut = packed;
	else {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->secondShortcut = packed;
	}
	return 1;
}

#if LANGUAGE == LANGUAGE_JAPANESE
/* Japanese drops the colon after "L" and "R" and draws the ability's name 8
   pixels further left when that name is wider than this. */
#define COLON_NAME_WIDTH_LIMIT 11
#elif LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
/* German, Spanish, French and Italian leave out the owner's name when the
   ability's name is wider than this. */
#define OWNER_NAME_WIDTH_LIMIT 10
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
/* `L`, before a wide shortcut name. */
extern const Uint8 MenuShortcutLString[];
/* `L:`, before the L shortcut's name. */
extern const Uint8 MenuShortcutLColonString[];
/* `R`, before a wide shortcut name. */
extern const Uint8 MenuShortcutRString[];
/* `R:`, before the R shortcut's name. */
extern const Uint8 MenuShortcutRColonString[];
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/* Draws the L and R shortcuts into window: a heading, then for each its
   Psynergy and the party member who uses it, or how to set it. German, Spanish,
   French and Italian leave out the party member's name beside a wide Psynergy
   name. Returns 1; owner is not used. */
Sint32 mnPsynergyMenuDrawShortcuts(Sint32 win, Sint32 owner)
{
	Sint32 left;
	Sint32 top;
	Sint32 width;
	Sint32 height;
	Uint8 *name;
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
	Bool wide;
#endif

	if(MainRoutineStateWork.firstShortcut != 0 &&
		MainRoutineStateWork.secondShortcut != 0)
		/* "L R:Change shortcut" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 1, win, 0, -8);
	else
		/* "Shortcut" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetL - 2, win, 0, -8);

	name = &MsgAbilityName[MainRoutineStateWork.firstShortcut & ACT_ID_MASK];
	mnTextGetMessageSizeStub(name, &left, &top, &width, &height);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
	if(width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
		wide = TRUE;
	else
		wide = FALSE;
#endif
	if(MainRoutineStateWork.firstShortcut != 0) {
		mnMessageArgumentPushStub(
			MainRoutineStateWork.firstShortcut & ACT_ID_MASK,
			MESSAGE_ARGUMENT_ABILITY);
		/* "L:" and the Psynergy's name */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 4, win, 0, 0);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
		if(!wide)
#endif
			mnTextDrawStringAtOffsetStub(
				(Sint32)syUnitGet(
					MainRoutineStateWork.firstShortcut >> ACT_UNIT_SHIFT),
				win, 80, 0);
	} else {
		/* "L:Set to L" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 2, win, 0, 0);
	}

	name = &MsgAbilityName[MainRoutineStateWork.secondShortcut & ACT_ID_MASK];
	mnTextGetMessageSizeStub(name, &left, &top, &width, &height);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
	if(width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
		wide = TRUE;
	else
		wide = FALSE;
#endif
	if(MainRoutineStateWork.secondShortcut != 0) {
		mnMessageArgumentPushStub(
			MainRoutineStateWork.secondShortcut & ACT_ID_MASK,
			MESSAGE_ARGUMENT_ABILITY);
		/* "R:" and the Psynergy's name */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 5, win, 0, 8);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
		if(!wide)
#endif
			mnTextDrawStringAtOffsetStub(
				(Sint32)syUnitGet(
					MainRoutineStateWork.secondShortcut >> ACT_UNIT_SHIFT),
				win, 80, 8);
	} else {
		/* "R:Set to R" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 3, win, 0, 8);
	}
	return 1;
}
#else
/* Japanese: draws "L" or "L:" and "R" or "R:" itself before each shortcut's
   name, dropping the colon for a wide name. Returns 1. */
Sint32 mnPsynergyMenuDrawShortcuts(Sint32 win, Sint32 owner)
{
	Sint32 left;
	Sint32 top;
	Sint32 width;
	Sint32 height;
	Uint8 *name;

	if(MainRoutineStateWork.firstShortcut != 0 &&
		MainRoutineStateWork.secondShortcut != 0)
		/* "L R:Change shortcut" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 1, win, 0, -8);
	else
		/* "Shortcut" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetL - 2, win, 0, -8);

	name = &MsgAbilityName[MainRoutineStateWork.firstShortcut & ACT_ID_MASK];
	mnTextGetMessageSizeStub(name, &left, &top, &width, &height);
	if(MainRoutineStateWork.firstShortcut != 0) {
		if(width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
			mnTextDrawStringInWindowStub(MenuShortcutLString, win, 0, 0);
			mnTextDrawMessageAsTilesStub((Sint32)name, win, 8, 0);
		} else {
			mnTextDrawStringInWindowStub(MenuShortcutLColonString, win, 0, 0);
			mnTextDrawMessageAsTilesStub((Sint32)name, win, 16, 0);
		}
		mnTextDrawStringAtOffsetStub(
			(Sint32)syUnitGet(
				MainRoutineStateWork.firstShortcut >> ACT_UNIT_SHIFT),
			win, 80, 0);
	} else {
		mnTextDrawStringInWindowStub(MenuShortcutLColonString, win, 0, 0);
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 2, win, 24, 0);
	}

	name = &MsgAbilityName[MainRoutineStateWork.secondShortcut & ACT_ID_MASK];
	mnTextGetMessageSizeStub(name, &left, &top, &width, &height);
	if(MainRoutineStateWork.secondShortcut != 0) {
		if(width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
			mnTextDrawStringInWindowStub(MenuShortcutRString, win, 0, 8);
			mnTextDrawMessageAsTilesStub((Sint32)name, win, 8, 8);
		} else {
			mnTextDrawStringInWindowStub(MenuShortcutRColonString, win, 0, 8);
			mnTextDrawMessageAsTilesStub((Sint32)name, win, 16, 8);
		}
		mnTextDrawStringAtOffsetStub(
			(Sint32)syUnitGet(
				MainRoutineStateWork.secondShortcut >> ACT_UNIT_SHIFT),
			win, 80, 8);
	} else {
		mnTextDrawStringInWindowStub(MenuShortcutRColonString, win, 0, 8);
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetR + 3, win, 24, 8);
	}
	return 1;
}
#endif

#undef OWNER_NAME_WIDTH_LIMIT

/* Loads each listed Psynergy's icon into its icon sprite (32 entries) and hides
   the icons of empty entries. */
void mnPsynergyMenuDrawPsynergyIcons(Uint16 *psynergies)
{
	Sint32		 i;
	WINDOW_SPR **icons;
	Uint16		*p;
	Sint32		 psynergyId;

	icons = ((PSYNERGY_MENU_STATE *)Ram_HeapSlots->menuState)->entryIcons;
	p = psynergies;
	for(i = 0; i < PSYNERGY_SLOTS; i++) {
		psynergyId = *p++;
		if(psynergyId != 0) {
			mnSelectionIconLoadIntoSlotStub(
				ICON_KIND_ABILITY, psynergyId, (Uint8)(*icons)->idx, 0);
		}
		icons++;
	}
	mnHideEmptyEntryIcons(psynergies);
}

/* Lists a unit's Psynergy into actions (cleared first) by mode (enum
   PSYNERGY_LIST_MODE) and returns how many. */
Sint32 mnPsynergyMenuCollectActions(UNIT *unit, Uint16 *actions, Sint32 mode)
{
	Sint32				i;
	Sint32				cnt;
	ABILITY_DEFINITION *ability;
	Sint32				ofs;

	cnt = 0;
	for(i = 0; i < PSYNERGY_SLOTS; i++)
		actions[i] = 0;

	if(mode == PSYNERGY_LIST_SHORTCUTS) {
		for(i = 0; i < PSYNERGY_SLOTS; i++) {
			if(unit->psynergy[i].encodedAction != 0) {
				if(gmAbilityGetDirectStub(
					   unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK)
						->fieldPsynergy != 0) {
					actions[cnt] = unit->psynergy[i].encodedAction;
					cnt++;
				}
			}
		}
	} else {
		for(i = 0; i < PSYNERGY_SLOTS; i++) {
			if(unit->psynergy[i].encodedAction != 0) {
				ability = gmAbilityGetDirectStub(
					unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy != 0 ||
					(ability->flg & ABILITY_USABLE_IN_FIELD) != 0) {
					actions[cnt] = unit->psynergy[i].encodedAction;
					cnt++;
				}
			}
		}
		if(mode != PSYNERGY_LIST_FIELD) {
			for(i = 0; i < PSYNERGY_SLOTS; i++) {
				if(unit->psynergy[i].encodedAction != 0) {
					ability =
						gmAbilityGetDirectStub(unit->psynergy[i].encodedAction &
							PSYNERGY_ABILITY_MASK);
					if(ability->fieldPsynergy == 0 &&
						(ability->flg & ABILITY_USABLE_IN_FIELD) == 0) {
						actions[cnt] = unit->psynergy[i].encodedAction;
						cnt++;
					}
				}
			}
		}
	}
	return cnt;
}

/* Sets list up over the Psynergy listed for the party member in side index,
   five a page, its cursor on the one chosen last there (or the last one). */
Sint32 mnPsynergyMenuBuildPagedList(PAGED_LIST *result, Sint32 idx)
{
	Sint32				 ownerState;
	PSYNERGY_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32				 entryCnt;
	Sint32				 row;
	Sint32				 page;
	Sint32				 pageCnt;
	Sint32				 selectedIdx;
	Sint32				 owner;

	ownerState = syUnitGet(menu->memberIds[idx]);
	entryCnt = menu->psynergyCnt;
	owner = menu->memberIds[idx];
	selectedIdx = menu->selectedIdxByMember[owner];
	if((selectedIdx + 1) > entryCnt) {
		selectedIdx = entryCnt - 1;
	}
	page = selectedIdx / LIST_PAGE_SIZE;
	row = selectedIdx % LIST_PAGE_SIZE;
	pageCnt = entryCnt / LIST_PAGE_SIZE;
	if(entryCnt % LIST_PAGE_SIZE != 0) {
		pageCnt++;
	}
	result->memberState = ownerState;
	result->page = page;
	result->pageCnt = pageCnt;
	result->row = row;
	result->entryCnt = entryCnt;
	result->selectedIdx = selectedIdx;
	return 1;
}

/* Updates the Psynergy list for the row under the cursor: puts the Psynergy's
   description in the info window, unless a kept message is there, and
   highlights its row of the five. window and scratch are not used. */
Sint32 mnPsynergyMenuShowSelectedRow(
	Sint32 win, Sint32 *scratch, PAGED_LIST *state)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 combined;
	Sint32				 row;

	menu = Ram_HeapSlots->menuState;
	combined = state->page * LIST_PAGE_SIZE;
	combined += state->row;
	state->selectedIdx = combined;

	if(syGameFlagGet(FLAG_INFO_WINDOW_HOLDS_MESSAGE) == 0) {
		/* As in the item list: the Japanese release clears the info window, and
		   in every edition but the Japanese it redraws the window's saved
		   rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub(menu->infoWin);
#else
		mnWindowSprClearListStub(menu->infoWin);
#endif
		sySynch(1);

		combined = state->selectedIdx;
		if(menu->psynergies[combined] != 0) {
			Sint32 masked = (Sint32)(MsgAbilityDescription +
				(menu->psynergies[combined] & ABILITY_DESCRIPTION_MASK));

			/* Japanese draws the description as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
			mnTextDrawMessageStub(masked, menu->infoWin, 0, 0);
#else
			mnTextDrawMessageAsTilesStub(masked, menu->infoWin, 0, 0);
#endif
			/* In the German, Spanish, French and Italian editions the menu
			   waits a tick after drawing it. */
#if PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION
			sySynch(1);
#endif
		}
	} else {
		/* BUG: clears FLAG_SCENE_TEMP_FF instead of
		   FLAG_INFO_WINDOW_HOLDS_MESSAGE, so a kept message keeps the
		   descriptions away until another screen clears it. */
#if FIX_KEPT_MESSAGE_FLAG_CLEAR
		syGameFlagClear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
#else
		syGameFlagClear(FLAG_SCENE_TEMP_FF);
#endif
	}

	for(row = 0; row < LIST_PAGE_SIZE; row++) {
		if(row == state->row) {
			mnWindowSetHighlightRect(
				menu->psynergyWin, 1, row * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
		} else {
			mnWindowSetHighlightRect(
				menu->psynergyWin, 1, row * 2 + 1, 14, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

/* The Japanese page draws its page icons, PP heading and names at other
   columns. */
#define PSYNERGY_PAGE_ICONS_X JAPANESE_OR(120, 112)
#define PSYNERGY_PP_HEADING_X JAPANESE_OR(72, 96)
#define PSYNERGY_NAME_X		  JAPANESE_OR(32, 16)

/* Draws one page of the Psynergy list: each name and PP cost, red when the PP
   is short and yellow when it cannot be used from the menu. */
Sint32 mnPsynergyMenuDrawPage(
	Sint32 win, Sint32 unused, const PAGED_LIST *state)
{
	Uint32				 firstEntry;
	Uint32				 visibleCnt;
	Uint8				 row;
	Sint32				 cursor;
	UNIT				*owner;
	ABILITY_DEFINITION	*ability;
	PSYNERGY_MENU_STATE *menu = Ram_HeapSlots->menuState;

	(void)unused;

	mnWindowSprRedrawSavedRectStub(win);
	mnWindowDrawDividerLineStub(win, 0, 11, 16, 11);

	if(PSYNERGY_MENU_SHORTCUT_HELP & menu->flg) {
		/* "L R:Set shortcut" */
		mnTextDrawMessageAsTilesStub((Sint32)MsgShortcutSetL - 1, win, 0, 88);
	} else {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgChangeCharacterHelp, win, CHANGE_HELP_X, 88);
	}

	firstEntry = state->page * LIST_PAGE_SIZE;
	visibleCnt = (Uint8)(state->entryCnt - firstEntry);
	if(visibleCnt > LIST_PAGE_SIZE) {
		visibleCnt = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, win, PSYNERGY_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(
		win, state->entryCnt, LIST_PAGE_SIZE, state->page, PAGE_NUMBERS_RIGHT);
	/* "PP" */
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgPsynergyChooseTarget + 2, win, PSYNERGY_PP_HEADING_X, 0);

	row = 0;
	if(visibleCnt > row) {
		cursor = firstEntry;
		do {
			owner = syUnitGet(menu->memberIds[0]);
			ability = gmAbilityGetDirectStub(
				PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

			if(ability->ppCost > owner->pp) {
				mnTextSetColStub(TEXT_COLOR_RED);
			} else if(mnPsynergyMenuIsActionRestricted(PSYNERGY_ABILITY_MASK &
						  menu->psynergies[cursor]) != 0) {
				mnTextSetColStub(TEXT_COLOR_YELLOW);
			} else {
				mnTextSetColStub(TEXT_COLOR_NORMAL);
			}

			mnTextDrawMessageAsTilesStub(
				(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) +
					(Sint32)MsgAbilityName,
				win, PSYNERGY_NAME_X, row * 16 + 8);
			mnTextDrawNumAtOffsetStub(
				ability->ppCost, 2, win, 104, row * 16 + 8);
			mnTextSetColStub(TEXT_COLOR_NORMAL);

			row++;
			cursor++;
		} while(visibleCnt > row);
	}

	return 1;
}

#undef PSYNERGY_PAGE_ICONS_X
#undef PSYNERGY_PP_HEADING_X
#undef PSYNERGY_NAME_X

/* The Japanese list puts its cursor further right. */
#define PSYNERGY_LIST_CURSOR_X JAPANESE_OR(98, 88)

Bool mnPsynergyMenuIsActionRestricted(Sint32 actionEntry);

/* The Psynergy list of the party member in side paneIdx, five a page. Up and
   Down move the cursor, Left and Right turn pages, L and R change party member,
   and Select held shows how to set a shortcut, L or R then setting it. Returns
   the chosen Psynergy entry, or -1 when the player backs out. */
Sint32 mnPsynergyMenuRunList(Sint32 paneIdx)
{
	PSYNERGY_MENU_STATE *menu;
	ABILITY_DEFINITION	*ability;
	WINDOW_SPR			*icon;
	Sint32				 win;
	Bool				 changed;
	Sint32				 moved;
	Sint32				 listMode;
	Sint32				 member;
	Sint32				 ownerCnt;
	Uint8				 i;
	UNIT				*owner;
	Sint32				 result;
	Bool				 redraw;
	Bool				 done;
	Sint32				 prev;
	Sint32				 shortcutHelp;
	Sint32				 scratch[5];
	PAGED_LIST			 state;

	menu = Ram_HeapSlots->menuState;
	result = 0;
	prev = 0;
	shortcutHelp = 0;
	menu->sideIcon[paneIdx]->motion = ICON_HIDDEN;
	mnWindowUpdateOrCreate(&menu->listWin, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	win = menu->listWin;
	done = FALSE;

	while(done == 0 && !syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		owner = syUnitGet(menu->memberIds[paneIdx]);
		if(menu->mode != PSYNERGY_MODE_USE)
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				owner, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
		else
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				owner, menu->psynergies, PSYNERGY_LIST_FIELD);
		mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
		mnPsynergyMenuBuildPagedList(&state, paneIdx);
		changed = TRUE;
		redraw = TRUE;
		menu->sideIcon[paneIdx]->motion = ICON_SHOWN;

		while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
			mnCursorPos(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

			if(changed != 0) {
				changed = FALSE;
				if(menu->psynergies[prev] != 0) {
					icon = menu->entryIcons[prev];
					mnIconShow(icon);
				}
				if(redraw != 0) {
					redraw = FALSE;
					sySynch(1);
					mnPsynergyMenuDrawPage(win, 0, &state);
				}
				mnPsynergyMenuShowSelectedRow(win, scratch, &state);
				menu->sideAction[paneIdx] = menu->psynergies[state.selectedIdx];
				menu->cursorIcon->motion = ICON_HIDDEN;
				if(menu->psynergies[state.selectedIdx] != 0) {
					icon = menu->entryIcons[state.selectedIdx];
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->memberCnt; i++)
					fdSprStartAnimationStub(menu->tabSprs[i], ANIM_STAND);
			}

			sySynch(1);
			prev = state.selectedIdx;

			if((PerWork.data & KEY_SELECT) == 0)
				moved = mnPagedListHandleInput(
					0, state.entryCnt, LIST_PAGE_SIZE, &state.row, &state.page);
			else
				moved = -1;
			if(moved == 1) {
				redraw = TRUE;
				changed = TRUE;
			}
			if(moved == 0)
				changed = TRUE;
			if(moved == -1)
				changed = FALSE;

			if(menu->mode == PSYNERGY_MODE_USE) {
				if((PerWork.push & KEY_SELECT) && shortcutHelp == 0) {
					ability = gmAbilityGetDirectStub(
						menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
					if(ability->fieldPsynergy == 0) {
						sySndPlayStub(SE_MENU_ERROR);
					} else {
						sySndPlayStub(SE_NOTE_MID);
						shortcutHelp = 1;
						menu->flg |= PSYNERGY_MENU_SHORTCUT_HELP;
						mnWindowClearInteriorTilesStub(win, 0, 88, 120, 96);
						mnTextDrawMessageAsTilesStub(
							MsgShortcutHelp, win, 0, 88);
					}
				}
				if((PerWork.data & KEY_SELECT) == 0 && shortcutHelp == 1) {
					shortcutHelp = 0;
					menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
					mnWindowClearInteriorTilesStub(win, 0, 88, 120, 96);
					mnTextDrawMessageAsTilesStub(
						MsgChangeCharacterHelp, win, CHANGE_HELP_X, 88);
				}
			}

			if(PerWork.push & KEY_A) {
				if(menu->mode != PSYNERGY_MODE_USE) {
					sySndPlayStub(SE_PSYNERGY_START);
					result = menu->psynergies[state.selectedIdx];
					done = TRUE;
					break;
				}
				if(menu->psynergies[state.selectedIdx] != 0) {
					if(mnPsynergyMenuIsActionRestricted(
						   menu->psynergies[state.selectedIdx]) != 0) {
						sySndPlayStub(SE_MENU_ERROR);
						continue;
					}
					ability = gmAbilityGetDirectStub(
						menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
					if(ability->ppCost > owner->pp) {
						sySndPlayStub(SE_MENU_ERROR);
					} else {
						sySndPlayStub(SE_NOTE_LOW);
						result = menu->psynergies[state.selectedIdx];
						done = TRUE;
						break;
					}
				}
			}

			if(PerWork.push & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}

			if(((PerWork.repeat & KEY_R) || (PerWork.repeat & KEY_L)) &&
				(PerWork.data & KEY_SELECT) == 0) {
				member = menu->tabIdx[paneIdx];
				ownerCnt = menu->memberCnt;
				listMode = menu->mode != PSYNERGY_MODE_USE
					? PSYNERGY_LIST_SHORTCUTS
					: PSYNERGY_LIST_FIELD;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				menu->selectedIdxByMember[menu->memberIds[paneIdx]] =
					state.selectedIdx;
				do {
					if(PerWork.repeat & KEY_R)
						member++;
					else
						member--;
					member = (member + ownerCnt) % ownerCnt;
					menu->selectedMember = menu->memberTbl[member];
					menu->memberIds[0] = menu->memberTbl[member];
					menu->psynergyCnt = mnPsynergyMenuCollectActions(
						syUnitGet(menu->memberIds[0]), menu->psynergies,
						listMode);
				} while(menu->psynergyCnt == 0);
				menu->tabIdx[paneIdx] = member;
				mnCreatePartyFigures(member / 4 * 4);
				mnMarkChosenMember(menu->auxiliaryWin, member, ownerCnt);
				menu->pageArrows[0]->motion = ICON_HIDDEN;
				menu->pageArrows[1]->motion = ICON_HIDDEN;
				sySynch(1);
				mnStatPanelDraw(menu->statWin, menu->memberTbl[member], 0,
					STATUS_PANEL_EXPERIENCE);
				mnReloadMemberPortrait(menu, menu->memberTbl[member]);
				break;
			}

			if((PerWork.push & KEY_L) && (PerWork.data & KEY_SELECT)) {
				ability =
					gmAbilityGetDirectStub(menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				} else if(mnPsynergyMenuSetShortcut(menu->memberIds[paneIdx],
							  menu->psynergies[state.selectedIdx],
							  PSYNERGY_SHORTCUT_L) != 0) {
					sySndPlayStub(SE_PSYNERGY_START);
					result = menu->psynergies[state.selectedIdx];
					menu->mode = PSYNERGY_MODE_SET_L;
					done = TRUE;
					break;
				}
			}

			if((PerWork.push & KEY_R) && (PerWork.data & KEY_SELECT)) {
				ability =
					gmAbilityGetDirectStub(menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				} else if(mnPsynergyMenuSetShortcut(menu->memberIds[paneIdx],
							  menu->psynergies[state.selectedIdx],
							  PSYNERGY_SHORTCUT_R) != 0) {
					sySndPlayStub(SE_PSYNERGY_START);
					result = menu->psynergies[state.selectedIdx];
					menu->mode = PSYNERGY_MODE_SET_R;
					done = TRUE;
					break;
				}
			}
		}
	}

	menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
	mnIconShow(menu->entryGridCursor);
	menu->sideRow[paneIdx] = state.selectedIdx;
	menu->selectedIdxByMember[menu->memberIds[paneIdx]] = state.selectedIdx;
	menu->sideAction[paneIdx] = result;
	if(syGameFlagGet(FLAG_MENU_START_PRESSED) != 0)
		result = -1;
	sySynch(1);
	return result;
}

#undef PSYNERGY_LIST_CURSOR_X

/* A Psynergy entry's action id, its PSYNERGY_ABILITY_MASK bits, taken out by
   shifts. */
#define PSYNERGY_ABILITY_OF(entry) (((Uint32)(entry) << 18) >> 18)

/* Returns 1 when actionEntry cannot be used from the menu: it has no field
   effect and is not usable both in the field and in battle. */
Bool mnPsynergyMenuIsActionRestricted(Sint32 actionEntry)
{
	ABILITY_DEFINITION *action =
		gmAbilityGetDirectStub(PSYNERGY_ABILITY_OF(actionEntry));

	if(action->fieldPsynergy != 0 ||
		(action->flg & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE)) ==
			(ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
		return FALSE;
	return TRUE;
}
