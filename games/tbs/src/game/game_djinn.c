/* Djinn: definitions, setting and standby, and their counts. Part of game.c. */

/* The most Djinn of one element a party member holds or sets. */
#define DJINN_HELD_PER_ELEMENT_MAX 10
/* Above any party member's count of Djinn: the fewest found so far before the
   search, and still when nobody has room. */
#define DJINN_COUNT_NONE		   999

/* What gmDjinnGiveToParty returns in place of a unit when nobody gets the
   Djinni; its one caller, fdDjinnJoinParty, plays the joining only for a
   unit. */
#define DJINN_GIVE_FOUND_BEFORE -1
#define DJINN_GIVE_NO_ROOM		-2

/* Returns the definition of Djinni `djinni` of `element`; anything out of range
   gives DJINNI_FLINT's. */
const DJINN_DEFINITION *gmDjinnGetDefinition(Uint32 element, Uint32 djinni)
{
	Sint32 entry = 0;

	if(element < ELEMENT_COUNT && djinni < DJINN_PER_ELEMENT)
		entry = element * DJINN_PER_ELEMENT + djinni;
	return &DjinnDefinitionTable[entry];
}

/* Gives a newly found Djinni to the party member holding the fewest Djinn who
   has room, on standby. Returns who got it, DJINN_GIVE_FOUND_BEFORE or
   DJINN_GIVE_NO_ROOM. */
Sint32 gmDjinnGiveToParty(Sint32 element, Sint32 djinn)
{
	Sint32 flag = djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST;
	Sint32 leastLoadedMember = 0;
	Sint32 fewestDjinn = DJINN_COUNT_NONE;
	Sint32 partySize;
	Sint32 partyIdx;
	Sint32 elementIdx;

	if(gmFlagGet(flag) != 0)
		return DJINN_GIVE_FOUND_BEFORE;

	partySize = gmPartyCountMembers();
	for(partyIdx = 0; partyIdx < partySize; partyIdx++) {
		UNIT *unit = gmUnitGet(MainRoutineStateWork.party[partyIdx]);

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
		return DJINN_GIVE_NO_ROOM;

	gmDjinnGive(leastLoadedMember, element, djinn);
	gmDjinnRecoveryAddStandby(leastLoadedMember, element, djinn);
	gmFlagSet(flag);
	return leastLoadedMember;
}

/* Gives a Djinni to a unit. Returns -1 when the unit holds
   DJINN_HELD_PER_ELEMENT_MAX of the element or has it already. */
Sint32 gmDjinnGive(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT *unit = gmUnitGet(unitId);

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
	UNIT				*unit = gmUnitGet(unitId);
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
	if(gmUnitGet(unitId)->djinnSet[element] & (1 << djinnIdx))
		return 1;
	return 0;
}

/* Sets a ready Djinni on the unit, counts it and works out the unit's class
   again. Returns 1 when it was set. */
Sint32 gmDjinnSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	UNIT  *unit = gmUnitGet(unitId);
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
	UNIT  *unit = gmUnitGet(unitId);
	Uint32 wasActive = gmDjinnIsSet(unitId, element, djinnIdx);

	if(wasActive != 0) {
		unit->djinnSetCnts[element]--;
		unit->djinnSet[element] &= ~(1 << djinnIdx);
		gmUnitUpdateClass(unitId);
	}
	return wasActive;
}

/* Takes the Djinni off its side's standby and recovery list, closing the gap.
   Returns 1 when it was there. */
Sint32 gmDjinnRecoveryRemove(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	DJINN_RECOVERY_LIST *list;
	Bool				 removed = FALSE;
	Sint32				 entryIdx;

	list = &gmDjinnGetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
	for(entryIdx = 0; entryIdx < list->cnt; entryIdx++) {
		if(element == list->entries[entryIdx].element &&
			djinnIdx == list->entries[entryIdx].idx) {
			list->cnt--;
			removed = TRUE;
			break;
		}
	}
	for(; entryIdx < list->cnt; entryIdx++) {
		list->entries[entryIdx] = list->entries[entryIdx + 1];
	}
	return removed;
}

/* Puts a unit's Djinni at the end of its side's list on standby. Nothing checks
   the list's capacity. */
Uint32 *gmDjinnRecoveryAddStandby(
	Uint32 unitId, Uint32 element, Uint32 djinnIdx)
{
	DJINN_RECOVERY_LIST *list;
	Sint32				 cnt;

	gmDjinnRecoveryRemove(unitId, element, djinnIdx);
	list = &gmDjinnGetRecoveryTable(unitId > PIERS)->list;
	cnt = list->cnt;
	list->entries[cnt].element = element;
	list->entries[cnt].idx = djinnIdx;
	list->entries[cnt].unitId = unitId;
	list->entries[cnt].turns = DJINN_RECOVERY_AFTER_BATTLE;
	list->cnt = cnt + 1;
	return (Uint32 *)&list->cnt;
}

/* Moves a Djinni from one unit to another, set or on standby as it was. Returns
   -1 when it cannot. */
Sint32 gmDjinnTransfer(Sint32 src, Sint32 element, Sint32 djinnIdx, Sint32 tgt)
{
	UNIT  *srcUnit = gmUnitGet(src);
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

/* Counts the party's Djinn on standby, by element into counts when it is
   given. */
Sint32 gmDjinnRecoveryCountStandby(Uint8 *cnts)
{
	Sint32				  standbyCnt = 0;
	DJINN_RECOVERY_TABLE *recoveryTbl =
		gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY);
	DJINN_RECOVERY_ENTRY *entry = recoveryTbl->list.entries;
	Sint32				  entryIdx;

	if(cnts != 0) {
		Sint32 element;

		/* Each element's count starts at 0. */
		for(element = 0; element < ELEMENT_COUNT; element++)
			cnts[element] = 0;
	}
	for(entryIdx = 0; entryIdx != recoveryTbl->list.cnt; entryIdx++) {
		if(entry->turns == DJINN_RECOVERY_AFTER_BATTLE) {
			if(cnts != 0)
				cnts[entry->element]++;
			standbyCnt++;
		}
		entry++;
	}
	return standbyCnt;
}

/* Returns the ability the Djinni unleashes. */
Sint32 gmDjinnGetAbilityId(Uint32 element, Uint32 djinni)
{
	return gmDjinnGetDefinition(element, djinni)->abilityId;
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
		UNIT *unit = gmUnitGet(unitIds[i]);

		if(element == -1) {
			djinnCnt += unit->djinnHeldCnts[ELEMENT_VENUS];
			djinnCnt += unit->djinnHeldCnts[ELEMENT_MERCURY];
			djinnCnt += unit->djinnHeldCnts[ELEMENT_MARS];
			djinnCnt += unit->djinnHeldCnts[ELEMENT_JUPITER];
		} else {
			djinnCnt += unit->djinnHeldCnts[element];
		}
	}
	return djinnCnt;
}
