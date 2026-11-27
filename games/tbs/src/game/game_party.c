/* Enemy units, the active party, coins and counters, and summons. Part of
   game.c. */

/* The letter that tells apart enemies of one kind: digits from 1 abroad, the
   alphabet in Japan. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define ENEMY_SUFFIX_FIRST '1' /* suffix 0's character */
#define ENEMY_SUFFIX_LAST  8   /* the last suffix shown, '9' */
#else
#define ENEMY_SUFFIX_FIRST 'A' /* suffix 0's character */
#define ENEMY_SUFFIX_LAST  25  /* the last suffix shown, 'Z' */
#endif

/* Fills a battle unit with an enemy: its stats, its name with its letter, its
   items, rewards and elements. A Djinni fought is put on the party's side
   (README.md, "The unit"). Returns 0 for an id out of range. */
Bool gmBattleUnitSet(Sint32 unitId, Sint32 enemyId, Sint32 suffix)
{
	UNIT				   *unit;
	const ENEMY_DEFINITION *enemy;
	Uint16					name[UNIT_NAME_SIZE];
	Sint32					i;
	Uint32					index;
	Sint32					copy;
	Sint32					count;

	index = enemyId - ENEMY_ID_FIRST;
	if(unitId < FIRST_ENEMY_UNIT)
		return FALSE;
	/* BUG: one too many: unit FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE passes, but
	   gmUnitGet has no seventh enemy unit and gives NULL. No caller passes
	   it. */
	if(unitId > FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE)
		return FALSE;
	if(index > ENEMY_ID_MAX - ENEMY_ID_FIRST)
		return FALSE;
	unit = gmUnitGet(unitId);
	syWordsClear(unit, BATTLE_UNIT_SIZE);
	if(index >= ENEMY_DEFINITION_COUNT)
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
	mnMessageDecodeRawStub(index + MsgEnemyName, name, UNIT_NAME_SIZE);
	for(i = 0; i < UNIT_NAME_SIZE - 1 && name[i] != 0; i++)
		unit->name[i] = name[i];
	/* Enemies of one kind are told apart by a letter after the name; past the
	   last letter the name stays bare. */
	if(suffix <= ENEMY_SUFFIX_LAST) {
		unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
		i++;
	}
	unit->name[i] = 0;
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	count = 0;
	for(i = 0; i < 4; i++) {
		if(enemy->items[i] != 0) {
			for(copy = 0; copy < enemy->cnts[i]; copy++) {
				if(count < INVENTORY_SLOTS)
					unit->inventory[count++] = enemy->items[i];
			}
		}
	}
	unit->aiState = enemy->aiState;
	unit->classId = CLASS_NONE;
	unit->id = enemyId;
	gmUnitCalcBaseElements(unitId, unit->baseElement);
	gmUnitCalc(unitId);
	unit->side = BATTLE_SIDE_ENEMY;
	switch(unit->id) {
	case ENEMY_VENUS_DJINNI_QUARTZ:
	case ENEMY_VENUS_DJINNI_VINE:
	case ENEMY_VENUS_DJINNI_UNUSED_BANE:
	case ENEMY_MERCURY_DJINNI_SLEET:
	case ENEMY_MERCURY_DJINNI_SPRITZ:
	case ENEMY_MERCURY_DJINNI_HAIL:
	case ENEMY_MERCURY_DJINNI_DEW:
	case ENEMY_MARS_DJINNI_FORGE:
	case ENEMY_MARS_DJINNI_CORONA:
	case ENEMY_MARS_DJINNI_FLASH:
	case ENEMY_JUPITER_DJINNI_BREEZE:
	case ENEMY_JUPITER_DJINNI_ZEPHYR:
	case ENEMY_JUPITER_DJINNI_SQUALL:
	case ENEMY_JUPITER_DJINNI_LUFF:
		unit->side = BATTLE_SIDE_PARTY;
		break;
	}
	return TRUE;
}

/* The party: the FLAG_IN_PARTY flags say who is in it, and
   MainRoutineStateWork.party lists them in order. */

/* Returns the number of party members, counted from its FLAG_IN_PARTY flags. */
Sint32 gmPartyCountMembers(void)
{
	Sint32 unitId;
	Sint32 count;

	count = 0;
	for(unitId = 0; unitId <= PIERS; unitId++) {
		if(gmFlagGet(FLAG_IN_PARTY(unitId)) != 0)
			count++;
	}
	return count;
}

/* Adds a character to the end of the party unless it is there already, and
   returns the party size. */
Sint32 gmPartyAddMember(Sint32 unitId)
{
	Sint32 partySize = gmPartyCountMembers();
	Sint32 partyIndex;

	gmFlagSet(FLAG_IN_PARTY(unitId));
	for(partyIndex = 0; partyIndex < partySize; partyIndex++) {
		if(MainRoutineStateWork.party[partyIndex] == unitId)
			return partySize;
	}
	MainRoutineStateWork.party[partyIndex] = unitId;
	return partySize + 1;
}

/* Takes character `unitId` out of the party: clears its game flag and closes
   the gap it leaves in MainRoutineStateWork.party. Returns the party size. */
Sint32 gmPartyRemoveMember(Sint32 unitId)
{
	Sint32 partySize = gmPartyCountMembers();
	Sint32 removedIndex;
	Sint32 partyIndex;

	gmFlagClear(FLAG_IN_PARTY(unitId));
	for(removedIndex = 0; removedIndex < partySize; removedIndex++) {
		if(MainRoutineStateWork.party[removedIndex] == unitId)
			break;
	}
	for(partyIndex = removedIndex; partyIndex < partySize - 1; partyIndex++)
		MainRoutineStateWork.party[partyIndex] =
			MainRoutineStateWork.party[partyIndex + 1];
	return gmPartyCountMembers();
}

/* Writes the party members to unitIds and returns how many there are. */
Sint32 gmPartyListMembers(Sint16 *unitIds)
{
	Sint32 count = 0;

	if(unitIds != NULL) {
		Sint32 index;

		count = gmPartyCountMembers();
		for(index = 0; index != count; index++) {
			*unitIds++ = MainRoutineStateWork.party[index];
		}
		*unitIds = BATTLE_UNIT_LIST_END;
	}
	return count;
}

/* Adds `amount` to the party's coins, kept between 0 and COIN_MAX, and returns
   the new total. */
Sint32 gmPartyAdjustCoins(Sint32 amount)
{
	Sint32				value;
	MAIN_ROUTINE_STATE *routine;

	routine = &MainRoutineStateWork;
	value = routine->holdings.coins;
	value += amount;
	if(value > COIN_MAX) {
		value = COIN_MAX;
	}
	if(value < 0) {
		value = 0;
	}
	routine->holdings.coins = value;
	return value;
}

/* Each adds amount to a counter, kept between 0 and max, and returns the new
   value: the shop credit and the Game Tickets given. */
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

/* Adds amount to the shop credit, kept from 0 to COIN_MAX. */
PARTY_DEFINE_ADJUST(gmPartyAdjustShopCredit, holdings.shopCredit, COIN_MAX)
/* Adds amount to the Game Tickets given, kept from 0 to SHOP_GIFTS_MAX. */
PARTY_DEFINE_ADJUST(gmPartyAdjustShopGifts, holdings.shopGifts, SHOP_GIFTS_MAX)

/* Writes the summons the party can call to output, in menu order, ending with
   SUMMON_END, and returns how many. */
Sint32 gmSummonListLearned(Uint8 *output)
{
	const Uint8 *summon = Summon_OrderList;
	const Uint8 *last = &Summon_OrderList[ARRAY_COUNT(Summon_OrderList) - 1];
	Sint32		 count = 0;

	do {
		Uint8 summonId = *summon++;

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
const SUMMON_DEFINITION *gmSummonDefinitionGetNear(Uint32 summonId)
{
	/* The order list holds every summon once. */
	if(summonId >= ARRAY_COUNT(Summon_OrderList))
		return NULL;
	return &Summon_DefinitionTable[summonId];
}

#undef ENEMY_SUFFIX_FIRST
#undef ENEMY_SUFFIX_LAST
#undef PARTY_DEFINE_ADJUST
