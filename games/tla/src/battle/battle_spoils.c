/* @file */
/* The spoils: coins, experience, items and level ups. */
/* Part of battle.c. */

/* What a level up raised, as places in the halfwords gmUnitTryLevelUp fills */
/* (LEVEL_UP_RESULT's fields, read here as an array): each is shown when */
/* it is not 0. */
typedef enum LEVEL_UP_GAIN {
	GAIN_HP = 2,
	GAIN_PP,
	GAIN_ATTACK,
	GAIN_DEFENSE,
	GAIN_AGILITY,
	GAIN_LUCK,
	GAIN_COUNT
} LEVEL_UP_GAIN;

/* Returns an item's rank among the spoils, by which the four kept are chosen */
/* and handed out best first: its item id (INVENTORY_ITEM_ID_MASK's nine */
/* bits), with ITEM_RARE's bit above it and ITEM_INDISPENSABLE's above that; */
/* 0 for none. */
u32 btSpoilsRankItem(u32 inventoryEntry) {
	u32 priority = 0;
	u32 itemId = inventoryEntry & INVENTORY_ITEM_ID_MASK;
	if(itemId == 0)
		return 0;
	{
		u8 flags = gmItemGetDirectStub(itemId)->flg;

		if(flags & ITEM_INDISPENSABLE)
			priority = 1;
		priority <<= 1;
		if(flags & ITEM_RARE)
			priority++;
		priority <<= 9;
		priority += itemId;
	}
	return priority;
}

/* Above every item's rank: the search for the lowest of the spoils' four */
/* starts here. */
#define SPOILS_RANK_CEILING (1 << 30)

/* Empties the battle's spoils and the party's pending item. Called by */
/* btRun. */
void btSpoilsClear(void) {
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	BATTLE_SPOILS *spoils = &battle->spoils;
	u16 *items;
	s32 index;

	MainRoutineStateWork.pendingItem = 0;
	spoils->coins = 0;
	spoils->experience = 0;
	spoils->defeated = 0;
	items = battle->spoils.items;
	for(index = 3; index >= 0; index--)
		items[index] = 0;
}

/* The name suffixes the scan reads back (NAME_SUFFIX_COUNT of them): the */
/* digits 1 to 9 outside Japan, and the letters A to Z in the Japanese release.
 */
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIRST_DIGIT_BASE  'A'
#define FIRST_DIGIT_COUNT 26
#else
#define FIRST_DIGIT_BASE  '1'
#define FIRST_DIGIT_COUNT 9
#endif

/* Returns an enemy name's suffix: its first digit less one (its first */
/* letter's place in the alphabet in Japanese), or FIRST_DIGIT_COUNT when it */
/* has none. */
s32 btStringFindFirstDigit(u8 *name) {
	s32 i;

	for(i = 0; i < UNIT_NAME_SIZE - 1; i++) {
		if((u8)(name[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
			return name[i] - FIRST_DIGIT_BASE;
	}
	return FIRST_DIGIT_COUNT;
}

/* Returns an enemy's coins with the Djinn bonus: its own, plus a six-sided die
 */
/* for every ten levels and one more, at least 30 percent of its own. */
static __inline__ s32 btEnemyCalcCoinReward(UNIT *unit, u16 *reward) {
	s32 rollIndex;
	s32 bonus = 0;
	s32 base;
	s32 minimumBonus;

	for(rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
		bonus += (syRandomBelow(6)) + 1;
	base = *reward;
	minimumBonus = base * 3 / 10;
	if(bonus < minimumBonus)
		bonus = minimumBonus;
	return bonus + base;
}

/* Returns an enemy's experience with the Djinn bonus: its own plus a */
/* four-sided die for every ten levels and one more, at least 30 percent more.
 */
static __inline__ s32 btEnemyCalcExperienceReward(UNIT *unit, u16 *reward) {
	s32 rollIndex;
	s32 bonus = 0;
	s32 base;
	s32 minimumBonus;

	for(rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
		bonus += (syRandomBelow(4)) + 1;
	base = *reward;
	minimumBonus = base * 3 / 10;
	if(bonus < minimumBonus)
		bonus = minimumBonus;
	return bonus + base;
}

/* Counts a fallen enemy and adds its spoils, The Broken Seal's */
/* btEnemyRecordDefeat with the battle state from its heap slot. This game */
/* also keeps the last two defeated enemies (their ids and */
/* btStringFindFirstDigit's first digit of their names, alternating slots, with
 */
/* a count up to 2) and sets the defeated flag only for the enemies that have */
/* one, below ENEMY_DEFEATED_FLAG_COUNT. */
s32 btEnemyRecordDefeat(s32 unitId, s32 djinnBonus) {
	UNIT *unit;
	const ENEMY_DEFINITION *enemy;
	BATTLE_STATE *battle;
	BATTLE_SPOILS *spoils;
	s32 itemIndex;
	s32 dropShift;
	s32 classSlot;
	s32 lowestItemPriority;
	s32 lowestSlot;
	s32 itemPriority;
	s32 slot;
	s32 classIndex;

	unit = syUnitGet(unitId);
	battle = Ram_HeapSlots->battleState;
	spoils = &battle->spoils;
	if(unitId >= 0 && unitId <= ALLY_MAX)
		return -1;
	if(unit->classId != CLASS_NONE)
		return -2;
	/* Queued for Regen Dance: the name letter is the first digit of the name.
	 */
	slot = spoils->queueHead = (spoils->queueHead + 1) % 2;
	spoils->queuedSummons[slot] = unit->enemyId;
	spoils->queuedLetters[slot] = btStringFindFirstDigit((u8 *)unit);
	if(spoils->queueCnt <= 1)
		spoils->queueCnt++;
	classSlot = 0;
	classIndex = 0;
	/* UB: the search runs to a sixth kind, one past enemyKinds: it reads */
	/* kindsEnd, the halfword after them. */
	if(battle->enemyKinds[0] != unit->enemyId) {
		do {
			if(++classIndex > (ENEMY_BATTLE_SIZE - 1))
				break;
		} while(battle->enemyKinds[classIndex] != unit->enemyId);
	}
	if(classIndex != ENEMY_BATTLE_SIZE)
		classSlot = classIndex;
	/* The first class to fall names the victory message. */
	/* XXX (no evidence): whether DEFEAT_BY_PARTY_MEMBER was meant; no state is
	 * 2, */
	/* so the test never fails. */
	if(battle->defeatState != 2) {
		if(classSlot < battle->firstDefeated)
			battle->firstDefeated = classSlot;
		if(spoils->defeated)
			battle->defeatState = DEFEAT_SEVERAL;
	}
	spoils->defeated++;
	if(syGameFlagGet(FLAG_MONSTER_BATTLE))
		return 0;
	if(unit->enemyId < ENEMY_DEFEATED_FLAG_COUNT)
		syGameFlagSet(ENEMY_DEFEATED_FLAG(unit->enemyId));
	enemy = gmEnemyGetDefinitionStub(unit->enemyId);
	if(djinnBonus != 0) {
		if(enemy->coins != 0)
			spoils->coins += btEnemyCalcCoinReward(unit, &enemy->coins);
		if(enemy->experience != 0)
			spoils->experience +=
				btEnemyCalcExperienceReward(unit, &enemy->experience);
	}
	else {
		spoils->coins += enemy->coins;
		spoils->experience += enemy->experience;
	}
	if(enemy->item == 0 || enemy->itemChance == 0)
		return 0;
	itemIndex = 0;
	if(spoils->items[0] != enemy->item) {
		do {
			if(++itemIndex > 3)
				break;
		} while(spoils->items[itemIndex] != enemy->item);
	}
	if(itemIndex == 4) {
		dropShift = enemy->itemChance;
		if(djinnBonus != 0)
			dropShift -= 2;
		if(dropShift < 0)
			dropShift = 0;
		/* Twice FIXED_ONE >> n against a 16-bit random number: 1 in 2^(n - 1).
		 */
		if((FIXED_ONE * 2 >> dropShift) > (btRandomStub() & 0xffff)) {
			lowestSlot = -1;
			lowestItemPriority = SPOILS_RANK_CEILING;
			for(itemIndex = 0; itemIndex <= 3; itemIndex++) {
				itemPriority = btSpoilsRankItem(spoils->items[itemIndex]);
				if(itemPriority < lowestItemPriority) {
					lowestItemPriority = itemPriority;
					lowestSlot = itemIndex;
				}
			}
			if((s32)btSpoilsRankItem(enemy->item) > lowestItemPriority)
				spoils->items[lowestSlot] = enemy->item;
		}
	}
	return 0;
}

/* Hands out the spoils after a victory: experience to each party member still
 */
/* standing, with every level gained, then the coins, then the items best */
/* first. */
void btAwardSpoils(void) {
	BATTLE_SPOILS *spoils;
	s32 living;
	u16 *list;
	s32 count;
	s32 i;
	s32 unitId;
	UNIT *unit;
	UNIT *backup;
	s32 entry;
	u32 learned;
	s32 j;
	s32 best;
	s32 bestSlot;
	s32 slot;
	u16 *found;
	s32 value;
	s16 gains[GAIN_COUNT];
	u16 units[10];

	spoils = &Ram_HeapSlots->battleState->spoils;
	if(spoils->experience != 0) {
		mnMessageArgumentPushStub(spoils->experience, MESSAGE_ARGUMENT_NUMBER);
		/* "You got N experience points." */
		mnBattleMessageShowAndWaitStub((s32)MsgGotExperience);
		btMessageWaitForAdvance();
	}
	list = units;
	living = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, list);
	count = btUnitListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, list);
	backup = syHeapAllocEwram(sizeof(UNIT));
	for(i = 0; i < count; i++) {
		unitId = list[i];
		unit = syUnitGet(unitId);
		if(i >= living)
			unit->experience += (u32)spoils->experience >> 1;
		else
			unit->experience += spoils->experience;
		while(syWordsCopyEntry(backup, unit, sizeof(UNIT)),
			  gmUnitTryLevelUpStub(unitId, gains) != 0) {
			sySndPlayStub(MUS_LEVEL_UP);
			mnMessageArgumentClearAllStub();
			mnMessageArgumentPushStub(unit->classId, MESSAGE_ARGUMENT_CLASS);
			mnMessageArgumentPushStub(list[i], MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(unit->level, MESSAGE_ARGUMENT_NUMBER);
			/* "<name> is now a level N ..." */
			mnBattleMessageShowAndWaitStub((s32)MsgLevelUp);
			btMessageWaitForAdvance();
			for(entry = 0; entry < PSYNERGY_SLOTS; entry++) {
				learned = unit->psynergy[entry].encodedAction;
				/* A class ability the copy lacks is newly learned. */
				if((learned & ACTION_ID_MASK) &&
				   (learned >> PSYNERGY_FROM_CLASS_SHIFT)) {
					for(j = 0; j < PSYNERGY_SLOTS; j++) {
						if(learned == backup->psynergy[j].encodedAction)
							break;
					}
					if(j == PSYNERGY_SLOTS) {
						mnMessageArgumentClearAllStub();
						mnMessageArgumentPushStub(unit->classId,
												  MESSAGE_ARGUMENT_CLASS);
						mnMessageArgumentPushStub(unitId,
												  MESSAGE_ARGUMENT_ACTOR);
						mnMessageArgumentPushStub(learned & ACTION_ID_MASK,
												  MESSAGE_ARGUMENT_ABILITY);
						sySndPlayStub(SE_CAST);
						/* "Mastered the <class>'s <ability>" */
						mnBattleMessageShowAndWaitStub((s32)MsgMasteredAbility);
						btMessageWaitForAdvance();
					}
				}
			}
			if(gains[GAIN_HP] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_HP],
										  MESSAGE_ARGUMENT_NUMBER);
				/* "Maximum HP rises by N!" */
				mnBattleMessageShowAndWaitStub((s32)MsgMaxHpRises);
				btMessageWaitForAdvance();
			}
			if(gains[GAIN_PP] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_PP],
										  MESSAGE_ARGUMENT_NUMBER);
				mnBattleMessageShowAndWaitStub((s32)MsgMaxPpRises);
				btMessageWaitForAdvance();
			}
			if(gains[GAIN_ATTACK] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_ATTACK],
										  MESSAGE_ARGUMENT_NUMBER);
				mnBattleMessageShowAndWaitStub((s32)MsgAttackRises);
				btMessageWaitForAdvance();
			}
			if(gains[GAIN_DEFENSE] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_DEFENSE],
										  MESSAGE_ARGUMENT_NUMBER);
				mnBattleMessageShowAndWaitStub((s32)MsgDefenseRises);
				btMessageWaitForAdvance();
			}
			if(gains[GAIN_AGILITY] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_AGILITY],
										  MESSAGE_ARGUMENT_NUMBER);
				mnBattleMessageShowAndWaitStub((s32)MsgAgilityRises);
				btMessageWaitForAdvance();
			}
			if(gains[GAIN_LUCK] != 0) {
				mnMessageArgumentPushStub(gains[GAIN_LUCK],
										  MESSAGE_ARGUMENT_NUMBER);
				/* "Luck rises by N!" */
				mnBattleMessageShowAndWaitStub((s32)MsgLuckRises);
				btMessageWaitForAdvance();
			}
		}
	}
	syHeapRelease(backup);
	if(spoils->coins != 0) {
		mnMessageArgumentPushStub(spoils->coins, MESSAGE_ARGUMENT_NUMBER);
		/* "You got N coins." */
		mnBattleMessageShowAndWaitStub((s32)MsgGotCoins);
		gmPartyAdjustCoinsStub(spoils->coins);
		btMessageWaitForAdvance();
	}
	found = &MainRoutineStateWork.pendingItem;
	for(;;) {
		bestSlot = -1;
		best = -1;
		for(slot = 0; slot <= 3; slot++) {
			if(spoils->items[slot] != 0) {
				value = btSpoilsRankItem(spoils->items[slot]);
				if(value >= best) {
					best = value;
					bestSlot = slot;
				}
			}
		}
		if(bestSlot == -1)
			break;
		mnMessageArgumentPushStub(spoils->items[bestSlot],
								  MESSAGE_ARGUMENT_ITEM);
		/* "You got <item>." */
		mnBattleMessageShowAndWaitStub((s32)MsgGotItem);
		btMessageWaitForAdvance();
		if(gmPartyInventoryAddStub(spoils->items[bestSlot]) == -1) {
			*found = spoils->items[bestSlot];
			break;
		}
		spoils->items[bestSlot] = 0;
	}
}
#undef FIRST_DIGIT_BASE
#undef FIRST_DIGIT_COUNT
