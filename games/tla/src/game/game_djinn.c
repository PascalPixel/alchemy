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
	Sint32 partyIndex;
	Sint32 elementIndex;

	if(syGameFlagGet(flag) != 0)
		return -1;

	partySize = gmPartyCountMembers();
	for(partyIndex = 0; partyIndex < partySize; partyIndex++) {
		UNIT *unit = syUnitGet(MainRoutineStateWork.party[partyIndex]);

		if(unit->djinnHeldCounts[element] < DJINN_HELD_PER_ELEMENT_MAX) {
			Sint32 ownedDjinn = 0;

			for(elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++)
				ownedDjinn += unit->djinnHeldCounts[elementIndex];
			if(fewestDjinn > ownedDjinn) {
				fewestDjinn = ownedDjinn;
				leastLoadedMember = MainRoutineStateWork.party[partyIndex];
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
Sint32 gmDjinnGive(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	UNIT *unit = syUnitGet(unitId);

	if(unit->djinnHeldCounts[element] >= DJINN_HELD_PER_ELEMENT_MAX)
		return -1;
	if((unit->djinnHeld[element] & (1 << djinnIndex)) != 0)
		return -1;
	unit->djinnHeldCounts[element]++;
	unit->djinnHeld[element] |= 1 << djinnIndex;
	return 0;
}

/* Returns 1 when the unit holds the Djinni and it can be set: not set, not
   recovering and not held by a summon. */
Bool gmDjinnCanSet(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	UNIT				*unit = syUnitGet(unitId);
	DJINN_RECOVERY_LIST *list;
	Sint32				 entryIndex;
	Sint32				 recoveryTurns;

	if(unit->djinnHeldCounts[element] == 0)
		return FALSE;
	if(unit->djinnSetCounts[element] >= DJINN_HELD_PER_ELEMENT_MAX) {
		unit->djinnSetCounts[element] = DJINN_HELD_PER_ELEMENT_MAX;
		return FALSE;
	}
	if((unit->djinnHeld[element] & (1 << djinnIndex)) == 0)
		return FALSE;
	if((unit->djinnSet[element] & (1 << djinnIndex)) != 0)
		return FALSE;

	list = &gmDjinnGetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
	for(entryIndex = 0; entryIndex < list->count; entryIndex++) {
		if(element == list->entries[entryIndex].element &&
			djinnIndex == list->entries[entryIndex].index)
			break;
	}
	/* Ready when not listed, on standby or recovered. */
	if(entryIndex == list->count ||
		((recoveryTurns = list->entries[entryIndex].turns) <= 0 &&
			recoveryTurns != DJINN_RECOVERY_SUMMONED))
		return TRUE;
	return FALSE;
}

/* Returns 1 when the Djinni is set on the unit. */
Uint32 gmDjinnIsSet(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	if(syUnitGet(unitId)->djinnSet[element] & (1 << djinnIndex))
		return 1;
	return 0;
}

/* Sets a ready Djinni on the unit, counts it and works out the unit's class
   again. Returns 1 when it was set. */
Sint32 gmDjinnSet(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	UNIT  *unit = syUnitGet(unitId);
	Sint32 isReady = gmDjinnCanSet(unitId, element, djinnIndex);

	if(isReady != 0) {
		if(unit->djinnHeld[element] & (1 << djinnIndex))
			unit->djinnSet[element] |= 1 << djinnIndex;
		else
			return 0;
		unit->djinnSetCounts[element]++;
		gmUnitUpdateClass(unitId);
	}
	return isReady;
}

/* Takes a set Djinni off the unit and works out its class again. Returns 1 when
   it was set. */
Uint32 gmDjinnPutOnStandby(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	UNIT  *unit = syUnitGet(unitId);
	Uint32 wasActive = gmDjinnIsSet(unitId, element, djinnIndex);

	if(wasActive != 0) {
		unit->djinnSetCounts[element]--;
		unit->djinnSet[element] &= ~(1 << djinnIndex);
		gmUnitUpdateClass(unitId);
	}
	return wasActive;
}

/* Takes the Djinni off its side's standby and recovery list, closing the gap.
   Returns 1 when it was there. */
Sint32 gmDjinnRecoveryRemove(Sint32 unitId, Sint32 element, Sint32 djinnIndex)
{
	DJINN_RECOVERY_LIST *list;
	Bool				 removed = FALSE;
	Sint32				 entryIndex;

	list = &gmDjinnGetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
	for(entryIndex = 0; entryIndex < list->count; entryIndex++) {
		if(element == list->entries[entryIndex].element &&
			djinnIndex == list->entries[entryIndex].index) {
			list->count--;
			removed = TRUE;
			break;
		}
	}
	for(; entryIndex < list->count; entryIndex++) {
		list->entries[entryIndex] = list->entries[entryIndex + 1];
	}
	return removed;
}

/* Puts a unit's Djinni at the end of its side's list on standby. Nothing checks
   the list's capacity. */
Uint32 *gmDjinnRecoveryAddStandby(
	Uint32 unitId, Uint32 element, Uint32 djinnIndex)
{
	DJINN_RECOVERY_LIST *list;
	Sint32				 count;

	gmDjinnRecoveryRemove(unitId, element, djinnIndex);
	list = &gmDjinnGetRecoveryTable(unitId > PIERS)->list;
	count = list->count;
	list->entries[count].element = element;
	list->entries[count].index = djinnIndex;
	list->entries[count].unitId = unitId;
	list->entries[count].turns = DJINN_RECOVERY_AFTER_BATTLE;
	list->count = count + 1;
	return (Uint32 *)&list->count;
}

/* Moves a Djinni from one unit to another, set or on standby as it was. Returns
   -1 when it cannot. */
Sint32 gmDjinnTransfer(
	Sint32 src, Sint32 element, Sint32 djinnIndex, Sint32 target)
{
	UNIT  *srcUnit = syUnitGet(src);
	Uint32 wasActive;

	if((srcUnit->djinnHeld[element] & (1 << djinnIndex)) != 0) {
		wasActive = gmDjinnIsSet(src, element, djinnIndex);
		if(gmDjinnGive(target, element, djinnIndex) == 0) {
			gmDjinnPutOnStandby(src, element, djinnIndex);
			srcUnit->djinnHeld[element] &= ~(1 << djinnIndex);
			srcUnit->djinnHeldCounts[element]--;

			if(wasActive != 0) {
				gmDjinnSet(target, element, djinnIndex);
			} else {
				gmDjinnRecoveryRemove(src, element, djinnIndex);
				gmDjinnRecoveryAddStandby(target, element, djinnIndex);
			}
			return 0;
		}
	}
	return -1;
}

/* Counts the party's Djinn on standby, and by element into `counts` when it is
   given. */
Sint32 gmDjinnRecoveryCountStandby(Uint8 *counts)
{
	Sint32				  standbyCount = 0;
	DJINN_RECOVERY_TABLE *recoveryTable =
		gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY);
	DJINN_RECOVERY_ENTRY *entry = recoveryTable->list.entries;
	Sint32				  entryIndex;

	if(counts != 0) {
		Sint32 element;

		/* Each element's count starts at 0. */
		for(element = 0; element < ELEMENT_COUNT; element++)
			counts[element] = 0;
	}
	for(entryIndex = 0; entryIndex != recoveryTable->list.count; entryIndex++) {
		if(entry->turns == DJINN_RECOVERY_AFTER_BATTLE) {
			if(counts != 0)
				counts[entry->element]++;
			standbyCount++;
		}
		entry++;
	}
	return standbyCount;
}

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
		result = unit->djinnHeldCounts[0];
		result += unit->djinnHeldCounts[1];
		result += unit->djinnHeldCounts[2];
		result += unit->djinnHeldCounts[3];
	} else {
		result = unit->djinnHeldCounts[element];
	}
	return result;
}

/* Returns how many Djinn of `element` the party holds, or of all elements for
   -1. */
Sint32 gmPartySumDjinnCounts(Sint32 element)
{
	Uint16 unitIds[16];
	Sint32 djinnCount = 0;
	Sint32 partySize = gmPartyListMembers(unitIds);
	Sint32 i;

	for(i = 0; i < partySize; i++) {
		UNIT *unit = syUnitGet(unitIds[i]);

		if(element == -1) {
			djinnCount += unit->djinnHeldCounts[0];
			djinnCount += unit->djinnHeldCounts[1];
			djinnCount += unit->djinnHeldCounts[2];
			djinnCount += unit->djinnHeldCounts[3];
		} else {
			djinnCount += unit->djinnHeldCounts[element];
		}
	}
	return djinnCount;
}

/* Counts the Djinn recovery down, then sets every Djinni each party member
   holds and can set, taking it off the recovery list. */
void gmDjinnSetAllForParty(void)
{
	Uint16 owners[16];
	Sint32 count = gmPartyListMembers((Sint16 *)owners);
	Sint32 i;
	Sint32 element;
	Sint32 djinn;

	btDjinnRecoveryCountDownTwentyStub();
	for(i = 0; i < count; i++) {
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
