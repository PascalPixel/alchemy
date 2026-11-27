/* Debug: the unit snapshot, the parameter sets and the battle test menu. Part
   of battle.c. */

#include "game/stubs.h"
#include "battle/presentation.h"

/* Unused: runs random encounters one after another, forever. */
UNUSED void btDebugBattleRunForever(void)
{
	for(;;)
		btRun(0);
}

/* After the first two parts of Karst and Agatio's battle
   (FORMATION_KARST_AND_AGATIO_A and _B), snapshots the five units of
   ScriptedBattleUnits that fought (Felix, Piers and Jenna, and the first two
   enemies) for btScriptedBattleRestoreUnits to bring back as the next part
   begins. */
void btScriptedBattleSaveUnits(void)
{
	Sint32		   j;
	SNAPSHOT	  *snapshot = RamScriptedBattleSnapshot;
	Sint32		   i = 0;
	Uint8		  *ids = ScriptedBattleUnits;
	SNAPSHOT_COPY *copies = RamScriptedBattleSnapshot->statuses;

	for(; i <= 4; i++, copies++) {
		UNIT *unit = syUnitGet(ids[i]);

		snapshot->listed[i] = 0;
		if(btUnitIsListed(ids[i])) {
			snapshot->listed[i] = 1;
			if(i > 2) {
				snapshot->vitals[i - 3].hp = unit->hp > 0 ? unit->hp : 1;
				snapshot->vitals[i - 3].pp = unit->pp;
			}
			if(i <= 2) {
				for(j = 0; j < ELEMENT_COUNT; j++)
					snapshot->summonBoosts[i][j] = unit->summonBoost[j];
			}
			copies->curse = unit->curse;
			copies->poison = unit->poison;
			copies->attackTurns = unit->attackTurns;
			copies->attackStage = unit->attackStage;
			copies->defenseTurns = unit->defenseTurns;
			copies->defenseStage = unit->defenseStage;
			copies->resistanceTurns = unit->resistanceTurns;
			copies->resistanceStage = unit->resistanceStage;
			copies->delusion = unit->delusion;
			copies->confusion = unit->confusion;
			copies->charm = unit->charm;
			copies->stun = unit->stun;
			copies->sleep = unit->sleep;
			copies->psySeal = unit->psySeal;
			copies->regenerate = unit->regenerate;
			copies->reflect = unit->reflect;
			copies->haunt = unit->haunt;
			copies->grimReaper = unit->grimReaper;
			copies->equipBonus[0] = unit->equipBonus[0];
			copies->equipBonus[1] = unit->equipBonus[1];
			copies->ready = unit->ready;
			copies->immobile = unit->immobile;
			copies->agilityTurns = unit->agilityTurns;
			copies->agilityStage = unit->agilityStage;
			copies->challenge = unit->challenge;
		}
	}
}

/* Restores the five units btScriptedBattleSaveUnits snapshotted, each one still
   in the battle and listed then: the first three's summon boosts, each one's
   statuses, and the last two's HP and PP. */
void btScriptedBattleRestoreUnits(void)
{
	Sint32		   i;
	Sint32		   j;
	SNAPSHOT	  *snapshot = RamScriptedBattleSnapshot;
	SNAPSHOT_COPY *copies = RamScriptedBattleSnapshot->statuses;

	for(i = 0; i <= 4; i++, copies++) {
		UNIT *unit = syUnitGet(ScriptedBattleUnits[i]);

		if(btUnitIsListed(ScriptedBattleUnits[i]) && snapshot->listed[i] != 0) {
			if(i <= 2) {
				for(j = 0; j < ELEMENT_COUNT; j++)
					unit->summonBoost[j] = snapshot->summonBoosts[i][j];
			}
			unit->curse = copies->curse;
			unit->poison = copies->poison;
			unit->attackTurns = copies->attackTurns;
			unit->attackStage = copies->attackStage;
			unit->defenseTurns = copies->defenseTurns;
			unit->defenseStage = copies->defenseStage;
			unit->resistanceTurns = copies->resistanceTurns;
			unit->resistanceStage = copies->resistanceStage;
			unit->delusion = copies->delusion;
			unit->confusion = copies->confusion;
			unit->charm = copies->charm;
			unit->stun = copies->stun;
			unit->sleep = copies->sleep;
			unit->psySeal = copies->psySeal;
			unit->regenerate = copies->regenerate;
			unit->reflect = copies->reflect;
			unit->haunt = copies->haunt;
			unit->grimReaper = copies->grimReaper;
			unit->equipBonus[0] = copies->equipBonus[0];
			unit->equipBonus[1] = copies->equipBonus[1];
			unit->ready = copies->ready;
			unit->immobile = copies->immobile;
			unit->agilityTurns = copies->agilityTurns;
			unit->agilityStage = copies->agilityStage;
			unit->challenge = copies->challenge;
			if(i > 2) {
				gmUnitSetHpStub(unit, snapshot->vitals[i - 3].hp);
				gmUnitSetPpStub(unit, snapshot->vitals[i - 3].pp);
			}
		}
	}
}

/* Works out a character's class, Psynergy and base elements again. */
void gmUnitUpdateClassStub(Sint32 owner);

/* From this parameter set on, an even set is loaded together with the odd one
   after it. */
#define DEBUG_PAIRED_SETS_FIRST 70

/* The last row of DebugParameterSets; the scan reports a missing parameter set
   past it. */
#define DEBUG_PARAMETER_SET_LAST_ROW 440

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
	/* Empty the party and the Djinn on standby; the parameter set's rows add
	   its party members back. */
	gmPartyRemoveMemberStub(ISAAC);
	gmPartyRemoveMemberStub(GARET);
	gmPartyRemoveMemberStub(IVAN);
	gmPartyRemoveMemberStub(MIA);
	gmPartyRemoveMemberStub(JENNA);
	gmPartyRemoveMemberStub(FELIX);
	gmPartyRemoveMemberStub(SHEBA);
	gmPartyRemoveMemberStub(PIERS);
	gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->list.cnt = notFound;

	curSet = 0;
	for(i = 0; i < ELEMENT_COUNT; i++)
		nextDjinn[i] = 0;

	/* Parameter sets are runs of rows each ended by a DEBUG_PARAMETER_SET_END
	   row; skip to the run numbered set and apply its rows, and from
	   DEBUG_PAIRED_SETS_FIRST on the odd run after it too. */
	for(row = 0;; row++) {
		unitId = DebugParameterSets[row].owner;
		if((Uint32)row > DEBUG_PARAMETER_SET_LAST_ROW) {
			notFound = TRUE;
			break;
		}
		if(unitId == DEBUG_PARAMETER_SET_END) {
			if(curSet < DEBUG_PAIRED_SETS_FIRST) {
				if(curSet == set)
					break;
			} else if(curSet == (set | 1)) {
				break;
			}
			curSet++;
			continue;
		}
		if(curSet < set)
			continue;

		gmPartyAddMemberStub(unitId);
		gmUnitLevelUpToStub(unitId, DebugParameterSets[row].level);

		/* Start from no Djinn and no abilities. */
		member = syUnitGet(unitId);
		for(i = 0; i < ELEMENT_COUNT; i++) {
			member->djinnHeldCnts[i] = 0;
			member->djinnSetCnts[i] = 0;
			member->djinnHeld[i] = 0;
			member->djinnSet[i] = 0;
		}
		for(i = 0; i < PSYNERGY_SLOTS; i++)
			*&member->psynergy[i].encodedAction = 0;
		for(i = 0; i < 2; i++) {
			if(DebugParameterSets[row].actions[i] != 0)
				gmUnitLearnPsynergyStub(
					unitId, DebugParameterSets[row].actions[i]);
		}

		/* Djinn are numbered across the party, so no two party members get the
		   same: from this game's own first Djinni of each element, wrapping
		   within its eighteen. */
		for(i = 0; i < ELEMENT_COUNT; i++) {
			for(djinnIdx = 0; djinnIdx < DebugParameterSets[row].djinnCnts[i];
				djinnIdx++) {
				gmDjinnGiveStub(unitId, i,
					(nextDjinn[i] + DJINN_OWN_FIRST) % DJINNI_SPARE_FIRST);
				gmDjinnSetStub(unitId, i,
					(nextDjinn[i] + DJINN_OWN_FIRST) % DJINNI_SPARE_FIRST);
				nextDjinn[i]++;
			}
		}

		/* Empty the inventory, removing slot 0 one time more than there are
		   slots, then add and equip the row's items. */
		for(i = 0; i < (INVENTORY_SLOTS + 1); i++)
			gmInventoryRemoveStub(unitId, 0);
		for(i = 0; i < 6; i++) {
			if(DebugParameterSets[row].items[i] != 0)
				gmInventoryEquipStub(unitId,
					gmInventoryAddItemStub(
						unitId, DebugParameterSets[row].items[i]));
		}
		gmUnitUpdateClassStub(unitId);
	}
	return notFound;
}

/* Does nothing; the debug battle menu calls it when Start is pressed, as The
   Broken Seal's calls BattleDebugSelectNoOperation on Select. */
void btDebugStartNoOperation(void)
{
}

/* The encounter the debug battle starts with (Moapa and two Knights), and The
   Broken Seal's Djinn tutorial encounter, here an unused Mars Djinni's battle:
   rows of formations.c. */
#define DEBUG_FIRST_ENCOUNTER	 69
#define DJINN_TUTORIAL_ENCOUNTER 28

/* Debug: the battle test menu, which never returns. It fights the first
   encounter again and again with auto battle on, unless Down is held at the
   start to open the chooser of encounter and parameter set. */
void btDebugBattleRunMenu(void)
{
	Sint32				startModeSet = 0;
	Sint32				down = PerWork.data & KEY_DOWN;
	Sint32				encounter;
	Sint32				set;
	Sint32				loadedSet;
	INPUT_STATE		   *input;
	MAIN_ROUTINE_STATE *mainState;
	Uint16			   *backdropId;

	gmMainRoutineStateInitDefaultsStub();
restart:
	syWindowLoadGraphics();
	syPrintClear();
	syInitEvent();
	syHeapInit();
	syVRAMCacheReset();
	encounter = DEBUG_FIRST_ENCOUNTER;
	syGameFlagSet(FLAG_AUTO_BATTLE);
	if(down) {
		set = 0;
		loadedSet = -1;
		syGameFlagClear(FLAG_AUTO_BATTLE);
		input = &PerWork;
		backdropId = (Uint16 *)&MainRoutineStateWork.battleBg;
		for(;;) {
			syGameFlagClear(FLAG_CHILDHOOD);
			sySynch(1);
			for(;;) {
				if(input->repeat & KEY_RIGHT)
					encounter++;
				if(input->repeat & KEY_LEFT)
					encounter--;
				if(input->repeat & KEY_UP)
					encounter -= DEBUG_LARGE_STEP;
				if(input->repeat & KEY_DOWN)
					encounter += DEBUG_LARGE_STEP;
				if(input->repeat & KEY_R)
					set++;
				if(input->repeat & KEY_L)
					set--;
				if(input->repeat & KEY_A)
					break;
				if(input->repeat & KEY_START)
					btDebugStartNoOperation();
				/* DEAD: once startModeSet is set, setting it again changes
				   nothing; the ROM keeps the test. */
				if((input->repeat & KEY_B) || startModeSet)
					startModeSet = 1;
				sySynch(1);
			}
			if(set != loadedSet) {
				gmMainRoutineStateInitDefaultsStub();
				btDebugParmSetLoad(set);
				loadedSet = set;
			}
			/* A fights; Down still held makes it a link battle. */
			if(input->data & KEY_DOWN)
				syGameFlagSet(FLAG_LINK_BATTLE);
			gmUnitCalcStub(ISAAC);
			*backdropId = RESOURCE_COLOSSO_BACKDROP;
			mainState = &MainRoutineStateWork;
			if(encounter == DJINN_TUTORIAL_ENCOUNTER)
				syGameFlagSet(FLAG_DJINN_LESSON);
			syGameFlagSet(FLAG_DEBUG_BATTLE);
			if(startModeSet)
				mainState->battleType = BATTLE_TYPE_DEBUG;
			btRun(encounter);
			/* Rebuild what the battle replaced, then return to the chooser. */
			DebugTextEnabled = 1;
			syWindowLoadGraphics();
			syPrintClear();
			syInitEvent();
			syHeapInit();
			syVRAMCacheReset();
		}
	}
	/* Without Down: the same auto battle, over and over. */
	syGameFlagSet(FLAG_DEBUG_BATTLE);
	btRun(DEBUG_FIRST_ENCOUNTER);
	goto restart;
}
