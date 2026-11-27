/* @file */
/* Debug: the parameter sets, the message viewer and the battle test menu. */
/* Part of battle.c. */

#include "battle_internal.h"

/* The first message of the viewer's second range. */
#if LANGUAGE == LANGUAGE_ENGLISH
#define DEBUG_SECOND_RANGE_FIRST MsgBecameCursed
#elif LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
#define DEBUG_SECOND_RANGE_FIRST MsgEnd
#else
#define DEBUG_SECOND_RANGE_FIRST MsgDebugSecondForm
#endif

/* How far R and L, or Up and Down in the battle test's chooser, step through a
 */
/* list at once. */
#define DEBUG_LARGE_STEP 10
/* The number a browsed message's number placeholder shows. */
#define DEBUG_SAMPLE_NUMBER 999

/* The last row of Debug_ParameterSets; the scan reports a missing parameter */
/* set past it. */
#define DEBUG_PARAMETER_SET_LAST_ROW 332

/* Debug: replaces the party with parameter set number set, each member at its
 */
/* level with its Djinn, abilities and equipped items. Returns 1 when the table
 */
/* ends before that set. */
s32 btDebugParmSetLoad(s32 set) {
	/* The next Djinni number of each element to hand out. */
	s32 nextDjinn[ELEMENT_COUNT];
	UNIT *member;
	s32 unitId;
	s32 notFound;
	s32 row;
	s32 djinnIndex;
	s32 currentSet;
	s32 i;

	notFound = 0;
	/* Empty the active party; the parameter set's rows add its party */
	/* members back. */
	gmPartyRemoveMemberStub(ISAAC);
	gmPartyRemoveMemberStub(GARET);
	gmPartyRemoveMemberStub(IVAN);
	gmPartyRemoveMemberStub(MIA);
	gmPartyRemoveMemberStub(JENNA);

	currentSet = 0;
	for(i = 0; i < ELEMENT_COUNT; i++)
		nextDjinn[i] = 0;

	/* Parameter sets are runs of rows each ended by a DEBUG_PARAMETER_SET_END
	 */
	/* row; skip to the run numbered set and apply its rows. */
	for(row = 0;; row++) {
		unitId = Debug_ParameterSets[row].unitId;
		if(row < 0 || row > DEBUG_PARAMETER_SET_LAST_ROW) {
			notFound = 1;
			break;
		}
		if(unitId == DEBUG_PARAMETER_SET_END) {
			if(currentSet == set)
				break;
			currentSet++;
			continue;
		}
		if(currentSet != set)
			continue;

		gmPartyAddMemberStub(unitId);
		gmUnitLevelUpToStub(unitId, Debug_ParameterSets[row].level);

		/* Start from no Djinn and no abilities. */
		member = gmUnitGetStub(unitId);
		for(i = 0; i < ELEMENT_COUNT; i++) {
			member->djinnHeldCnts[i] = 0;
			member->djinnSetCnts[i] = 0;
			member->djinnHeld[i] = 0;
			member->djinnSet[i] = 0;
		}
		for(i = 0; i < PSYNERGY_SLOTS; i++)
			*&member->psynergy[i].encodedAction = 0;
		for(i = 0; i < 2; i++) {
			if(Debug_ParameterSets[row].abilities[i] != 0)
				gmUnitLearnPsynergyStub(unitId,
										Debug_ParameterSets[row].abilities[i]);
		}

		/* Djinn are numbered across the party, so no two party members get the
		 */
		/* same. */
		for(i = 0; i < ELEMENT_COUNT; i++) {
			for(djinnIndex = 0;
				djinnIndex < Debug_ParameterSets[row].djinnCnts[i];
				djinnIndex++) {
				gmDjinnGiveStub(unitId, i, nextDjinn[i]);
				gmDjinnSetStub(unitId, i, nextDjinn[i]);
				nextDjinn[i]++;
			}
		}

		/* Empty the inventory, removing slot 0 one time more than there are */
		/* slots, then add and equip the row's items. */
		for(i = 0; i < INVENTORY_SLOTS + 1; i++)
			gmInventoryRemoveStub(unitId, 0);
		for(i = 0; i < 4; i++) {
			if(Debug_ParameterSets[row].items[i] != 0)
				gmInventoryEquipStub(
					unitId, gmInventoryAddItemStub(
								unitId, Debug_ParameterSets[row].items[i]));
		}
	}
	return notFound;
}

/* The highest message index the viewer reaches. */
#define DEBUG_VIEW_LIMIT                                                       \
	(MsgLaliveroHowDidSearchForSheba -                                         \
	 MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

/* Debug: a message viewer that never returns, showing one message at a time */
/* with sample arguments. Left and Right step it, L and R by ten, Up, Down and
 */
/* B pick the range, and A shows it again. */
void btDebugBattleViewMessages(void) {
	u16 text[64];
	s32 secondRange = 0;
	UNIT *unit = gmUnitGetStub(ISAAC);
	s32 i;
	s32 index;
	WINDOW *messageWindow;
	s32 message;

	/* Party member 0's name is the sample a `<unit>` blank shows. */
	mnMessageDecodeRawUnboundedStub(MsgDebugSampleName, text);
	for(i = 0; i < UNIT_NAME_LENGTH; i++) {
		unit->name[i] = text[i];
		if(text[i] == 0)
			break;
	}
	unit->name[UNIT_NAME_LENGTH] = 0;
	mnWindowSystemInitFieldStub();
	sySndPlayStub(MUS_EMPTY);
	for(;;) {
		index = 0;
		scIoWrite16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
						DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
					&REG_DISPCNT);
		/* The fastest text speed (window/message_speed.c: 0 to 2, slowest
		 * first). */
		MainRoutineStateWork.msgSpeed = 2;
		for(;;) {
			/* Sample arguments for the message's placeholders. */
			mnMessageArgumentClearAllStub();
			mnMessageArgumentPushStub(DEBUG_SAMPLE_NUMBER,
									  MESSAGE_ARGUMENT_NUMBER);
			mnMessageArgumentPushStub(0, MESSAGE_ARGUMENT_CLASS);
			mnMessageArgumentPushStub(1, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(1, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPushStub(2, MESSAGE_ARGUMENT_ABILITY);
			/* The window at tile (2, 10) for the first range, (2, 2) for the */
			/* second. */
			if(secondRange == 0) {
				message = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
				messageWindow =
					mnMessageOpenWindowStub(index + message, 2, 10, 4);
			}
			else {
				message = DEBUG_SECOND_RANGE_FIRST;
				messageWindow =
					mnMessageOpenWindowStub(index + message, 2, 2, 4);
			}
			sySynch(10);

			/* Wait for a key that picks another message, or for A once the */
			/* message has been shown in full. */
			for(;;) {
				if(KeysRepeat & KEY_B) {
					if(secondRange != 0) {
						secondRange = 0;
					}
					else {
						index++;
						secondRange = 1;
					}
				}
				if(KeysRepeat & KEY_RIGHT)
					index++;
				if(KeysRepeat & KEY_LEFT)
					index -= 2;
				if(KeysRepeat & KEY_UP)
					secondRange = 1;
				if(KeysRepeat & KEY_DOWN)
					secondRange = 0;
				if(KeysRepeat & KEY_R)
					index += DEBUG_LARGE_STEP;
				if(KeysRepeat & KEY_L)
					index -= DEBUG_LARGE_STEP;
				if(index < 0)
					index = 0;
				if(index >= (u32)DEBUG_VIEW_LIMIT)
					index = DEBUG_VIEW_LIMIT;
				if(KeysRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
					break;
				if(mnMessageAllTypedStub() != 0 && (KeysRepeat & KEY_A))
					break;
				sySynch(1);
			}
			mnWindowCloseAllStub(1);
			/* Close the window and clear its page arrow. */
			mnWindowCloseStub(messageWindow, 1);
			((WIN_STATE *)WindowStateWork[0])->pageArrowShown = 0;
		}
	}
}

/* The encounter the debug battle starts with (two or three Kobolds), and */
/* the Djinn tutorial's: rows of formations.c. */
#define DEBUG_FIRST_ENCOUNTER	 257
#define DJINN_TUTORIAL_ENCOUNTER 28

/* Debug: the battle test menu, which never returns. It fights the first */
/* encounter again and again with auto battle on, unless Down is held at the */
/* start to open the chooser of encounter and parameter set. */
void btDebugBattleRunMenu(void) {
	s32 startModeSet = 0;
	s32 encounter;
	s32 set;
	s32 loadedSet;

	gmMainRoutineStateInitDefaultsStub();
restart:
	syWindowLoadGraphics();
	syDebugTextClear();
	syInitEvent();
	syHeapInit();
	syVramCacheReset();
	gmFlagSetStub(FLAG_AUTO_BATTLE);
	encounter = DEBUG_FIRST_ENCOUNTER;
	if(KeysHeld & KEY_DOWN) {
		loadedSet = -1;
		gmFlagClearStub(FLAG_AUTO_BATTLE);
		set = 0;
		for(;;) {
			gmFlagClearStub(FLAG_CHILDHOOD);
			sySynch(1);
			for(;;) {
				if(KeysRepeat & KEY_RIGHT)
					encounter++;
				if(KeysRepeat & KEY_LEFT)
					encounter--;
				if(KeysRepeat & KEY_UP)
					encounter -= DEBUG_LARGE_STEP;
				if(KeysRepeat & KEY_DOWN)
					encounter += DEBUG_LARGE_STEP;
				if(KeysRepeat & KEY_R)
					set++;
				if(KeysRepeat & KEY_L)
					set--;
				if(KeysRepeat & KEY_A)
					break;
				if(KeysRepeat & KEY_START)
					btDebugBattleViewMessages();
				if(KeysRepeat & KEY_SELECT)
					btDebugSelectNoOp();
				if((KeysRepeat & KEY_B) || startModeSet) {
					startModeSet = 1;
					btSetType(BATTLE_TYPE_DEBUG);
				}
				if(set != loadedSet) {
					gmMainRoutineStateInitDefaultsStub();
					btDebugParmSetLoad(set);
					loadedSet = set;
				}
				sySynch(1);
			}
			/* A fights; Down still held makes it a link battle. */
			if(KeysHeld & KEY_DOWN)
				gmFlagSetStub(FLAG_LINK_BATTLE);
			gmUnitCalcStub(ISAAC);
			MainRoutineStateWork.battleBg = RESOURCE_SUHALLA_GATE_BACKDROP;
			if(encounter == DJINN_TUTORIAL_ENCOUNTER)
				gmFlagSetStub(FLAG_DJINN_LESSON);
			gmFlagSetStub(FLAG_DEBUG_BATTLE);
			btRun(encounter);
			/* Rebuild what the battle replaced, then return to the chooser. */
			syWindowLoadGraphics();
			syDebugTextClear();
			syInitEvent();
			syHeapInit();
			syVramCacheReset();
		}
	}
	/* Without Down: the same auto battle, over and over. */
	gmFlagSetStub(FLAG_DEBUG_BATTLE);
	btRun(DEBUG_FIRST_ENCOUNTER);
	goto restart;
}

#undef DEBUG_LARGE_STEP
#undef DEBUG_SAMPLE_NUMBER
