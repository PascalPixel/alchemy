/* Enemy formations: the enemies chosen, levelled and lettered. Part of
   battle.c. */

#include "battle_internal.h"

/* How many name letters tell the enemies of one kind apart: 26 in Japanese, 9
   in the localizations. */
#define ENEMY_NAME_SUFFIX_COUNT JAPANESE_OR(26, 9)
/* The letters a kind's mask holds, one bit each. */
#define NAME_LETTER_MASK_BITS	32
/* The formation a battle falls back on when its own is out of range or has no
   sure member: a lone Vermin. */
#define FORMATION_FALLBACK		1
/* The random swaps that shuffle a FORMATION_LAYOUT_SHUFFLED formation. */
#define FORMATION_SHUFFLE_SWAPS 10

/* btFormationAverageLevel's reasons a formation may not be chosen: no member
   always appears, an enemy above MONSTER_BATTLE_OPEN_LEVEL is one the party has
   not defeated, or no member has a definition. */
#define FORMATION_NO_SURE_MEMBER  -1
#define FORMATION_UNMET_ENEMY	  -2
#define FORMATION_NO_ENEMY		  -3
/* The monster battles offer an enemy up to this level whether or not the party
   has defeated it. */
#define MONSTER_BATTLE_OPEN_LEVEL 3

/* Returns a formation's average enemy level for the monster battles, or a
   negative reason it may not be chosen. */
Sint32 btFormationAverageLevel(Sint32 formationId)
{
	const FORMATION		   *formation;
	const ENEMY_DEFINITION *enemy;
	Uint32					memberIdx;
	Sint32					enemyId;
	Sint32					levelTotal;
	Sint32					enemyCnt;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32 levels[7];

	enemyCnt = 0;
	levelTotal = 0;
	formation = &Formations[formationId];
	for(memberIdx = 0; memberIdx < FORMATION_MEMBERS; memberIdx++) {
		if(formation->minCnts[memberIdx] != 0)
			break;
	}
	if(memberIdx == FORMATION_MEMBERS)
		return FORMATION_NO_SURE_MEMBER;
	/* The average counts each member that may appear once, whatever its
	   numbers. */
	for(memberIdx = 0; memberIdx < FORMATION_MEMBERS; memberIdx++) {
		if(formation->maxCnts[memberIdx] == 0)
			continue;
		enemyId = formation->memberIds[memberIdx];
		enemy = gmEnemyGetDefinitionStub(enemyId + ENEMY_ID_FIRST);
		if(enemy == 0)
			continue;
		if(enemy->level > MONSTER_BATTLE_OPEN_LEVEL &&
			!gmFlagGetStub(FLAG_MONSTER_BATTLE_ANY_ENEMY) &&
			!gmFlagGetStub(ENEMY_DEFEATED_FLAG(enemyId + ENEMY_ID_FIRST)))
			return FORMATION_UNMET_ENEMY;
		levelTotal += enemy->level;
		enemyCnt++;
	}
	if(enemyCnt == 0)
		return FORMATION_NO_ENEMY;
	return levelTotal / enemyCnt;
}

/* A formation that may be chosen for the party's level, with its enemies'
   average level. */
typedef struct FORMATION_CANDIDATE {
	Sint16 formationId; /* the formation's row */
	Sint16 score; /* its average level (btFormationAverageLevel); -1 empty */
} FORMATION_CANDIDATE;

/* The sprite slots one of member's enemies takes: two for a large enemy. */
#define FORMATION_MEMBER_SLOTS(formation, member) \
	(2 - (btSprIsSmall((formation)->memberIds[member] + ENEMY_ID_FIRST) != 0))

/* The most formations a monster battle chooses among. */
#define FORMATION_POOL_SIZE		  32
/* How far above the party's level a monster battle's formation may be. */
#define FORMATION_LEVEL_MARGIN	  3
/* Above any formation's average level: where the search for the pool's lowest
   entry starts. */
#define FORMATION_SCORE_ABOVE_ALL 999

/* A unit's base stats as they were before its level changed. */
typedef struct BATTLE_BASE_STATS {
	Sint16 baseHp;
	Sint16 basePp;
	Sint16 hpGauge; /* copied, not read */
	Sint16 ppGauge; /* copied, not read */
	Uint16 baseAttack;
	Uint16 baseDefense;
	Uint16 baseAgility;
	Uint8  baseLuck;  /* copied, not read */
	Uint8  actionCnt; /* copied, not read */
	Uint8  hpRegen;	  /* copied, not read */
	Uint8  ppRegen;	  /* copied, not read */
	Uint8  unused[2]; /* Unit's baseUnused */
	BATTLE_ELEMENT_STATS
	element[ELEMENT_COUNT]; /* the base power is read */
} BATTLE_BASE_STATS;

/* Chooses a monster battle's formation for the party's level plus its winning
   streak, at random among those at most 3 above it, and returns it with the
   levels its enemies must gain in outMargin. */
Sint32 btFormationSelectLevelMatchedCandidate(Sint32 *outMargin)
{
	Sint32				 matchCnt = 0;
	FORMATION_CANDIDATE *pool =
		syHeapAllocEWRAM(FORMATION_POOL_SIZE * sizeof *pool);
	Uint16 party[8];
	Sint32 levelTotal = 0;
	Sint32 partySize = btPartyPrepareMembers(party);
	Sint32 i;
	Sint32 entry;
	Sint32 tgtLevel;
	Sint32 result;

	if(partySize > 0) {
		const Uint16 *member = party;

		i = partySize;
		do {
			levelTotal += gmUnitGetStub(*member++)->level;
		} while(--i != 0);
	}

	/* The flag byte is -1 once a series has ended, which takes a level off. */
	tgtLevel = levelTotal / partySize;
	tgtLevel += (Sint8)gmFlagGetByteStub(FLAG_MONSTER_BATTLE_WINS);
	if(tgtLevel <= 0)
		tgtLevel = 1;
	if(tgtLevel > UNIT_LEVEL_MAX)
		tgtLevel = UNIT_LEVEL_MAX;

	for(i = 0; i < FORMATION_POOL_SIZE; i++)
		pool[i].score = -1;

	/* Forget the defeated enemies the monster battles exclude. */
	for(i = 0; i < ARRAY_COUNT(BattleFormationExcludedEnemies); i++) {
		gmEnemyGetDefinitionStub(BattleFormationExcludedEnemies[i]);
		gmFlagClearStub(ENEMY_DEFEATED_FLAG(BattleFormationExcludedEnemies[i]));
	}

	/* Each qualifying formation replaces the pool's lowest entry; empty entries
	   score -1, so the pool fills first. */
	for(i = 0; i < ARRAY_COUNT(Formations); i++) {
		Sint32 score = btFormationAverageLevel(i);

		if(score >= 0 && score <= tgtLevel + FORMATION_LEVEL_MARGIN) {
			Sint32 lowestIdx = -1;
			Sint32 lowestLevel = FORMATION_SCORE_ABOVE_ALL;

			for(entry = 0; entry < FORMATION_POOL_SIZE; entry++) {
				if(pool[entry].score < lowestLevel) {
					lowestLevel = pool[entry].score;
					lowestIdx = entry;
				}
			}
			if(lowestIdx >= 0) {
				pool[lowestIdx].score = score;
				pool[lowestIdx].formationId = i;
				matchCnt++;
			}
		}
	}

	/* matchCnt includes formations evicted from the pool. */
	if(matchCnt > FORMATION_POOL_SIZE)
		matchCnt = FORMATION_POOL_SIZE;

	if(matchCnt != 0) {
		FORMATION_CANDIDATE *chosen = &pool[syRandom16Below(matchCnt)];

		result = chosen->formationId;
		*outMargin = tgtLevel - chosen->score;
	} else {
		*outMargin = 0;
		result = FORMATION_FALLBACK;
	}

	syHeapRelease(pool);
	return result;
}

/* The element power an enemy gains a level (btEnemyAdjustLevel); its other
   stats gain tenths of a point. */
#define ELEMENT_POWER_GAIN 15

/* Raises an enemy by levels, or lowers it for a negative count, each stat by a
   fixed amount a level and no lower than 70 percent of what it was. */
UNREAD_RESULT btEnemyAdjustLevel(Sint32 unitId, Sint32 levels)
{
	BATTLE_BASE_STATS *base = syHeapAllocEWRAM(sizeof *base);
	UNIT			  *unit = gmUnitGetStub(unitId);
	Sint32			   value;
	Sint32			   element;

	syWordsCopy(base, &unit->baseHp, sizeof *base);

	value = unit->baseHp + levels * 97 / 10;
	CLAMP(value, base->baseHp * 7 / 10, UNIT_HP_MAX);
	unit->baseHp = value;

	value = unit->basePp + levels * 15 / 10;
	CLAMP(value, base->basePp * 7 / 10, UNIT_HP_MAX);
	unit->basePp = value;

	value = unit->baseAttack + levels * 123 / 10;
	CLAMP(value, base->baseAttack * 7 / 10, UNIT_STAT_MAX);
	unit->baseAttack = value;

	value = unit->baseDefense + levels * 33 / 10;
	CLAMP(value, base->baseDefense * 7 / 10, UNIT_STAT_MAX);
	unit->baseDefense = value;

	value = unit->baseAgility + levels * 51 / 10;
	CLAMP(value, base->baseAgility * 7 / 10, UNIT_STAT_MAX);
	unit->baseAgility = value;

	for(element = 0; element < ELEMENT_COUNT; element++) {
		value = unit->baseElement[element].power + levels * ELEMENT_POWER_GAIN;
		CLAMP(value, base->element[element].power * 7 / 10, UNIT_ELEMENT_MAX);
		unit->baseElement[element].power = value;
	}

	unit->level += levels;
	gmUnitCalcStub(unitId);
	syHeapRelease(base);
}

/* Gives an enemy its name letter, which tells it from others of its kind:
   digits abroad and the alphabet in Japanese. Returns -1 when five kinds are
   already listed. */
Sint32 btEnemyClaimNameLetter(Sint32 enemyId, Sint32 unused)
{
	BATTLE_STATE *battle;
	Sint32		  kindCnt;
	Sint32		  kindIdx;
	Sint32		  attempt;
	Sint32		  suffix;

	battle = BattleStateWork;
	kindCnt = battle->enemyKindCnt;
	for(kindIdx = 0; kindIdx < kindCnt; kindIdx++) {
		if(battle->enemyKinds[kindIdx] == enemyId)
			break;
	}
	if(kindIdx != kindCnt) {
		attempt = 0;
		/* The second of its kind: letters 0 and 1 both taken. */
		if(battle->lastNameLetter[kindIdx] < 0) {
			battle->lastNameLetter[kindIdx] = 1;
			battle->nameLetterMasks[kindIdx] = 3;
			return ENEMY_LETTER_FIRST_TOO | 1;
		}
		/* The next letter after the last given that is free; after
		   NAME_LETTER_MASK_BITS tries the last one tried is reused. */
		for(; attempt < NAME_LETTER_MASK_BITS; attempt++) {
			suffix =
				(battle->lastNameLetter[kindIdx] + 1) % ENEMY_NAME_SUFFIX_COUNT;
			battle->lastNameLetter[kindIdx] = suffix;
			if((battle->nameLetterMasks[kindIdx] & (1 << (Sint8)suffix)) == 0)
				break;
		}
		battle->nameLetterMasks[kindIdx] |= 1
			<< battle->lastNameLetter[kindIdx];
		return battle->lastNameLetter[kindIdx];
	}
	if(kindCnt <= 4) {
		battle->lastNameLetter[kindCnt] = -1;
		battle->enemyKinds[kindCnt] = enemyId;
		battle->nameLetterMasks[kindCnt] = 0;
		battle->enemyKindCnt = kindCnt + 1;
		return ENEMY_NAME_SUFFIX_COUNT;
	}
	return -1;
}

/* Frees a departing enemy's name letter for the next of its kind. */
UNREAD_RESULT btEnemyReleaseNameLetter(Sint32 unitId)
{
	BATTLE_STATE *battle = BattleStateWork;
	UNIT		 *unit;
	Sint32		  cnt = battle->enemyKindCnt;
	Sint32		  idx;
	Sint32		  nameLength;
	Sint32		  suffixIdx;
	Sint32		  enemyId;

	unit = gmUnitGetStub(unitId);
	if(unit->classId != CLASS_NONE)
		return;

	enemyId = unit->id;
	for(idx = 0; idx < cnt; idx++) {
		if(battle->enemyKinds[idx] == enemyId)
			break;
	}
	if(idx == cnt || battle->nameLetterMasks[idx] == 0)
		return;

	for(nameLength = 0; nameLength < UNIT_NAME_SIZE - 1; nameLength++) {
		if(unit->name[nameLength] == 0)
			break;
	}

	/* The name's last character is taken as its letter, even in a name with
	   none, and that letter is freed. */
	/* UB: an empty name shifts 1 by NAME_LETTER_MASK_BITS, past the mask, which
	   the hardware gives as 0, so no letter is freed; UBFIX skips the clear. */
	suffixIdx = NAME_LETTER_MASK_BITS;
	if(nameLength > 0)
		suffixIdx = unit->name[nameLength - 1] - ENEMY_NAME_LETTER_FIRST;
#if FIX_UNDEFINED_SHIFT
	if(suffixIdx < NAME_LETTER_MASK_BITS)
#endif
		battle->nameLetterMasks[idx] &= ~(1 << suffixIdx);
}

/* Gives the first enemy of the kind on the field the first letter (0), appended
   to its name, once a second of its kind arrives. */
UNREAD_RESULT btEnemyLetterFirstOfKind(Sint32 enemyId)
{
	UNIT  *unit;
	Sint32 enemyIdx;
	Sint32 nameIdx;

	for(enemyIdx = 0; enemyIdx <= ENEMY_BATTLE_SIZE - 1; enemyIdx++) {
		unit = gmUnitGetStub(enemyIdx + FIRST_ENEMY_UNIT);
		if(unit->side != BATTLE_SIDE_ENEMY)
			continue;
		if(unit->id != enemyId)
			continue;
		/* The name's end, then the letter and a new terminator after it. */
		for(nameIdx = 0; nameIdx <= UNIT_NAME_SIZE - 2; nameIdx++) {
			if(unit->name[nameIdx] == 0) {
				unit->name[nameIdx] = ENEMY_NAME_LETTER_FIRST;
				unit->name[nameIdx + 1] = 0;
				break;
			}
		}
		return;
	}
}

/* Returns the enemy id of a random member of the formation that may appear. */
Sint32 btFormationGetRandomMember(Sint32 formationId)
{
	Sint32			 choices[FORMATION_MEMBERS];
	const FORMATION *formation;
	Uint32			 choiceCnt = 0;
	Sint32			 member;

	if(formationId < 0 || formationId >= FORMATION_COUNT)
		formationId = FORMATION_FALLBACK;
	formation = &Formations[formationId];
	for(member = 0; member < FORMATION_MEMBERS; member++) {
		if(formation->maxCnts[member] != 0)
			choices[choiceCnt++] =
				formation->memberIds[member] + ENEMY_ID_FIRST;
	}
	return choices[syRandom16Below(choiceCnt)];
}

/* Builds the battle's enemies from a formation and returns how many there are:
   each member its fewest times and at random up to its most while the sprite
   slots hold them, laid out as the formation says. A monster battle chooses the
   formation for the party's level instead. */
Sint32 btFormationBuildEnemyList(Sint32 formationId)
{
	Uint16		 *enemyKinds;
	BATTLE_STATE *battle;
	FORMATION	 *formation;
	Sint32		  enemyCnt;
	Sint32		  levelAdjustment;
	Uint16		  listBuf[UNIT_MAX];
	Sint32		  extraCnts[FORMATION_MEMBERS];
	Sint32		  memberCnts[FORMATION_MEMBERS];
	Sint32		  memberOrder[FORMATION_MEMBERS];
	Sint32		  freeSprSlots;
	Bool		  changed;
	Sint32		  additionalRoom;
	Sint32		  sprSlots;
	Sint32		  fittingCnt;
	Sint32		  enemyId;
	Sint32		  j;
	Sint32		  remainingKinds;
	Sint32		  copies;
	Uint32		  i;
	Sint32		  memberIdx;

	battle = BattleStateWork;
	levelAdjustment = 0;
	battle->enemyKindCnt = 0;
	enemyKinds = listBuf;
	if(gmFlagGetStub(FLAG_MONSTER_BATTLE))
		formationId = btFormationSelectLevelMatchedCandidate(&levelAdjustment);
	if(formationId < 0 || formationId >= FORMATION_COUNT)
		formationId = FORMATION_FALLBACK;
	formation = &Formations[formationId];
	for(memberIdx = 0;
		memberIdx < FORMATION_MEMBERS && formation->minCnts[memberIdx] == 0;
		memberIdx++)
		;
	if(memberIdx == FORMATION_MEMBERS)
		formation = &Formations[FORMATION_FALLBACK];

	/* The enemy sprite slots left once every member's minimum is placed; a
	   large enemy takes two. */
	freeSprSlots = ENEMY_SPRITE_SLOTS;
	enemyCnt = 0;
	for(i = 0; i < FORMATION_MEMBERS; i++) {
		if(formation->minCnts[i] != 0) {
			Sint32 requiredSlots = FORMATION_MEMBER_SLOTS(formation, i);

			freeSprSlots -= requiredSlots * formation->minCnts[i];
		}
	}

	/* Each member's extra enemies: a random number from 0 to as many more as
	   its maximum allows and the free slots would hold on their own. */
	for(i = 0; i < FORMATION_MEMBERS; i++) {
		Sint32 minimum;
		Sint32 maximum;

		memberCnts[i] = minimum = formation->minCnts[i];
		maximum = formation->maxCnts[i];
		additionalRoom = maximum - minimum;
		if(additionalRoom > 0) {
			Sint32 memberSlots = FORMATION_MEMBER_SLOTS(formation, i);

			fittingCnt = freeSprSlots / memberSlots;
			if(fittingCnt < additionalRoom)
				additionalRoom = fittingCnt;
			extraCnts[i] = ((additionalRoom + 1) * syRandom()) >> 16;
		} else {
			extraCnts[i] = 0;
		}
	}

	/* The extras then take the shared slots one per member a pass, in member
	   order, until none fits. */
	do {
		changed = FALSE;
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			enemyId = formation->memberIds[i] + ENEMY_ID_FIRST;
			if(extraCnts[i] != 0) {
				sprSlots = 2 - (btSprIsSmall(enemyId) != 0);
				if(sprSlots > freeSprSlots) {
					extraCnts[i] = 0;
				} else {
					memberCnts[i]++;
					extraCnts[i]--;
					freeSprSlots -= sprSlots;
					changed = TRUE;
				}
			}
		}
	} while(changed);

	/* The list of enemy ids, in the order the battle type gives, ended by 0. */
	battle->layout = formation->layout;
	switch(battle->layout) {
	case FORMATION_LAYOUT_SHUFFLED:
		/* The members' order shuffled by ten random swaps. */
		for(i = 0; i < FORMATION_MEMBERS; i++)
			memberOrder[i] = i;
		for(i = 0; i < FORMATION_SHUFFLE_SWAPS; i++) {
			Sint32 first = syRandom16Below(FORMATION_MEMBERS);
			Sint32 second = syRandom16Below(FORMATION_MEMBERS);
			Sint32 swap = memberOrder[first];

			memberOrder[first] = memberOrder[second];
			memberOrder[second] = swap;
		}
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			Sint32 copyIdx;
			Sint32 slot = memberOrder[i];

			for(copyIdx = 0; copyIdx < memberCnts[slot]; copyIdx++)
				enemyKinds[enemyCnt++] =
					formation->memberIds[slot] + ENEMY_ID_FIRST;
		}
		break;
	case FORMATION_LAYOUT_INTERLEAVED:
		/* Each enemy drawn from a random member with any left. */
		for(;;) {
			remainingKinds = 0;
			for(i = 0; i < FORMATION_MEMBERS; i++) {
				if(memberCnts[i] != 0)
					memberOrder[remainingKinds++] = i;
			}
			if(remainingKinds == 0)
				break;
			j = memberOrder[syRandom16Below(remainingKinds)];
			enemyKinds[enemyCnt++] = formation->memberIds[j] + ENEMY_ID_FIRST;
			memberCnts[j]--;
		}
		break;
	default:
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			copies = memberCnts[i];
			for(j = 0; j < copies; j++)
				enemyKinds[enemyCnt++] =
					formation->memberIds[i] + ENEMY_ID_FIRST;
		}
		break;
	}
	/* No kind has fallen yet (a slot past the last), and the enemy units start
	   cleared. */
	enemyKinds[enemyCnt] = 0;
	battle->firstDefeated = ENEMY_BATTLE_SIZE;
	battle->defeatState = DEFEAT_ONE;
	for(i = FIRST_ENEMY_UNIT; i < FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE; i++)
		syWordsClear(gmUnitGetStub(i), BATTLE_UNIT_SIZE);

	i = 0;
	/* Each enemy unit, with its name letter (btEnemyClaimNameLetter) and, in a
	   monster battle, the level margin. */
	if(enemyKinds[0] != 0) {
		for(;;) {
			Sint32 suffixCode = btEnemyClaimNameLetter(enemyKinds[i], 1);

			if(suffixCode & ENEMY_LETTER_FIRST_TOO)
				btEnemyLetterFirstOfKind(enemyKinds[i]);
			gmBattleUnitSetStub(i + FIRST_ENEMY_UNIT, enemyKinds[i],
				suffixCode & ENEMY_LETTER_MASK);
			/* DEAD: the unit is fetched and not used; the ROM keeps the
			   call. */
			gmUnitGetStub(i + FIRST_ENEMY_UNIT);
			if(levelAdjustment != 0)
				btEnemyAdjustLevel(i + FIRST_ENEMY_UNIT, levelAdjustment);
			i++;
			if(i >= ENEMY_BATTLE_SIZE || enemyKinds[i] == 0)
				break;
		}
	}
	return i;
}

#undef NAME_LETTER_MASK_BITS
#undef FORMATION_FALLBACK
#undef FORMATION_SHUFFLE_SWAPS
#undef ELEMENT_POWER_GAIN
#undef FORMATION_SCORE_ABOVE_ALL
#undef FORMATION_NO_SURE_MEMBER
#undef FORMATION_UNMET_ENEMY
#undef FORMATION_NO_ENEMY
#undef MONSTER_BATTLE_OPEN_LEVEL
