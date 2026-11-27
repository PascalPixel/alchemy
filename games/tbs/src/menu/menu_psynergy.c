/* @file */
/* The Psynergy menu: the field menu's Psynergy screen, the party member and */
/* Psynergy lists, targets, and the L and R shortcuts. */
/* Part of menu.c. */

#include "menu_internal.h"

/* A Psynergy shortcut, and the field's Psynergy request packed the same way: */
/* its party member above bit 10, the Psynergy's id below. */
#define PSYNERGY_SHORTCUT_MEMBER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK	   0x3ff

/* The bits of a listed Psynergy the info window's description is found by: */
/* nine, where the list's other reads take PSYNERGY_ABILITY_MASK's fourteen. */
#define ABILITY_DESCRIPTION_MASK 0x1ff

/* PSYNERGY_MENU_STATE.flg: set on going back to the list after a use or a */
/* cancelled target, which nothing reads; and while Select shows the shortcut */
/* help in the list. */
#define PSYNERGY_MENU_BACK_TO_LIST	1
#define PSYNERGY_MENU_SHORTCUT_HELP 2

/* The German, Spanish, French and Italian Psynergy menus save the window */
/* tiles they draw over and restore them after. */
#if TBS_PATCH_2
#define PSYNERGY_PROMPT_TILES	   BG_CHAR_BLOCK(1)
#define PSYNERGY_PROMPT_TILES_SIZE 8192
#endif

/* The highlight over the Psynergy list's rows. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define PSYNERGY_DETAIL_ROW_X	  0
#define PSYNERGY_DETAIL_ROW_WIDTH 15
#else
#define PSYNERGY_DETAIL_ROW_X	  1
#define PSYNERGY_DETAIL_ROW_WIDTH 14
#endif

/* The Psynergy list's page icons, "PP" label, names and hand cursor. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define PSYNERGY_PAGE_ICONS_X  112
#define PSYNERGY_PP_LABEL_X	   96
#define PSYNERGY_NAME_X		   16
#define PSYNERGY_LIST_CURSOR_X 88
#else
#define PSYNERGY_PAGE_ICONS_X  120
#define PSYNERGY_PP_LABEL_X	   72
#define PSYNERGY_NAME_X		   32
#define PSYNERGY_LIST_CURSOR_X 98
#endif

/* The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over
 */
/* the field, which stops meanwhile. */
s32 mnPsynergyMenuRun(void) {
#if TBS_PATCH_2
	void *saved = syHeapAllocEwram(PSYNERGY_PROMPT_TILES_SIZE);
#endif
	PSYNERGY_MENU_STATE *menu =
		syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	s32 member;
	s32 unused;
	s32 psynergy;
	s32 result;

	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	menu->memberCnt = gmPartyListMembersStub(menu->memberTbl);
	mnItemMenuInit(0, 3, 0, 7);
	menu->msgWin = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
#if TBS_PATCH_2
	mnSuspendFieldTasks();
	syWordsCopy(saved, PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE);
	syWordsFill(PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE,
				PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif
	/* Let the player choose. */
	mnStartWatchingStart();
	result = mnPsynergyMenuRunSteps(&member, &unused, &psynergy);
	mnStopWatchingStart();
	if(result == 1) {
		FIELD_STATE *target = FieldStateWork;
		u16 request;
		/* The action looked up here is not used. */
		mnAbilityGet(PSYNERGY_ABILITY_MASK & menu->sideAction[0]);
		request = psynergy | (member << PSYNERGY_SHORTCUT_MEMBER_SHIFT);
		target->psynergy = request;
	}
	/* Close the menu and give the screen back to the field. */
	mnWindowSprClearListStub((void *)menu->statWin);
	WINDOW_STATE->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	ActorSystemStateWork->suspended = 0;
#if TBS_PATCH_2
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy(PSYNERGY_PROMPT_TILES, saved, PSYNERGY_PROMPT_TILES_SIZE);
	WINDOW_STATE->holdTilemapUpload = 0;
	syHeapRelease(saved);
	sySynch(1);
	mnResumeFieldTasks();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	WINDOW_STATE->holdTilemapUpload = 0;
	gmPartyClearUnusableShortcutsStub();
	return result;
}

/* The steps of the Psynergy menu (mnPsynergyMenuRunSteps). */
typedef enum PSYNERGY_MENU_STEP {
	PSYNERGY_STEP_MEMBER,
	PSYNERGY_STEP_LIST,
	/* how the Psynergy is used (mnPsynergyMenuClassifySelectedPsynergy) */
	PSYNERGY_STEP_CLASSIFY,
	PSYNERGY_STEP_TARGET,
	PSYNERGY_STEP_APPLY,
} PSYNERGY_MENU_STEP;

/* Runs the Psynergy menu: choose a party member and one of their Psynergy, */
/* then use it on a party member (or the whole party), or, in the shortcut */
/* modes, set it as the L or R shortcut. */
s32 mnPsynergyMenuRunSteps(s32 *outMember, s32 *unused, s32 *outAction) {
	PSYNERGY_MENU_STATE *menu;
	s32 useResult;
	s32 step;
	s32 done;
	s32 chosenAction;
	s32 targetMode;
	s32 actionEntry;
	s32 chosenTarget;
	ABILITY_DEFINITION *action;

	step = PSYNERGY_STEP_MEMBER;
	done = 0;
	menu = MenuState;
	useResult = 0;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		switch(step) {
		/* Choose whose Psynergy to look at. */
		case PSYNERGY_STEP_MEMBER: {
			s32 chosen;

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
		 * or */
		/* R shortcut and start again. */
		case PSYNERGY_STEP_LIST:
			sySynch(1);
			gmUnitGetStub(menu->memberIds[0]);
			if(menu->psynergyCnt == 0) {
				step = PSYNERGY_STEP_MEMBER;
			}
			else {
				switch(menu->mode) {
				case PSYNERGY_MODE_USE:
					mnItemMenuDrawMessage(0, MsgPsynergyChooseAbility);
					break;
				case PSYNERGY_MODE_SET_L:
					mnItemMenuDrawMessage(0, MsgPsynergyChooseForL);
					break;
				case PSYNERGY_MODE_SET_R:
					mnItemMenuDrawMessage(0, MsgPsynergyChooseForR);
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
													  chosenAction,
													  PSYNERGY_SHORTCUT_L);
							mnWindowSprClearListStub((void *)menu->infoWin);
							mnItemMenuShowMessage(MsgShortcutSetL, -1, -1);
#if !TBS_PATCH_1
							mnWindowSprRedrawSavedRectStub(menu->infoWin);
#endif
						}
						else {
							mnPsynergyMenuSetShortcut(menu->memberIds[0],
													  chosenAction,
													  PSYNERGY_SHORTCUT_R);
							mnWindowSprClearListStub((void *)menu->infoWin);
							mnItemMenuShowMessage(MsgShortcutSetR, -1, -1);
#if !TBS_PATCH_1
							mnWindowSprRedrawSavedRectStub(menu->infoWin);
#endif
						}
						step = PSYNERGY_STEP_MEMBER;
					}
				}
			}
			break;

		/* Choose the party member to use it on. */
		case PSYNERGY_STEP_TARGET:
			mnItemMenuDrawMessage(0, MsgPsynergyChooseTarget);
			chosenTarget = mnPsynergyMenuSelectTarget(0);
			step = PSYNERGY_STEP_APPLY;
			if(chosenTarget == -1) {
				menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
				step = PSYNERGY_STEP_LIST;
			}
			break;

		/* How is it used? On one party member, on the whole party, or on the
		 * field */
		/* once the menu closes. */
		case PSYNERGY_STEP_CLASSIFY:
			targetMode = mnPsynergyMenuClassifySelectedPsynergy();
			if(targetMode == ITEM_USE_MODE_CHOOSE_MEMBER) {
				step = PSYNERGY_STEP_TARGET;
				break;
			}
			if(targetMode == ITEM_USE_MODE_WHOLE_PARTY) {
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
		 * the */
		/* target when it failed on one party member. */
		case PSYNERGY_STEP_APPLY:
			chosenTarget = 0;
			actionEntry = menu->sideAction[0];
			useResult = mnAbilityApplyToTargets(actionEntry, menu->memberIds[0],
												menu->memberIds[1], 0);
			if(menu->memberIds[1] == MENU_TARGET_WHOLE_PARTY) {
				menu->memberIds[1] = menu->memberIds[0];
				chosenTarget = MENU_TARGET_WHOLE_PARTY;
			}
			if(useResult != -1) {
				action =
					mnAbilityGet(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
				gmUnitAddPpStub(menu->memberIds[0], -action->ppCost);
			}
			gmUnitCalcStub(menu->memberIds[0]);
			if(useResult != -1) {
				mnStatPanelDraw(menu->statWin, menu->memberIds[1], 0,
								STATUS_PANEL_EXPERIENCE);
				mnAbilityPlayUseSnd(menu->sideAction[0] &
									PSYNERGY_ABILITY_MASK);
				mnWindowSprClearListStub((void *)menu->infoWin);
				mnItemMenuShowMessage(menu->useResult + MsgItemUseResult, 0,
									  -1);
			}
			else {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub((void *)menu->infoWin);
				mnItemMenuShowMessage(menu->useResult + MsgItemUseResult,
									  useResult, useResult);
			}
			if(useResult != -1) {
				useResult = 1;
				menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
				step = PSYNERGY_STEP_LIST;
			}
			else {
				menu->skipSlide = 1;
				if(chosenTarget == MENU_TARGET_WHOLE_PARTY) {
					menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
					step = PSYNERGY_STEP_LIST;
				}
				else {
					step = PSYNERGY_STEP_TARGET;
				}
			}
			break;

		default:
			done = 1;
			break;
		}
	}

	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
		useResult = -1;
	}
	return useResult;
}

/* Says how the selected Psynergy is used from the menu, as an item is (enum */
/* ITEM_USE_MODE): ITEM_USE_MODE_FROM_FIELD once the menu closes when the scene
 */
/* has an event for its field Psynergy (fieldPsynergy) where the leader faces,
 */
/* ITEM_USE_MODE_WHOLE_PARTY for ABILITY_RANGE_ALL, */
/* ITEM_USE_MODE_CHOOSE_MEMBER when it targets ABILITY_TARGET_ALLIES, and */
/* ITEM_USE_MODE_FROM_FIELD otherwise. */
s32 mnPsynergyMenuClassifySelectedPsynergy(void) {
	ABILITY_DEFINITION *action;
	s32 classification;

	action = mnAbilityGet(PSYNERGY_ABILITY_MASK &
						  ((PSYNERGY_MENU_STATE *)MenuState)->sideAction[0]);
	if(scHasPsynergyTriggerStub(action->fieldPsynergy) != 0) {
		return ITEM_USE_MODE_FROM_FIELD;
	}
	if(action->range == ABILITY_RANGE_ALL) {
		return ITEM_USE_MODE_WHOLE_PARTY;
	}
	if(action->tgtMode != ABILITY_TARGET_ALLIES) {
		classification = ITEM_USE_MODE_FROM_FIELD;
	}
	else {
		classification = ITEM_USE_MODE_CHOOSE_MEMBER;
	}
	return classification;
}

/* Lets the player choose a party member in side partySlot (0 or 1), starting */
/* from the one chosen last, with each party member's Psynergy shown as the */
/* cursor passes. */
s32 mnPsynergyMenuSelectPartySlot(s32 partySlot) {
	PSYNERGY_MENU_STATE *menu = MenuState;
	s32 memberIndex = menu->tabIdx[partySlot];
	WINDOW_SPR *icon;
	UNIT *unit;
	u16 *actions;
	s32 result;

	result = 0;
	icon = menu->sideIcon[partySlot];
	icon->motion = ICON_SHOWN;
	icon->anmStep = result;
	menu->cursorIcon->motion = ICON_HIDDEN;
	menu->tabCnts[partySlot] = menu->memberCnt;

	if(memberIndex == -1) {
		menu->tabIdx[partySlot] = 0;
		memberIndex = 0;
	}
	else {
		mnCursorSlide(MENU_MEMBER_CURSOR_X(memberIndex), 16);
	}

	unit = gmUnitGetStub(menu->memberTbl[memberIndex]);
	actions = menu->psynergies;
	menu->psynergyCnt =
		mnPsynergyMenuCollectActions(unit, actions, PSYNERGY_LIST_FIELD);
	result = mnPsynergyMenuSelectMember(menu->memberTbl, actions);
	icon = menu->sideIcon[partySlot];
	mnIconShow(icon);
	sySynch(1);
	return result;
}

/* The party member row of the Psynergy menu. */
s32 mnPsynergyMenuSelectMember(u16 *memberIds, u16 *unused) {
	PSYNERGY_MENU_STATE *menu;
	s32 selection;
	s32 memberCount;
	s32 redraw;
	s32 result;
	UNIT *unit;
	WINDOW *window;
	s32 shown;
	s32 i;
	u16 *actions;
	s32 shortcutCount;
	s8 fieldCount;

	menu = MenuState;
	selection = menu->tabIdx[0];
	memberCount = menu->tabCnts[0];
	redraw = 1;
	result = 0;
	shown = 0;
	menu->mode = result;
	unit = gmUnitGetStub(memberIds[selection]);
	if(mnWindowUpdateOrCreate(&menu->psynergyWin, 13, 3, 17, 10,
							  WINDOW_FLAG_INSTANT))
		mnSpawnIconEntries((ITEM_MENU_STATE *)menu, menu->psynergyWin);
	if(mnWindowUpdateOrCreate(&menu->shortcutWin, 13, 13, 17, 4,
							  WINDOW_FLAG_INSTANT)) {
		menu->cursorIcon = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->shortcutWin, 0, result);
		menu->cursorIcon->motion = ICON_HIDDEN;
	}
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = 0;
			selection = (memberCount + selection) % memberCount;
			window = menu->statWin;
			unit = gmUnitGetStub(memberIds[selection]);
			mnPsynergyMenuRefreshMemberPsynergy(memberIds[selection]);
			mnStatPanelDraw(window, memberIds[selection], 0,
							STATUS_PANEL_EXPERIENCE);
			mnPsynergyMenuDrawShortcuts(menu->shortcutWin,
										memberIds[selection]);
			mnReloadMemberPortrait(menu, memberIds[selection]);
			for(i = 0; i < 4; i++)
				menu->rowPositions[i] = MENU_MEMBER_Y;
			/* Raise the chosen party member. */
			menu->rowPositions[selection] = MENU_MEMBER_RAISED_Y;
			if(!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown) {
				mnWindowSprClearListStub((void *)menu->infoWin);
#if TBS_PATCH_1
				mnWindowSprRedrawSavedRectStub((void *)menu->infoWin);
#endif
				mnDrawCoins(menu->infoWin);
				shown = 1;
			}
			else {
				gmFlagClearStub(FLAG_MENU_INFO_KEPT);
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		sySynch(1);
		if(KeysPressed & KEY_A) {
			if(menu->psynergyCnt) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = memberIds[selection];
				break;
			}
			sySndPlayStub(SE_MENU_ERROR);
		}
		/* L or R: set that shortcut, if the party member has field Psynergy for
		 * it. */
		if((KeysPressed & KEY_L) || (KeysPressed & KEY_R)) {
			result = memberIds[selection];
			if(KeysPressed & KEY_L)
				menu->mode = PSYNERGY_MODE_SET_L;
			else
				menu->mode = PSYNERGY_MODE_SET_R;
			actions = syHeapAlloc(32 * sizeof(u16));
			fieldCount = mnPsynergyMenuCollectActions((UNIT *)unit, actions,
													  PSYNERGY_LIST_SHORTCUTS);
			syHeapRelease(actions);
			shortcutCount = fieldCount;
			if(shortcutCount == 0) {
				menu->mode = shortcutCount;
				sySndPlayStub(SE_MENU_ERROR);
			}
			else {
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
		}
		if(KeysPressed & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(KeysRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection--;
			redraw = 1;
		}
		if(KeysRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection++;
			redraw = 1;
		}
	}
	menu->tabIdx[0] = selection;
	menu->selectedMember = memberIds[selection];
	menu->memberIds[0] = memberIds[selection];
	return result;
}

/* Shows party member memberId's Psynergy icons in the Psynergy window, eight */
/* to a row, or "no Psynergy" when they have none. */
void mnPsynergyMenuRefreshMemberPsynergy(s32 memberId) {
	u16 *psynergies;
	PSYNERGY_MENU_STATE *menu;
	UNIT *member;

	menu = MenuState;
	member = gmUnitGetStub(memberId);
	psynergies = menu->psynergies;
	menu->psynergyCnt =
		mnPsynergyMenuCollectActions(member, psynergies, PSYNERGY_LIST_FIELD);
	mnWindowSprRedrawSavedRectStub((void *)menu->psynergyWin);
	mnPsynergyMenuPlaceEntryIcons(108, 32, 8);
	mnPsynergyMenuDrawPsynergyIcons(psynergies);
	if(menu->psynergyCnt == 0) {
		mnTextDrawMessageAsTilesStub(MsgPsynergyMenuEmpty, menu->psynergyWin, 0,
									 24);
	}
}

UNUSED s32 mnPsynergyMenuReturnTrue(void) { return 1; }

UNUSED void mnPsynergyMenuNoOp(void) {}

/* Chooses the party member to use the selected Psynergy on. */
s32 mnPsynergyMenuSelectTarget(s32 targetMode) {

	PSYNERGY_MENU_STATE *menu;
	s32 selection;
	s32 count;
	s32 redraw;
	s32 result;
	s32 shown;
	WINDOW_SPR *marker;

	menu = MenuState;
	selection = menu->tabIdx[1];
	count = menu->memberCnt;
	redraw = 1;
	result = 0;
	shown = 0;
	gmUnitGetStub(menu->memberTbl[menu->tabIdx[0]]);
	mnCursorSlide(MENU_MEMBER_CURSOR_X(selection), 16);
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = 0;
			selection = (selection + count) % count;
			gmUnitGetStub(menu->memberTbl[selection]);
			/* The marker over the party member, three tiles a party member; no
			 * screen */
			/* makes it, so it is NULL (mnCursorCreate). */
			marker = menu->sideIcon[1];
			marker->spr.oam.x = marker->x =
				(((menu->auxiliaryWin)->x + selection * 3) << 3) - 2;
			if(targetMode == 0) {
				mnStatPanelDraw(menu->statWin, menu->memberTbl[selection], 0,
								STATUS_PANEL_EXPERIENCE);
				mnReloadMemberPortrait(menu, menu->memberTbl[selection]);
				if(!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown) {
					MENU_RESET_DESCRIPTION_WINDOW((void *)menu->infoWin);
					MENU_DRAW_DESCRIPTION(
						(menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
							MsgAbilityDescription,
						menu->infoWin, 0, 0);
					shown = 1;
				}
				else {
					gmFlagClearStub(FLAG_MENU_INFO_KEPT);
				}
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		sySynch(1);
		if(KeysPressed & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = menu->memberTbl[selection];
			break;
		}
		if(KeysPressed & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(KeysRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection--;
			redraw = 1;
		}
		if(KeysRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selection++;
			redraw = 1;
		}
	}
	/* The marker, still NULL. */
	marker = menu->sideIcon[1];
	menu->tabIdx[1] = selection;
	mnIconShow(marker);
	marker->motion = ICON_HIDDEN;
	sySynch(1);
	menu->tabIdx[1] = selection;
	menu->selectedMember = menu->memberTbl[selection];
	menu->memberIds[1] = menu->memberTbl[selection];
	return result;
}

/* Sets the L or R Psynergy shortcut (PSYNERGY_SHORTCUT) to abilityId as */
/* memberId uses it, packed as the field's Psynergy request is. Returns 1. */
s32 mnPsynergyMenuSetShortcut(s32 memberId, s32 abilityId, s32 shortcutIndex) {
	s32 id = abilityId & PSYNERGY_ABILITY_MASK;
	s32 code = (memberId << PSYNERGY_SHORTCUT_MEMBER_SHIFT) | id;

	if(shortcutIndex == PSYNERGY_SHORTCUT_L) {
		MainRoutineStateWork.firstShortcut = code;
	}
	else {
		MainRoutineStateWork.secondShortcut = code;
	}
	return 1;
}

/* Writes the L and R Psynergy shortcuts into window: each one's Psynergy and */
/* party member, or that it is empty, under a heading that says how to change */
/* them. A name too wide for the line (measured as a window, over 10 tiles; 11
 */
/* in Japanese) hides the party member (international) or drops the colon after
 */
/* "L" and "R" (Japanese). Returns 1. */
#if LANGUAGE != LANGUAGE_JAPANESE

s32 mnPsynergyMenuDrawShortcuts(WINDOW *window, s32 memberId) {
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	s32 wide;

	if(MainRoutineStateWork.firstShortcut != 0 &&
	   MainRoutineStateWork.secondShortcut != 0)
		mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, window, 0, -8);
	else
		mnTextDrawMessageAsTilesStub(MsgShortcutLabel, window, 0, -8);

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if(MainRoutineStateWork.firstShortcut != 0) {
		mnMessageArgumentPushStub(MainRoutineStateWork.firstShortcut &
									  PSYNERGY_SHORTCUT_ID_MASK,
								  MESSAGE_ARGUMENT_ABILITY);
		mnTextDrawMessageAsTilesStub(MsgShortcutNameL, window, 0, 0);
		if(wide == 0)
			mnTextDrawStringAtOffsetStub(
				gmUnitGetStub(MainRoutineStateWork.firstShortcut >>
							  PSYNERGY_SHORTCUT_MEMBER_SHIFT)
					->name,
				window, 80, 0);
	}
	else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, window, 0, 0);
	}

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if(MainRoutineStateWork.secondShortcut != 0) {
		mnMessageArgumentPushStub(MainRoutineStateWork.secondShortcut &
									  PSYNERGY_SHORTCUT_ID_MASK,
								  MESSAGE_ARGUMENT_ABILITY);
		mnTextDrawMessageAsTilesStub(MsgShortcutNameR, window, 0, 8);
		if(wide == 0)
			mnTextDrawStringAtOffsetStub(
				gmUnitGetStub(MainRoutineStateWork.secondShortcut >>
							  PSYNERGY_SHORTCUT_MEMBER_SHIFT)
					->name,
				window, 80, 8);
		mnTextSetColStub(TEXT_COLOR_NORMAL);
	}
	else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, window, 0, 8);
	}
	return 1;
}
#else

s32 mnPsynergyMenuDrawShortcuts(WINDOW *window, s32 member) {
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	u16 wide;

	if(MainRoutineStateWork.firstShortcut != 0 &&
	   MainRoutineStateWork.secondShortcut != 0)
		mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, window, 0, -8);
	else
		mnTextDrawMessageAsTilesStub(MsgShortcutLabel, window, 0, -8);

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if(wide)
		mnTextDrawStringInWindowStub((u8 *)Menu_ShortcutLString, window, 0, 0);
	else
		mnTextDrawStringInWindowStub((u8 *)Menu_ShortcutLColonString, window, 0,
									 0);
	if(MainRoutineStateWork.firstShortcut != 0) {
		if(wide)
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.firstShortcut &
										  PSYNERGY_SHORTCUT_ID_MASK) +
											 MsgAbilityName,
										 window, 8, 0);
		else
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.firstShortcut &
										  PSYNERGY_SHORTCUT_ID_MASK) +
											 MsgAbilityName,
										 window, 16, 0);
		mnTextDrawStringAtOffsetStub(
			gmUnitGetStub(MainRoutineStateWork.firstShortcut >>
						  PSYNERGY_SHORTCUT_MEMBER_SHIFT)
				->name,
			window, 80, 0);
	}
	else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, window, 24, 0);
	}

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if(wide)
		mnTextDrawStringInWindowStub((u8 *)Menu_ShortcutRString, window, 0, 8);
	else
		mnTextDrawStringInWindowStub((u8 *)Menu_ShortcutRColonString, window, 0,
									 8);
	if(MainRoutineStateWork.secondShortcut != 0) {
		if(wide)
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.secondShortcut &
										  PSYNERGY_SHORTCUT_ID_MASK) +
											 MsgAbilityName,
										 window, 8, 8);
		else
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.secondShortcut &
										  PSYNERGY_SHORTCUT_ID_MASK) +
											 MsgAbilityName,
										 window, 16, 8);
		mnTextDrawStringAtOffsetStub(
			gmUnitGetStub(MainRoutineStateWork.secondShortcut >>
						  PSYNERGY_SHORTCUT_MEMBER_SHIFT)
				->name,
			window, 80, 8);
		mnTextSetColStub(TEXT_COLOR_NORMAL);
	}
	else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, window, 24, 8);
	}
	return 1;
}
#endif

/* Sets up a Psynergy grid screen: the cursor window with the party's figures,
 */
/* a 30 by 15 window below with a hidden grid cursor and a portrait, and 16 */
/* Psynergy icon sprites in two rows of eight from (96, 8), on an 8 by 2 grid.
 */
/* Nothing calls it. */
void mnPsynergyMenuCreateEntryGrid(void) {
	WINDOW *window;
	PSYNERGY_MENU_STATE *menu;
	WINDOW_SPR *cursor;
	s32 index;
	s32 x;
	s32 y;
	WINDOW_SPR **sprite;

	menu = MenuState;
	window = mnCursorCreate(menu);
	mnCreatePartyFigures((void *)window, 2, 2, 8, 0);

	window = mnWindowCreateStub(0, 5, 30, 15, WINDOW_FLAG_INSTANT);
	menu->psynergyWin = window;
	menu->selectedColumn = 0;
	menu->selectedRow = 0;
	menu->columnCnt = ICON_GRID_ROW;
	menu->rowCnt = 2;

	cursor = (WINDOW_SPR *)mnIconCreateHandCursor(window, 0, 4);
	cursor->motion = ICON_HIDDEN;
	menu->entryGridCursor = cursor;
	mnPortraitCreateStub(ISAAC, 0, 0, window, 0, 0);

	y = 8;
	index = 0;
	sprite = &menu->entryIcons[0];
	x = 96;
	do {
		*sprite++ = mnWindowSprCreateFromResourceStub(ICON_KIND_ABILITY, index,
													  window, x, y);
		index++;
		x += 16;
	} while(index < ICON_GRID_ROW);

	index = ICON_GRID_ROW;
	y = 24;
	sprite = &menu->entryIcons[ICON_GRID_ROW];
	x = 96;
	do {
		*sprite++ = mnWindowSprCreateFromResourceStub(ICON_KIND_ABILITY, index,
													  window, x, y);
		index++;
		x += 16;
	} while(index < 2 * ICON_GRID_ROW);
}

/* Releases the party figures and closes the Psynergy grid screen's windows. */
/* Nothing calls it. */
void mnPsynergyMenuCloseWindows(void) {
	PSYNERGY_MENU_STATE *menu;

	menu = MenuState;
	mnReleasePartyFigures();
	mnWindowCloseStub((void *)menu->auxiliaryWin, 1);
	mnWindowCloseStub((void *)menu->psynergyWin, 1);
	mnWindowCloseStub((void *)menu->msgWin, 1);
}

/* Loads each listed Psynergy's icon into its icon sprite (32 entries) and */
/* hides the icons of empty entries. */
void mnPsynergyMenuDrawPsynergyIcons(u16 *psynergies) {
	s32 i;
	WINDOW_SPR **icons;
	u16 *p;
	s32 psynergyId;

	icons = ((PSYNERGY_MENU_STATE *)MenuState)->entryIcons;
	p = psynergies;
	for(i = 0; i < PSYNERGY_SLOTS; i++) {
		psynergyId = *p++;
		if(psynergyId != 0) {
			mnSelectionIconLoadIntoSlotStub(ICON_KIND_ABILITY, psynergyId,
											(u8)(*icons)->idx, 0);
		}
		icons++;
	}
	mnHideEmptyEntryIcons(psynergies);
}

/* Lists unit's Psynergy into actions (PSYNERGY_SLOTS entries, cleared first) */
/* and returns how many there are, as mode (PSYNERGY_LIST_MODE) chooses; any
 */
/* mode other than PSYNERGY_LIST_SHORTCUTS and PSYNERGY_LIST_FIELD lists as */
/* PSYNERGY_LIST_ALL. */
s32 mnPsynergyMenuCollectActions(UNIT *unit, u16 *actions, s32 mode) {
	s32 clearIndex;
	s32 passCount;
	s32 actionCount;
	s32 slotIndex;
	s32 passIndex;

	passCount = (mode != PSYNERGY_LIST_FIELD) ? 4 : 3;
	for(clearIndex = 0; clearIndex < PSYNERGY_SLOTS; clearIndex++)
		actions[clearIndex] = 0;
	actionCount = 0;

	/* fieldPsynergy is the Psynergy's field effect. Passes 1 and 2 add nothing.
	 */
	if(mode == PSYNERGY_LIST_SHORTCUTS) {
		for(slotIndex = 0; slotIndex < PSYNERGY_SLOTS; slotIndex++) {
			if(unit->psynergy[slotIndex].encodedAction != 0) {
				if(mnAbilityGet(unit->psynergy[slotIndex].encodedAction &
								PSYNERGY_ABILITY_MASK)
					   ->fieldPsynergy != 0) {
					actions[actionCount] =
						unit->psynergy[slotIndex].encodedAction;
					actionCount++;
				}
			}
		}
	}
	else {
		for(passIndex = 0; passIndex < passCount; passIndex++) {
			for(slotIndex = 0; slotIndex < PSYNERGY_SLOTS; slotIndex++) {
				if(unit->psynergy[slotIndex].encodedAction != 0) {
					ABILITY_DEFINITION *ability =
						mnAbilityGet(unit->psynergy[slotIndex].encodedAction &
									 PSYNERGY_ABILITY_MASK);

					if(passIndex == 0 &&
					   (ability->fieldPsynergy != 0 ||
						(ability->flg & ABILITY_USABLE_IN_FIELD) != 0)) {
						actions[actionCount] =
							unit->psynergy[slotIndex].encodedAction;
						actionCount++;
					}
					else if(passIndex == 1) {
					}
					else if(passIndex == 2) {
					}
					else if(passIndex == 3 && ability->fieldPsynergy == 0 &&
							(ability->flg & ABILITY_USABLE_IN_FIELD) == 0) {
						actions[actionCount] =
							unit->psynergy[slotIndex].encodedAction;
						actionCount++;
					}
				}
			}
		}
	}

	return actionCount;
}

/* Fills result with the Psynergy list of the party member in side index: their
 */
/* unit, how many Psynergy are listed, and the page and row of the one chosen */
/* last for them (kept within the list), five to a page. Returns 1. */
s32 mnPsynergyMenuBuildPagedList(PAGED_LIST *result, s32 index) {
	s32 unitBits;
	PSYNERGY_MENU_STATE *menu = MenuState;
	s32 entryCount;
	s32 row;
	s32 page;
	s32 pageCount;
	s32 selectedIndex;
	s32 memberId;

	unitBits = (s32)gmUnitGetStub(menu->memberIds[index]);
	entryCount = menu->psynergyCnt;
	memberId = menu->memberIds[index];
	selectedIndex = menu->selectedIdxByMember[memberId];
	if((selectedIndex + 1) > entryCount) {
		selectedIndex = entryCount - 1;
	}
	page = selectedIndex / LIST_PAGE_SIZE;
	row = selectedIndex % LIST_PAGE_SIZE;
	pageCount = entryCount / LIST_PAGE_SIZE;
	if(entryCount % LIST_PAGE_SIZE != 0) {
		pageCount++;
	}
	result->memberState = unitBits;
	result->page = page;
	result->pageCnt = pageCount;
	result->row = row;
	result->entryCnt = entryCount;
	result->selectedIdx = selectedIndex;
	return 1;
}

/* Updates the Psynergy list for the row under the cursor: puts the Psynergy's
 */
/* description in the info window, unless a kept message is there, and */
/* highlights its row of the five. window and memberState are not used. */
s32 mnPsynergyMenuShowSelectedRow(WINDOW *window, s32 *memberState,
								  PAGED_LIST *state) {
	PSYNERGY_MENU_STATE *menu;
	s32 actionIndex;
	s32 rowIndex;

	menu = MenuState;
	actionIndex = state->page * LIST_PAGE_SIZE;
	actionIndex += state->row;
	state->selectedIdx = actionIndex;

	if(gmFlagGetStub(FLAG_MENU_INFO_KEPT) == 0) {
		MENU_RESET_DESCRIPTION_WINDOW(menu->infoWin);
		sySynch(1);

		actionIndex = state->selectedIdx;
		if(menu->psynergies[actionIndex] != 0) {
			s32 description =
				(menu->psynergies[actionIndex] & ABILITY_DESCRIPTION_MASK) +
				MsgAbilityDescription;
			MENU_DRAW_DESCRIPTION(description, menu->infoWin, 0, 0);
		}
	}
	else {
		/* BUG: clears FLAG_SCENE_TEMP_FF instead of FLAG_MENU_INFO_KEPT, so a
		 * kept */
		/* message keeps the descriptions away until another screen clears it.
		 */
#ifdef BUGFIX
		gmFlagClearStub(FLAG_MENU_INFO_KEPT);
#else
		gmFlagClearStub(FLAG_SCENE_TEMP_FF);
#endif
	}

	for(rowIndex = 0; rowIndex < LIST_PAGE_SIZE; rowIndex++) {
		if(rowIndex == state->row) {
			mnWindowSetHighlightRect((void *)menu->psynergyWin,
									 PSYNERGY_DETAIL_ROW_X, rowIndex * 2 + 1,
									 PSYNERGY_DETAIL_ROW_WIDTH, 1,
									 HIGHLIGHT_PALETTE);
		}
		else {
			mnWindowSetHighlightRect(
				(void *)menu->psynergyWin, PSYNERGY_DETAIL_ROW_X,
				rowIndex * 2 + 1, PSYNERGY_DETAIL_ROW_WIDTH, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

/* Draws one page of the Psynergy list into window: up to five names with their
 */
/* icons and PP costs, Psynergy the party member cannot afford in the no-HP */
/* colour and one restricted here in the low-HP colour, the page numbers, and */
/* the help line (how to set a shortcut while Select is held, else how to */
/* change party member). */
s32 mnPsynergyMenuDrawPage(WINDOW *window, s32 unused,
						   const PAGED_LIST *pageState) {
	u32 firstEntry;
	u32 visibleCount;
	u8 row;
	s32 cursor;
	UNIT *member;
	ABILITY_DEFINITION *ability;
	PSYNERGY_MENU_STATE *menu = MenuState;

	(void)unused;

	mnWindowSprRedrawSavedRectStub((void *)window);
	mnWindowDrawDividerLineStub(window, 0, 11, 16, 11);

	if(PSYNERGY_MENU_SHORTCUT_HELP & menu->flg) {
		mnTextDrawMessageAsTilesStub(MsgShortcutHelp, window, 0, 88);
	}
	else {
		mnTextDrawMessageAsTilesStub(MsgChangeCharacterHelp, window,
									 CHANGE_CHARACTER_HELP_X, 88);
	}

	firstEntry = pageState->page * LIST_PAGE_SIZE;
	visibleCount = (u8)(pageState->entryCnt - firstEntry);
	if(visibleCount > LIST_PAGE_SIZE) {
		visibleCount = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PSYNERGY_PAGE_ICONS_X,
				   34);
	mnPagedListDrawPageNumbers(window, pageState->entryCnt, LIST_PAGE_SIZE,
							   pageState->page, PAGE_NUMBERS_RIGHT);
	mnTextDrawMessageAsTilesStub(MsgPsynergyPp, window, PSYNERGY_PP_LABEL_X, 0);

	row = 0;
	if(visibleCount > row) {
		cursor = firstEntry;
		do {
			member = gmUnitGetStub(menu->memberIds[0]);
			ability =
				mnAbilityGet(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

			if(ability->ppCost > member->pp) {
				mnTextSetColStub(TEXT_COLOR_NO_HP);
			}
			else if(mnPsynergyMenuIsActionRestricted(
						PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) !=
					0) {
				mnTextSetColStub(TEXT_COLOR_LOW_HP);
			}
			else {
				mnTextSetColStub(TEXT_COLOR_NORMAL);
			}

			mnTextDrawMessageAsTilesStub(
				(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) +
					MsgAbilityName,
				window, PSYNERGY_NAME_X, row * 16 + 8);
			mnTextDrawNumAtOffsetStub(ability->ppCost, 2, window, 104,
									  row * 16 + 8);
			mnTextSetColStub(TEXT_COLOR_NORMAL);

			row++;
			cursor++;
		} while(visibleCount > row);
	}

	return 1;
}

/* The Psynergy list of the party member in sideIndex, five a page. */
s32 mnPsynergyMenuRunList(s32 sideIndex) {
	PSYNERGY_MENU_STATE *menu;
	ABILITY_DEFINITION *ability;
	WINDOW_SPR *icon;
	WINDOW *window;
	s32 changed;
	s32 moved;
	s8 listMode;
	s32 member;
	u8 i;
	s32 result;
	s32 redraw;
	s32 done;
	UNIT *memberUnit;
	s32 previous;
	s32 shortcutHelp;
	s32 memberState[5];
	PAGED_LIST state;

	menu = MenuState;
	result = 0;
	previous = 0;
	shortcutHelp = 0;
	menu->sideIcon[sideIndex]->motion = ICON_HIDDEN;
	mnWindowUpdateOrCreate(&menu->listWin, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	window = menu->listWin;
	done = 0;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		memberUnit = gmUnitGetStub(menu->memberIds[sideIndex]);
		if(menu->mode != PSYNERGY_MODE_USE) {
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				memberUnit, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
		}
		else {
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				memberUnit, menu->psynergies, PSYNERGY_LIST_FIELD);
		}
		mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
		mnPsynergyMenuBuildPagedList(&state, sideIndex);
		redraw = 1;
		changed = 1;
		menu->sideIcon[sideIndex]->motion = ICON_SHOWN;

		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
			mnCursorPos(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

			if(changed != 0) {
				changed = 0;
				if(menu->psynergies[previous] != 0) {
					icon = menu->entryIcons[previous];
					mnIconShow(icon);
				}
				if(redraw != 0) {
					redraw = 0;
					sySynch(1);
					mnPsynergyMenuDrawPage(window, 0, &state);
				}
				mnPsynergyMenuShowSelectedRow(window, memberState, &state);
				menu->sideAction[sideIndex] =
					menu->psynergies[state.selectedIdx];
				menu->cursorIcon->motion = ICON_HIDDEN;
				if(menu->psynergies[state.selectedIdx] != 0) {
					icon = menu->entryIcons[state.selectedIdx];
					/* Swell the selected icon, in depth slot 250, as in */
					/* mnItemMenuRunList. */
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->memberCnt; i++) {
					fdSprStartAnimationStub(menu->tabSprs[i], ANIM_STAND);
				}
			}

			sySynch(1);
			previous = state.selectedIdx;

			if((KeysHeld & KEY_SELECT) == 0) {
				moved = mnPagedListHandleInput(
					0, state.entryCnt, LIST_PAGE_SIZE, &state.row, &state.page);
			}
			else {
				moved = -1;
			}
			if(moved == 1) {
				redraw = 1;
				changed = 1;
			}
			if(moved == 0) {
				changed = 1;
			}
			if(moved == -1) {
				changed = 0;
			}

			/* Select shows the shortcut help while held, for field Psynergy. */
			if(menu->mode == PSYNERGY_MODE_USE) {
				if((KeysPressed & KEY_SELECT) != 0 && shortcutHelp == 0) {
					ability = mnAbilityGet(menu->psynergies[state.selectedIdx] &
										   PSYNERGY_ABILITY_MASK);
					if(ability->fieldPsynergy == 0) {
						sySndPlayStub(SE_MENU_ERROR);
					}
					else {
						sySndPlayStub(SE_NOTE_MID); /* the Use sound */
						shortcutHelp = 1;
						menu->flg |= PSYNERGY_MENU_SHORTCUT_HELP;
						mnWindowClearInteriorTilesStub((void *)window, 0, 88,
													   120, 96);
						mnTextDrawMessageAsTilesStub(MsgShortcutHelp, window, 0,
													 88);
					}
				}
				if((KeysHeld & KEY_SELECT) == 0 && shortcutHelp == 1) {
					shortcutHelp = 0;
					menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
					mnWindowClearInteriorTilesStub((void *)window, 0, 88, 120,
												   96);
					mnTextDrawMessageAsTilesStub(MsgChangeCharacterHelp, window,
												 CHANGE_CHARACTER_HELP_X, 88);
				}
			}

			/* A: set the shortcut, or use the Psynergy if allowed */
			/* and affordable. */
			if((KeysPressed & KEY_A) != 0) {
				if(menu->mode != PSYNERGY_MODE_USE) {
					sySndPlayStub(SE_PSYNERGY_START); /* the shortcut sound */
					result = menu->psynergies[state.selectedIdx];
					done = 1;
					break;
				}
				if(menu->psynergies[state.selectedIdx] != 0) {
					if(mnPsynergyMenuIsActionRestricted(
						   menu->psynergies[state.selectedIdx]) != 0) {
						sySndPlayStub(SE_MENU_ERROR);
						continue;
					}
					ability = mnAbilityGet(menu->psynergies[state.selectedIdx] &
										   PSYNERGY_ABILITY_MASK);
					if(ability->ppCost > memberUnit->pp) {
						sySndPlayStub(SE_MENU_ERROR);
					}
					else {
						sySndPlayStub(SE_NOTE_LOW); /* the choice sound */
						result = menu->psynergies[state.selectedIdx];
						done = 1;
						break;
					}
				}
			}

			if((KeysPressed & KEY_B) != 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = 1;
				break;
			}

			/* L and R: the previous or next party member with Psynergy to list.
			 */
			if(((KeysRepeat & KEY_R) != 0 || (KeysRepeat & KEY_L) != 0) &&
			   (KeysHeld & KEY_SELECT) == 0) {
				listMode = menu->mode != PSYNERGY_MODE_USE
							   ? PSYNERGY_LIST_SHORTCUTS
							   : PSYNERGY_LIST_FIELD;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				menu->selectedIdxByMember[menu->memberIds[sideIndex]] =
					state.selectedIdx;
				member = menu->tabIdx[sideIndex];
				do {
					if((KeysRepeat & KEY_R) != 0) {
						member++;
					}
					else {
						member--;
					}
					member = (member + menu->memberCnt) % menu->memberCnt;
					menu->selectedMember = menu->memberTbl[member];
					menu->memberIds[0] = menu->memberTbl[member];
					menu->psynergyCnt = mnPsynergyMenuCollectActions(
						gmUnitGetStub(menu->memberIds[0]), menu->psynergies,
						listMode);
				} while(menu->psynergyCnt == 0);
				menu->tabIdx[sideIndex] = member;
				for(i = 0; i < 4; i++) {
					menu->rowPositions[i] = MENU_MEMBER_Y;
				}
				menu->rowPositions[member] = MENU_MEMBER_RAISED_Y;
				mnStatPanelDraw(menu->statWin, menu->memberTbl[member], 0,
								STATUS_PANEL_EXPERIENCE);
				mnReloadMemberPortrait(menu, menu->memberTbl[member]);
				break;
			}

			/* Select + L or R: make the field Psynergy that shortcut. */
			if((KeysPressed & KEY_L) != 0 && (KeysHeld & KEY_SELECT) != 0) {
				ability = mnAbilityGet(menu->psynergies[state.selectedIdx] &
									   PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				}
				else {
					sySndPlayStub(SE_PSYNERGY_START); /* the shortcut sound */
					if(mnPsynergyMenuSetShortcut(
						   menu->memberIds[sideIndex],
						   menu->psynergies[state.selectedIdx],
						   PSYNERGY_SHORTCUT_L) != 0) {
						result = menu->psynergies[state.selectedIdx];
						menu->mode = PSYNERGY_MODE_SET_L;
						done = 1;
						break;
					}
				}
			}

			if((KeysPressed & KEY_R) != 0 && (KeysHeld & KEY_SELECT) != 0) {
				ability = mnAbilityGet(menu->psynergies[state.selectedIdx] &
									   PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				}
				else {
					if(mnPsynergyMenuSetShortcut(
						   menu->memberIds[sideIndex],
						   menu->psynergies[state.selectedIdx],
						   PSYNERGY_SHORTCUT_R) != 0) {
						sySndPlayStub(
							SE_PSYNERGY_START); /* the shortcut sound */
						result = menu->psynergies[state.selectedIdx];
						menu->mode = PSYNERGY_MODE_SET_R;
						done = 1;
						break;
					}
				}
			}
		}
	}

	menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
	mnIconShow(menu->entryGridCursor);
	menu->sideRow[sideIndex] = state.selectedIdx;
	menu->selectedIdxByMember[menu->memberIds[sideIndex]] = state.selectedIdx;
	menu->sideAction[sideIndex] = result;
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
		result = -1;
	}
	sySynch(1);
	return result;
}

/* A Psynergy entry's action id, its PSYNERGY_ABILITY_MASK bits, taken out by */
/* shifts. */
#define PSYNERGY_ABILITY_OF(entry) (((u32)(entry) << 18) >> 18)

/* Returns 1 when actionEntry cannot be used from the menu: it has no field */
/* effect and is not usable both in the field and in battle. */
s32 mnPsynergyMenuIsActionRestricted(s32 actionEntry) {
	ABILITY_DEFINITION *action = mnAbilityGet(PSYNERGY_ABILITY_OF(actionEntry));

	if(action->fieldPsynergy != 0 ||
	   (action->flg & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE)) ==
		   (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
		return 0;
	return 1;
}

#undef PSYNERGY_PROMPT_TILES
#undef PSYNERGY_PROMPT_TILES_SIZE
#undef PSYNERGY_DETAIL_ROW_X
#undef PSYNERGY_DETAIL_ROW_WIDTH
#undef PSYNERGY_PAGE_ICONS_X
#undef PSYNERGY_PP_LABEL_X
#undef PSYNERGY_NAME_X
#undef PSYNERGY_LIST_CURSOR_X
#undef PSYNERGY_SHORTCUT_MEMBER_SHIFT
#undef PSYNERGY_SHORTCUT_ID_MASK
#undef ABILITY_DESCRIPTION_MASK
#undef PSYNERGY_ABILITY_OF
#undef PSYNERGY_MENU_BACK_TO_LIST
#undef PSYNERGY_MENU_SHORTCUT_HELP
