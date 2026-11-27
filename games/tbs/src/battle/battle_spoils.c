/* @file */
/* The spoils: coins, experience, items and level ups. */
/* Part of battle.c. */

#include "battle_internal.h"

/* Returns an item's rank among the spoils: an indispensable item first, then a
 */
/* rare one, then the higher item id; 0 for none. */
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
/* PSYNERGY_ENTRY.encodedAction's top bit: the action comes from the class. */
#define PSYNERGY_FROM_CLASS_SHIFT 15

/* Empties the battle's spoils (BATTLE_STATE.spoils) and */
/* MainRoutineStateWork.pendingItem. */
void btSpoilsClear(void) {
	BATTLE_STATE *battle = BattleStateWork;
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

/* Returns an enemy's coins with the Djinn bonus: its own, plus a six-sided */
/* die for every ten levels and one more, at least 30 percent of its own. */
static __inline__ s32 btEnemyCalcCoinReward(UNIT *unit, const u16 *reward) {
	s32 rollIndex;
	s32 bonus = 0;
	s32 base;
	s32 minimumBonus;

	for(rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
		bonus += syRandom16Below(6) + 1;
	base = *reward;
	minimumBonus = base * 3 / 10;
	if(bonus < minimumBonus)
		bonus = minimumBonus;
	return bonus + base;
}

/* Returns an enemy's experience with the Djinn bonus: its own plus a die for */
/* every ten levels, at least 30 percent more. */
static __inline__ s32 btEnemyCalcExperienceReward(UNIT *unit,
												  const u16 *reward) {
	s32 rollIndex;
	s32 bonus = 0;
	s32 base;
	s32 minimumBonus;

	for(rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
		bonus += syRandom16Below(4) + 1;
	base = *reward;
	minimumBonus = base * 3 / 10;
	if(bonus < minimumBonus)
		bonus = minimumBonus;
	return bonus + base;
}

/* Counts a fallen enemy and adds its coins, experience and perhaps its item to
 */
/* the spoils, more with the Djinn bonus. Returns -1 for a party unit, -2 for a
 */
/* party member fought as an enemy, else 0. */
s32 btEnemyRecordDefeat(s32 unitId, s32 djinnBonus) {
	UNIT *unit;
	const ENEMY_DEFINITION *enemy;
	BATTLE_STATE *battle;
	BATTLE_SPOILS *spoils;
	s32 i;
	s32 dropShift;
	s32 classSlot;
	s32 lowestItemPriority;
	s32 lowestSlot;
	s32 itemPriority;

	unit = gmUnitGetStub(unitId);
	battle = BattleStateWork;
	spoils = &battle->spoils;
	if(unitId >= 0 && unitId <= ALLY_MAX)
		return -1;
	if(unit->classId != CLASS_NONE)
		return -2;
	classSlot = 0;
	for(i = 0; i < ENEMY_BATTLE_SIZE && battle->enemyKinds[i] != unit->id; i++)
		;
	if(i != ENEMY_BATTLE_SIZE)
		classSlot = i;
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
	if(gmFlagGetStub(FLAG_MONSTER_BATTLE))
		return 0;
	gmFlagSetStub(ENEMY_DEFEATED_FLAG(unit->id));
	enemy = btEnemyGetDefinitionStub(unit->id);
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
	/* An item already kept drops no second time. */
	for(i = 0; i <= 3 && spoils->items[i] != enemy->item; i++)
		;
	if(i != 4)
		return 0;
	dropShift = enemy->itemChance;
	if(djinnBonus != 0)
		dropShift -= 2;
	if(dropShift < 0)
		dropShift = 0;
	/* Twice FIXED_ONE >> n against a 16-bit random number: 1 in 2^(n - 1). */
	if((FIXED_ONE * 2 >> dropShift) > (btRandomStub() & 0xffff)) {
		lowestSlot = -1;
		lowestItemPriority = SPOILS_RANK_CEILING;
		for(i = 0; i <= 3; i++) {
			itemPriority = btSpoilsRankItem(spoils->items[i]);
			if(itemPriority < lowestItemPriority) {
				lowestItemPriority = itemPriority;
				lowestSlot = i;
			}
		}
		if((s32)btSpoilsRankItem(enemy->item) > lowestItemPriority)
			spoils->items[lowestSlot] = enemy->item;
	}
	return 0;
}

/* Hands out the spoils after a victory: experience to each party member still
 */
/* standing, with every level gained, then the coins, then the items best */
/* first. */
void btAwardSpoils(void) {
	BATTLE_SPOILS *spoils;
	s32 count;
	UNIT *unit;
	UNIT *beforeLevelUp;
	s32 abilityIndex;
	u32 encodedAbility;
	s32 oldAbilityIndex;
	s32 highestItemPriority;
	s32 highestItemSlot;
	s32 slot;
	u16 *overflowItem;
	s32 itemPriority;
	LEVEL_UP_RESULT gains; /* Read stat increases as signed values. */
	u16 units[8];

	spoils = &BattleStateWork->spoils;
	if(spoils->experience != 0) {
		mnMessageArgumentPushStub(spoils->experience, MESSAGE_ARGUMENT_NUMBER);
		mnBattleMessageShowAndWaitStub(MsgExpGained);
		btMessageWaitForAdvance();
	}
	/* Experience and level ups: each party member is copied before every level
	 * it */
	/* may gain, so that what the level brought can be read against the copy. */
	count = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, units);
	beforeLevelUp = syHeapAllocEwram(sizeof(UNIT));
	{
		s32 unitIndex;

		for(unitIndex = 0; unitIndex < count; unitIndex++) {
			s32 unitId = units[unitIndex];

			unit = gmUnitGetStub(unitId);
			unit->experience += spoils->experience;
			while(syWordsCopy(beforeLevelUp, unit, sizeof(UNIT)),
				  gmUnitTryLevelUpStub(unitId, &gains) != 0) {
				sySndPlayStub(MUS_LEVEL_UP);
				mnMessageArgumentClearAllStub();
				mnMessageArgumentPushStub(unit->classId,
										  MESSAGE_ARGUMENT_CLASS);
				mnMessageArgumentPushStub(units[unitIndex],
										  MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(unit->level, MESSAGE_ARGUMENT_NUMBER);
				mnBattleMessageShowAndWaitStub(MsgLevelUp);
				btMessageWaitForAdvance();
				/* A class ability the copy lacks is newly learned. */
				for(abilityIndex = 0; abilityIndex < PSYNERGY_SLOTS;
					abilityIndex++) {
					encodedAbility = unit->psynergy[abilityIndex].encodedAction;
					if((encodedAbility & PSYNERGY_ABILITY_MASK) &&
					   (encodedAbility >> PSYNERGY_FROM_CLASS_SHIFT)) {
						for(oldAbilityIndex = 0;
							oldAbilityIndex < PSYNERGY_SLOTS;
							oldAbilityIndex++) {
							if(encodedAbility ==
							   beforeLevelUp->psynergy[oldAbilityIndex]
								   .encodedAction)
								break;
						}
						if(oldAbilityIndex == PSYNERGY_SLOTS) {
							mnMessageArgumentClearAllStub();
							mnMessageArgumentPushStub(unit->classId,
													  MESSAGE_ARGUMENT_CLASS);
							mnMessageArgumentPushStub(unitId,
													  MESSAGE_ARGUMENT_ACTOR);
							mnMessageArgumentPushStub(encodedAbility &
														  PSYNERGY_ABILITY_MASK,
													  MESSAGE_ARGUMENT_ABILITY);
							sySndPlayStub(SE_CAST);
							mnBattleMessageShowAndWaitStub(MsgAbilityMastered);
							btMessageWaitForAdvance();
						}
					}
				}
				if((s16)gains.hp != 0) {
					mnMessageArgumentPushStub((s16)gains.hp,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgMaxHpRises);
					btMessageWaitForAdvance();
				}
				if((s16)gains.pp != 0) {
					mnMessageArgumentPushStub((s16)gains.pp,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgMaxPpRises);
					btMessageWaitForAdvance();
				}
				if((s16)gains.attack != 0) {
					mnMessageArgumentPushStub((s16)gains.attack,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgAttackRises);
					btMessageWaitForAdvance();
				}
				if((s16)gains.defense != 0) {
					mnMessageArgumentPushStub((s16)gains.defense,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgDefenseRises);
					btMessageWaitForAdvance();
				}
				if((s16)gains.agility != 0) {
					mnMessageArgumentPushStub((s16)gains.agility,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgAgilityRises);
					btMessageWaitForAdvance();
				}
				if((s16)gains.luck != 0) {
					mnMessageArgumentPushStub((s16)gains.luck,
											  MESSAGE_ARGUMENT_NUMBER);
					mnBattleMessageShowAndWaitStub(MsgLuckRises);
					btMessageWaitForAdvance();
				}
			}
		}
	}
	syHeapRelease(beforeLevelUp);
	if(spoils->coins != 0) {
		mnMessageArgumentPushStub(spoils->coins, MESSAGE_ARGUMENT_NUMBER);
		mnBattleMessageShowAndWaitStub(MsgCoinsGained);
		gmPartyAdjustCoinsStub(spoils->coins);
		btMessageWaitForAdvance();
	}
	/* The items, the highest ranked first; the first the inventory cannot */
	/* take is left in OverflowItem. */
	overflowItem = &OverflowItem;
	for(;;) {
		highestItemSlot = -1;
		highestItemPriority = -1;
		for(slot = 0; slot <= 3; slot++) {
			if(spoils->items[slot] != 0) {
				itemPriority = btSpoilsRankItem(spoils->items[slot]);
				if(itemPriority >= highestItemPriority) {
					highestItemPriority = itemPriority;
					highestItemSlot = slot;
				}
			}
		}
		if(highestItemSlot == -1)
			break;
		mnMessageArgumentPushStub(spoils->items[highestItemSlot],
								  MESSAGE_ARGUMENT_ITEM);
		mnBattleMessageShowAndWaitStub(MsgItemGained);
		btMessageWaitForAdvance();
		if(gmPartyInventoryAddStub(spoils->items[highestItemSlot]) == -1) {
			*overflowItem = spoils->items[highestItemSlot];
			break;
		}
		spoils->items[highestItemSlot] = 0;
	}
}

/* Does nothing; the debug battle menu (btDebugBattleRunMenu) calls it. */
void btDebugSelectNoOp(void) {}

#undef SPOILS_RANK_CEILING
#undef PSYNERGY_FROM_CLASS_SHIFT
