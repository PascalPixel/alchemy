/* Enemy units, the active party, coins and counters, and summons. Part of
   game.c. */

/* A unit whose name suffix is below NAME_SUFFIX_COUNT gets one more character
   after its name, NAME_SUFFIX_BASE plus the suffix: the digits 1 to 9 outside
   Japan, and the letters A to Z in the Japanese release (btStringFindFirstDigit
   reads it back). */
#define NAME_SUFFIX_BASE JAPANESE_OR('A', '1')

/* Fills a battle unit with an enemy: its stats, its name with its suffix, its
   items, rewards and elements. A Djinni fought is put on the party's side (this
   folder's README.md, "The unit"). Returns 0 for an id out of range. */
Bool gmBattleUnitSet(Sint32 unitId, Sint32 enemyId, Sint32 suffix)
{
	UNIT				   *unit;
	const ENEMY_DEFINITION *enemy;
	Uint16					name[UNIT_NAME_SIZE];
	Sint32					i;
	Uint32					idx;
	Sint32					copy;
	Sint32					cnt;

	idx = enemyId - ENEMY_ID_FIRST;
	if(unitId < FIRST_ENEMY_UNIT)
		return FALSE;
	/* BUG: one too many: unit FIRST_ENEMY_UNIT + 6 passes, but syUnitGet has no
	   seventh enemy unit and gives NULL. No caller passes it. */
	if(unitId > FIRST_ENEMY_UNIT + 6)
		return FALSE;
	if(idx > ENEMY_ID_MAX - ENEMY_ID_FIRST)
		return FALSE;
	unit = syUnitGet(unitId);
	syWordsClear(unit, BATTLE_UNIT_SIZE);
	if(idx >= ENEMY_COUNT)
		idx = 0;
	enemy = &EnemyDefinitions[idx];
	unit->level = enemy->level;
	unit->baseHp = enemy->hp;
	unit->hp = enemy->hp;
	unit->hpMax = enemy->hp;
	unit->basePp = enemy->pp;
	unit->pp = enemy->pp;
	unit->ppMax = enemy->pp;
	unit->hpGauge = UNIT_GAUGE_FULL;
	unit->ppGauge = UNIT_GAUGE_FULL;
	unit->baseAttack = enemy->attack;
	unit->baseDefense = enemy->defense;
	unit->baseAgility = enemy->agility;
	unit->baseLuck = enemy->luck;
	unit->baseActionCnt = enemy->actionCnt;
	unit->baseHpRegen = enemy->hpRegen;
	unit->basePpRegen = enemy->ppRegen;
	mnMessageDecodeRawStub(idx + (Sint32)MsgEnemyName, name, UNIT_NAME_SIZE);
	for(i = 0; i < UNIT_NAME_SIZE - 1 && name[i] != 0; i++)
		unit->name[i] = name[i];
	if(suffix <= NAME_SUFFIX_COUNT - 1) {
		unit->name[i] = NAME_SUFFIX_BASE + suffix;
		i++;
	}
	unit->name[i] = 0;
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	cnt = 0;
	for(i = 0; i < (Sint32)ARRAY_COUNT(enemy->items); i++) {
		if(enemy->items[i] != 0) {
			for(copy = 0; copy < enemy->cnts[i]; copy++) {
				if(cnt < INVENTORY_SLOTS)
					unit->inventory[cnt++] = enemy->items[i];
			}
		}
	}
	unit->aiState = (BATTLE_AI_STATE){0};
	unit->classId = CLASS_NONE;
	unit->enemyId = enemyId;
	gmUnitCalcBaseElements(unitId, unit->baseElement);
	gmUnitCalc(unitId);
	unit->side = BATTLE_SIDE_ENEMY;
	/* The Djinn fought, and the Doom Dragon's nine rows, are put on the party's
	   side. */
	if((Uint16)(unit->enemyId - ENEMY_VENUS_DJINNI_UNUSED_QUARTZ) <=
		ENEMY_JUPITER_DJINNI_GALE - ENEMY_VENUS_DJINNI_UNUSED_QUARTZ)
		unit->side = BATTLE_SIDE_PARTY;
	if((Uint16)(unit->enemyId - ENEMY_DOOM_DRAGON_A) <=
		ENEMY_DOOM_DRAGON_I - ENEMY_DOOM_DRAGON_A)
		unit->side = BATTLE_SIDE_PARTY;
	return TRUE;
}

/* Returns the number of party members, counted from its FLAG_IN_PARTY flags. */
Sint32 gmPartyCountMembers(void)
{
	Sint32 unitId;
	Sint32 cnt;

	cnt = 0;
	for(unitId = 0; unitId <= PIERS; unitId++) {
		if(syGameFlagGet(FLAG_IN_PARTY(unitId)) != 0)
			cnt++;
	}
	return cnt;
}

/* Adds a character to the end of the party unless it is there already, and
   returns the party size. */
Sint32 gmPartyAddMember(Sint32 unitId)
{
	Sint32 partySize = gmPartyCountMembers();
	Sint32 partyIdx;

	syGameFlagSet(FLAG_IN_PARTY(unitId));
	for(partyIdx = 0; partyIdx < partySize; partyIdx++) {
		if(MainRoutineStateWork.party[partyIdx] == unitId)
			return partySize;
	}
	MainRoutineStateWork.party[partyIdx] = unitId;
	return partySize + 1;
}

/* Takes character `unitId` out of the party: clears its game flag and closes
   the gap it leaves in MainRoutineStateWork.party. Returns the party size. */
Sint32 gmPartyRemoveMember(Sint32 unitId)
{
	Sint32 partySize = gmPartyCountMembers();
	Sint32 removedIdx;
	Sint32 partyIdx;

	syGameFlagClear(FLAG_IN_PARTY(unitId));
	for(removedIdx = 0; removedIdx < partySize; removedIdx++) {
		if(MainRoutineStateWork.party[removedIdx] == unitId)
			break;
	}
	for(partyIdx = removedIdx; partyIdx < partySize - 1; partyIdx++)
		MainRoutineStateWork.party[partyIdx] =
			MainRoutineStateWork.party[partyIdx + 1];
	return gmPartyCountMembers();
}

/* Writes the party members to `unitIds`, ended by BATTLE_UNIT_LIST_END, and
   returns how many there are. */
Sint32 gmPartyListMembers(Sint16 *unitIds)
{
	Sint32 cnt = 0;

	if(unitIds != NULL) {
		Sint32 idx;

		cnt = gmPartyCountMembers();
		for(idx = 0; idx != cnt; idx++) {
			*unitIds++ = MainRoutineStateWork.party[idx];
		}
		*unitIds = BATTLE_UNIT_LIST_END;
	}
	return cnt;
}

/* Adds `amount` to the party's coins, kept between 0 and COIN_MAX, and returns
   the new total. */
Sint32 gmPartyAdjustCoins(Sint32 amount)
{
	Sint32				value;
	MAIN_ROUTINE_STATE *state;

	state = &MainRoutineStateWork;
	value = state->coins;
	value += amount;
	if(value > COIN_MAX) {
		value = COIN_MAX;
	}
	if(value < 0) {
		value = 0;
	}
	state->coins = value;
	return value;
}

/* Each adds `amount` to a counter, kept between 0 and `max`, and returns the
   new value: the shop credit and the Game Tickets given. */
#define PARTY_DEFINE_ADJUST(name, field, max) \
	Sint32 name(Sint32 amount) \
	{ \
		Sint32				value; \
		MAIN_ROUTINE_STATE *progress = &MainRoutineStateWork; \
\
		value = progress->field; \
		value += amount; \
		if(value > max) \
			value = max; \
		if(value < 0) \
			value = 0; \
		progress->field = value; \
		return value; \
	}

PARTY_DEFINE_ADJUST(gmPartyAdjustShopCredit, shopCredit, COIN_MAX)
PARTY_DEFINE_ADJUST(gmPartyAdjustShopGifts, shopGifts, SHOP_GIFTS_MAX)

/* Writes the summons the party can call to `output`, in menu order, ending with
   SUMMON_END, and returns how many. */
Sint32 gmSummonListLearned(Uint8 *output)
{
	const Uint8 *summon = SummonOrderList;
	const Uint8 *last = &SummonOrderList[SUMMON_COUNT - 1];
	Sint32		 cnt = 0;

	do {
		Uint8 summonId = *summon++;

		if(gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY)->summonMask &
			(1 << summonId)) {
			*output++ = summonId;
			cnt++;
		}
	} while(summon <= last);
	*output = SUMMON_END;
	return cnt;
}

/* Returns summon `summonId`'s definition, or NULL past the last. */
const SUMMON_DEFINITION *gmSummonDefinitionGetNear(Uint32 summonId)
{
	if(summonId > SUMMON_COUNT - 1)
		return NULL;
	return &SummonDefinitionTable[summonId];
}

/* Lets the party call summon `summonId` from now on, setting its bit in the
   party's summon mask. A summon tablet and Lemuria's spring call it. */
void gmSummonLearn(Sint32 summonId)
{
	gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY)->summonMask |= 1 << summonId;
}

#undef NAME_SUFFIX_BASE
