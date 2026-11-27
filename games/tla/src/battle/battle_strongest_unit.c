/* @file */
/* Picking a unit on one side: a random living target, or the living unit with
 */
/* the highest Attack. */
/* Part of battle.c. */

/* Picks a random living target on one side: the party (PLACE_PARTY | slot) */
/* when party is set, otherwise the enemies (PLACE_ENEMY | slot); 0 when none */
/* lives. */
s32 btAIChooseRandomTarget(s32 party) {
	BATTLE_STATE *battle;
	u16 targets[6];
	s32 count;
	s32 i;

	count = 0;
	battle = Ram_HeapSlots->battleState;

	if(party != 0) {
		for(i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->partyUnits[i] != BATTLE_UNIT_REMOVED &&
			   syUnitGet(battle->partyUnits[i])->hp != 0)
				targets[count++] = i | PLACE_PARTY;
		}
	}
	else {
		for(i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->enemyIds[i] != BATTLE_UNIT_REMOVED &&
			   syUnitGet(battle->enemyIds[i])->hp != 0)
				targets[count++] = i | PLACE_ENEMY;
		}
	}
	if(count == 0)
		return 0;
	return targets[(u32)(count * btRandomStub()) >> 16];
}

/* Finds the living unit with the highest Attack on unitId's side (the party */
/* for ids up to ALLY_MAX, else the enemies), other than unitId itself; unitId
 */
/* when no other lives. */
s32 btUnitFindStrongestAlly(u32 unitId) {
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	u32 best = unitId;
	s32 bestAttack = syUnitGet(unitId)->attack;
	s32 i;
	s32 id;

	if(unitId <= ALLY_MAX) {
		for(i = 0; (id = battle->partyUnits[i]) != BATTLE_UNIT_LIST_END; i++) {
			if(id != BATTLE_UNIT_REMOVED && syUnitGet(id)->hp != 0) {
				if(best == unitId ||
				   (id != unitId && syUnitGet(id)->attack > bestAttack)) {
					best = id;
					bestAttack = syUnitGet(best)->attack;
				}
			}
		}
	}
	else {
		for(i = 0; (id = battle->enemyIds[i]) != BATTLE_UNIT_LIST_END; i++) {
			if(id != BATTLE_UNIT_REMOVED && syUnitGet(id)->hp != 0) {
				if(best == unitId ||
				   (id != unitId && syUnitGet(id)->attack > bestAttack)) {
					best = id;
					bestAttack = syUnitGet(best)->attack;
				}
			}
		}
	}
	return best;
}
