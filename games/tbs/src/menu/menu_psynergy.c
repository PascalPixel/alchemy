/* The Psynergy menu: the field menu's Psynergy screen, the party member and
   Psynergy lists, targets, and the L and R shortcuts. Part of menu.c. */

#include "menu_internal.h"

/* A Psynergy shortcut, and the field's Psynergy request packed the same way:
   its party member above bit 10, the Psynergy's id below. */
#define PSYNERGY_SHORTCUT_MEMBER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK	   0x3ff

/* The bits of a listed Psynergy the info window's description is found by:
   nine, where the list's other reads take PSYNERGY_ABILITY_MASK's fourteen. */
#define ABILITY_DESCRIPTION_MASK 0x1ff

/* PSYNERGY_MENU_STATE.flg: set on going back to the list after a use or a
   cancelled target, which nothing reads; and while Select shows the shortcut
   help in the list. */
#define PSYNERGY_MENU_BACK_TO_LIST	1
#define PSYNERGY_MENU_SHORTCUT_HELP 2

/* The German, Spanish, French and Italian Psynergy menus save the window tiles
   they draw over and restore them after. */
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
#define PSYNERGY_PROMPT_TILES	   BG_CHAR_BLOCK(1)
#define PSYNERGY_PROMPT_TILES_SIZE 8192
#endif

/* The highlight over the Psynergy list's rows. */
#define PSYNERGY_DETAIL_ROW_X	  JAPANESE_OR(1, 0)
#define PSYNERGY_DETAIL_ROW_WIDTH JAPANESE_OR(14, 15)

/* The Psynergy list's page icons, "PP" label, names and hand cursor. */
#define PSYNERGY_PAGE_ICONS_X  JAPANESE_OR(120, 112)
#define PSYNERGY_PP_LABEL_X	   JAPANESE_OR(72, 96)
#define PSYNERGY_NAME_X		   JAPANESE_OR(32, 16)
#define PSYNERGY_LIST_CURSOR_X JAPANESE_OR(98, 88)

/* The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over
   the field, which stops meanwhile. */
Sint32 mnPsynergyMenuRun(void)
{
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	void *saved = syHeapAllocEWRAM(PSYNERGY_PROMPT_TILES_SIZE);
#endif
	PSYNERGY_MENU_STATE *menu =
		syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	Sint32 member;
	Sint32 unused;
	Sint32 psynergy;
	Sint32 result;

	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	menu->memberCnt = gmPartyListMembersStub(menu->memberTbl);
	mnItemMenuInit(0, 3, 0, 7);
	menu->msgWin = mnWindowCreateStub(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	mnSuspendFieldEvents();
	syWordsCopy(saved, PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE);
	syWordsFill(
		PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE, PIXELS_4BPP(3));
	mnTextUseExtraTilesStub(1);
#endif
	/* Let the player choose. */
	mnStartWatchingStart();
	result = mnPsynergyMenuRunSteps(&member, &unused, &psynergy);
	mnStopWatchingStart();
	if(result == 1) {
		FIELD_STATE *target = FieldStateWork;
		Uint16		 request;
		/* The action looked up here is not used. */
		gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK & menu->sideAction[0]);
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
#if PSYNERGY_MENU_EXTRA_TEXT_TILES
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy(PSYNERGY_PROMPT_TILES, saved, PSYNERGY_PROMPT_TILES_SIZE);
	WINDOW_STATE->holdTilemapUpload = 0;
	syHeapRelease(saved);
	sySynch(1);
	mnResumeFieldEvents();
#endif
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	WINDOW_STATE->holdTilemapUpload = 0;
	fdPartyClearUnusableShortcutsStub();
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

/* Runs the Psynergy menu: choose a party member and one of their Psynergy, then
   use it on a party member (or the whole party), or, in the shortcut modes, set
   it as the L or R shortcut. */
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
	menu = MenuState;
	useResult = 0;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
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
			gmUnitGetStub(menu->memberIds[0]);
			if(menu->psynergyCnt == 0) {
				step = PSYNERGY_STEP_MEMBER;
			} else {
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
								chosenAction, PSYNERGY_SHORTCUT_L);
							mnWindowSprClearListStub((void *)menu->infoWin);
							mnItemMenuShowMessage(MsgShortcutSetL, -1, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
							mnWindowSprRedrawSavedRectStub(menu->infoWin);
#endif
						} else {
							mnPsynergyMenuSetShortcut(menu->memberIds[0],
								chosenAction, PSYNERGY_SHORTCUT_R);
							mnWindowSprClearListStub((void *)menu->infoWin);
							mnItemMenuShowMessage(MsgShortcutSetR, -1, -1);
#if LANGUAGE == LANGUAGE_JAPANESE
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
			chosenTgt = mnPsynergyMenuSelectTarget(0);
			step = PSYNERGY_STEP_APPLY;
			if(chosenTgt == -1) {
				menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
				step = PSYNERGY_STEP_LIST;
			}
			break;

		/* How is it used? On one party member, on the whole party, or on the
		   field once the menu closes. */
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
		   the target when it failed on one party member. */
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
				mnWindowSprClearListStub((void *)menu->infoWin);
				mnItemMenuShowMessage(
					menu->useResult + MsgItemUseResult, 0, -1);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub((void *)menu->infoWin);
				mnItemMenuShowMessage(
					menu->useResult + MsgItemUseResult, useResult, useResult);
			}
			if(useResult != -1) {
				useResult = 1;
				menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
				step = PSYNERGY_STEP_LIST;
			} else {
				menu->skipSlide = 1;
				if(chosenTgt == MENU_TARGET_WHOLE_PARTY) {
					menu->flg |= PSYNERGY_MENU_BACK_TO_LIST;
					step = PSYNERGY_STEP_LIST;
				} else {
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

/* Says how the selected Psynergy is used from the menu, as an item is (enum
   ITEM_USE_MODE): ITEM_USE_MODE_FROM_FIELD once the menu closes when the scene
   has a script for its field Psynergy (fieldPsynergy) where the leader faces,
   ITEM_USE_MODE_WHOLE_PARTY for ABILITY_RANGE_ALL, ITEM_USE_MODE_CHOOSE_MEMBER
   when it targets ABILITY_TARGET_ALLIES, and ITEM_USE_MODE_FROM_FIELD
   otherwise. */
Sint32 mnPsynergyMenuClassifySelectedPsynergy(void)
{
	ABILITY_DEFINITION *action;
	Sint32				classification;

	action = gmAbilityGetDirectStub(PSYNERGY_ABILITY_MASK &
		((PSYNERGY_MENU_STATE *)MenuState)->sideAction[0]);
	if(fdHasPsynergyCueStub(action->fieldPsynergy) != 0) {
		return ITEM_USE_MODE_FROM_FIELD;
	}
	if(action->range == ABILITY_RANGE_ALL) {
		return ITEM_USE_MODE_WHOLE_PARTY;
	}
	if(action->tgtMode != ABILITY_TARGET_ALLIES) {
		classification = ITEM_USE_MODE_FROM_FIELD;
	} else {
		classification = ITEM_USE_MODE_CHOOSE_MEMBER;
	}
	return classification;
}

/* Lets the player choose a party member in side partySlot (0 or 1), starting
   from the one chosen last, with each party member's Psynergy shown as the
   cursor passes. */
Sint32 mnPsynergyMenuSelectPartySlot(Sint32 partySlot)
{
	PSYNERGY_MENU_STATE *menu = MenuState;
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
		mnCursorSlide(MENU_MEMBER_CURSOR_X(memberIdx), 16);
	}

	unit = gmUnitGetStub(menu->memberTbl[memberIdx]);
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
Sint32 mnPsynergyMenuSelectMember(Uint16 *memberIds, Uint16 *unused)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 selection;
	Sint32				 memberCnt;
	Bool				 redraw;
	Sint32				 result;
	UNIT				*unit;
	WINDOW				*win;
	Bool				 shown;
	Sint32				 i;
	Uint16				*actions;
	Sint32				 shortcutCnt;
	Sint8				 fieldCnt;

	menu = MenuState;
	selection = menu->tabIdx[0];
	memberCnt = menu->tabCnts[0];
	redraw = TRUE;
	result = 0;
	shown = FALSE;
	menu->mode = result;
	unit = gmUnitGetStub(memberIds[selection]);
	if(mnWindowUpdateOrCreate(
		   &menu->psynergyWin, 13, 3, 17, 10, WINDOW_FLAG_INSTANT))
		mnSpawnIconEntries((ITEM_MENU_STATE *)menu, menu->psynergyWin);
	if(mnWindowUpdateOrCreate(
		   &menu->shortcutWin, 13, 13, 17, 4, WINDOW_FLAG_INSTANT)) {
		menu->cursorIcon = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ITEM, 0, menu->shortcutWin, 0, result);
		menu->cursorIcon->motion = ICON_HIDDEN;
	}
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = FALSE;
			selection = (memberCnt + selection) % memberCnt;
			win = menu->statWin;
			unit = gmUnitGetStub(memberIds[selection]);
			mnPsynergyMenuRefreshMemberPsynergy(memberIds[selection]);
			mnStatPanelDraw(
				win, memberIds[selection], 0, STATUS_PANEL_EXPERIENCE);
			mnPsynergyMenuDrawShortcuts(
				menu->shortcutWin, memberIds[selection]);
			mnReloadMemberPortrait(menu, memberIds[selection]);
			for(i = 0; i < 4; i++)
				menu->rowPositions[i] = MENU_MEMBER_Y;
			/* Raise the chosen party member. */
			menu->rowPositions[selection] = MENU_MEMBER_RAISED_Y;
			if(!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown) {
				mnWindowSprClearListStub((void *)menu->infoWin);
#if LANGUAGE != LANGUAGE_JAPANESE
				mnWindowSprRedrawSavedRectStub((void *)menu->infoWin);
#endif
				mnDrawCoins(menu->infoWin);
				shown = TRUE;
			} else {
				gmFlagClearStub(FLAG_MENU_INFO_KEPT);
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		sySynch(1);
		if(PerPush & KEY_A) {
			if(menu->psynergyCnt) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = memberIds[selection];
				break;
			}
			sySndPlayStub(SE_MENU_ERROR);
		}
		/* L or R: set that shortcut, if the party member has field Psynergy for
		   it. */
		if((PerPush & KEY_L) || (PerPush & KEY_R)) {
			result = memberIds[selection];
			if(PerPush & KEY_L)
				menu->mode = PSYNERGY_MODE_SET_L;
			else
				menu->mode = PSYNERGY_MODE_SET_R;
			actions = syHeapAlloc(32 * sizeof(Uint16));
			fieldCnt = mnPsynergyMenuCollectActions(
				(UNIT *)unit, actions, PSYNERGY_LIST_SHORTCUTS);
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
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
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
	menu->tabIdx[0] = selection;
	menu->selectedMember = memberIds[selection];
	menu->memberIds[0] = memberIds[selection];
	return result;
}

/* Shows party member memberId's Psynergy icons in the Psynergy window, eight to
   a row, or "no Psynergy" when they have none. */
void mnPsynergyMenuRefreshMemberPsynergy(Sint32 memberId)
{
	Uint16				*psynergies;
	PSYNERGY_MENU_STATE *menu;
	UNIT				*member;

	menu = MenuState;
	member = gmUnitGetStub(memberId);
	psynergies = menu->psynergies;
	menu->psynergyCnt =
		mnPsynergyMenuCollectActions(member, psynergies, PSYNERGY_LIST_FIELD);
	mnWindowSprRedrawSavedRectStub((void *)menu->psynergyWin);
	mnPsynergyMenuPlaceEntryIcons(108, 32, 8);
	mnPsynergyMenuDrawPsynergyIcons(psynergies);
	if(menu->psynergyCnt == 0) {
		mnTextDrawMessageAsTilesStub(
			MsgPsynergyMenuEmpty, menu->psynergyWin, 0, 24);
	}
}

UNUSED Sint32 mnPsynergyMenuReturnTrue(void)
{
	return 1;
}

UNUSED void mnPsynergyMenuNoOperation(void)
{
}

/* Chooses the party member to use the selected Psynergy on. */
Sint32 mnPsynergyMenuSelectTarget(Sint32 tgtMode)
{

	PSYNERGY_MENU_STATE *menu;
	Sint32				 selection;
	Sint32				 cnt;
	Bool				 redraw;
	Sint32				 result;
	Bool				 shown;
	WINDOW_SPR			*marker;

	menu = MenuState;
	selection = menu->tabIdx[1];
	cnt = menu->memberCnt;
	redraw = TRUE;
	result = 0;
	shown = FALSE;
	gmUnitGetStub(menu->memberTbl[menu->tabIdx[0]]);
	mnCursorSlide(MENU_MEMBER_CURSOR_X(selection), 16);
	while(!gmFlagGetStub(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			redraw = FALSE;
			selection = (selection + cnt) % cnt;
			gmUnitGetStub(menu->memberTbl[selection]);
			/* The marker over the party member, three tiles a party member; no
			   screen makes it, so it is NULL (mnCursorCreate). */
			marker = menu->sideIcon[1];
			marker->spr.oam.x = marker->x =
				(((menu->auxiliaryWin)->x + selection * 3) << 3) - 2;
			if(tgtMode == 0) {
				mnStatPanelDraw(menu->statWin, menu->memberTbl[selection], 0,
					STATUS_PANEL_EXPERIENCE);
				mnReloadMemberPortrait(menu, menu->memberTbl[selection]);
				if(!gmFlagGetStub(FLAG_MENU_INFO_KEPT) && !shown) {
					MENU_RESET_DESCRIPTION_WINDOW((void *)menu->infoWin);
					MENU_DRAW_DESCRIPTION(
						(menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
							MsgAbilityDescription,
						menu->infoWin, 0, 0);
					shown = TRUE;
				} else {
					gmFlagClearStub(FLAG_MENU_INFO_KEPT);
				}
			}
		}
		mnCursorPos(MENU_MEMBER_CURSOR_X(selection), 16);
		sySynch(1);
		if(PerPush & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = menu->memberTbl[selection];
			break;
		}
		if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
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

/* Sets the L or R Psynergy shortcut (PSYNERGY_SHORTCUT) to abilityId as
   memberId uses it, packed as the field's Psynergy request is. Returns 1. */
Sint32 mnPsynergyMenuSetShortcut(
	Sint32 memberId, Sint32 abilityId, Sint32 shortcutIdx)
{
	Sint32 id = abilityId & PSYNERGY_ABILITY_MASK;
	Sint32 code = (memberId << PSYNERGY_SHORTCUT_MEMBER_SHIFT) | id;

	if(shortcutIdx == PSYNERGY_SHORTCUT_L) {
		MainRoutineStateWork.firstShortcut = code;
	} else {
		MainRoutineStateWork.secondShortcut = code;
	}
	return 1;
}

/* Writes the L and R Psynergy shortcuts into window: each one's Psynergy and
   party member, or that it is empty, under a heading that says how to change
   them. A name too wide for the line (measured as a window, over 10 tiles; 11
   in Japanese) hides the party member (international) or drops the colon after
   "L" and "R" (Japanese). Returns 1. */
#if LANGUAGE != LANGUAGE_JAPANESE

Sint32 mnPsynergyMenuDrawShortcuts(WINDOW *win, Sint32 memberId)
{
	Sint32 left;
	Sint32 top;
	Sint32 width;
	Sint32 height;
	Bool   wide;

	if(MainRoutineStateWork.firstShortcut != 0 &&
		MainRoutineStateWork.secondShortcut != 0)
		mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, win, 0, -8);
	else
		mnTextDrawMessageAsTilesStub(MsgShortcutLabel, win, 0, -8);

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 10)
		wide = TRUE;
	else
		wide = FALSE;
	if(MainRoutineStateWork.firstShortcut != 0) {
		mnMessageArgumentPushStub(
			MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK,
			MESSAGE_ARGUMENT_ABILITY);
		mnTextDrawMessageAsTilesStub(MsgShortcutNameL, win, 0, 0);
		if(wide == 0)
			mnTextDrawStringAtOffsetStub(
				gmUnitGetStub(MainRoutineStateWork.firstShortcut >>
					PSYNERGY_SHORTCUT_MEMBER_SHIFT)
					->name,
				win, 80, 0);
	} else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, win, 0, 0);
	}

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 10)
		wide = TRUE;
	else
		wide = FALSE;
	if(MainRoutineStateWork.secondShortcut != 0) {
		mnMessageArgumentPushStub(
			MainRoutineStateWork.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK,
			MESSAGE_ARGUMENT_ABILITY);
		mnTextDrawMessageAsTilesStub(MsgShortcutNameR, win, 0, 8);
		if(wide == 0)
			mnTextDrawStringAtOffsetStub(
				gmUnitGetStub(MainRoutineStateWork.secondShortcut >>
					PSYNERGY_SHORTCUT_MEMBER_SHIFT)
					->name,
				win, 80, 8);
		mnTextSetColStub(TEXT_COLOR_NORMAL);
	} else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, win, 0, 8);
	}
	return 1;
}
#else

Sint32 mnPsynergyMenuDrawShortcuts(WINDOW *win, Sint32 member)
{
	Sint32 left;
	Sint32 top;
	Sint32 width;
	Sint32 height;
	Uint16 wide;

	if(MainRoutineStateWork.firstShortcut != 0 &&
		MainRoutineStateWork.secondShortcut != 0)
		mnTextDrawMessageAsTilesStub(MsgShortcutChangeHelp, win, 0, -8);
	else
		mnTextDrawMessageAsTilesStub(MsgShortcutLabel, win, 0, -8);

	mnTextGetMessageSizeStub(
		(MainRoutineStateWork.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
			MsgAbilityName,
		&left, &top, &width, &height);
	if(width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if(wide)
		mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutLString, win, 0, 0);
	else
		mnTextDrawStringInWindowStub(
			(Uint8 *)MenuShortcutLColonString, win, 0, 0);
	if(MainRoutineStateWork.firstShortcut != 0) {
		if(wide)
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.firstShortcut &
											 PSYNERGY_SHORTCUT_ID_MASK) +
					MsgAbilityName,
				win, 8, 0);
		else
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.firstShortcut &
											 PSYNERGY_SHORTCUT_ID_MASK) +
					MsgAbilityName,
				win, 16, 0);
		mnTextDrawStringAtOffsetStub(
			gmUnitGetStub(MainRoutineStateWork.firstShortcut >>
				PSYNERGY_SHORTCUT_MEMBER_SHIFT)
				->name,
			win, 80, 0);
	} else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyL, win, 24, 0);
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
		mnTextDrawStringInWindowStub((Uint8 *)MenuShortcutRString, win, 0, 8);
	else
		mnTextDrawStringInWindowStub(
			(Uint8 *)MenuShortcutRColonString, win, 0, 8);
	if(MainRoutineStateWork.secondShortcut != 0) {
		if(wide)
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.secondShortcut &
											 PSYNERGY_SHORTCUT_ID_MASK) +
					MsgAbilityName,
				win, 8, 8);
		else
			mnTextDrawMessageAsTilesStub((MainRoutineStateWork.secondShortcut &
											 PSYNERGY_SHORTCUT_ID_MASK) +
					MsgAbilityName,
				win, 16, 8);
		mnTextDrawStringAtOffsetStub(
			gmUnitGetStub(MainRoutineStateWork.secondShortcut >>
				PSYNERGY_SHORTCUT_MEMBER_SHIFT)
				->name,
			win, 80, 8);
		mnTextSetColStub(TEXT_COLOR_NORMAL);
	} else {
		mnTextDrawMessageAsTilesStub(MsgShortcutEmptyR, win, 24, 8);
	}
	return 1;
}
#endif

/* Sets up a Psynergy grid screen: the cursor window with the party's figures, a
   30 by 15 window below with a hidden grid cursor and a portrait, and 16
   Psynergy icon sprites in two rows of eight from (96, 8), on an 8 by 2 grid.
   Nothing calls it. */
void mnPsynergyMenuCreateEntryGrid(void)
{
	WINDOW				*win;
	PSYNERGY_MENU_STATE *menu;
	WINDOW_SPR			*cursor;
	Sint32				 idx;
	Sint32				 x;
	Sint32				 y;
	WINDOW_SPR		   **spr;

	menu = MenuState;
	win = mnCursorCreate(menu);
	mnCreatePartyFigures((void *)win, 2, 2, 8, 0);

	win = mnWindowCreateStub(0, 5, 30, 15, WINDOW_FLAG_INSTANT);
	menu->psynergyWin = win;
	menu->selectedColumn = 0;
	menu->selectedRow = 0;
	menu->columnCnt = ICON_GRID_ROW;
	menu->rowCnt = 2;

	cursor = (WINDOW_SPR *)mnIconCreateHandCursor(win, 0, 4);
	cursor->motion = ICON_HIDDEN;
	menu->entryGridCursor = cursor;
	mnPortraitCreateStub(ISAAC, 0, 0, win, 0, 0);

	y = 8;
	idx = 0;
	spr = &menu->entryIcons[0];
	x = 96;
	do {
		*spr++ = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ABILITY, idx, win, x, y);
		idx++;
		x += 16;
	} while(idx < ICON_GRID_ROW);

	idx = ICON_GRID_ROW;
	y = 24;
	spr = &menu->entryIcons[ICON_GRID_ROW];
	x = 96;
	do {
		*spr++ = mnWindowSprCreateFromResourceStub(
			ICON_KIND_ABILITY, idx, win, x, y);
		idx++;
		x += 16;
	} while(idx < 2 * ICON_GRID_ROW);
}

/* Releases the party figures and closes the Psynergy grid screen's windows.
   Nothing calls it. */
void mnPsynergyMenuCloseWindows(void)
{
	PSYNERGY_MENU_STATE *menu;

	menu = MenuState;
	mnReleasePartyFigures();
	mnWindowCloseStub((void *)menu->auxiliaryWin, 1);
	mnWindowCloseStub((void *)menu->psynergyWin, 1);
	mnWindowCloseStub((void *)menu->msgWin, 1);
}

/* Loads each listed Psynergy's icon into its icon sprite (32 entries) and hides
   the icons of empty entries. */
void mnPsynergyMenuDrawPsynergyIcons(Uint16 *psynergies)
{
	Sint32		 i;
	WINDOW_SPR **icons;
	Uint16		*p;
	Sint32		 psynergyId;

	icons = ((PSYNERGY_MENU_STATE *)MenuState)->entryIcons;
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

/* Lists unit's Psynergy into actions (PSYNERGY_SLOTS entries, cleared first)
   and returns how many there are, as mode (PSYNERGY_LIST_MODE) chooses; any
   mode other than PSYNERGY_LIST_SHORTCUTS and PSYNERGY_LIST_FIELD lists as
   PSYNERGY_LIST_ALL. */
Sint32 mnPsynergyMenuCollectActions(UNIT *unit, Uint16 *actions, Sint32 mode)
{
	Sint32 clearIdx;
	Sint32 passCnt;
	Sint32 actionCnt;
	Sint32 slotIdx;
	Sint32 passIdx;

	passCnt = (mode != PSYNERGY_LIST_FIELD) ? 4 : 3;
	for(clearIdx = 0; clearIdx < PSYNERGY_SLOTS; clearIdx++)
		actions[clearIdx] = 0;
	actionCnt = 0;

	/* fieldPsynergy is the Psynergy's field effect. Passes 1 and 2 add
	   nothing. */
	if(mode == PSYNERGY_LIST_SHORTCUTS) {
		for(slotIdx = 0; slotIdx < PSYNERGY_SLOTS; slotIdx++) {
			if(unit->psynergy[slotIdx].encodedAction != 0) {
				if(gmAbilityGetDirectStub(
					   unit->psynergy[slotIdx].encodedAction &
					   PSYNERGY_ABILITY_MASK)
						->fieldPsynergy != 0) {
					actions[actionCnt] = unit->psynergy[slotIdx].encodedAction;
					actionCnt++;
				}
			}
		}
	} else {
		for(passIdx = 0; passIdx < passCnt; passIdx++) {
			for(slotIdx = 0; slotIdx < PSYNERGY_SLOTS; slotIdx++) {
				if(unit->psynergy[slotIdx].encodedAction != 0) {
					ABILITY_DEFINITION *ability = gmAbilityGetDirectStub(
						unit->psynergy[slotIdx].encodedAction &
						PSYNERGY_ABILITY_MASK);

					if(passIdx == 0 &&
						(ability->fieldPsynergy != 0 ||
							(ability->flg & ABILITY_USABLE_IN_FIELD) != 0)) {
						actions[actionCnt] =
							unit->psynergy[slotIdx].encodedAction;
						actionCnt++;
					} else if(passIdx == 1) {
					} else if(passIdx == 2) {
					} else if(passIdx == 3 && ability->fieldPsynergy == 0 &&
						(ability->flg & ABILITY_USABLE_IN_FIELD) == 0) {
						actions[actionCnt] =
							unit->psynergy[slotIdx].encodedAction;
						actionCnt++;
					}
				}
			}
		}
	}

	return actionCnt;
}

/* Fills result with the Psynergy list of the party member in side index: their
   unit, how many Psynergy are listed, and the page and row of the one chosen
   last for them (kept within the list), five to a page. Returns 1. */
Sint32 mnPsynergyMenuBuildPagedList(PAGED_LIST *result, Sint32 idx)
{
	Sint32				 unitBits;
	PSYNERGY_MENU_STATE *menu = MenuState;
	Sint32				 entryCnt;
	Sint32				 row;
	Sint32				 page;
	Sint32				 pageCnt;
	Sint32				 selectedIdx;
	Sint32				 memberId;

	unitBits = (Sint32)gmUnitGetStub(menu->memberIds[idx]);
	entryCnt = menu->psynergyCnt;
	memberId = menu->memberIds[idx];
	selectedIdx = menu->selectedIdxByMember[memberId];
	if((selectedIdx + 1) > entryCnt) {
		selectedIdx = entryCnt - 1;
	}
	page = selectedIdx / LIST_PAGE_SIZE;
	row = selectedIdx % LIST_PAGE_SIZE;
	pageCnt = entryCnt / LIST_PAGE_SIZE;
	if(entryCnt % LIST_PAGE_SIZE != 0) {
		pageCnt++;
	}
	result->memberState = unitBits;
	result->page = page;
	result->pageCnt = pageCnt;
	result->row = row;
	result->entryCnt = entryCnt;
	result->selectedIdx = selectedIdx;
	return 1;
}

/* Updates the Psynergy list for the row under the cursor: puts the Psynergy's
   description in the info window, unless a kept message is there, and
   highlights its row of the five. window and memberState are not used. */
Sint32 mnPsynergyMenuShowSelectedRow(
	WINDOW *win, Sint32 *memberState, PAGED_LIST *state)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 actionIdx;
	Sint32				 rowIdx;

	menu = MenuState;
	actionIdx = state->page * LIST_PAGE_SIZE;
	actionIdx += state->row;
	state->selectedIdx = actionIdx;

	if(gmFlagGetStub(FLAG_MENU_INFO_KEPT) == 0) {
		MENU_RESET_DESCRIPTION_WINDOW(menu->infoWin);
		sySynch(1);

		actionIdx = state->selectedIdx;
		if(menu->psynergies[actionIdx] != 0) {
			Sint32 description =
				(menu->psynergies[actionIdx] & ABILITY_DESCRIPTION_MASK) +
				MsgAbilityDescription;
			MENU_DRAW_DESCRIPTION(description, menu->infoWin, 0, 0);
		}
	} else {
		/* BUG: clears FLAG_SCENE_TEMP_FF instead of FLAG_MENU_INFO_KEPT, so a
		   kept message keeps the descriptions away until another screen clears
		   it. */
#if FIX_KEPT_MESSAGE_FLAG_CLEAR
		gmFlagClearStub(FLAG_MENU_INFO_KEPT);
#else
		gmFlagClearStub(FLAG_SCENE_TEMP_FF);
#endif
	}

	for(rowIdx = 0; rowIdx < LIST_PAGE_SIZE; rowIdx++) {
		if(rowIdx == state->row) {
			mnWindowSetHighlightRect((void *)menu->psynergyWin,
				PSYNERGY_DETAIL_ROW_X, rowIdx * 2 + 1,
				PSYNERGY_DETAIL_ROW_WIDTH, 1, HIGHLIGHT_PALETTE);
		} else {
			mnWindowSetHighlightRect((void *)menu->psynergyWin,
				PSYNERGY_DETAIL_ROW_X, rowIdx * 2 + 1,
				PSYNERGY_DETAIL_ROW_WIDTH, 1, WINDOW_PALETTE);
		}
	}

	sySynch(1);
	return 1;
}

/* Draws one page of the Psynergy list into window: up to five names with their
   icons and PP costs, Psynergy the party member cannot afford in the no-HP
   colour and one restricted here in the low-HP colour, the page numbers, and
   the help line (how to set a shortcut while Select is held, else how to change
   party member). */
Sint32 mnPsynergyMenuDrawPage(
	WINDOW *win, Sint32 unused, const PAGED_LIST *pageState)
{
	Uint32				 firstEntry;
	Uint32				 visibleCnt;
	Uint8				 row;
	Sint32				 cursor;
	UNIT				*member;
	ABILITY_DEFINITION	*ability;
	PSYNERGY_MENU_STATE *menu = MenuState;

	(void)unused;

	mnWindowSprRedrawSavedRectStub((void *)win);
	mnWindowDrawDividerLineStub(win, 0, 11, 16, 11);

	if(PSYNERGY_MENU_SHORTCUT_HELP & menu->flg) {
		mnTextDrawMessageAsTilesStub(MsgShortcutHelp, win, 0, 88);
	} else {
		mnTextDrawMessageAsTilesStub(
			MsgChangeCharacterHelp, win, CHANGE_CHARACTER_HELP_X, 88);
	}

	firstEntry = pageState->page * LIST_PAGE_SIZE;
	visibleCnt = (Uint8)(pageState->entryCnt - firstEntry);
	if(visibleCnt > LIST_PAGE_SIZE) {
		visibleCnt = LIST_PAGE_SIZE;
	}

	mnSetPageIcons(LIST_PAGE_SIZE, firstEntry, win, PSYNERGY_PAGE_ICONS_X, 34);
	mnPagedListDrawPageNumbers(win, pageState->entryCnt, LIST_PAGE_SIZE,
		pageState->page, PAGE_NUMBERS_RIGHT);
	mnTextDrawMessageAsTilesStub(MsgPsynergyPp, win, PSYNERGY_PP_LABEL_X, 0);

	row = 0;
	if(visibleCnt > row) {
		cursor = firstEntry;
		do {
			member = gmUnitGetStub(menu->memberIds[0]);
			ability = gmAbilityGetDirectStub(
				PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

			if(ability->ppCost > member->pp) {
				mnTextSetColStub(TEXT_COLOR_NO_HP);
			} else if(mnPsynergyMenuIsActionRestricted(PSYNERGY_ABILITY_MASK &
						  menu->psynergies[cursor]) != 0) {
				mnTextSetColStub(TEXT_COLOR_LOW_HP);
			} else {
				mnTextSetColStub(TEXT_COLOR_NORMAL);
			}

			mnTextDrawMessageAsTilesStub(
				(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) +
					MsgAbilityName,
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

/* The Psynergy list of the party member in sideIdx, five a page. */
Sint32 mnPsynergyMenuRunList(Sint32 sideIdx)
{
	PSYNERGY_MENU_STATE *menu;
	ABILITY_DEFINITION	*ability;
	WINDOW_SPR			*icon;
	WINDOW				*win;
	Bool				 changed;
	Sint32				 moved;
	Sint8				 listMode;
	Sint32				 member;
	Uint8				 i;
	Sint32				 result;
	Bool				 redraw;
	Bool				 done;
	UNIT				*memberUnit;
	Sint32				 prev;
	Sint32				 shortcutHelp;
	Sint32				 memberState[5];
	PAGED_LIST			 state;

	menu = MenuState;
	result = 0;
	prev = 0;
	shortcutHelp = 0;
	menu->sideIcon[sideIdx]->motion = ICON_HIDDEN;
	mnWindowUpdateOrCreate(&menu->listWin, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
	win = menu->listWin;
	done = FALSE;

	while(done == 0 && gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		memberUnit = gmUnitGetStub(menu->memberIds[sideIdx]);
		if(menu->mode != PSYNERGY_MODE_USE) {
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				memberUnit, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
		} else {
			menu->psynergyCnt = mnPsynergyMenuCollectActions(
				memberUnit, menu->psynergies, PSYNERGY_LIST_FIELD);
		}
		mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
		mnPsynergyMenuBuildPagedList(&state, sideIdx);
		redraw = TRUE;
		changed = TRUE;
		menu->sideIcon[sideIdx]->motion = ICON_SHOWN;

		while(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
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
				mnPsynergyMenuShowSelectedRow(win, memberState, &state);
				menu->sideAction[sideIdx] = menu->psynergies[state.selectedIdx];
				menu->cursorIcon->motion = ICON_HIDDEN;
				if(menu->psynergies[state.selectedIdx] != 0) {
					icon = menu->entryIcons[state.selectedIdx];
					/* Swell the selected icon, in depth slot 250, as in
					   mnItemMenuRunList. */
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 0;
					icon->depth = OAM_DEPTH_FRONT - 5;
				}
				for(i = 0; i < menu->memberCnt; i++) {
					fdSprStartAnimationStub(menu->tabSprs[i], ANIM_STAND);
				}
			}

			sySynch(1);
			prev = state.selectedIdx;

			if((PerData & KEY_SELECT) == 0) {
				moved = mnPagedListHandleInput(
					0, state.entryCnt, LIST_PAGE_SIZE, &state.row, &state.page);
			} else {
				moved = -1;
			}
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

			/* Select shows the shortcut help while held, for field Psynergy. */
			if(menu->mode == PSYNERGY_MODE_USE) {
				if((PerPush & KEY_SELECT) != 0 && shortcutHelp == 0) {
					ability = gmAbilityGetDirectStub(
						menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
					if(ability->fieldPsynergy == 0) {
						sySndPlayStub(SE_MENU_ERROR);
					} else {
						sySndPlayStub(SE_NOTE_MID); /* the Use sound */
						shortcutHelp = 1;
						menu->flg |= PSYNERGY_MENU_SHORTCUT_HELP;
						mnWindowClearInteriorTilesStub(
							(void *)win, 0, 88, 120, 96);
						mnTextDrawMessageAsTilesStub(
							MsgShortcutHelp, win, 0, 88);
					}
				}
				if((PerData & KEY_SELECT) == 0 && shortcutHelp == 1) {
					shortcutHelp = 0;
					menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
					mnWindowClearInteriorTilesStub((void *)win, 0, 88, 120, 96);
					mnTextDrawMessageAsTilesStub(MsgChangeCharacterHelp, win,
						CHANGE_CHARACTER_HELP_X, 88);
				}
			}

			/* A: set the shortcut, or use the Psynergy if allowed and
			   affordable. */
			if((PerPush & KEY_A) != 0) {
				if(menu->mode != PSYNERGY_MODE_USE) {
					sySndPlayStub(SE_PSYNERGY_START); /* the shortcut sound */
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
					if(ability->ppCost > memberUnit->pp) {
						sySndPlayStub(SE_MENU_ERROR);
					} else {
						sySndPlayStub(SE_NOTE_LOW); /* the choice sound */
						result = menu->psynergies[state.selectedIdx];
						done = TRUE;
						break;
					}
				}
			}

			if((PerPush & KEY_B) != 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				done = TRUE;
				break;
			}

			/* L and R: the previous or next party member with Psynergy to
			   list. */
			if(((PerRepeat & KEY_R) != 0 || (PerRepeat & KEY_L) != 0) &&
				(PerData & KEY_SELECT) == 0) {
				listMode = menu->mode != PSYNERGY_MODE_USE
					? PSYNERGY_LIST_SHORTCUTS
					: PSYNERGY_LIST_FIELD;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				menu->selectedIdxByMember[menu->memberIds[sideIdx]] =
					state.selectedIdx;
				member = menu->tabIdx[sideIdx];
				do {
					if((PerRepeat & KEY_R) != 0) {
						member++;
					} else {
						member--;
					}
					member = (member + menu->memberCnt) % menu->memberCnt;
					menu->selectedMember = menu->memberTbl[member];
					menu->memberIds[0] = menu->memberTbl[member];
					menu->psynergyCnt = mnPsynergyMenuCollectActions(
						gmUnitGetStub(menu->memberIds[0]), menu->psynergies,
						listMode);
				} while(menu->psynergyCnt == 0);
				menu->tabIdx[sideIdx] = member;
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
			if((PerPush & KEY_L) != 0 && (PerData & KEY_SELECT) != 0) {
				ability =
					gmAbilityGetDirectStub(menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				} else {
					sySndPlayStub(SE_PSYNERGY_START); /* the shortcut sound */
					if(mnPsynergyMenuSetShortcut(menu->memberIds[sideIdx],
						   menu->psynergies[state.selectedIdx],
						   PSYNERGY_SHORTCUT_L) != 0) {
						result = menu->psynergies[state.selectedIdx];
						menu->mode = PSYNERGY_MODE_SET_L;
						done = TRUE;
						break;
					}
				}
			}

			if((PerPush & KEY_R) != 0 && (PerData & KEY_SELECT) != 0) {
				ability =
					gmAbilityGetDirectStub(menu->psynergies[state.selectedIdx] &
						PSYNERGY_ABILITY_MASK);
				if(ability->fieldPsynergy == 0) {
					sySndPlayStub(SE_MENU_ERROR);
				} else {
					if(mnPsynergyMenuSetShortcut(menu->memberIds[sideIdx],
						   menu->psynergies[state.selectedIdx],
						   PSYNERGY_SHORTCUT_R) != 0) {
						/* the shortcut sound */
						sySndPlayStub(SE_PSYNERGY_START);
						result = menu->psynergies[state.selectedIdx];
						menu->mode = PSYNERGY_MODE_SET_R;
						done = TRUE;
						break;
					}
				}
			}
		}
	}

	menu->flg &= ~PSYNERGY_MENU_SHORTCUT_HELP;
	mnIconShow(menu->entryGridCursor);
	menu->sideRow[sideIdx] = state.selectedIdx;
	menu->selectedIdxByMember[menu->memberIds[sideIdx]] = state.selectedIdx;
	menu->sideAction[sideIdx] = result;
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) != 0) {
		result = -1;
	}
	sySynch(1);
	return result;
}

/* A Psynergy entry's action id, its PSYNERGY_ABILITY_MASK bits, taken out by
   shifts. */
#define PSYNERGY_ABILITY_OF(entry) (((Uint32)(entry) << 18) >> 18)

#include "menu/psynergy_menu_is_action_restricted.c"

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
