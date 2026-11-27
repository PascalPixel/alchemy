/* Djinn: definitions, setting and standby, and their counts. Part of game.c. */

/* The most Djinn of one element a party member holds or sets. */
#define DJINN_HELD_PER_ELEMENT_MAX 10
/* Above any party member's count of Djinn: the fewest found so far before the
   search, and still when nobody has room. */
#define DJINN_COUNT_NONE		   999

/* Returns the definition of Djinni `djinn` of `element`; anything out of range
   gives DJINNI_FLINT's. */
const DJINN_DEFINITION *gmDjinnGetDefinition(Uint32 element, Uint32 djinn)
{
	Sint32 entry = 0;

	if(element < ELEMENT_COUNT && djinn < DJINN_PER_ELEMENT)
		entry = element * DJINN_PER_ELEMENT + djinn;
	return &DjinnDefinitionTable[entry];
}

/* Gives a newly found Djinni to the party member holding the fewest Djinn who
   has room, on standby. Returns who got it, -1 if found before, -2 when nobody
   has room. */
Sint32 gmDjinnGiveToParty(Sint32 element, Sint32 djinn)
{
	Sint32 flag = djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST;
	Sint32 leastLoadedMember = 0;
	Sint32 fewestDjinn = DJINN_COUNT_NONE;
	Sint32 partySize;
	Sint32 partyIdx;
	Sint32 elementIdx;

	if(syGameFlagGet(flag) != 0)
		return -1;

	partySize = gmPartyCountMembers();
	for(partyIdx = 0; partyIdx < partySize; partyIdx++) {
		UNIT *unit = syUnitGet(MainRoutineStateWork.party[partyIdx]);

		if(unit->djinnHeldCnts[element] < DJINN_HELD_PER_ELEMENT_MAX) {
			Sint32 ownedDjinn = 0;

			for(elementIdx = 0; elementIdx < ELEMENT_COUNT; elementIdx++)
				ownedDjinn += unit->djinnHeldCnts[elementIdx];
			if(fewestDjinn > ownedDjinn) {
				fewestDjinn = ownedDjinn;
				leastLoadedMember = MainRoutineStateWork.party[partyIdx];
			}
		}
	}

	if(fewestDjinn == DJINN_COUNT_NONE)
		return -2;

	gmDjinnGive(leastLoadedMember, element, djinn);
	gmDjinnRecoveryAddStandby(leastLoadedMember, element, djinn);
	syGameFlagSet(flag);
	return leastLoadedMember;
}

/* Gives a Djinni to a unit. Returns -1 when the unit holds
   DJINN_HELD_PER_ELEMENT_MAX of the element or has it already. */
Sint32 gmDjinnGive(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT *unit = syUnitGet(unitId);

	if(unit->djinnHeldCnts[element] >= DJINN_HELD_PER_ELEMENT_MAX)
		return -1;
	if((unit->djinnHeld[element] & (1 << djinnIdx)) != 0)
		return -1;
	unit->djinnHeldCnts[element]++;
	unit->djinnHeld[element] |= 1 << djinnIdx;
	return 0;
}

/* Returns 1 when the unit holds the Djinni and it can be set: not set, not
   recovering and not held by a summon. */
Bool gmDjinnCanSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT				*unit = syUnitGet(unitId);
	DJINN_RECOVERY_LIST *list;
	Sint32				 entryIdx;
	Sint32				 recoveryTurns;

	if(unit->djinnHeldCnts[element] == 0)
		return FALSE;
	if(unit->djinnSetCnts[element] >= DJINN_HELD_PER_ELEMENT_MAX) {
		unit->djinnSetCnts[element] = DJINN_HELD_PER_ELEMENT_MAX;
		return FALSE;
	}
	if((unit->djinnHeld[element] & (1 << djinnIdx)) == 0)
		return FALSE;
	if((unit->djinnSet[element] & (1 << djinnIdx)) != 0)
		return FALSE;

	list = &gmDjinnGetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
	for(entryIdx = 0; entryIdx < list->cnt; entryIdx++) {
		if(element == list->entries[entryIdx].element &&
			djinnIdx == list->entries[entryIdx].idx)
			break;
	}
	/* Ready when not listed, on standby or recovered. */
	if(entryIdx == list->cnt ||
		((recoveryTurns = list->entries[entryIdx].turns) <= 0 &&
			recoveryTurns != DJINN_RECOVERY_SUMMONED))
		return TRUE;
	return FALSE;
}

/* Returns 1 when the Djinni is set on the unit. */
Uint32 gmDjinnIsSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	if(syUnitGet(unitId)->djinnSet[element] & (1 << djinnIdx))
		return 1;
	return 0;
}

/* Sets a ready Djinni on the unit, counts it and works out the unit's class
   again. Returns 1 when it was set. */
Sint32 gmDjinnSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT  *unit = syUnitGet(unitId);
	Sint32 isReady = gmDjinnCanSet(unitId, element, djinnIdx);

	if(isReady != 0) {
		if(unit->djinnHeld[element] & (1 << djinnIdx))
			unit->djinnSet[element] |= 1 << djinnIdx;
		else
			return 0;
		unit->djinnSetCnts[element]++;
		gmUnitUpdateClass(unitId);
	}
	return isReady;
}

/* Takes a set Djinni off the unit and works out its class again. Returns 1 when
   it was set. */
Uint32 gmDjinnPutOnStandby(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT  *unit = syUnitGet(unitId);
	Uint32 wasActive = gmDjinnIsSet(unitId, element, djinnIdx);

	if(wasActive != 0) {
		unit->djinnSetCnts[element]--;
		unit->djinnSet[element] &= ~(1 << djinnIdx);
		gmUnitUpdateClass(unitId);
	}
	return wasActive;
}

#include "game/djinn_recovery.c"

/* Moves a Djinni from one unit to another, set or on standby as it was. Returns
   -1 when it cannot. */
Sint32 gmDjinnTransfer(Sint32 src, Sint32 element, Sint32 djinnIdx, Sint32 tgt)
{
	UNIT  *srcUnit = syUnitGet(src);
	Uint32 wasActive;

	if((srcUnit->djinnHeld[element] & (1 << djinnIdx)) != 0) {
		wasActive = gmDjinnIsSet(src, element, djinnIdx);
		if(gmDjinnGive(tgt, element, djinnIdx) == 0) {
			gmDjinnPutOnStandby(src, element, djinnIdx);
			srcUnit->djinnHeld[element] &= ~(1 << djinnIdx);
			srcUnit->djinnHeldCnts[element]--;

			if(wasActive != 0) {
				gmDjinnSet(tgt, element, djinnIdx);
			} else {
				gmDjinnRecoveryRemove(src, element, djinnIdx);
				gmDjinnRecoveryAddStandby(tgt, element, djinnIdx);
			}
			return 0;
		}
	}
	return -1;
}

#include "game/djinn_recovery_count_standby.c"

/* Returns the ability the Djinni performs when it is unleashed. */
Uint16 gmDjinnGetAbilityId(Uint32 element, Uint32 djinn)
{
	return gmDjinnGetDefinition(element, djinn)->abilityId;
}

/* Returns how many Djinn of `element` the unit holds, or of every element for
   -1. */
Sint32 gmOwnerSumDjinnCounts(Sint32 unitId, Sint32 element)
{
	UNIT  *unit;
	Sint32 result;

	unit = syUnitGet(unitId);
	if(element == -1) {
		result = unit->djinnHeldCnts[0];
		result += unit->djinnHeldCnts[1];
		result += unit->djinnHeldCnts[2];
		result += unit->djinnHeldCnts[3];
	} else {
		result = unit->djinnHeldCnts[element];
	}
	return result;
}

/* Returns how many Djinn of `element` the party holds, or of all elements for
   -1. */
Sint32 gmPartySumDjinnCounts(Sint32 element)
{
	Uint16 unitIds[16];
	Sint32 djinnCnt = 0;
	Sint32 partySize = gmPartyListMembers(unitIds);
	Sint32 i;

	for(i = 0; i < partySize; i++) {
		UNIT *unit = syUnitGet(unitIds[i]);

		if(element == -1) {
			djinnCnt += unit->djinnHeldCnts[0];
			djinnCnt += unit->djinnHeldCnts[1];
			djinnCnt += unit->djinnHeldCnts[2];
			djinnCnt += unit->djinnHeldCnts[3];
		} else {
			djinnCnt += unit->djinnHeldCnts[element];
		}
	}
	return djinnCnt;
}

/* Counts the Djinn recovery down, then sets every Djinni each party member
   holds and can set, taking it off the recovery list. */
void gmDjinnSetAllForParty(void)
{
	Uint16 owners[16];
	Sint32 cnt = gmPartyListMembers((Sint16 *)owners);
	Sint32 i;
	Sint32 element;
	Sint32 djinn;

	btDjinnRecoveryCountDownTwentyStub();
	for(i = 0; i < cnt; i++) {
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinn = 0; djinn < DJINN_PER_ELEMENT; djinn++) {
				if(gmDjinnCanSet(owners[i], element, djinn)) {
					gmDjinnSet(owners[i], element, djinn);
					gmDjinnRecoveryRemove(owners[i], element, djinn);
				}
			}
		}
	}
}
