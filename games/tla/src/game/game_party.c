/* @file */
/* Enemy units, the active party, coins and counters, and summons. */
/* Part of game.c. */

/* A unit whose name suffix is below NAME_SUFFIX_COUNT gets one more character
 */
/* after its name, NAME_SUFFIX_BASE plus the suffix: the digits 1 to 9 outside
 */
/* Japan, and the letters A to Z in the Japanese release */
/* (btStringFindFirstDigit reads it back). */
#if LANGUAGE == LANGUAGE_JAPANESE
#define NAME_SUFFIX_BASE 'A'
#else
#define NAME_SUFFIX_BASE '1'
#endif

/* Fills a battle unit with an enemy: its stats, its name with its suffix, its
 */
/* items, rewards and elements. A Djinni fought is put on the party's side */
/* (this folder's README.md, "The unit"). Returns 0 for an id out of range. */
s32 gmBattleUnitSet(s32 unitId, s32 enemyId, s32 suffix) {
	UNIT *unit;
	const ENEMY_DEFINITION *enemy;
	u16 name[UNIT_NAME_SIZE];
	s32 i;
	u32 index;
	s32 copy;
	s32 count;

	index = enemyId - ENEMY_ID_FIRST;
	if(unitId < FIRST_ENEMY_UNIT)
		return 0;
	/* BUG: one too many: unit FIRST_ENEMY_UNIT + 6 passes, but syUnitGet has no
	 */
	/* seventh enemy unit and gives NULL. No caller passes it. */
	if(unitId > FIRST_ENEMY_UNIT + 6)
		return 0;
	if(index > ENEMY_ID_MAX - ENEMY_ID_FIRST)
		return 0;
	unit = syUnitGet(unitId);
	syWordsClear(unit, BATTLE_UNIT_SIZE);
	if(index >= ENEMY_COUNT)
		index = 0;
	enemy = &Enemy_Definitions[index];
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
	mnMessageDecodeRawStub(index + (s32)MsgEnemyName, name, UNIT_NAME_SIZE);
	for(i = 0; i < UNIT_NAME_SIZE - 1 && name[i] != 0; i++)
		unit->name[i] = name[i];
	if(suffix <= NAME_SUFFIX_COUNT - 1) {
		unit->name[i] = NAME_SUFFIX_BASE + suffix;
		i++;
	}
	unit->name[i] = 0;
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	count = 0;
	for(i = 0; i < (s32)ARRAY_COUNT(enemy->items); i++) {
		if(enemy->items[i] != 0) {
			for(copy = 0; copy < enemy->cnts[i]; copy++) {
				if(count < INVENTORY_SLOTS)
					unit->inventory[count++] = enemy->items[i];
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
	 */
	/* side. */
	if((u16)(unit->enemyId - ENEMY_VENUS_DJINNI_UNUSED_QUARTZ) <=
	   ENEMY_JUPITER_DJINNI_GALE - ENEMY_VENUS_DJINNI_UNUSED_QUARTZ)
		unit->side = BATTLE_SIDE_PARTY;
	if((u16)(unit->enemyId - ENEMY_DOOM_DRAGON_A) <=
	   ENEMY_DOOM_DRAGON_I - ENEMY_DOOM_DRAGON_A)
		unit->side = BATTLE_SIDE_PARTY;
	return 1;
}

/* Returns the number of party members, counted from its FLAG_IN_PARTY flags. */
s32 gmPartyCountMembers(void) {
	s32 unitId;
	s32 count;

	count = 0;
	for(unitId = 0; unitId <= PIERS; unitId++) {
		if(syGameFlagGet(FLAG_IN_PARTY(unitId)) != 0)
			count++;
	}
	return count;
}

/* Adds a character to the end of the party unless it is there already, and */
/* returns the party size. */
s32 gmPartyAddMember(s32 unitId) {
	s32 partySize = gmPartyCountMembers();
	s32 partyIndex;

	syGameFlagSet(FLAG_IN_PARTY(unitId));
	for(partyIndex = 0; partyIndex < partySize; partyIndex++) {
		if(MainRoutineStateWork.party[partyIndex] == unitId)
			return partySize;
	}
	MainRoutineStateWork.party[partyIndex] = unitId;
	return partySize + 1;
}

/* Takes character `unitId` out of the party: clears its game flag and closes */
/* the gap it leaves in MainRoutineStateWork.party. Returns the party size. */
s32 gmPartyRemoveMember(s32 unitId) {
	s32 partySize = gmPartyCountMembers();
	s32 removedIndex;
	s32 partyIndex;

	syGameFlagClear(FLAG_IN_PARTY(unitId));
	for(removedIndex = 0; removedIndex < partySize; removedIndex++) {
		if(MainRoutineStateWork.party[removedIndex] == unitId)
			break;
	}
	for(partyIndex = removedIndex; partyIndex < partySize - 1; partyIndex++)
		MainRoutineStateWork.party[partyIndex] =
			MainRoutineStateWork.party[partyIndex + 1];
	return gmPartyCountMembers();
}

/* Writes the party members to `unitIds`, ended by BATTLE_UNIT_LIST_END, and */
/* returns how many there are. */
s32 gmPartyListMembers(s16 *unitIds) {
	s32 count = 0;

	if(unitIds != NULL) {
		s32 index;

		count = gmPartyCountMembers();
		for(index = 0; index != count; index++) {
			*unitIds++ = MainRoutineStateWork.party[index];
		}
		*unitIds = BATTLE_UNIT_LIST_END;
	}
	return count;
}

/* Adds `amount` to the party's coins, kept between 0 and COIN_MAX, and */
/* returns the new total. */
s32 gmPartyAdjustCoins(s32 amount) {
	s32 value;
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

/* Each adds `amount` to a counter, kept between 0 and `max`, and returns the */
/* new value: the shop credit and the Game Tickets given. */
#define PARTY_DEFINE_ADJUST(name, field, max)                                  \
	s32 name(s32 amount) {                                                     \
		s32 value;                                                             \
		MAIN_ROUTINE_STATE *progress = &MainRoutineStateWork;                  \
                                                                               \
		value = progress->field;                                               \
		value += amount;                                                       \
		if(value > max)                                                        \
			value = max;                                                       \
		if(value < 0)                                                          \
			value = 0;                                                         \
		progress->field = value;                                               \
		return value;                                                          \
	}

PARTY_DEFINE_ADJUST(gmPartyAdjustShopCredit, shopCredit, COIN_MAX)
PARTY_DEFINE_ADJUST(gmPartyAdjustShopGifts, shopGifts, SHOP_GIFTS_MAX)

/* Writes the summons the party can call to `output`, in menu order, ending */
/* with SUMMON_END, and returns how many. */
s32 gmSummonListLearned(u8 *output) {
	const u8 *summon = Summon_OrderList;
	const u8 *last = &Summon_OrderList[SUMMON_COUNT - 1];
	s32 count = 0;

	do {
		u8 summonId = *summon++;

		if(gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY)->summonMask &
		   (1 << summonId)) {
			*output++ = summonId;
			count++;
		}
	} while(summon <= last);
	*output = SUMMON_END;
	return count;
}

/* Returns summon `summonId`'s definition, or NULL past the last. */
const SUMMON_DEFINITION *gmSummonDefinitionGetNear(u32 summonId) {
	if(summonId > SUMMON_COUNT - 1)
		return NULL;
	return &Summon_DefinitionTable[summonId];
}

/* Lets the party call summon `summonId` from now on, setting its bit in the */
/* party's summon mask. A summon tablet and Lemuria's spring call it. */
void gmSummonLearn(s32 summonId) {
	gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY)->summonMask |= 1 << summonId;
}

#undef NAME_SUFFIX_BASE
