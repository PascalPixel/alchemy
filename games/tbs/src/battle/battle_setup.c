/* @file */
/* The battle camera each frame, the opening announcement, the unit lists, */
/* and the party's battle state and Djinn as a battle starts and ends. */
/* Part of battle.c. */

#include "battle_internal.h"

/* Each frame of a turn the camera closes this share of the gap to its target */
/* yaw, one sixteenth, so it slows as it nears it. */
#define CAMERA_YAW_EASE 16

/* Runs each frame of a battle: pauses a link battle whose connection is lost,
 */
/* turns the camera a sixteenth of the way toward its target yaw, places it, */
/* and refreshes the background's zoom. */
void btCameraUpdate(void) {
	void **slots = &HeapSlots[HEAP_SLOT_CAMERA];
	BATTLE_CAMERA *camera = slots[0];
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE_BACKGROUND);
	BATTLE_STATE *battle =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE);
	POS eyeOffset;
	s32 *focus;
	s16 delta;

	/* The link check: a missed frame counts only while the link is down, and */
	/* any frame with it up resets the count. */
	if(battle->isLink != 0) {
		if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
			battle->linkMisses++;
			if(battle->linkMisses > LINK_MAX_MISSED_FRAMES) {
				battle->linkPaused = 1;
			}
		}
		else {
			u32 linkId = SIO_MULTI_PLAYER_ID();
			if(battle->linkSide != linkId) {
				battle->linkPaused = 1;
			}
			battle->linkMisses = 0;
		}
	}

	if(view->frames != 0) {
		delta = view->tgtYaw - camera->yaw;
		delta /= CAMERA_YAW_EASE;
		/* The step is added to the yaw read as an unsigned halfword. */
		camera->yaw = (u16)camera->yaw + delta;
		view->frames--;
	}

	focus = camera->focus;
	if(camera->followPos != 0) {
		focus = camera->followPos;
	}

	syInitMatrix();
	syMatrixTranslateVector(focus);
	syRotY(camera->yaw);
	syRotX(camera->pitch);

	/* The eye sits distance behind the focus, along the turned view. */
	eyeOffset.x = 0;
	eyeOffset.y = 0;
	eyeOffset.z = camera->distance;
	syCalcPoint(&eyeOffset.x, (s32 *)camera);

	/* The plane centred on the projection's centre, at a zoom of 1. */
	if(view->keepRange == 0) {
		btCameraSetRange(toFIXED(BATTLE_PROJECTION_CENTER_X),
						 toFIXED(BATTLE_PROJECTION_CENTER_Y), 0, 0, FIXED_ONE);
	}
}

/* Announces the enemies at the start of a battle, "<enemy> appeared!" for */
/* each, then whether the party strikes first or is caught by surprise. */
void btIntroAnnounceEncounter(s32 enemyCount) {
	s16 enemies[8];
	BATTLE_STATE *battle = BattleStateWork;
	s32 i;

	mnMessageArgumentClearAllStub();
	btEnemyList(enemies);
	for(i = 0; i != enemyCount; i++) {
		mnMessageArgumentPushStub((u16)enemies[i], MESSAGE_ARGUMENT_ACTOR);
		if(i == enemyCount - 1)
			mnBattleMessageShowAndWaitStub(MsgLastEnemyAppeared);
		else
			mnBattleMessageShowAndWaitStub(MsgEnemyAppeared);
		btMessageWaitForAdvance();
	}

	mnBattleMessageCloseStub();
	/* Unit 0's name stands for the party in either message. */
	if(battle->firstStrike == FIRST_STRIKE_PARTY) {
		mnMessageArgumentClearAllStub();
		mnMessageArgumentPushStub(0, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgPartyStrikesFirst);
		btMessageWaitForAdvance();
	}
	else if(battle->firstStrike == FIRST_STRIKE_ENEMIES) {
		mnMessageArgumentClearAllStub();
		mnMessageArgumentPushStub(0, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgPartySurprised);
		btMessageWaitForAdvance();
	}
}

/* Fills the battle's two unit lists: the party in its order, and the enemies */
/* left to right, or for a fixed formation out from the centre. */
void btUnitCollect(void) {
	u16 unitIds[UNIT_MAX];
	BATTLE_STATE *battle;
	s32 count;
	s32 i;
	s32 position;
	s32 formation;

	battle = BattleStateWork;
	count = btPartyPrepareMembers(unitIds);
	for(i = 0; i < count; i++)
		battle->partyUnits[i] = unitIds[i];
	battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

	/* The shuffled and interleaved formations keep the enemies in unit order;
	 * the */
	/* others put the first in the centre and the rest either side of it. */
	count = btEnemyList(unitIds);
	formation = battle->layout;
	switch(formation) {
	case FORMATION_LAYOUT_SHUFFLED:
	case FORMATION_LAYOUT_INTERLEAVED:
		for(i = 0; i < count; i++)
			battle->enemyIds[i] = unitIds[i];
		break;
	case FORMATION_LAYOUT_CENTERED:
	default:
		for(i = 0; i < count; i++) {
			position = BattleFormation_CenteredOffsets[i] + count / 2;
			battle->enemyIds[position] = unitIds[i];
		}
		break;
	}
	battle->enemyIds[count] = BATTLE_UNIT_LIST_END;
}

/* Unused: moves 16 bytes of VRAM down by 16 and clears the 20 from 12 bytes */
/* past where they land. */
UNUSED s32 btShiftVramTiles(void) {
	/* XXX (no evidence): what the bytes moved are, tiles of character block 0
	 * or */
	/* entries of screen block 0; nothing calls this in either game. */
	syDMAXCopy((const void *)0x06000290, (void *)0x06000280,
			   DMA_COPY_HALFWORDS(8), REG_DMA3);
	return syWordsClear((void *)0x0600028c, 20);
}

/* Unused: clears BG0's vertical scroll. */
UNUSED void btClearBg0Vofs(void) { scIoWrite16(0, &REG_BG0VOFS); }

/* Does nothing; btRun calls it with 1 as the battle is set up. */
void btPartyBattleSetupNoOp(s32 unused) {}

/* Clears the party's battle-only states, and works their stats out again, as a
 */
/* battle ends. */
UNREAD_RESULT btPartyClearBattleStatuses(void) {
	u16 unitIds[10];
	s32 count;
	s32 i;

	count = btPartyPrepareMembers(unitIds);

	for(i = 0; i < count; i++) {
		UNIT *unit = gmUnitGetStub(unitIds[i]);
		s32 element;

		for(element = 0; element < ELEMENT_COUNT; element++)
			unit->summonBoost[element] = 0;

		unit->attackTurns = 0;
		unit->attackStage = 0;
		unit->defenseTurns = 0;
		unit->defenseStage = 0;
		unit->resistanceTurns = 0;
		unit->resistanceStage = 0;
		unit->delusion = 0;
		unit->confusion = 0;
		unit->charm = 0;
		unit->stun = 0;
		unit->sleep = 0;
		unit->psySeal = 0;
		unit->regenerate = 0;
		unit->reflect = 0;
		unit->grimReaper = 0;
		unit->equipBonus[0] = 0;
		unit->equipBonus[1] = 0;
		unit->ready = 0;
		unit->immobile = 0;
		unit->agilityTurns = 0;
		unit->agilityStage = 0;
		unit->challenge = 0;

		gmUnitCalcStub(unitIds[i]);
	}
}

/* Brings the Djinn recovery list up to date as a battle starts, and outside a
 */
/* link battle sets again each Djinni waiting for a battle's end. */
UNREAD_RESULT btDjinnRecoveryReconcile(void) {
	u16 unitIds[10];
	s32 count;
	s32 i;
	s32 unitId;
	s32 element;
	s32 djinni;

	count = btPartyPrepareMembers(unitIds);

	/* A party unit (0 to ALLY_MAX) uses the party's list, any other the */
	/* opponent's. */
	for(i = 0; i < count; i++) {
		unitId = unitIds[i];
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
				if(gmDjinnCanSetStub(unitId, element, djinni) != 0) {
					DJINN_RECOVERY_LIST *list =
						&gmDjinnGetRecoveryTableStub(
							 (unitId < 0 || unitId > ALLY_MAX)
								 ? DJINN_RECOVERY_OPPONENT
								 : DJINN_RECOVERY_PARTY)
							 ->list;
					s32 j;

					for(j = 0; j < list->cnt; j++) {
						if(element == list->entries[j].element &&
						   djinni == list->entries[j].idx)
							break;
					}
					if(j == list->cnt)
						btDjinnRecoveryAddStandbyStub(unitId, element, djinni);
				}
			}
		}
	}

	/* Link battles leave the Djinn waiting on the list. */
	if(gmFlagGetStub(FLAG_LINK_BATTLE) != 0)
		return;

	{
		DJINN_RECOVERY_LIST *list =
			&gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->list;

		for(i = 0; i < list->cnt; i++) {
			DJINN_RECOVERY_ENTRY *entry = &list->entries[i];

			if(entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
			   btObjectGet(entry->unitId) == 0) {
				u8 id = entry->unitId;
				u8 element = entry->element;
				u8 index = entry->idx;

				gmDjinnSetStub(id, element, index);
				btDjinnRecoveryRemoveStub(id, element, index);
			}
		}
	}
}

/* The summon mask's bits, one a summon id; gmSummonDefinitionGet gives NULL */
/* for the ids past the last summon. */
#define SUMMON_MASK_BITS 32

/* Works out which summons the party can call, by the Djinn it holds of each */
/* element, set or not; whether enough are on standby is checked when one is */
/* chosen. */
DJINN_RECOVERY_TABLE *btSummonUpdateAvailability(void) {
	u8 totals[ELEMENT_COUNT];
	u16 partyMembers[10];
	s32 partySize;
	s32 element;
	u32 availableMask;

	partySize = btPartyPrepareMembers(partyMembers);
	availableMask = 0;

	/* The party's Djinn of each element. */
	for(element = 0; element < ELEMENT_COUNT; element++) {
		totals[element] = 0;
		{
			s32 partySlot;

			for(partySlot = 0; partySlot < partySize; partySlot++) {
				UNIT *member = gmUnitGetStub(partyMembers[partySlot]);

				totals[element] += member->djinnHeldCnts[element];
			}
		}
	}

	/* Then element counts the summons; the inner loop stops at the first
	 * element */
	/* whose requirement is not met. */
	for(element = 0; element < SUMMON_MASK_BITS; element++) {
		const SUMMON_DEFINITION *summon = gmSummonDefinitionGet(element);
		s32 elementsMet;

		if(summon != 0) {
			const u8 *required = summon->djinnRequired;

			elementsMet = 0;
			if(totals[0] >= required[0]) {
				const u8 *total = totals;

				do {
					if(++elementsMet >= ELEMENT_COUNT)
						break;
					total++;
					required++;
				} while(*total >= *required);
			}

			if(elementsMet == ELEMENT_COUNT)
				availableMask |= 1u << element;
		}
	}

	{
		DJINN_RECOVERY_TABLE *table =
			gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY);
		table->summonMask = availableMask;
		return table;
	}
}

/* Unused: does nothing. */
UNUSED void btSummonReservedNoOp(void) {}

#undef CAMERA_YAW_EASE
#undef SUMMON_MASK_BITS
