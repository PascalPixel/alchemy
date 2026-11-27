/* Enemy formations: the enemies chosen, levelled and told apart by name
   suffixes, and each enemy class's sprite, palette, size and attack effect.
   Part of battle.c. */

/* The most formations a monster battle chooses among. */
#define FORMATION_POOL_SIZE		  32
/* How far above the party's level a monster battle's formation may be. */
#define FORMATION_LEVEL_MARGIN	  3
/* Above any formation's average level: where the search for the pool's lowest
   entry starts. */
#define FORMATION_SCORE_ABOVE_ALL 999

/* Chooses a monster battle's formation for the party's level plus its winning
   streak, at random among those at most FORMATION_LEVEL_MARGIN above it, and
   returns it with the levels its enemies must gain in outMargin. */
Sint32 btFormationSelectLevelMatchedCandidate(Sint32 *outMargin)
{
	Sint32				 matchCnt = 0;
	FORMATION_CANDIDATE *pool =
		syHeapAllocEWRAM(FORMATION_POOL_SIZE * sizeof *pool);
	Uint16 party[8];
	Sint32 tgtLevel = 0;
	Sint32 unitCnt = btPartyPrepareMembers(party);
	Sint32 i;
	Sint32 poolIdx;
	Sint32 result;

	if(unitCnt > 0) {
		Uint16 *unitCursor = party;

		i = unitCnt;
		do {
			Uint8 level = syUnitGet(*unitCursor)->level;

			i--;
			unitCursor++;
			tgtLevel += level;
		} while(i != 0);
	}

	/* The flag byte is -1 once a series has ended, which takes a level off. */
	tgtLevel = tgtLevel / unitCnt;
	tgtLevel += (Sint8)syGameFlagGetByte(FLAG_MONSTER_BATTLE_WINS);
	if(tgtLevel <= 0)
		tgtLevel = 1;
	if(tgtLevel > UNIT_LEVEL_MAX)
		tgtLevel = UNIT_LEVEL_MAX;

	for(i = 0; i < FORMATION_POOL_SIZE; i++)
		pool[i].score = -1;

	/* A boss beaten in another of its forms marks one form beaten: the
	   Serpent's four-beam one, the Knight's second, Moapa's third, and the Doom
	   Dragon's first only once its last has fallen. */
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_ONE_BEAM)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_TWO_BEAMS)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_THREE_BEAMS)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT_B));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_B)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
	syGameFlagClear(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));
	if(syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_H)))
		syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));

	/* Forget the defeated classes the monster battles exclude. */
	for(i = 0; i < ARRAY_COUNT(BattleFormationExcludedEnemies); i++)
		syGameFlagClear(ENEMY_DEFEATED_FLAG(BattleFormationExcludedEnemies[i]));
	/* BUG: the three enemies formations 241 to 248 are made of are set by their
	   enemy ids, not their defeated flags, so they stay unavailable unless
	   beaten, and FLAG_DJINNI(DJINNI_SPRITZ), FLAG_DJINNI(DJINNI_ETHER) and
	   flag 0xe6 are set instead. */
#if FIX_MONSTER_BATTLE_DEFEATED_FLAGS
	syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_PUNCH_ANT));
	syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_GIANT_BAT));
	syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_WILD_WOLF));
#else
	syGameFlagSet(ENEMY_PUNCH_ANT);
	syGameFlagSet(ENEMY_GIANT_BAT);
	syGameFlagSet(ENEMY_WILD_WOLF);
#endif

	/* Each qualifying formation replaces the pool's lowest entry; empty entries
	   score -1, so the pool fills first. */
	for(i = 0; (Uint32)i < FORMATION_COUNT; i++) {
		Sint32 score = btFormationAverageLevel(i);

		if(score >= 0 && score <= tgtLevel + FORMATION_LEVEL_MARGIN) {
			Sint32 lowestIdx = -1;
			Sint32 lowestLevel;

			for(lowestLevel = FORMATION_SCORE_ABOVE_ALL, poolIdx = 0;
				poolIdx < FORMATION_POOL_SIZE; poolIdx++) {
				if(pool[poolIdx].score < lowestLevel) {
					lowestLevel = pool[poolIdx].score;
					lowestIdx = poolIdx;
				}
			}

			if(lowestIdx >= 0) {
				pool[lowestIdx].score = score;
				pool[lowestIdx].recordId = i;
				matchCnt++;
			}
		}
	}

	/* matchCnt includes formations evicted from the pool. */
	if(matchCnt > FORMATION_POOL_SIZE)
		matchCnt = FORMATION_POOL_SIZE;

	if(matchCnt != 0) {
		FORMATION_CANDIDATE *chosen = &pool[(matchCnt * syRandom()) >> 16];

		result = chosen->recordId;
		*outMargin = tgtLevel - chosen->score;
	} else {
		*outMargin = matchCnt;
		result = 1;
	}

	syHeapRelease(pool);
	return result;
}

/* What an enemy gains a level (btEnemyAdjustLevel), each stat in tenths of a
   point: 9.7 HP, 1.5 PP, 12.3 Attack, 3.3 Defense and 5.1 Agility; and its
   element power in whole points. */
#define HP_GAIN_TENTHS		97
#define PP_GAIN_TENTHS		15
#define ATTACK_GAIN_TENTHS	123
#define DEFENSE_GAIN_TENTHS 33
#define AGILITY_GAIN_TENTHS 51
#define ELEMENT_POWER_GAIN	15

/* Raises an enemy by levels, or lowers it for a negative count, each stat by a
   fixed amount a level and no lower than 70 percent of what it was. */
UNREAD_RESULT btEnemyAdjustLevel(Sint32 unitId, Sint32 levels)
{
	BATTLE_BASE_STATS *base = syHeapAllocEWRAM(sizeof *base);
	UNIT			  *unit = syUnitGet(unitId);
	Sint32			   value;
	Sint32			   element;

	syWordsCopyEntry(base, &unit->baseHp, sizeof *base);

	value = unit->baseHp + levels * HP_GAIN_TENTHS / 10;
	CLAMP(value, base->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
	unit->baseHp = value;

	value = unit->basePp + levels * PP_GAIN_TENTHS / 10;
	CLAMP(value, base->basePp * 7 / 10, UNIT_ENEMY_PP_MAX);
	unit->basePp = value;

	value = unit->baseAttack + levels * ATTACK_GAIN_TENTHS / 10;
	CLAMP(value, base->baseAttack * 7 / 10, UNIT_STAT_MAX);
	unit->baseAttack = value;

	value = unit->baseDefense + levels * DEFENSE_GAIN_TENTHS / 10;
	CLAMP(value, base->baseDefense * 7 / 10, UNIT_STAT_MAX);
	unit->baseDefense = value;

	value = unit->baseAgility + levels * AGILITY_GAIN_TENTHS / 10;
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

/* Raises a unit's base HP by half and base Attack and Defense by a quarter, at
   least 70 percent of the old values and at most UNIT_ENEMY_HP_MAX and
   UNIT_STAT_MAX: an enemy on the HARD level (FLAG_HARD_LEVEL). */
void btUnitStrengthen(Sint32 unitId)
{
	BATTLE_BASE_STATS *saved = syHeapAllocEWRAM(sizeof *saved);
	UNIT			  *unit = syUnitGet(unitId);
	Sint32			   value;

	syWordsCopyEntry(saved, &unit->baseHp, sizeof *saved);
	value = unit->baseHp * 3 / 2;
	CLAMP(value, saved->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
	unit->baseHp = value;
	value = unit->baseAttack * 5 / 4;
	CLAMP(value, saved->baseAttack * 7 / 10, UNIT_STAT_MAX);
	unit->baseAttack = value;
	value = unit->baseDefense * 5 / 4;
	CLAMP(value, saved->baseDefense * 7 / 10, UNIT_STAT_MAX);
	unit->baseDefense = value;
	gmUnitCalcStub(unitId);
	syHeapRelease(saved);
}

/* The suffixes a class's mask holds, one bit each. */
#define NAME_SUFFIX_MASK_BITS 32

/* Gives an enemy of the class its name suffix, which tells it from others of
   its kind: digits abroad and the alphabet in Japanese. Returns -1 when five
   classes are already listed. */
Sint32 btEnemyClaimNameLetter(Sint32 classId, Sint32 unused)
{
	BATTLE_STATE *battle;
	Sint32		  classCnt;
	Sint32		  classIdx;
	Sint32		  attempt;

	battle = Ram_HeapSlots->battleState;
	classCnt = battle->enemyKindCnt;
	for(classIdx = 0; classIdx < classCnt; classIdx++) {
		if(battle->enemyKinds[classIdx] == classId)
			break;
	}
	if(classIdx != classCnt) {
		attempt = 0;
		/* The second of its class: suffixes 0 and 1 both taken. */
		if(battle->lastNameLetter[classIdx] < 0) {
			battle->lastNameLetter[classIdx] = 1;
			battle->nameLetterMasks[classIdx] = 3;
			return ENEMY_LETTER_FIRST_TOO | 1;
		}
		/* The next suffix after the last given that is free; after 32 tries the
		   last one tried is reused. */
		for(; attempt < NAME_SUFFIX_MASK_BITS; attempt++) {
			battle->lastNameLetter[classIdx] =
				(battle->lastNameLetter[classIdx] + 1) % NAME_SUFFIX_COUNT;
			if((battle->nameLetterMasks[classIdx] &
				   (1 << battle->lastNameLetter[classIdx])) == 0)
				break;
		}
		battle->nameLetterMasks[classIdx] |= 1
			<< battle->lastNameLetter[classIdx];
		return battle->lastNameLetter[classIdx];
	}
	if(classCnt <= 4) {
		battle->lastNameLetter[classCnt] = -1;
		battle->enemyKinds[classCnt] = classId;
		battle->nameLetterMasks[classCnt] = 0;
		battle->enemyKindCnt = classCnt + 1;
		return NAME_SUFFIX_COUNT;
	}
	return -1;
}

/* Frees a departing enemy's name suffix for the next of its class. */
UNREAD_RESULT btEnemyReleaseNameLetter(Sint32 unitId)
{
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	UNIT		 *unit;
	Sint32		  cnt = battle->enemyKindCnt;
	Sint32		  idx;
	Sint32		  nameLength;
	Sint32		  suffix;
	Sint32		  classId;

	unit = syUnitGet(unitId);
	if(unit->classId != CLASS_NONE)
		return;

	classId = unit->enemyId;
	for(idx = 0; idx < cnt; idx++) {
		if(battle->enemyKinds[idx] == classId)
			break;
	}
	if(idx == cnt || battle->nameLetterMasks[idx] == 0)
		return;

	for(nameLength = 0; nameLength < UNIT_NAME_SIZE - 1; nameLength++) {
		if(unit->name[nameLength] == 0)
			break;
	}

	/* UB: an empty name shifts by NAME_SUFFIX_MASK_BITS, which the ARM turns
	   into 0, so no suffix is freed. A name with no suffix instead uses its
	   last character as one. */
	suffix = NAME_SUFFIX_MASK_BITS;
#if FIX_UNDEFINED_SHIFT
	if(nameLength == 0)
		return;
#endif
	if(nameLength > 0)
		suffix = unit->name[nameLength - 1] - ENEMY_NAME_LETTER_FIRST;
	battle->nameLetterMasks[idx] &= ~(1 << suffix);
}

/* Gives the first enemy of the class on the field the first suffix (0),
   appended to its name, once a second of its class arrives. */
Sint32 btEnemyLetterFirstOfKind(Sint32 enemyId)
{
	UNIT  *unit;
	Sint32 enemyIdx;
	Sint32 nameIdx;
	Sint32 terminatorIdx;
	Uint8  nameByte;

	for(enemyIdx = 0; enemyIdx < ENEMY_BATTLE_SIZE; enemyIdx++) {
		unit = syUnitGet(enemyIdx + FIRST_ENEMY_UNIT);
		nameByte = unit->side;
		if(nameByte != BATTLE_SIDE_ENEMY)
			continue;
		if(unit->enemyId != enemyId)
			continue;
		/* nameByte still holds BATTLE_SIDE_ENEMY here, used as the index after
		   the suffix; nameIdx holds the terminator, 0. */
		nameIdx = 0;
		if(unit->name[0] == 0) {
			unit->name[0] = ENEMY_NAME_LETTER_FIRST;
			unit->name[nameByte] = nameIdx;
			return;
		}
		/* The name's end, then the suffix and a new terminator after it. */
	scan_slots:
		nameIdx++;
		if(nameIdx > UNIT_NAME_SIZE - 2)
			return;
		nameByte = unit->name[nameIdx];
		if(nameByte != 0)
			goto scan_slots;
		terminatorIdx = nameIdx + 1;
		unit->name[nameIdx] = ENEMY_NAME_LETTER_FIRST;
		unit->name[terminatorIdx] = nameByte;
		return;
	}
}

/* Returns the class of a random member of the formation that may appear. */
Sint32 btFormationGetRandomMember(Sint32 recordId)
{
	Sint32	  *choicesBase;
	Sint32	   choices[FORMATION_MEMBERS];
	FORMATION *record;
	Sint32	  *choiceCursor;
	Uint8	  *maximumCnt;
	Uint16	  *memberId;
	Sint32	   remaining;
	Uint32	   cnt;

	cnt = 0;
	if((Uint32)recordId >= FORMATION_COUNT)
		recordId = 1;

	record = &Formations[recordId];
	choicesBase = choices;
	memberId = record->memberIds;
	maximumCnt = record->maxCnts;
	choiceCursor = choicesBase;
	for(remaining = 0; remaining < FORMATION_MEMBERS; remaining++) {
		if(*maximumCnt++ != 0) {
			Sint32 id = *memberId;

			cnt++;
			*choiceCursor++ = id + ENEMY_ID_FIRST;
		}
		memberId++;
	}

	return choices[(syRandom() * cnt) >> 16];
}

/* The enemy sprite slots an enemy of the class takes: one for a small enemy,
   two for any other. */
#define ENEMY_SLOTS_TAKEN(classId) (2 - (btSprIsSmall(classId) != 0))

/* Builds the battle's enemies from a formation and returns how many there are:
   each member its fewest times and at random up to its most while the sprite
   slots hold them, laid out as the formation says. */
Sint32 btFormationBuildEnemyList(Sint32 recordId)
{
	Uint16		 *enemyClasses;
	BATTLE_STATE *battle;
	FORMATION	 *record;
	Sint32		  freeSprSlots;
	Sint32		  enemyCnt;
	Sint32		  levelAdjustment;
	Uint16		  listBuf[UNIT_MAX];
	Sint32		  extraCnts[FORMATION_MEMBERS];
	Sint32		  memberCnts[FORMATION_MEMBERS];
	Sint32		  memberOrder[FORMATION_MEMBERS];
	Bool		  changed;
	Sint32		  additionalRoom;
	Sint32		  sprSlots;
	Sint32		  fittingCnt;
	Sint32		  classId;
	Sint32		  remainingKinds;
	Sint32		  copies;
	Uint32		  i;
	Sint32		  memberIdx;

	battle = (BATTLE_STATE *)Ram_HeapSlots->battleState;
	levelAdjustment = 0;
	battle->enemyKindCnt = 0;
	enemyClasses = listBuf;
	if(syGameFlagGet(FLAG_MONSTER_BATTLE))
		recordId = btFormationSelectLevelMatchedCandidate(&levelAdjustment);
	if((Uint32)recordId >= FORMATION_COUNT)
		recordId = 1;
	record = &Formations[recordId];
	for(memberIdx = 0;
		memberIdx < FORMATION_MEMBERS && record->minCnts[memberIdx] == 0;
		memberIdx++)
		;
	if(memberIdx == FORMATION_MEMBERS)
		record = &Formations[1];

	freeSprSlots = ENEMY_SPRITE_SLOTS;
	enemyCnt = 0;
	for(i = 0; i < FORMATION_MEMBERS; i++) {
		if(record->minCnts[i] != 0) {
			Sint32 requiredSlots =
				ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

			freeSprSlots -= requiredSlots * record->minCnts[i];
		}
	}

	for(i = 0; i < FORMATION_MEMBERS; i++) {
		Sint32 minimum;
		Sint32 maximum;

		memberCnts[i] = minimum = record->minCnts[i];
		maximum = record->maxCnts[i];
		additionalRoom = maximum - minimum;
		if(additionalRoom > 0) {
			Sint32 memberSlots =
				ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

			fittingCnt = freeSprSlots / memberSlots;
			if(fittingCnt < additionalRoom)
				additionalRoom = fittingCnt;
			extraCnts[i] = ((additionalRoom + 1) * syRandom()) >> 16;
		} else {
			extraCnts[i] = 0;
		}
	}

	do {
		changed = FALSE;
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			classId = record->memberIds[i] + ENEMY_ID_FIRST;
			if(extraCnts[i] != 0) {
				sprSlots = ENEMY_SLOTS_TAKEN(classId);
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

	battle->layout = record->layout;
	switch(battle->layout) {
	case FORMATION_LAYOUT_SHUFFLED:
		for(i = 0; i < FORMATION_MEMBERS; i++)
			memberOrder[i] = i;
		for(i = 0; i <= 9; i++) {
			Sint32 first = syRandomBelow(FORMATION_MEMBERS);
			Sint32 second = syRandomBelow(FORMATION_MEMBERS);
			Sint32 swap = memberOrder[first];

			memberOrder[first] = memberOrder[second];
			memberOrder[second] = swap;
		}
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			Sint32 copyIdx;
			Sint32 slot = memberOrder[i];

			for(copyIdx = 0; copyIdx < memberCnts[slot]; copyIdx++)
				enemyClasses[enemyCnt++] =
					record->memberIds[slot] + ENEMY_ID_FIRST;
		}
		break;
	case FORMATION_LAYOUT_INTERLEAVED:
		for(;;) {
			Sint32 member;

			remainingKinds = 0;
			for(i = 0; i < FORMATION_MEMBERS; i++) {
				if(memberCnts[i] != 0)
					memberOrder[remainingKinds++] = i;
			}
			if(remainingKinds == 0)
				break;
			/* One of the members still owed a copy, at random. */
			member = memberOrder[(syRandom() * remainingKinds) >> 16];
			enemyClasses[enemyCnt++] =
				record->memberIds[member] + ENEMY_ID_FIRST;
			memberCnts[member]--;
		}
		break;
	default:
		for(i = 0; i < FORMATION_MEMBERS; i++) {
			for(copies = 0; copies < memberCnts[i]; copies++)
				enemyClasses[enemyCnt++] =
					record->memberIds[i] + ENEMY_ID_FIRST;
		}
		break;
	}
	enemyClasses[enemyCnt] = 0;
	battle->firstDefeated = ENEMY_BATTLE_SIZE;
	battle->defeatState = DEFEAT_ONE;
	for(i = FIRST_ENEMY_UNIT; i < (FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE); i++)
		syWordsClearEntry(syUnitGet(i), BATTLE_UNIT_SIZE);

	i = 0;
	if(enemyClasses[0] != 0) {
		for(;;) {
			/* Repeated enemy classes get distinct suffixes within this
			   encounter. */
			Sint32 suffix = btEnemyClaimNameLetter(enemyClasses[i], 1);
			UNIT  *unit;
			Sint32 enemyId;

			if(suffix & ENEMY_LETTER_FIRST_TOO)
				btEnemyLetterFirstOfKind(enemyClasses[i]);
			gmBattleUnitSetStub(i + FIRST_ENEMY_UNIT, enemyClasses[i],
				suffix & ENEMY_LETTER_MASK);
			unit = syUnitGet(i + FIRST_ENEMY_UNIT);
			if(levelAdjustment != 0)
				btEnemyAdjustLevel(i + FIRST_ENEMY_UNIT, levelAdjustment);
			else if(!syGameFlagGet(FLAG_MONSTER_BATTLE) &&
				syGameFlagGet(FLAG_HARD_LEVEL))
				btUnitStrengthen(i + FIRST_ENEMY_UNIT);
			battle->forceField = 0;
			enemyId = unit->enemyId;
			switch(enemyId) {
			case ENEMY_POSEIDON:
			case ENEMY_POSEIDON_SECOND_ACTION:
			case ENEMY_POSEIDON_3:
				if(!syGameFlagGet(FLAG_MONSTER_BATTLE))
					battle->forceField = 1;
				break;
			}
			i++;
			if(i >= ENEMY_BATTLE_SIZE || enemyClasses[i] == 0)
				break;
		}
	}
	return i;
}

/* Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen
   enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked. */
Uint32 btSprGetDefeatSnd(Sint32 idx)
{
	Uint32 kind = BattleSprites[idx].defeatSnd;

	if((Sint32)kind > 4)
		kind = -1U;
	return kind;
}

/* The last enemy with a row in BattleSprites (battle_sprites.c). */
#define ENEMY_CLASS_LAST 386

/* Returns the class's battle sprite, row 0's for a class out of range. */
Sint32 btSprGetSpr(Sint32 idx)
{
	if((Uint32)idx > ENEMY_CLASS_LAST)
		return BattleSprites[0].anm;
	return BattleSprites[idx].anm;
}

/* Returns the class's sprite palette. For a class out of range it returns row
   0's sprite, not a palette. */
Sint32 btSprGetPalette(Sint32 idx)
{
	if((Uint32)idx > ENEMY_CLASS_LAST)
		return BattleSprites[0].anm;
	return BattleSprites[idx].pal;
}

/* Returns 1 for a small class, which takes one enemy sprite slot instead of two
   and stands closer to its neighbours; 0 out of range. */
Sint32 btSprIsSmall(Sint32 idx)
{

	Bool   result;
	Uint32 flg;

	if((Uint32)idx > ENEMY_CLASS_LAST)
		return 0;
	result = FALSE;
	flg = BattleSprites[idx].isSmall;
	if(flg)
		result = TRUE;
	return result;
}

/* Returns the battle effect a class's plain attack shows, 1 for none or out of
   range. */
Uint32 btSprGetAttackEffect(Uint32 idx)
{
	Uint16 value;

	if(idx > ENEMY_CLASS_LAST)
		return 1;
	value = BattleSprites[idx].attackEff;
	return value ? value : 1;
}

/* Returns how high a class hops as it runs up to attack, in pixels; 0 out of
   range. */
Uint32 btSprGetAttackHop(Uint32 idx)
{
	Uint16 value;

	if(idx > ENEMY_CLASS_LAST)
		return 0;
	value = BattleSprites[idx].attackHop;
	return value ? value : 0;
}

/* Returns 1 for a class whose blow lands with a heavier impact; 0 out of
   range. */
Sint32 btSprHasHeavyImpact(Sint32 idx)
{
	if((Uint32)idx > ENEMY_CLASS_LAST)
		return 0;
	return BattleSprites[idx].heavyImpact;
}

/* Returns the height an animation is drawn at over a class, in pixels, or 0 for
   its size's default; 0 out of range. */
Uint32 btSprGetEffectHeight(Uint32 idx)
{
	if(idx > ENEMY_CLASS_LAST)
		return 0;
	return BattleSprites[idx].effHeight;
}
