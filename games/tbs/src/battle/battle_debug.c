/* Debug: the parameter sets, the message viewer and the battle test menu. Part
   of battle.c. */

#include "battle_internal.h"

/* The first message of the viewer's second range. */
#define DEBUG_SECOND_RANGE_FIRST \
	BY_LANGUAGE(MsgDebugSecondForm, MsgBecameCursed, MsgEnd, \
		MsgDebugSecondForm, MsgEnd, MsgDebugSecondForm)

/* How far R and L, or Up and Down in the battle test's chooser, step through a
   list at once. */
#define DEBUG_LARGE_STEP	10
/* The number a browsed message's number placeholder shows. */
#define DEBUG_SAMPLE_NUMBER 999

/* The last row of DebugParameterSets; the scan reports a missing parameter set
   past it. */
#define DEBUG_PARAMETER_SET_LAST_ROW 332

/* Debug: replaces the party with parameter set number set, each member at its
   level with its Djinn, abilities and equipped items. Returns 1 when the table
   ends before that set. */
Sint32 btDebugParmSetLoad(Sint32 set)
{
	/* The next Djinni number of each element to hand out. */
	Sint32 nextDjinn[ELEMENT_COUNT];
	UNIT  *member;
	Sint32 unitId;
	Bool   notFound;
	Sint32 row;
	Sint32 djinnIdx;
	Sint32 curSet;
	Sint32 i;

	notFound = FALSE;
	/* Empty the active party; the parameter set's rows add its party members
	   back. */
	gmPartyRemoveMemberStub(ISAAC);
	gmPartyRemoveMemberStub(GARET);
	gmPartyRemoveMemberStub(IVAN);
	gmPartyRemoveMemberStub(MIA);
	gmPartyRemoveMemberStub(JENNA);

	curSet = 0;
	for(i = 0; i < ELEMENT_COUNT; i++)
		nextDjinn[i] = 0;

	/* Parameter sets are runs of rows each ended by a DEBUG_PARAMETER_SET_END
	   row; skip to the run numbered set and apply its rows. */
	for(row = 0;; row++) {
		unitId = DebugParameterSets[row].unitId;
		if(row < 0 || row > DEBUG_PARAMETER_SET_LAST_ROW) {
			notFound = TRUE;
			break;
		}
		if(unitId == DEBUG_PARAMETER_SET_END) {
			if(curSet == set)
				break;
			curSet++;
			continue;
		}
		if(curSet != set)
			continue;

		gmPartyAddMemberStub(unitId);
		gmUnitLevelUpToStub(unitId, DebugParameterSets[row].level);

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
			if(DebugParameterSets[row].abilities[i] != 0)
				gmUnitLearnPsynergyStub(
					unitId, DebugParameterSets[row].abilities[i]);
		}

		/* Djinn are numbered across the party, so no two party members get the
		   same. */
		for(i = 0; i < ELEMENT_COUNT; i++) {
			for(djinnIdx = 0; djinnIdx < DebugParameterSets[row].djinnCnts[i];
				djinnIdx++) {
				gmDjinnGiveStub(unitId, i, nextDjinn[i]);
				gmDjinnSetStub(unitId, i, nextDjinn[i]);
				nextDjinn[i]++;
			}
		}

		/* Empty the inventory, removing slot 0 one time more than there are
		   slots, then add and equip the row's items. */
		for(i = 0; i < INVENTORY_SLOTS + 1; i++)
			gmInventoryRemoveStub(unitId, 0);
		for(i = 0; i < 4; i++) {
			if(DebugParameterSets[row].items[i] != 0)
				gmInventoryEquipStub(unitId,
					gmInventoryAddItemStub(
						unitId, DebugParameterSets[row].items[i]));
		}
	}
	return notFound;
}

/* The highest message index the viewer reaches. */
#define DEBUG_VIEW_LIMIT \
	(MsgLaliveroHowDidSearchForSheba - \
		MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

/* Debug: a message viewer that never returns, showing one message at a time
   with sample arguments. Left and Right step it, L and R by ten, Up, Down and B
   pick the range, and A shows it again. */
void btDebugBattleViewMessages(void)
{
	Uint16	text[64];
	Sint32	secondRange = 0;
	UNIT   *unit = gmUnitGetStub(ISAAC);
	Sint32	i;
	Sint32	idx;
	WINDOW *msgWin;
	Sint32	msg;

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
		idx = 0;
		IO_WRITE16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
				DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
			&REG_DISPCNT);
		/* The fastest text speed (window/message_speed.c: 0 to 2, slowest
		   first). */
		MainRoutineStateWork.msgSpeed = 2;
		for(;;) {
			/* Sample arguments for the message's placeholders. */
			mnMessageArgumentClearAllStub();
			mnMessageArgumentPushStub(
				DEBUG_SAMPLE_NUMBER, MESSAGE_ARGUMENT_NUMBER);
			mnMessageArgumentPushStub(0, MESSAGE_ARGUMENT_CLASS);
			mnMessageArgumentPushStub(1, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(1, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPushStub(2, MESSAGE_ARGUMENT_ABILITY);
			/* The window at tile (2, 10) for the first range, (2, 2) for the
			   second. */
			if(secondRange == 0) {
				msg = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
				msgWin = mnMessageOpenWindowStub(idx + msg, 2, 10, 4);
			} else {
				msg = DEBUG_SECOND_RANGE_FIRST;
				msgWin = mnMessageOpenWindowStub(idx + msg, 2, 2, 4);
			}
			sySynch(10);

			/* Wait for a key that picks another message, or for A once the
			   message has been shown in full. */
			for(;;) {
				if(PerRepeat & KEY_B) {
					if(secondRange != 0) {
						secondRange = 0;
					} else {
						idx++;
						secondRange = 1;
					}
				}
				if(PerRepeat & KEY_RIGHT)
					idx++;
				if(PerRepeat & KEY_LEFT)
					idx -= 2;
				if(PerRepeat & KEY_UP)
					secondRange = 1;
				if(PerRepeat & KEY_DOWN)
					secondRange = 0;
				if(PerRepeat & KEY_R)
					idx += DEBUG_LARGE_STEP;
				if(PerRepeat & KEY_L)
					idx -= DEBUG_LARGE_STEP;
				if(idx < 0)
					idx = 0;
				if(idx >= (Uint32)DEBUG_VIEW_LIMIT)
					idx = DEBUG_VIEW_LIMIT;
				if(PerRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
					break;
				if(mnMessageAllTypedStub() != 0 && (PerRepeat & KEY_A))
					break;
				sySynch(1);
			}
			mnWindowCloseAllStub(1);
			/* Close the window and clear its page arrow. */
			mnWindowCloseStub(msgWin, 1);
			((WIN_STATE *)WindowStateWork[0])->pageArrowShown = 0;
		}
	}
}

/* The encounter the debug battle starts with (two or three Kobolds), and the
   Djinn tutorial's: rows of formations.c. */
#define DEBUG_FIRST_ENCOUNTER	 257
#define DJINN_TUTORIAL_ENCOUNTER 28

/* Debug: the battle test menu, which never returns. It fights the first
   encounter again and again with auto battle on, unless Down is held at the
   start to open the chooser of encounter and parameter set. */
void btDebugBattleRunMenu(void)
{
	Sint32 startModeSet = 0;
	Sint32 encounter;
	Sint32 set;
	Sint32 loadedSet;

	gmMainRoutineStateInitDefaultsStub();
restart:
	syWindowLoadGraphics();
	syPrintClear();
	syInitEvent();
	syHeapInit();
	syVRAMCacheReset();
	gmFlagSetStub(FLAG_AUTO_BATTLE);
	encounter = DEBUG_FIRST_ENCOUNTER;
	if(PerData & KEY_DOWN) {
		loadedSet = -1;
		gmFlagClearStub(FLAG_AUTO_BATTLE);
		set = 0;
		for(;;) {
			gmFlagClearStub(FLAG_CHILDHOOD);
			sySynch(1);
			for(;;) {
				if(PerRepeat & KEY_RIGHT)
					encounter++;
				if(PerRepeat & KEY_LEFT)
					encounter--;
				if(PerRepeat & KEY_UP)
					encounter -= DEBUG_LARGE_STEP;
				if(PerRepeat & KEY_DOWN)
					encounter += DEBUG_LARGE_STEP;
				if(PerRepeat & KEY_R)
					set++;
				if(PerRepeat & KEY_L)
					set--;
				if(PerRepeat & KEY_A)
					break;
				if(PerRepeat & KEY_START)
					btDebugBattleViewMessages();
				if(PerRepeat & KEY_SELECT)
					btDebugSelectNoOperation();
				if((PerRepeat & KEY_B) || startModeSet) {
					startModeSet = 1;
					SET_BATTLE_TYPE(BATTLE_TYPE_DEBUG);
				}
				if(set != loadedSet) {
					gmMainRoutineStateInitDefaultsStub();
					btDebugParmSetLoad(set);
					loadedSet = set;
				}
				sySynch(1);
			}
			/* A fights; Down still held makes it a link battle. */
			if(PerData & KEY_DOWN)
				gmFlagSetStub(FLAG_LINK_BATTLE);
			gmUnitCalcStub(ISAAC);
			MainRoutineStateWork.battleBg = RESOURCE_SUHALLA_GATE_BACKDROP;
			if(encounter == DJINN_TUTORIAL_ENCOUNTER)
				gmFlagSetStub(FLAG_DJINN_LESSON);
			gmFlagSetStub(FLAG_DEBUG_BATTLE);
			btRun(encounter);
			/* Rebuild what the battle replaced, then return to the chooser. */
			syWindowLoadGraphics();
			syPrintClear();
			syInitEvent();
			syHeapInit();
			syVRAMCacheReset();
		}
	}
	/* Without Down: the same auto battle, over and over. */
	gmFlagSetStub(FLAG_DEBUG_BATTLE);
	btRun(DEBUG_FIRST_ENCOUNTER);
	goto restart;
}

#undef DEBUG_LARGE_STEP
#undef DEBUG_SAMPLE_NUMBER
